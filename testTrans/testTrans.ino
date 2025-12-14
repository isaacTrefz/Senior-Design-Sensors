#include "BluetoothSerial.h"
#include "HX711.h"

// Check if Bluetooth is properly enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// Pin configuration for Load Cells
#define DOUT 23
#define CLK 22
#define DOUT2 19
#define CLK2 18

HX711 scale;
HX711 scale2;

void setup() {
  // Standard Serial for debugging via USB
  Serial.begin(115200);
  
  // Bluetooth Serial setup
  // Name: "ESP32_LoadCell_Sender"
  SerialBT.begin("ESP32_LoadCell_Sender"); 
  Serial.println("Bluetooth started! Pair with 'ESP32_LoadCell_Sender'");

  // Initialize HX711
  Serial.println("Initializing Load Cells...");
  scale.begin(DOUT, CLK);
  scale2.begin(DOUT2, CLK2);

  // Tare the scales (set to zero)
  Serial.println("Taring... Remove any weight.");
  scale.tare();
  scale2.tare(); 
  Serial.println("Tare complete.");
}

void loop() {
  long reading1 = 0;
  long reading2 = 0;
  bool ready1 = scale.is_ready();
  bool ready2 = scale2.is_ready();

  // Read Sensor 1
  if (ready1) {
    reading1 = scale.read();
  } else {
    Serial.println("Scale 1 not ready");
  }

  // Read Sensor 2
  if (ready2) {
    reading2 = scale2.read();
  } else {
    Serial.println("Scale 2 not ready");
  }

  // Send data via Bluetooth if a client is connected
  if (SerialBT.hasClient()) {
    // Format: "val1,val2\n"
    SerialBT.print(reading1);
    SerialBT.print(",");
    SerialBT.println(reading2);
  }

  // Debug output to USB Serial
  Serial.print("Sent (BT): ");
  Serial.print(reading1);
  Serial.print(",");
  Serial.println(reading2);

  delay(500); // Adjust delay as needed
}
