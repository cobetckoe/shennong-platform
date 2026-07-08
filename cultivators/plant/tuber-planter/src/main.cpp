/**
 * 神农平台 - 块茎种植机
 * 硬件：立创地阔星(STM32F103) + ZS3L
 */

#include <Arduino.h>
#include <Wire.h>

// 配置
#define DEVICE_ID       0x02
#define DEVICE_TYPE     0x02  // 块茎
#define ZIGBEE_BAUD     115200

// 控制规则
typedef struct {
    uint8_t rule_id, device_type;
    float temp_high, temp_low;
    float humidity_high, humidity_low;
    uint8_t spray, fan, led;
} __attribute__((packed)) rule_t;

// 传感器数据
typedef struct {
    float temperature, humidity, light, ph;
} __attribute__((packed)) sensor_t;

// GPIO定义（立创地阔星）
#define SPRAY_PIN   PB0
#define FAN_PIN     PB1
#define LED_PIN     PB2

static rule_t rule = {1, DEVICE_TYPE, 30, 20, 80, 50, 1, 1, 1};
static sensor_t sensor;
static HardwareSerial ZigbeeSerial(USART2);  // PA2/PA3

void setup() {
    Serial.begin(115200);
    ZigbeeSerial.begin(ZIGBEE_BAUD);
    
    pinMode(SPRAY_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    
    digitalWrite(SPRAY_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    
    Serial.println("Tuber Planter Started");
}

void readSensors() {
    // TODO: 实际传感器读取
    sensor.temperature = 25.0;
    sensor.humidity = 60.0;
    sensor.light = 5000.0;
    sensor.ph = 7.0;
}

void execute() {
    digitalWrite(FAN_PIN, sensor.temperature > rule.temp_high ? HIGH : LOW);
    digitalWrite(SPRAY_PIN, (sensor.humidity < rule.humidity_low && rule.spray) ? HIGH : LOW);
    digitalWrite(LED_PIN, rule.led ? HIGH : LOW);
}

void sendHeartbeat() {
    uint8_t hb[9] = {0x01, DEVICE_ID};
    ZigbeeSerial.write(hb, 9);
}

void sendSensorData() {
    uint8_t data[17] = {0x02, DEVICE_ID};
    memcpy(&data[2], &sensor, sizeof(sensor));
    ZigbeeSerial.write(data, sizeof(sensor) + 2);
}

void loop() {
    static uint32_t lastHeartbeat = 0, lastSensor = 0;
    uint32_t now = millis();
    
    // 30秒心跳
    if (now - lastHeartbeat >= 30000) {
        sendHeartbeat();
        lastHeartbeat = now;
    }
    
    // 5秒传感器数据
    if (now - lastSensor >= 5000) {
        readSensors();
        sendSensorData();
        execute();
        lastSensor = now;
    }
    
    // 接收控制规则
    if (ZigbeeSerial.available()) {
        uint8_t rx[64];
        int len = ZigbeeSerial.readBytes(rx, sizeof(rx));
        if (len > 0 && rx[0] == 0x02) {
            memcpy(&rule, &rx[1], sizeof(rule));
            execute();
        }
    }
    
    delay(10);
}
