#include <Arduino.h>
#include "display.h"
#include "weightObject.h"
#include "timer.h"


void setup() {
  Serial.begin(115200);
  initDisplay();
  initWeight();
  initTimer();
  delay(2000);
}

void loop() {
  float weight = getWeightValue();
  const char * time = getCurrentTime();
  showDisplay(time, weight);
  delay(1000);
}