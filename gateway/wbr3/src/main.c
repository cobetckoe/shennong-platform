/**
 * Shennong Platform - WBR3 Gateway
 * 
 * Hardware: WBR3 (ESP32-C3) + ZS3L (Zigbee)
 * SDK: TuyaOS
 * Function: WiFi + Zigbee bridge
 */

#include "tuya_iot.h"
#include <string.h>

// Product config
#define PRODUCT_KEY     "your_product_key"
#define PRODUCT_SECRET  "your_product_secret"
#define DEVICE_NAME     "shennong_gateway"
#define DEVICE_SECRET   "your_device_secret"

// UART config (ZS3L)
#define ZIGBEE_UART     1
#define ZIGBEE_BAUD     115200

// DP IDs
#define DP_SENSOR       1
#define DP_RULE         2

// Control rule
typedef struct {
    uint8_t rule_id;
    uint8_t device_type;
    uint8_t species_id;
    float temp_high, temp_low;
    float humidity_high, humidity_low;
    uint8_t spray, fan, led;
} __attribute__((packed)) rule_t;

// Sub device
typedef struct {
    uint8_t id;
    uint8_t addr[8];
    bool online;
    uint32_t last_heartbeat;
} device_t;

static device_t devices[64];
static rule_t rules[16];
static uint8_t device_count = 0;
static uint8_t rule_count = 0;

// DP command callback
static void dp_cmd_cb(uint8_t dp_id, uint8_t *data, uint16_t len) {
    if (dp_id == DP_RULE && len >= sizeof(rule_t)) {
        rule_t rule;
        memcpy(&rule, data, sizeof(rule));
        
        // Store rule
        bool found = false;
        for (int i = 0; i < rule_count; i++) {
            if (rules[i].rule_id == rule.rule_id) {
                rules[i] = rule;
                found = true;
                break;
            }
        }
        if (!found && rule_count < 16) rules[rule_count++] = rule;
        
        // Forward to devices
        uint8_t tx[32] = {0x02};
        memcpy(&tx[1], &rule, sizeof(rule));
        tuya_uart_send(ZIGBEE_UART, tx, sizeof(rule) + 1);
    }
}

// UART receive callback
static void uart_rx(uint8_t *data, uint16_t len) {
    if (len < 1) return;
    
    if (data[0] == 0x01 && len >= 9) {  // Heartbeat
        uint8_t id = data[1];
        bool found = false;
        for (int i = 0; i < device_count; i++) {
            if (devices[i].id == id) {
                memcpy(devices[i].addr, &data[2], 8);
                devices[i].online = true;
                devices[i].last_heartbeat = tuya_system_get_millisecond();
                found = true;
                break;
            }
        }
        if (!found && device_count < 64) {
            devices[device_count].id = id;
            memcpy(devices[device_count].addr, &data[2], 8);
            devices[device_count].online = true;
            devices[device_count].last_heartbeat = tuya_system_get_millisecond();
            device_count++;
        }
    } else if (data[0] == 0x02 && len >= 17) {  // Sensor data
        tuya_iot_dp_report(DP_SENSOR, DP_TYPE_STRING, &data[1], len - 1);
    }
}

void app_main(void) {
    tuya_iot_config_t config = {
        .product_key = PRODUCT_KEY,
        .product_secret = PRODUCT_SECRET,
        .device_name = DEVICE_NAME,
        .device_secret = DEVICE_SECRET,
        .dp_cmd_cb = dp_cmd_cb,
    };
    tuya_iot_init(&config);
    tuya_uart_init(ZIGBEE_UART, ZIGBEE_BAUD, uart_rx);
    
    while (1) {
        tuya_iot_loop();
        
        uint32_t now = tuya_system_get_millisecond();
        for (int i = 0; i < device_count; i++) {
            if (devices[i].online && (now - devices[i].last_heartbeat > 60000)) {
                devices[i].online = false;
            }
        }
        
        tuya_system_sleep(10);
    }
}
