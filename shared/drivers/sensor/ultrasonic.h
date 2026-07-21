/**
 * 传感器驱动 - HC-SR04 超声波测距
 */

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "hal/gpio.h"

typedef struct {
    uint8_t port;
    uint8_t trig_pin;
    uint8_t echo_pin;
    float height_offset;  /* 传感器安装高度偏移(cm) */
} ultrasonic_t;

float ultrasonic_read_cm(ultrasonic_t *dev);

#endif