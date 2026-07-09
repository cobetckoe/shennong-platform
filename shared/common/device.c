/**
 * 设备公共框架 - 实现
 */

#include "device.h"
#include "config.h"
#include "hal/uart.h"
#include "hal/delay.h"
#include <string.h>

static control_rule_t rule = {0};
static sensor_data_t sensor;

void device_send(uint8_t *data, uint16_t len) {
    hal_uart_send(ZIGBEE_PORT, data, len);
}

control_rule_t* device_get_rule(void) {
    return &rule;
}

sensor_data_t* device_get_sensor(void) {
    return &sensor;
}

void device_init(uint8_t device_id) {
    hal_init();
    hal_uart_init(ZIGBEE_PORT, ZIGBEE_BAUD);
    (void)device_id;
}

void device_loop(uint8_t device_id) {
    uint32_t last_hb = 0, last_sensor = 0;

    while (1) {
        uint32_t now = hal_tick_ms();

        if (now - last_hb >= HB_INTERVAL) {
            uint8_t hb[9] = {CMD_HEARTBEAT, device_id};
            device_send(hb, 9);
            last_hb = now;
        }

        if (now - last_sensor >= SENSOR_INTERVAL) {
            read_sensors();
            uint8_t data[17] = {CMD_SENSOR_DATA, device_id};
            memcpy(&data[2], &sensor, sizeof(sensor));
            device_send(data, sizeof(sensor) + 2);
            execute();
            last_sensor = now;
        }

        uint8_t rx[64];
        if (hal_uart_recv(ZIGBEE_PORT, rx, sizeof(rx), UART_TIMEOUT)
            && rx[0] == CMD_CONTROL_RULE) {
            memcpy(&rule, &rx[1], sizeof(rule));
            execute();
        }

        hal_delay_ms(10);
    }
}
