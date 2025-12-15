#include "BluetoothSerial.h"
#include "HX711.h"

// Check if Bluetooth is properly enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled!
#endif

BluetoothSerial SerialBT;

//[cite_start]// Pin configuration for Load Cells [cite: 2]
#define DOUT 22
#define CLK 23
#define DOUT2 18
#define CLK2 19

// Pin configuration for Analog Reads
#define ANA_PIN_1 32
#define ANA_PIN_2 33
#define ANA_PIN_3 34
#define ANA_PIN_4 35

HX711 scale;
HX711 scale2;

void setup() {
  Serial.begin(115200);

  //[cite_start]// Bluetooth Serial setup [cite: 4]
  SerialBT.begin("ESP32_LoadCell_Sender"); 
  Serial.println("Bluetooth started! Pair with 'ESP32_LoadCell_Sender'");

  //[cite_start]// Initialize HX711 [cite: 5]
  Serial.println("Initializing Load Cells...");
  scale.begin(DOUT, CLK);
  scale2.begin(DOUT2, CLK2);

  //[cite_start]// Tare the scales [cite: 6]
  Serial.println("Taring... Remove any weight.");
  scale.tare();
  scale2.tare(); 
  Serial.println("Tare complete.");

  // NEW: Set attenuation to allow reading up to ~3.3V
  analogSetAttenuation(ADC_11db);
}

void loop() {
  long reading1 = 0;
  long reading2 = 0;
  int ana1 = 0;
  int ana2 = 0;
  int ana3 = 0;
  int ana4 = 0;

  bool ready1 = scale.is_ready();
  bool ready2 = scale2.is_ready();

  //[cite_start]// Read Sensor 1 [cite: 8]
  if (ready1) {
    reading1 = scale.read();
  } else {
    Serial.println("Scale 1 not ready");
  }

  //[cite_start]// Read Sensor 2 [cite: 10]
  if (ready2) {
    reading2 = scale2.read();
  } else {
    Serial.println("Scale 2 not ready");
  }

  // NEW: Read Analog Pins
  ana1 = analogRead(ANA_PIN_1);
  ana2 = analogRead(ANA_PIN_2);
  ana3 = analogRead(ANA_PIN_3);
  ana4 = analogRead(ANA_PIN_4);

  //[cite_start]// Send data via Bluetooth if a client is connected [cite: 12]
  if (SerialBT.hasClient()) {
    // Format: "load1,load2,ana1,ana2,ana3,ana4"
    SerialBT.print(reading1);
    SerialBT.print(",");
    SerialBT.print(reading2);
    SerialBT.print(",");
    SerialBT.print(ana1);
    SerialBT.print(",");
    SerialBT.print(ana2);
    SerialBT.print(",");
    SerialBT.print(ana3);
    SerialBT.print(",");
    SerialBT.println(ana4);
  }

  // Debug output to USB Serial
  Serial.print("Load: ");
  Serial.print(reading1); Serial.print(",");
  Serial.print(reading2);
  Serial.print(" | Analog: ");
  Serial.print(ana1); Serial.print(",");
  Serial.print(ana2); Serial.print(",");
  Serial.print(ana3); Serial.print(",");
  Serial.println(ana4);

  delay(500); //[cite_start]// Adjust delay as needed [cite: 14]
}