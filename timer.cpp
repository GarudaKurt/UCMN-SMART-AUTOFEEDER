#include "timer.h"
#include "conveyorControl.h"

#include <RTClib.h>
#include <Arduino.h>

RTC_DS1307 rtc;

#define relay_lightsUV 10
#define relay_lightsLamp 11
#define relay_water 12

bool conveyorActive = false;

void initTimer() {
    // Check if RTC is connected
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

static void lampON() {
  Serial.println("Light bulb ON!");
  digitalWrite(relay_lightsLamp, LOW);
}
static void lampOFF() {
  Serial.println("Light bulb OFF!");
  digitalWrite(relay_lightsLamp, HIGH);
}
static void uvlightON() {
  digitalWrite(relay_lightsUV, LOW);
}
static void uvlightOFF() {
  digitalWrite(relay_lightsUV, HIGH);
}
static void waterON() {
  digitalWrite(relay_water, LOW);
}
static void waterOFF() {
  digitalWrite(relay_water, HIGH);
}

const char* getCurrentTime() {
  static char timeStr[6];
  static int lastExecutedMinute = -1;
  static bool isConveyorRunning = false;
  static unsigned long conveyorStartTime = 0;
  const unsigned long runDuration = 40000; 

  static bool uvLightActive = false;
  static unsigned long uvStartTime = 0;
  const unsigned long uvRunDuration = 120000;

  DateTime now = rtc.now();
  int currentMinutes = now.minute();
  int currentSeconds = now.second();
  unsigned long currentMillis = millis();

  Serial.print("Minutes: ");
  Serial.print(currentMinutes);
  Serial.print(" | Seconds: ");
  Serial.println(currentSeconds);

  // Start UV light every 5 minutes
  if (!uvLightActive && currentMinutes % 5 == 0 && currentMinutes != lastExecutedMinute) {
    Serial.println("Turning UV Light ON.");
    uvlightON();
    waterON();
    uvLightActive = true;
    uvStartTime = currentMillis;
    lastExecutedMinute = currentMinutes;
  }

  if (!isConveyorRunning && currentMinutes % 2 == 0 && currentMinutes != lastExecutedMinute) {
    Serial.println("Starting conveyor.");
    runConveyor();
    lampON();
    isConveyorRunning = true;  
    conveyorStartTime = currentMillis; 
    lastExecutedMinute = currentMinutes; 
  // We stop the conveyor after 10 seconds
  if (isConveyorRunning && (currentMillis - conveyorStartTime >= runDuration)) {
    Serial.println("Stopping conveyor.");
    conveyorStop(); 
    lampOFF();
    isConveyorRunning = false;
  } 

  sprintf(timeStr, "%02d:%02d", currentMinutes, currentSeconds);
  return timeStr;
}
