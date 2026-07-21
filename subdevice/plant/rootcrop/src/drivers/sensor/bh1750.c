/**
 * 传感器驱动 - BH1750 I2C光照 (实现)
 */

#include "bh1750.h"

#define BH1750_POWER_ON    0x01
#define BH1750_RESET       0x07
#define BH1750_CONT_H_RES  0x10

void bh1750_init(bh1750_t *dev) {
    hal_i2c_write_byte(dev->i2c, dev->addr, 0x00, BH1750_POWER_ON);
    hal_i2c_write_byte(dev->i2c, dev->addr, 0x00, BH1750_CONT_H_RES);
}

float bh1750_read(bh1750_t *dev) {
    uint16_t raw = hal_i2c_read_word(dev->i2c, dev->addr, BH1750_CONT_H_RES);
    return (float)raw / 1.2f;   /* 转换为lux */
}
