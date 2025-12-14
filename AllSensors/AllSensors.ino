#include "HX711.h"

// --- Load Cell Pin Configuration ---
#define DOUT 22
#define CLK 23
#define DOUT2 18
#define CLK2 19

// --- Analog Pin Configuration (ADC1) ---
// We use ADC1 pins because ADC2 pins cannot be used when WiFi is active.
#define ANALOG_PIN_1 32
#define ANALOG_PIN_2 33
#define ANALOG_PIN_3 34
#define ANALOG_PIN_4 35

HX711 scale;
HX711 scale2;

void setup() {
  Serial.begin(115200);
  Serial.println("HX711 & Analog Read Test");

  // Initialize HX711s
  scale.begin(DOUT, CLK);
  scale2.begin(DOUT2, CLK2);

  // Optional: Set analog read resolution (default is 12-bit: 0-4095)
  // analogReadResolution(12); 

  Serial.println("Taring scales... Remove any weight.");
  scale.tare();  // Reset scale to zero
  scale2.tare();
  
  delay(2000);
  Serial.println("Tare complete.");
}

void loop() {
  Serial.println("-----------------------------");

  // --- Load Cell 1 ---
  if (scale.is_ready()) {
    long reading = scale.read();
    Serial.print("Load Cell 1 Raw: ");
    Serial.println(reading);
  } else {
    Serial.println("Load Cell 1 not ready.");
  }

  // --- Load Cell 2 ---
  if (scale2.is_ready()) {
    long reading = scale2.read();
    Serial.print("Load Cell 2 Raw: ");
    Serial.println(reading);
  } else {
    Serial.println("Load Cell 2 not ready.");
  }

  // --- Analog Readings ---
  int val1 = analogRead(ANALOG_PIN_1);
  int val2 = analogRead(ANALOG_PIN_2);
  int val3 = analogRead(ANALOG_PIN_3);
  int val4 = analogRead(ANALOG_PIN_4);

  Serial.print("Analog 1 (Pin 32): "); Serial.println(val1);
  Serial.print("Analog 2 (Pin 33): "); Serial.println(val2);
  Serial.print("Analog 3 (Pin 34): "); Serial.println(val3);
  Serial.print("Analog 4 (Pin 35): "); Serial.println(val4);

  delay(500);
}