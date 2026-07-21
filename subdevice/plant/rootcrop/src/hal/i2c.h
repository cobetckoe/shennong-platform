/**
 * 硬件抽象层 - I2C
 */

#ifndef HAL_I2C_H
#define HAL_I2C_H

#include <stdint.h>

typedef enum {
    HAL_I2C_1 = 0,
    HAL_I2C_2 = 1
} hal_i2c_port_t;

void hal_i2c_init(hal_i2c_port_t port, uint32_t speed);
uint8_t hal_i2c_read_byte(hal_i2c_port_t port, uint8_t addr, uint8_t reg);
void hal_i2c_write_byte(hal_i2c_port_t port, uint8_t addr, uint8_t reg, uint8_t val);
uint16_t hal_i2c_read_word(hal_i2c_port_t port, uint8_t addr, uint8_t reg);

#endif
