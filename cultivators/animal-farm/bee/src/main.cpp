/**
 * Shennong - Bee Hive
 * Type: 0x30 (Bee)
 */

#include <Arduino.h>

#define DEVICE_ID       0x30
#define DEVICE_TYPE     0x30

typedef struct {
    uint8_t rule_id, device_type;
    float temp_high, temp_low;
    float humidity_high, humidity_low;
    uint8_t spray, fan, led;
} __attribute__((packed)) rule_t;

typedef struct {
    float temperature, humidity, light, ph;
} __attribute__((packed)) sensor_t;

#define SPRAY_PIN   PB0
#define FAN_PIN     PB1
#define LED_PIN     PB2

static rule_t rule = {1, DEVICE_TYPE, 35, 15, 80, 40, 0, 1, 0};
static sensor_t sensor;
static HardwareSerial ZigbeeSerial(USART2);

void setup() {
    Serial.begin(115200);
    ZigbeeSerial.begin(115200);
    pinMode(FAN_PIN, OUTPUT);
}

void readSensors() {
    sensor = {28.0, 60.0, 0.0, 0.0};
}

void execute() {
    digitalWrite(FAN_PIN, sensor.temperature > rule.temp_high ? HIGH : LOW);
}

void loop() {
    static uint32_t lastHb = 0, lastSensor = 0;
    uint32_t now = millis();
    
    if (now - lastHb >= 30000) {
        uint8_t hb[9] = {0x01, DEVICE_ID};
        ZigbeeSerial.write(hb, 9);
        lastHb = now;
    }
    
    if (now - lastSensor >= 5000) {
        readSensors();
        uint8_t data[17] = {0x02, DEVICE_ID};
        memcpy(&data[2], &sensor, sizeof(sensor));
        ZigbeeSerial.write(data, sizeof(sensor) + 2);
        execute();
        lastSensor = now;
    }
    
    if (ZigbeeSerial.available()) {
        uint8_t rx[64];
        if (ZigbeeSerial.readBytes(rx, sizeof(rx)) > 0 && rx[0] == 0x02) {
            memcpy(&rule, &rx[1], sizeof(rule));
            execute();
        }
    }
    
    delay(10);
}
