#include <Arduino.h>
#include "display.h"
#include "weightObject.h"
#include "timer.h"
#include "conveyorControl.h"
#include "myservo.h"
#include "controlMotor.h"

unsigned long prev = 0;
unsigned long prev1 = 0;


void setup() {
  Serial.begin(115200);
  initDisplay();
  initWeight();
  initTimer();
  initConveyor();
  initServo();
  initStepperConveyor();
  delay(2000);
}

void loop() {
  float weight = getWeightValue();
  const char *time = getCurrentTime();
  showDisplay(time, weight);
  
  //activateSUPPLY();  // Check timers

  //Serial.println("I will run this!");
  //runConveyor();   // Run conveyor only when activated
}
