#include "display.h"
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
static bool sorted = false;

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
  String type = "";
  if (size >= 8.40 && size < 9) {
    Serial.println("SMALL");
    type = "SMALL";
    cntS++;
  } else if (size >= 9.35 && size < 10) {
    Serial.println("MEDIUM");
    type = "MEDIUM";
    cntM++;
  } else if(size >= 10  && size < 11.00) {
    Serial.println("LARGE");
    type = "LARGE";
    cntL++;
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

