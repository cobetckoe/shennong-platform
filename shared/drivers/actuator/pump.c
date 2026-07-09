/**
 * 执行器驱动 - 泵类 (实现)
 */

#include "pump.h"

void pump_init(pump_t *dev) {
    hal_gpio_init(dev->port, dev->pin, HAL_GPIO_OUTPUT_PP);
    pump_off(dev);
}

void pump_on(pump_t *dev) {
    hal_gpio_write(dev->port, dev->pin, HAL_GPIO_HIGH);
}

void pump_off(pump_t *dev) {
    hal_gpio_write(dev->port, dev->pin, HAL_GPIO_LOW);
}

void pump_control(pump_t *dev, uint8_t enable) {
    enable ? pump_on(dev) : pump_off(dev);
}
