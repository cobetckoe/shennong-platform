#include <Arduino.h>

#define DEVICE_TYPE 0x05
#define NODE_ID 0x15
#define SENSOR_INTERVAL 5000

#define MESH_RX_PIN 16
#define MESH_TX_PIN 17
HardwareSerial MeshSerial(2);

unsigned long lastSensorTime = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  MeshSerial.begin(115200, SERIAL_8N1, MESH_RX_PIN, MESH_TX_PIN);
  Serial.println("Mushroom culture device started.");
}

void loop() {
  if (millis() - lastSensorTime > SENSOR_INTERVAL) {
    Serial.println("[INFO] collect mushroom environment data");
    lastSensorTime = millis();
  }
}
