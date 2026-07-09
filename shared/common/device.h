/**
 * 设备公共框架
 *
 * 所有子设备共用的主循环和通信逻辑。
 * 设备只需声明硬件配置并实现采集/控制函数。
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>
#include "protocols.h"

/* 设备必须实现的两个函数 */
void read_sensors(void);
void execute(void);

/* 公共API */
void device_init(uint8_t device_id);
void device_loop(uint8_t device_id);
void device_send(uint8_t *data, uint16_t len);
control_rule_t* device_get_rule(void);
sensor_data_t* device_get_sensor(void);

#endif
