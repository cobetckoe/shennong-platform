/**
 * 执行器驱动 - 风扇 (实现)
 */

#include "fan.h"

void fan_init(fan_t *dev) {
    hal_gpio_init(dev->port, dev->pin, HAL_GPIO_OUTPUT_PP);
    fan_off(dev);
}

void fan_on(fan_t *dev) {
    hal_gpio_write(dev->port, dev->pin, HAL_GPIO_HIGH);
}

void fan_off(fan_t *dev) {
    hal_gpio_write(dev->port, dev->pin, HAL_GPIO_LOW);
}

void fan_control(fan_t *dev, uint8_t enable) {
    enable ? fan_on(dev) : fan_off(dev);
}
