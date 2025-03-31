#include "conveyorControl.h"
#include <Arduino.h>
#include <Wire.h>

#define ENA 2   // Speed control for Motor A
#define ENB 7   // Speed control for Motor B
#define IN1 3   // Motor A Direction
#define IN2 4
#define IN3 5   // Motor B Direction
#define IN4 6

unsigned long prevTime1 = 0;
unsigned long prevTime2 = 0;
unsigned long prevTime3 = 0;
unsigned long conveyorStartTime = 0;
bool conveyorRunning = false;

int speedA = 255; // Adjust speed (0-255)
int speedB = 50;

#define activateConveyor 120000

void initConveyor() {
  Wire.begin();
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

// Move forward every 1 second (for manure)
void conveyorForward_Manure() {
  if (millis() - prevTime1 >= 1000) {
    prevTime1 = millis();
    Serial.println("Manure Conveyor Forward");
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speedB);
  }
}

// Reverse every 5 seconds (for manure)
void conveyorReverse_Manure() {
  if (millis() - prevTime2 >= 5000) {
    prevTime2 = millis();
    Serial.println("Manure Conveyor Reverse");
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, speedB);
  }
}

// Move forward every 1 second (for collection)
void conveyorForward_Collect() {
  if (millis() - prevTime3 >= 1000) {
    prevTime3 = millis();
    Serial.println("Collection Conveyor Forward");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speedA);
  }
}

// Stop the conveyor **only after a full run cycle**
void conveyorStop() {
  if (millis() - conveyorStartTime >= 40000) {  // Stops after 10 seconds
    Serial.println("I will stop!");
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    conveyorRunning = false;  // Reset state
  }
}

// Master function to control the conveyor
void runConveyor() {
  if (!conveyorRunning) {
    conveyorRunning = true;
    conveyorStartTime = millis();
    Serial.println("Starting Conveyor...");
  }

  // Run conveyor functions
  conveyorForward_Manure();
  conveyorReverse_Manure();
  conveyorForward_Collect();
  conveyorStop();
}
