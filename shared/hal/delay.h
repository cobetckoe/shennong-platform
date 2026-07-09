/**
 * 硬件抽象层 - 延时
 */

#ifndef HAL_DELAY_H
#define HAL_DELAY_H

#include <stdint.h>

void hal_init(void);
void hal_delay_ms(uint32_t ms);
uint32_t hal_tick_ms(void);

#endif
