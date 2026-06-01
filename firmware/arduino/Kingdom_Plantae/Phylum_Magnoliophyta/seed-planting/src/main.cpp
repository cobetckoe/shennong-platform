#include <Arduino.h>

// 配置
#define DEVICE_TYPE 0x01  // 种子种植机
#define NODE_ID 0x10
#define SENSOR_INTERVAL 5000

// UART
#define MESH_RX_PIN 16
#define MESH_TX_PIN 17
HardwareSerial MeshSerial(2);

void setup() {
  Serial.begin(115200);
  delay(2000);
  MeshSerial.begin(115200, SERIAL_8N1, MESH_RX_PIN, MESH_TX_PIN);
  
  Serial.println("Seed planting device started.");
  Serial.printf("Device Type: 0x%02X, Node ID: 0x%02X\n", DEVICE_TYPE, NODE_ID);
}

void loop() {
  // 传感器采集和数据上报逻辑
  delay(1000);
}
