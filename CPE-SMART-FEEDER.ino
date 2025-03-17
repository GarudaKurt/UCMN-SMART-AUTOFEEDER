#include <Arduino.h>
#include "display.h"
#include "weightObject.h"
#include "timer.h"
#include "conveyorControl.h"

void setup() {
  Serial.begin(115200);
  initDisplay();
  initWeight();
  initTimer();
  initConveyor();
  delay(2000);
}

void loop() {
  float weight = getWeightValue();
  const char *time = getCurrentTime();
  showDisplay(time, weight);
}
