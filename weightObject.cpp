#include "weightObject.h"
#include "HX711.h"

#define DT_PIN 3
#define SCK_PIN 4

float calibration_factor = -7050;  

HX711 scale;

void initWeight() {
// Start Load Cell
  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale(calibration_factor); // Set calibration factor
  scale.tare(); // Reset scale to 0
}

float getWeightValue() {
  float weight = scale.get_units(10); // Read weight in grams
  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println(" g");
  if(weight < 0 ) weight = 0.00;
  return weight;
}
