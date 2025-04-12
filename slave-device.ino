#include <Arduino.h>
#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_NeoPixel.h>


#define LED_PIN  13
#define LED_COUNT 20

Adafruit_NeoPixel stripLight(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

AccelStepper stepper;
#define SLAVE_ADDRESS 8

bool isRunning = false;
unsigned long startTime = 0;
const unsigned long conveyorRunDuration = 60000;

void setup() {
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData); // I2C receive event
  stripLight.begin();
  stripLight.show();  

  stepper.setMaxSpeed(1000);
  stepper.setSpeed(500);
}

void loop() {
  if (isRunning) {
    if (millis() - startTime < conveyorRunDuration) {
      stepper.runSpeed();
      setColor(255,255,255);
      stripLight.show();
    } else {
      isRunning = false;
      setColor(0,0,0);
      stripLight.show();
      Serial.println("Stepper stopped after 1 minute");
    }
  }
}

void receiveData(int byteCount) {
  char buffer[16];
  int i = 0;
  while (Wire.available() && i < 15) {
    buffer[i++] = Wire.read();
  }
  buffer[i] = '\0';

  if (strcmp(buffer, "activate") == 0) {
    Serial.println("Received activate command");
    isRunning = true;
    startTime = millis();
  }
}

void setColor(int r, int g, int b) {
  for (int i = 0; i < stripLight.numPixels(); i++) {
    stripLight.setPixelColor(i, stripLight.Color(r, g, b));
  }
  stripLight.show();
}
