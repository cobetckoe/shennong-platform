#include <Arduino.h>

// 配置
#define DEVICE_TYPE 0x02  // 块茎种植机
#define NODE_ID 0x12
#define SENSOR_INTERVAL 5000

// 立创地阔星开发板（STM32F103C8）
// 使用Serial1作为Meshtastic通信串口
HardwareSerial &MeshSerial = Serial1;

void setup() {
  Serial.begin(115200);
  delay(2000);
  MeshSerial.begin(115200);
  
  Serial.println("Tuber planter started.");
  Serial.printf("Device Type: 0x%02X, Node ID: 0x%02X\n", DEVICE_TYPE, NODE_ID);
}

void loop() {
  // 传感器采集和数据上报逻辑
  delay(SENSOR_INTERVAL);
}