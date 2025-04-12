#include "timer.h"
#include "conveyorControl.h"
#include "myservo.h"
#include <RTClib.h>
#include <Arduino.h>

RTC_DS1307 rtc;

#define relay_lightsUV 10
#define relay_water 12

bool isLight_running = false;
bool isFood_running = false;

const unsigned long intervalLight = 60000;   // 1 minute
const unsigned long intervalFood = 10000;    // 10 seconds
unsigned long prevTime_light = 0;
unsigned long prevTime_food = 0;

int currentMinutes = 0; 

void initTimer() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  pinMode(relay_lightsUV, OUTPUT);
  pinMode(relay_water, OUTPUT);

  digitalWrite(relay_lightsUV, HIGH);
  digitalWrite(relay_water, HIGH);
}

static void uvlightON() { digitalWrite(relay_lightsUV, LOW); }
static void uvlightOFF() { digitalWrite(relay_lightsUV, HIGH); }
static void waterON() { digitalWrite(relay_water, LOW); }
static void waterOFF() { digitalWrite(relay_water, HIGH); }

const char* getCurrentTime() {
  static char timeStr[6];
  static int lastExecutedMinute = -1;

  DateTime now = rtc.now();
  currentMinutes = now.minute();
  int currentSeconds = now.second();
  unsigned long currentMillis = millis();

  Serial.print("Minutes: ");
  Serial.print(currentMinutes);
  Serial.print(" | Seconds: ");
  Serial.println(currentSeconds);

  if (currentMinutes % 2 == 0 && currentMinutes != lastExecutedMinute) {
    isLight_running = true;
    isFood_running = true;

    prevTime_light = currentMillis;
    prevTime_food = currentMillis;

    uvlightON();
    waterON();
    feederON();

    Serial.println("Light and Food ON");

    lastExecutedMinute = currentMinutes;
  }

  if (isLight_running && (currentMillis - prevTime_light >= intervalLight)) {
    uvlightOFF();
    isLight_running = false;
    Serial.println("Light OFF after 1 min");
  }

  if (isFood_running && (currentMillis - prevTime_food >= intervalFood)) {
    waterOFF();
    feederOFF();
    isFood_running = false;
    Serial.println("Food OFF after 10 sec");
  }

  sprintf(timeStr, "%02d:%02d", currentMinutes, currentSeconds);
  return timeStr;
}

int getMins() {
  return currentMinutes;
}
