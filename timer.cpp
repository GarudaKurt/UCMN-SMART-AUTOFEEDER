#include "timer.h"
#include "conveyorControl.h"
#include "myservo.h"
#include <RTClib.h>
#include <Arduino.h>

RTC_DS1307 rtc;

#define relay_lightsUV 10
#define relay_lightsLamp 11
#define relay_water 12

bool conveyorActive = false;
bool uvLightActive = false;
bool waterActive = false;

unsigned long uvStartTime = 0;
unsigned long waterStartTime = 0;

const unsigned long uvRunDuration = 120000; // 2 minutes (120,000 ms)
const unsigned long uvCycleInterval = 300000; // 5 minutes (300,000 ms)
const unsigned long waterRunDuration = 10000; // 10 seconds (10,000 ms)
const unsigned long waterCycleInterval = 180000; // 3 minutes (180,000 ms)
const unsigned long conveyorRunDuration = 40000; // 40 seconds

void initTimer() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  pinMode(relay_lightsUV, OUTPUT);
  pinMode(relay_lightsLamp, OUTPUT);
  pinMode(relay_water, OUTPUT);

  digitalWrite(relay_lightsUV, HIGH);
  digitalWrite(relay_lightsLamp, HIGH);
  digitalWrite(relay_water, HIGH);
}

static void lampON() { digitalWrite(relay_lightsLamp, LOW); }
static void lampOFF() { digitalWrite(relay_lightsLamp, HIGH); }
static void uvlightON() { digitalWrite(relay_lightsUV, LOW); }
static void uvlightOFF() { digitalWrite(relay_lightsUV, HIGH); }
static void waterON() { digitalWrite(relay_water, LOW); }
static void waterOFF() { digitalWrite(relay_water, HIGH); }

const char* getCurrentTime() {
  static char timeStr[6];
  static int lastExecutedMinute = -1;
  static bool isConveyorRunning = false;

  static unsigned long conveyorStartTime = 0;  // Fix: Make it static
  static unsigned long uvStartTime = 0;
  static unsigned long waterStartTime = 0;

  DateTime now = rtc.now();
  int currentMinutes = now.minute();
  int currentSeconds = now.second();
  unsigned long currentMillis = millis();

  Serial.print("Minutes: ");
  Serial.print(currentMinutes);
  Serial.print(" | Seconds: ");
  Serial.println(currentSeconds);

  // UV Light Control
  if (!uvLightActive && (currentMillis - uvStartTime >= uvCycleInterval)) {
    Serial.println("Turning UV Light ON.");
    uvlightON();
    lampON();
    uvLightActive = true;
    uvStartTime = currentMillis;
  }
  if (uvLightActive && (currentMillis - uvStartTime >= uvRunDuration)) {
    Serial.println("Turning UV Light OFF.");
    uvlightOFF();
    lampOFF();
    uvLightActive = false;
  }

  // Water Control
  if (!waterActive && (currentMillis - waterStartTime >= waterCycleInterval)) {
    Serial.println("Turning Water ON.");
    feederON();
    waterON();
    waterActive = true;
    waterStartTime = currentMillis;
  }
  if (waterActive && (currentMillis - waterStartTime >= waterRunDuration)) {
    Serial.println("Turning Water OFF.");
    waterOFF();
    feederOFF();
    waterActive = false;
  }

  // Conveyor Control
  if (!isConveyorRunning && currentMinutes % 2 == 0 && currentMinutes != lastExecutedMinute) {
    Serial.println("Run conveyor!");
    runConveyor();
    isConveyorRunning = true;
    conveyorStartTime = currentMillis;
    lastExecutedMinute = currentMinutes;
  }
  if (isConveyorRunning && (currentMillis - conveyorStartTime >= conveyorRunDuration)) {
    Serial.println("Stopping conveyor.");
    conveyorStop();
    isConveyorRunning = false;
  }

  sprintf(timeStr, "%02d:%02d", currentMinutes, currentSeconds);
  return timeStr;
}
