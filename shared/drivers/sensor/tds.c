/**
 * 传感器驱动 - TDS 水质检测 (实现)
 */

#include "tds.h"

#define TDS_SAMPLES     30
#define TDS_VOLTAGE_REF 5.0f
#define TDS_ADC_MAX     1023.0f

int tds_read_ppm(tds_t *dev) {
    uint32_t sum = 0;
    
    /* 多次采样取平均 */
    for (int i = 0; i < TDS_SAMPLES; i++) {
        sum += hal_adc_read(dev->port, dev->channel);
    }
    
    float voltage = (sum / (float)TDS_SAMPLES) * TDS_VOLTAGE_REF / TDS_ADC_MAX;
    
    /* 温度补偿 */
    float compensation = 1.0f + 0.02f * (dev->temperature - 25.0f);
    if (compensation <= 0) compensation = 1.0f;
    voltage /= compensation;
    
    /* 电压转TDS值 */
    int tds = (int)(voltage * 500.0f);
    
    /* 限制范围 */
    if (tds < 0) tds = 0;
    if (tds > 1000) tds = 1000;
    
    return tds;
}