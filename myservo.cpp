#include "myservo.h"
#include <Servo.h>
#include <stddef.h>
#include <Arduino.h>

Servo sort, discharge, stop, feeder;

#define sortPin 44
#define dischargePin 45
#define stopPin 46
#define feedPin 47

void initServo() {
  sort.attach(sortPin);
  discharge.attach(dischargePin);
  stop.attach(stopPin);
  feeder.attach(feedPin);
  
  sort.write(90);
  discharge.write(90);
  stop.write(90);
  feeder.write(90);

}

void servoStop() {
  stop.write(180);
}
void resetStop(){
  stop.write(90);
}

void feederON() {
  feeder.write(120);
}
void feederOFF() {
  feeder.write(90);
}

void servoDischarge() {
  discharge.write(180);
}
void resetDischarge() {
   discharge.write(90);
}
void servoSorting(int angle) {
  sort.write(angle);
}
void resetSorting() {
  sort.write(90);
}

void servoReset() {
  sort.write(90);
  discharge.write(90);
  stop.write(90);
}

