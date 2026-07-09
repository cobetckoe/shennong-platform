/**
 * 传感器驱动 - BH1750 I2C光照
 */

#ifndef BH1750_H
#define BH1750_H

#include "hal/i2c.h"

typedef struct {
    hal_i2c_port_t i2c;
    uint8_t addr;           /* I2C地址 (默认0x23) */
} bh1750_t;

float bh1750_read(bh1750_t *dev);

#endif
