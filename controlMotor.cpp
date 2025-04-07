#include "controlMotor.h"
#include <Arduino.h>
#include <Wire.h>
#include <AccelStepper.h>

#define DIR_PIN 2
#define STEP_PIN 3
#define ENABLE_PIN 4

AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

unsigned long prevStepperTime = 0;
unsigned long stepperStartTime = 0;
bool stepperRunning = false;

void initStepperConveyor() {
  Wire.begin();
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);  // Enable driver
}

void conveyorForward_Collect_Stepper() {
  if (millis() - prevStepperTime >= 1000) {
    prevStepperTime = millis();
    Serial.println("Stepper Forward");
    stepper.move(800);  // Adjust steps per second here
  }
  stepper.run();
}

void conveyorStop_Stepper() {
  if (millis() - stepperStartTime >= 40000) {
    Serial.println("Stepper Stopped");
    stepper.stop();
    digitalWrite(ENABLE_PIN, HIGH); // Disable motor driver
    stepperRunning = false;
  }
}

void runStepperConveyor() {
  if (!stepperRunning) {
    stepperRunning = true;
    stepperStartTime = millis();
    Serial.println("Starting Stepper Conveyor...");
    digitalWrite(ENABLE_PIN, LOW); // Enable driver
  }

  conveyorForward_Collect_Stepper();
  conveyorStop_Stepper();
}
