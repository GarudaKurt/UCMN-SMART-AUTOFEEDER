#include "timer.h"
#include <RTClib.h>
#include <Arduino.h>

RTC_DS1307 rtc;

void initTimer() {
    // Check if RTC is connected
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
}

const char* getCurrentTime() {
  static char timeStr[6];  // Buffer to store formatted time (MM:SS)
  DateTime now = rtc.now();  // Get current time from RTC

  int currentMinutes = now.minute();
  int currentSeconds = now.second();

  Serial.print("Minutes: ");
  Serial.print(currentMinutes);
  Serial.print(" | Seconds: ");
  Serial.println(currentSeconds);

  // Format time as MM:SS
  sprintf(timeStr, "%02d:%02d", currentMinutes, currentSeconds);
  return timeStr;  // Return formatted string
}


bool acitivateConveyor() {
  DateTime now = rtc.now();  // Get current time from RTC
  int currentMinutes = now.minute();  // Extract minutes
  if(currentMinutes == 60)
    return true;
  return false;
}
