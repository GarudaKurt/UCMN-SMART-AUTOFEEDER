#include "controlMotor.h"
#include <Arduino.h>
#include <Wire.h>
#include <AccelStepper.h>

// Define stepper pins
#define STEP_PIN 3      // Step pin
#define DIR_PIN 2       // Direction pin

// Microstepping control pins
#define M0_PIN 7
#define M1_PIN 6
#define M2_PIN 5

// Steps per revolution for the motor
const float stepsPerRevolution = 200;
// Microstepping multiplier (1, 2, 4, 8, 16, or 32)
int microstepSetting = 1;

// Set the desired RPM and the max RPM
float desiredRPM = 60; // Set the desired speed in rpm (revolutions per minute)
float MaxRPM = 300; // Set max speed in rpm (revolutions per minute)

// Calculate and set the desired and max speed in steps per second
float speedStepsPerSec = (microstepSetting * stepsPerRevolution*desiredRPM) / 60.0;
float Max_Speed_StepsPerSec = microstepSetting * stepsPerRevolution * MaxRPM / 60; // Specify max speed in steps/sec (converted from RPM)

// AccelStepper instance in driver mode
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);


unsigned long prevStepperTime = 0;
unsigned long stepperStartTime = 0;
bool stepperRunning = false;

void initStepperConveyor() {
  Wire.begin();
  pinMode(M0_PIN, OUTPUT);
  pinMode(M1_PIN, OUTPUT);
  pinMode(M2_PIN, OUTPUT);

  // Set microstepping mode (adjust as needed: HIGH or LOW)
  digitalWrite(M0_PIN, LOW);  // Set to LOW or HIGH for desired microstep setting
  digitalWrite(M1_PIN, LOW);  // Set to LOW or HIGH for desired microstep setting
  digitalWrite(M2_PIN, LOW);  // Set to LOW or HIGH for desired microstep setting

}

void conveyorForward_Collect_Stepper() {
  if (millis() - prevStepperTime >= 1000) {
    prevStepperTime = millis();
    Serial.println("Stepper Forward");
    stepper.setMaxSpeed(Max_Speed_StepsPerSec);
    stepper.setSpeed(speedStepsPerSec);
  }
  stepper.runSpeed();
}


void runStepperConveyor() {
  // Start stepper conveyor if not already running
  if (!stepperRunning) {
    stepperRunning = true;
    stepperStartTime = millis();  // Start timer when the conveyor is activated
    Serial.println("Starting Stepper Conveyor...");
    // digitalWrite(ENABLE_PIN, LOW);  // Enable driver (uncomment if needed)
  }

  conveyorForward_Collect_Stepper();

  if (millis() - stepperStartTime >= conveyorRunDuration) {
    // Stop after 40 seconds
    conveyorStop_Stepper();
  }
}

void conveyorStop_Stepper() {
  if (millis() - stepperStartTime >= conveyorRunDuration) {
    Serial.println("Stepper Stopped");
    stepper.setSpeed(0);  // Stop the motor
    stepperRunning = false;
  }
}



