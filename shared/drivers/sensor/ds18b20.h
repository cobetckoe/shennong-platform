/**
 * 传感器驱动 - DS18B20 单总线温度
 */

#ifndef DS18B20_H
#define DS18B20_H

#include "hal/gpio.h"

typedef struct {
    uint8_t port;
    uint8_t pin;
} ds18b20_t;

float ds18b20_read(ds18b20_t *dev);

#endif
