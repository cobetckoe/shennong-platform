/**
 * 传感器驱动 - HC-SR04 超声波测距 (实现)
 */

#include "ultrasonic.h"
#include "hal/delay.h"
#include "hal/gpio.h"

float ultrasonic_read_cm(ultrasonic_t *dev) {
    /* 初始化引脚 */
    hal_gpio_init(dev->port, dev->trig_pin, HAL_GPIO_OUTPUT_PP);
    hal_gpio_init(dev->port, dev->echo_pin, HAL_GPIO_INPUT_PULLUP);
    
    /* 发送Trig脉冲 (约10us) */
    hal_gpio_write(dev->port, dev->trig_pin, HAL_GPIO_LOW);
    hal_delay_ms(1);  /* 使用ms代替us，确保足够长 */
    hal_gpio_write(dev->port, dev->trig_pin, HAL_GPIO_HIGH);
    hal_delay_ms(1);
    hal_gpio_write(dev->port, dev->trig_pin, HAL_GPIO_LOW);
    
    /* 等待Echo高电平开始 */
    uint32_t start = hal_tick_ms();
    while (hal_gpio_read(dev->port, dev->echo_pin) == HAL_GPIO_LOW) {
        if (hal_tick_ms() - start > 30) return 0;
    }
    
    /* 测量高电平持续时间 */
    start = hal_tick_ms();
    while (hal_gpio_read(dev->port, dev->echo_pin) == HAL_GPIO_HIGH) {
        if (hal_tick_ms() - start > 30) return 0;
    }
    uint32_t duration = hal_tick_ms() - start;
    
    /* 计算距离: 声速340m/s = 0.34cm/ms, 往返除以2 */
    float distance = duration * 0.34f / 2.0f;
    
    /* 计算植物高度 */
    float height = dev->height_offset - distance;
    return (height > 0) ? height : 0;
}