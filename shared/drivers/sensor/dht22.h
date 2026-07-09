/**
 * 传感器驱动 - DHT22 温湿度
 */

#ifndef DHT22_H
#define DHT22_H

#include "hal/gpio.h"

typedef struct {
    uint8_t port;
    uint8_t pin;
} dht22_t;

float dht22_read_temp(dht22_t *dev);
float dht22_read_humi(dht22_t *dev);

#endif
