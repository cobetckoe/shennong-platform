/**
 * 硬件抽象层 - I2C实现 (STM32F103)
 */

#include "i2c.h"
#include "stm32f1xx_hal.h"

static I2C_HandleTypeDef hi2c_list[2];

void hal_i2c_init(hal_i2c_port_t port, uint32_t speed) {
    I2C_HandleTypeDef *h = &hi2c_list[port];

    if (port == HAL_I2C_1) {
        __HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitTypeDef g = {0};
        g.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        g.Mode = GPIO_MODE_AF_OD;
        g.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &g);
        h->Instance = I2C1;
    } else {
        __HAL_RCC_I2C2_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitTypeDef g = {0};
        g.Pin = GPIO_PIN_10 | GPIO_PIN_11;
        g.Mode = GPIO_MODE_AF_OD;
        g.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &g);
        h->Instance = I2C2;
    }

    h->Init.ClockSpeed = speed;
    h->Init.DutyCycle = I2C_DUTYCYCLE_2;
    h->Init.OwnAddress1 = 0;
    h->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    h->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    h->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    h->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(h);
}

uint8_t hal_i2c_read_byte(hal_i2c_port_t port, uint8_t addr, uint8_t reg) {
    uint8_t val = 0;
    HAL_I2C_Mem_Read(&hi2c_list[port], addr << 1, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
    return val;
}

void hal_i2c_write_byte(hal_i2c_port_t port, uint8_t addr, uint8_t reg, uint8_t val) {
    HAL_I2C_Mem_Write(&hi2c_list[port], addr << 1, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
}

uint16_t hal_i2c_read_word(hal_i2c_port_t port, uint8_t addr, uint8_t reg) {
    uint8_t buf[2] = {0};
    HAL_I2C_Mem_Read(&hi2c_list[port], addr << 1, reg, I2C_MEMADD_SIZE_8BIT, buf, 2, 100);
    return (buf[0] << 8) | buf[1];
}
