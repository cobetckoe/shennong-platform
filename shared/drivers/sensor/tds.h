/**
 * 传感器驱动 - TDS 水质检测 (ADC)
 */

#ifndef TDS_H
#define TDS_H

#include "hal/adc.h"

typedef struct {
    hal_adc_port_t port;
    hal_adc_channel_t channel;
    float temperature;  /* 温度补偿值 */
} tds_t;

int tds_read_ppm(tds_t *dev);

#endif