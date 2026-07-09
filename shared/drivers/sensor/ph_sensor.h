/**
 * 传感器驱动 - pH模拟量
 */

#ifndef PH_SENSOR_H
#define PH_SENSOR_H

#include "hal/adc.h"

typedef struct {
    hal_adc_port_t adc;
    hal_adc_channel_t channel;
    float offset;           /* pH偏移校准 */
    float scale;            /* pH斜率校准 */
} ph_sensor_t;

float ph_read(ph_sensor_t *dev);

#endif
