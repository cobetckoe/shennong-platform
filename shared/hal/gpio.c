/**
 * 硬件抽象层 - GPIO实现 (STM32F103)
 */

#include "gpio.h"
#include "stm32f1xx_hal.h"

static GPIO_TypeDef* get_port(uint8_t port) {
    switch (port) {
        case 0: return GPIOA;
        case 1: return GPIOB;
        case 2: return GPIOC;
        default: return GPIOA;
    }
}

void hal_gpio_init(uint8_t port, uint8_t pin, hal_gpio_mode_t mode) {
    GPIO_InitTypeDef g = {0};
    g.Pin = 1 << pin;
    switch (mode) {
        case HAL_GPIO_OUTPUT_PP:
            g.Mode = GPIO_MODE_OUTPUT_PP;
            g.Speed = GPIO_SPEED_FREQ_LOW;
            break;
        case HAL_GPIO_INPUT_PULLUP:
            g.Mode = GPIO_MODE_INPUT;
            g.Pull = GPIO_PULLUP;
            break;
        case HAL_GPIO_INPUT_FLOATING:
            g.Mode = GPIO_MODE_INPUT;
            g.Pull = GPIO_NOPULL;
            break;
    }
    HAL_GPIO_Init(get_port(port), &g);
}

void hal_gpio_write(uint8_t port, uint8_t pin, hal_gpio_state_t state) {
    HAL_GPIO_WritePin(get_port(port), 1 << pin,
        state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

hal_gpio_state_t hal_gpio_read(uint8_t port, uint8_t pin) {
    return HAL_GPIO_ReadPin(get_port(port), 1 << pin) ? HAL_GPIO_HIGH : HAL_GPIO_LOW;
}
