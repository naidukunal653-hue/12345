/*
  ============================================
  LYMPHIX - Load Cell Module
  ============================================

  Hardware:
  ESP32
  Load Cell
  HX711

  HX711:
  DOUT -> GPIO 4
  SCK  -> GPIO 5

  Output:
  Time
  Raw ADC
  Load (grams)
  Load (kg)
*/

#include "HX711.h"

// -----------------------------
// HX711 pins
// -----------------------------
#define HX711_DOUT 4
#define HX711_SCK  5

HX711 scale;

// -----------------------------
// IMPORTANT
// Replace this with YOUR
// calibration factor.
// -----------------------------
float CALIBRATION_FACTOR = 210.865005;

// -----------------------------
// Sampling interval
// -----------------------------
const unsigned long SAMPLE_INTERVAL = 1000;

unsigned long lastSampleTime = 0;


void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("========================================");
  Serial.println("       LYMPHIX LOAD CELL MODULE");
  Serial.println("========================================");

  // Start HX711
  scale.begin(HX711_DOUT, HX711_SCK);

  if (!scale.is_ready()) {

    Serial.println("ERROR: HX711 NOT DETECTED");

    while (1) {
      delay(1000);
    }
  }

  Serial.println("HX711: CONNECTED");

  // Apply calibration
  scale.set_scale(CALIBRATION_FACTOR);

  Serial.println("Calibration factor applied.");

  // -----------------------------
  // Tare
  // -----------------------------

  Serial.println();
  Serial.println("Remove all weight.");
  Serial.println("Taring...");

  delay(3000);

  scale.tare(20);

  Serial.println("TARE COMPLETE.");

  Serial.println();
  Serial.println("LYMPHIX LOAD MONITOR READY.");
  Serial.println();

  // CSV header
  Serial.println("time_ms,raw,load_g,load_kg");
}


void loop() {

  unsigned long currentTime = millis();

  if (currentTime - lastSampleTime >= SAMPLE_INTERVAL) {

    lastSampleTime = currentTime;

    if (scale.is_ready()) {

      // Raw HX711 value
      long rawValue = scale.read();

      // Calibrated measurement
      float loadGrams = scale.get_units(10);

      // Convert grams → kilograms
      float loadKg = loadGrams / 1000.0;

      // -----------------------------
      // CSV output
      // -----------------------------

      Serial.print(currentTime);
      Serial.print(",");

      Serial.print(rawValue);
      Serial.print(",");

      Serial.print(loadGrams, 2);
      Serial.print(",");

      Serial.println(loadKg, 4);

    }
    else {

      Serial.println("ERROR,HX711_NOT_READY");

    }
  }
}