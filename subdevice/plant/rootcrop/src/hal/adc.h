/**
 * 硬件抽象层 - ADC
 */

#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdint.h>

typedef enum {
    HAL_ADC_1 = 0
} hal_adc_port_t;

typedef enum {
    HAL_ADC_CH0  = 0,
    HAL_ADC_CH1  = 1,
    HAL_ADC_CH2  = 2,
    HAL_ADC_CH3  = 3,
    HAL_ADC_CH4  = 4,
    HAL_ADC_CH5  = 5,
    HAL_ADC_CH6  = 6,
    HAL_ADC_CH7  = 7,
    HAL_ADC_CH8  = 8,
    HAL_ADC_CH9  = 9,
    HAL_ADC_CH10 = 10,
    HAL_ADC_CH11 = 11,
    HAL_ADC_CH12 = 12,
    HAL_ADC_CH13 = 13,
    HAL_ADC_CH14 = 14,
    HAL_ADC_CH15 = 15
} hal_adc_channel_t;

void hal_adc_init(hal_adc_port_t port);
uint16_t hal_adc_read(hal_adc_port_t port, hal_adc_channel_t channel);

#endif
