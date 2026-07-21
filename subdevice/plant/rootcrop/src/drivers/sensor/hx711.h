/**
 * 传感器驱动 - HX711 称重
 */

#ifndef HX711_H
#define HX711_H

#include "hal/gpio.h"

typedef struct {
    uint8_t sck_port;
    uint8_t sck_pin;
    uint8_t dout_port;
    uint8_t dout_pin;
    float scale_factor;     /* 校准系数 */
    float offset;           /* 零点偏移 */
} hx711_t;

float hx711_read(hx711_t *dev);

#endif
