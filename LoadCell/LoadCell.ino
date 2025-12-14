#include "HX711.h"

 

// Pin configuration

#define DOUT 22
#define CLK 23
#define DOUT2 18
#define CLK2 19


 

HX711 scale;
HX711 scale2;

 

void setup() {

  Serial.begin(115200);
  Serial.println("HX711 Load Cell Test");

 

  // Initialize HX711

  scale.begin(DOUT, CLK);
  scale2.begin(DOUT2, CLK2);

 

  // Optional: Tare the scale (zero it)

  Serial.println("Taring... Remove any weight.");

  scale.tare();  // Reset scale to zero
  scale2.tare(); 

  delay(2000);

  Serial.println("Tare complete.");

}

 

void loop() {

  // Check if HX711 is ready

  if (scale.is_ready()) {

    long reading = scale.read();  // Raw ADC reading

    Serial.print("Raw reading: ");

    Serial.println(reading);

  } else {

    Serial.println("HX711 not ready.");

  }

    if (scale2.is_ready()) {

    long reading = scale2.read();  // Raw ADC reading

    Serial.print("Raw reading2: ");

    Serial.println(reading);

  } else {

    Serial.println("HX711 not ready.");

  }

 

  delay(500);

}

