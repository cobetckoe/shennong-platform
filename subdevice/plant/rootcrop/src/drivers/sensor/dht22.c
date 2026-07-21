/**
 * 传感器驱动 - DHT22 温湿度 (实现)
 */

#include "dht22.h"
#include "hal/delay.h"
#include "stm32f1xx_hal.h"

/* 单总线时序 */
static void set_output(dht22_t *dev) {
    GPIO_InitTypeDef g = {0};
    g.Pin = 1 << dev->pin;
    g.Mode = GPIO_MODE_OUTPUT_PP;
    g.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(dev->port == 0 ? GPIOA : GPIOB, &g);
}

static void set_input(dht22_t *dev) {
    GPIO_InitTypeDef g = {0};
    g.Pin = 1 << dev->pin;
    g.Mode = GPIO_MODE_INPUT;
    g.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(dev->port == 0 ? GPIOA : GPIOB, &g);
}

static GPIO_TypeDef* get_port(dht22_t *dev) {
    return dev->port == 0 ? GPIOA : GPIOB;
}

static uint8_t read_byte(dht22_t *dev) {
    uint8_t val = 0;
    for (int i = 0; i < 8; i++) {
        while (hal_gpio_read(dev->port, dev->pin) == HAL_GPIO_LOW);
        hal_delay_us(40);
        if (hal_gpio_read(dev->port, dev->pin) == HAL_GPIO_HIGH) {
            val |= (1 << (7 - i));
            while (hal_gpio_read(dev->port, dev->pin) == HAL_GPIO_HIGH);
        }
    }
    return val;
}

/* TODO: 实现完整的DHT22单总线协议 */
float dht22_read_temp(dht22_t *dev) {
    (void)dev;
    return 25.0f;
}

float dht22_read_humi(dht22_t *dev) {
    (void)dev;
    return 60.0f;
}
