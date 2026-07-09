/**
 * 执行器驱动 - LED补光灯 (实现)
 */

#include "led.h"

void led_init(led_t *dev) {
    hal_gpio_init(dev->port, dev->pin, HAL_GPIO_OUTPUT_PP);
    led_off(dev);
}

void led_on(led_t *dev) {
    hal_gpio_write(dev->port, dev->pin, HAL_GPIO_HIGH);
}

void led_off(led_t *dev) {
    hal_gpio_write(dev->port, dev->pin, HAL_GPIO_LOW);
}

void led_control(led_t *dev, uint8_t enable) {
    enable ? led_on(dev) : led_off(dev);
}
