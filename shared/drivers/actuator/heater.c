/**
 * 执行器驱动 - 加热片 (实现)
 */

#include "heater.h"

void heater_init(heater_t *dev) {
    hal_gpio_init(dev->port, dev->pin, HAL_GPIO_OUTPUT_PP);
    heater_off(dev);
}

void heater_on(heater_t *dev) {
    hal_gpio_write(dev->port, dev->pin, HAL_GPIO_HIGH);
}

void heater_off(heater_t *dev) {
    hal_gpio_write(dev->port, dev->pin, HAL_GPIO_LOW);
}

void heater_control(heater_t *dev, uint8_t enable) {
    enable ? heater_on(dev) : heater_off(dev);
}
