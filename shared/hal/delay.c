/**
 * 硬件抽象层 - 延时实现 (STM32F103)
 */

#include "delay.h"
#include "stm32f1xx_hal.h"

void hal_init(void) {
    HAL_Init();
}

void hal_delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}

uint32_t hal_tick_ms(void) {
    return HAL_GetTick();
}
