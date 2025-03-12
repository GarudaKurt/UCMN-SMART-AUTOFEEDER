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
  // Update counters based on size
  String type = ""; // Default type
  if (size >= 9.35 && size < 10) {
    Serial.println("SMALL");
    type = "SMALL";
    cntS++;
  } else if (size >= 10 && size <= 12.00) {
    Serial.println("MEDIUM");
    type = "MEDIUM";
    cntM++;
  } else if(size >= 12  && size < 14.00) {
    Serial.println("LARGE");
    type = "LARGE";
    cntL++;
  }

  // Update display without flickering
  lcd.setCursor(3, 0);
  lcd.print("Smart Poultry    ");  // Spaces to clear previous text

  lcd.setCursor(3, 1);
  lcd.print("Time: ");
  lcd.print(time);
  lcd.print("    ");  // Spaces to overwrite old time

  lcd.setCursor(1, 2);
  lcd.print("EGG TYPE: ");
  lcd.print(type);
  lcd.print("      ");  // Extra spaces to clear previous long text

  lcd.setCursor(0, 3);
  lcd.print("CNT: S:");
  lcd.print(cntS);
  lcd.print(" |M:");
  lcd.print(cntM);
  lcd.print(" |L:");
  lcd.print(cntL);
  lcd.print("   ");  // Extra spaces to clear old numbers
}

