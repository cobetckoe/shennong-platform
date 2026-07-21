/**
 * 神农 T5 网关 - Zigbee 子设备管理器
 */

#ifndef ZIGBEE_MANAGER_H
#define ZIGBEE_MANAGER_H

#include "protocols.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t zigbee_id;
    uint8_t dev_type;
    char name[32];
    uint8_t subdev_id;
    bool online;
    bool registered;
    sensor_data_t last_sensor;
    uint8_t last_relay;
    uint32_t last_seen;
} device_info_t;

typedef void (*zigbee_rx_cb_t)(uint8_t dev_id, uint8_t cmd, const uint8_t *data, uint16_t len);

void zigbee_manager_init(zigbee_rx_cb_t cb);
void zigbee_manager_loop(void);

void zigbee_manager_on_heartbeat(uint8_t dev_id, const uint8_t *data, uint16_t len);
void zigbee_manager_on_sensor_data(uint8_t dev_id, const uint8_t *data, uint16_t len);
void zigbee_manager_on_manual_cmd(const uint8_t *data, uint16_t len);

void zigbee_manager_send_relay(uint8_t dev_id, uint8_t mask, uint8_t state);
void zigbee_manager_send_rule(uint8_t dev_id, const control_rule_t *rule);
void zigbee_manager_send_scene(uint8_t dev_id, const subdev_scene_t *scene);

/* 外部 Zigbee 设备控制 (涂鸦灯具/开关/传感器等) */
bool zigbee_manager_send_ext_dp(uint16_t ext_addr, uint8_t ep,
                                 uint8_t dpid, uint8_t type, uint32_t value);
bool zigbee_manager_control_ext_switch(uint16_t ext_addr, uint8_t ep, bool on);
bool zigbee_manager_control_ext_brightness(uint16_t ext_addr, uint8_t ep, uint8_t brightness);
bool zigbee_manager_control_ext_color_temp(uint16_t ext_addr, uint8_t ep, uint16_t temp);

void zigbee_manager_report_all(void);
void zigbee_manager_report_sensor(uint8_t dev_id);
void zigbee_manager_report_disease(uint8_t dev_id, const void *result);
void zigbee_manager_update_image_status(uint8_t dev_id, const void *result);

const sensor_data_t* zigbee_manager_get_latest_sensor(uint8_t dev_id);
const device_info_t* zigbee_manager_get_device(uint8_t index);
uint8_t zigbee_manager_get_relay(uint8_t dev_id);
uint8_t zigbee_manager_get_online_count(void);
uint8_t zigbee_manager_get_total_count(void);

#endif
