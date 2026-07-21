/**
 * 传感器驱动 - SGP30 VOC/CO2 (I2C)
 */

#ifndef SGP30_H
#define SGP30_H

#include "hal/i2c.h"

typedef struct {
    hal_i2c_port_t port;
    uint8_t addr;
    int16_t co2;
    int16_t tvoc;
} sgp30_t;

void sgp30_init(sgp30_t *dev);
uint8_t sgp30_measure(sgp30_t *dev);

#endif