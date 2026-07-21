/**
 * 执行器驱动 - 风扇
 */

#ifndef FAN_H
#define FAN_H

#include "hal/gpio.h"

typedef struct {
    uint8_t port;
    uint8_t pin;
} fan_t;

void fan_init(fan_t *dev);
void fan_on(fan_t *dev);
void fan_off(fan_t *dev);
void fan_control(fan_t *dev, uint8_t enable);

#endif
