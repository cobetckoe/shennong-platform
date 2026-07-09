/**
 * 执行器驱动 - 水泵/喷淋泵/雾化器/曝气泵
 *
 * 这些都是开关量控制的泵类，共用同一套驱动。
 * 通过设备配置区分用途。
 */

#ifndef PUMP_H
#define PUMP_H

#include "hal/gpio.h"

typedef struct {
    uint8_t port;
    uint8_t pin;
} pump_t;

void pump_init(pump_t *dev);
void pump_on(pump_t *dev);
void pump_off(pump_t *dev);
void pump_control(pump_t *dev, uint8_t enable);

#endif
