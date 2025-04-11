#include "display.h"
#include "myservo.h"

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
static bool sorted = false;
unsigned long prevTime = 0;

static int cntS = 0;
static int cntM = 0;
static int cntL = 0;

void initDisplay() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(3, 2);
  lcd.print("Initialized..");
  delay(2000);
  lcd.clear();
}

void showDisplay(const char* time, float size) {
  static int step = 0;
  static unsigned long stateStart = 0;
  static String type = "";
  static bool newEgg = false;

  static unsigned long prevClassify = 0;
  if (millis() - prevClassify > 2000 && step == 0) {
    prevClassify = millis();
    if (size >= 8.40 && size < 9) {
      type = "SMALL";
      cntS++;
      newEgg = true;
    } else if (size >= 9.35 && size < 10) {
      type = "MEDIUM";
      cntM++;
      newEgg = true;
    } else if (size >= 10  && size < 11.00) {
      type = "LARGE";
      cntL++;
      newEgg = true;
    }

    if (newEgg) {
      stateStart = millis();
      step = 1;
      servoStop();
    }
  }

  switch (step) {
    case 1:
      if (millis() - stateStart >= 2000) {
        resetStop();
        if (type == "SMALL") {
          servoSorting(90);
        } else if (type == "MEDIUM") {
          servoSorting(45);
        } else if (type == "LARGE") {
          servoSorting(130);
        }
        servoDischarge();
        stateStart = millis();
        step = 2;
      }
      break;
    case 2:
      if (millis() - stateStart >= 3000) {
        resetSorting();
        resetDischarge();
        step = 0;
        newEgg = false;
        type = "";
      }
      break;
  }

  lcd.setCursor(3, 0);
  lcd.print("Smart Poultry    "); 

  lcd.setCursor(3, 1);
  lcd.print("Time: ");
  lcd.print(time);
  lcd.print("    ");  

  lcd.setCursor(1, 2);
  lcd.print("EGG TYPE: ");
  lcd.print(type);
  lcd.print("      ");

  lcd.setCursor(0, 3);
  lcd.print("CNT: SM:");
  lcd.print(cntS);
  lcd.print(" MD:");
  lcd.print(cntM);
  lcd.print(" LG:");
  lcd.print(cntL);
  lcd.print("   ");  
}
