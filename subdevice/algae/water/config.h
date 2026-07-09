/**
 * 藻类培养池 - 硬件配置
 */

#ifndef DEV_WATER_CFG_H
#define DEV_WATER_CFG_H

#include "hal/gpio.h"
#include "hal/i2c.h"
#include "hal/adc.h"

#define DEV_ID          0x30
#define DEV_TYPE        0x30

#define WATER_TEMP_PORT 0
#define WATER_TEMP_PIN  10      /* DS18B20防水 */
#define WATER_LIGHT_I2C 0
#define WATER_LIGHT_ADDR 0x23   /* BH1750 */
#define WATER_PH_ADC    0
#define WATER_PH_CH     2

#define AIR_PORT        1
#define AIR_PIN         0       /* 曝气泵 */
#define PUMP_PORT       1
#define PUMP_PIN        1       /* 循环泵 */
#define LED_PORT        1
#define LED_PIN         2       /* 补光灯 */

#endif
