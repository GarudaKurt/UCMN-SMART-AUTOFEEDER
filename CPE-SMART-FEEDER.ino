#include <Arduino.h>
#include <Wire.h>
#include "display.h"
#include "weightObject.h"
#include "timer.h"
#include "conveyorControl.h"
#include "myservo.h"

#define SLAVE_ADDRESS 8
bool hasSent = false;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  initDisplay();
  initWeight();
  initTimer();
  initConveyor();
  initServo();
  delay(2000);
}

void loop() {
  float weight = getWeightValue();
  const char *time = getCurrentTime();
  showDisplay(time, weight);
  int mins = getMins();

  if (mins % 2 == 0 && !hasSent) {
    Serial.println("Activate conveyor");
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write("activate");
    Wire.endTransmission();
    hasSent = true;
  }

  if (mins % 2 != 0) {
    hasSent = false;
  }
}
