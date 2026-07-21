/**
 * 根菜种植机 - 硬件配置
 * 修改硬件接线只需改这个文件
 */

#ifndef DEV_ROOTCROP_CFG_H
#define DEV_ROOTCROP_CFG_H

#include "hal/gpio.h"
#include "hal/i2c.h"
#include "hal/adc.h"

#define SOIL_DHT_PORT   0
#define SOIL_DHT_PIN    10
#define LIGHT_I2C       HAL_I2C_1
#define LIGHT_ADDR      0x23
#define PH_ADC          HAL_ADC_1
#define PH_CH           HAL_ADC_CH0

#define PUMP_PORT       1
#define PUMP_PIN        0
#define FAN_PORT        1
#define FAN_PIN         1
#define LED_PORT        1
#define LED_PIN         2

#endif
