#include <Arduino.h>

// 配置
#define DEVICE_TYPE 0x05  // 菌类种植机
#define NODE_ID 0x15
#define SENSOR_INTERVAL 5000

// 立创地阔星开发板（STM32F103C8）
// 使用Serial1作为Meshtastic通信串口
HardwareSerial &MeshSerial = Serial1;

unsigned long lastSensorTime = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  MeshSerial.begin(115200);
  
  Serial.println("Fungi planter started.");
  Serial.printf("Device Type: 0x%02X, Node ID: 0x%02X\n", DEVICE_TYPE, NODE_ID);
}

void loop() {
  if (millis() - lastSensorTime > SENSOR_INTERVAL) {
    Serial.println("[INFO] Collecting environment data...");
    lastSensorTime = millis();
  }
}