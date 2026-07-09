/**
 * 传感器驱动 - pH模拟量 (实现)
 */

#include "ph_sensor.h"

#define PH_VOLTAGE_RANGE   3.3f
#define PH_ADC_RESOLUTION  4096.0f
#define PH_NEUTRAL_MV      2500.0f     /* pH7对应电压 */

float ph_read(ph_sensor_t *dev) {
    uint16_t raw = hal_adc_read(dev->adc, dev->channel);
    float mv = (float)raw / PH_ADC_RESOLUTION * PH_VOLTAGE_RANGE * 1000.0f;
    float ph = 7.0f + (mv - PH_NEUTRAL_MV) / (-59.16f);  /* 能斯特方程简化 */
    return ph * dev->scale + dev->offset;
}
