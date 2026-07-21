/**
 * 硬件抽象层 - GPIO
 *
 * 屏蔽STM32寄存器操作，提供统一的GPIO接口。
 * 移植到其他MCU只需修改此文件。
 */

#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>

typedef enum {
    HAL_GPIO_LOW = 0,
    HAL_GPIO_HIGH = 1
} hal_gpio_state_t;

typedef enum {
    HAL_GPIO_OUTPUT_PP,
    HAL_GPIO_INPUT_PULLUP,
    HAL_GPIO_INPUT_FLOATING
} hal_gpio_mode_t;

/**
 * 初始化引脚
 * @param port  端口号 (0=PA, 1=PB, ...)
 * @param pin   引脚号 (0-15)
 * @param mode  模式
 */
void hal_gpio_init(uint8_t port, uint8_t pin, hal_gpio_mode_t mode);

/**
 * 输出高低电平
 */
void hal_gpio_write(uint8_t port, uint8_t pin, hal_gpio_state_t state);

/**
 * 读取输入电平
 */
hal_gpio_state_t hal_gpio_read(uint8_t port, uint8_t pin);

#endif
