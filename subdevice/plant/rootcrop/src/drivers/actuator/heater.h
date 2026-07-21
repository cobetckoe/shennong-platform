/**
 * 执行器驱动 - 加热片
 */

#ifndef HEATER_H
#define HEATER_H

#include "hal/gpio.h"

typedef struct {
    uint8_t port;
    uint8_t pin;
} heater_t;

void heater_init(heater_t *dev);
void heater_on(heater_t *dev);
void heater_off(heater_t *dev);
void heater_control(heater_t *dev, uint8_t enable);

#endif
