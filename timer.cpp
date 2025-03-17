#include "timer.h"
#include "conveyorControl.h"

#include <RTClib.h>
#include <Arduino.h>

RTC_DS1307 rtc;

#define relay_conveyor 24
#define relay_lights 25
#define relay_water 26
#define relay_feeder 27

#define activateConveyor 120000
#define activateWater  180000

bool conveyorActive = false;

void initTimer() {
    // Check if RTC is connected
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  pinMode(relay_conveyor, OUTPUT);
  pinMode(relay_lights, OUTPUT);
  pinMode(relay_water, OUTPUT);
  pinMode(relay_feeder, OUTPUT);

  digitalWrite(relay_conveyor, LOW);
  digitalWrite(relay_lights, LOW);
  digitalWrite(relay_water, LOW);
  digitalWrite(relay_feeder, LOW);
}

const char* getCurrentTime() {
  static char timeStr[6];
  static int lastExecutedMinute = -1;
  static bool isConveyorRunning = false;
  static unsigned long conveyorStartTime = 0;
  const unsigned long runDuration = 10000;

  DateTime now = rtc.now();
  int currentMinutes = now.minute();
  int currentSeconds = now.second();
  unsigned long currentMillis = millis();

  Serial.print("Minutes: ");
  Serial.print(currentMinutes);
  Serial.print(" | Seconds: ");
  Serial.println(currentSeconds);

  // We start conveyor every even minute
  if (!isConveyorRunning && currentMinutes % 2 == 0 && currentMinutes != lastExecutedMinute) {
    Serial.println("Starting conveyor.");
    runConveyor();
    isConveyorRunning = true;  // Mark conveyor as running
    conveyorStartTime = currentMillis; // Store start time
    lastExecutedMinute = currentMinutes; // We must prevent multiple triggers in the same minute
  }

  // We stop the conveyor after 10 seconds
  if (isConveyorRunning && (currentMillis - conveyorStartTime >= runDuration)) {
    Serial.println("Stopping conveyor.");
    conveyorStop(); 
    isConveyorRunning = false; // Reset state for next cycle
  } 

  sprintf(timeStr, "%02d:%02d", currentMinutes, currentSeconds);
  return timeStr;
}


/*
void activateSUPPLY() {
  static unsigned long lastConveyorActivation = 0;
  static unsigned long lastPumpActivation = 0;
  static unsigned long lastFeederActivation = 0;

  unsigned long currentMillis = millis();

  // Conveyor activation every 2 minutes (120,000 ms)
  if (currentMillis - lastConveyorActivation >= activateConveyor) {  
    lastConveyorActivation = currentMillis;
    Serial.println("Activate Conveyor");
    conveyorActive = true;
  }

  // Water pump activation every 3 minutes (180,000 ms)
  if (currentMillis - lastPumpActivation >= activateWater) {  
    lastPumpActivation = currentMillis;
    Serial.println("Activate Water pump!");
  }

  // Feeder activation at 6 AM and 5 PM (once per hour)
  DateTime now = rtc.now();
  int currentHour = now.hour();
  if ((currentHour == 6 || currentHour == 17) && lastFeederActivation != currentHour) {  
    lastFeederActivation = currentHour;
    Serial.println("Feeder Activate!");
  }
}*/

/*
void activateConveyor() {
  static int cnt = 0;
  static int lastCheckedHour = -1;
  static int lastFeederCheckedHour = -1; 

  DateTime now = rtc.now();
  int currentHour = now.hour();
  int currentMinutes = now.minute();

  Serial.print("Current hr: ");
  Serial.println(currentHour);
  // Conveyor activation (1hr)
  if (currentMinutes == 0 && currentHour != lastCheckedHour) {  
    lastCheckedHour = currentHour;
    cnt += 1;

    if (cnt % 2 == 0) {
      Serial.println("Activate Lights");
      digitalWrite(relay_water, HIGH);
    } else {
      Serial.println("Activate Conveyor");
      digitalWrite(relay_lights, HIGH);
      digitalWrite(relay_conveyor, HIGH);
    }
  }

  // Feeder activation (6 AM and 5 PM)
  if ((currentHour == 6 || currentHour == 17) && currentHour != lastFeederCheckedHour) {  
    lastFeederCheckedHour = currentHour;
    Serial.println("Feeder Activate!");
  }
}


*/

