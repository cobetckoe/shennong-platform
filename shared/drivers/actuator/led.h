/**
 * 执行器驱动 - LED补光灯
 */

#ifndef LED_H
#define LED_H

#include "hal/gpio.h"

typedef struct {
    uint8_t port;
    uint8_t pin;
} led_t;

void led_init(led_t *dev);
void led_on(led_t *dev);
void led_off(led_t *dev);
void led_control(led_t *dev, uint8_t enable);

#endif
