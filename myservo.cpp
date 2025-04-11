#include "myservo.h"
#include <Servo.h>
#include <stddef.h>
#include <Arduino.h>

Servo sort, discharge, stop, feeder;

#define sortPin 40
#define dischargePin 45
#define stopPin 44
#define feedPin 41

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
  Serial.println("Activate Servo stop");
  stop.write(180);
}
void resetStop(){
  stop.write(90);
}

void feederON() {
  Serial.println("Activate feeder on");
  feeder.write(120);
}
void feederOFF() {
  feeder.write(90);
}

void servoDischarge() {
  Serial.println("I will discharge");
  discharge.write(180);
}
void resetDischarge() {
   discharge.write(90);
}
void servoSorting(int angle) {
  Serial.println("I wass call to sort servo!");
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
