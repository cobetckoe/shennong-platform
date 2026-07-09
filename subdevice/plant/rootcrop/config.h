/**
 * 根菜种植机 - 硬件配置
 *
 * 在此定义引脚和硬件实例，main.cpp只做组装。
 * 修改硬件接线只需改这个文件。
 */

#ifndef DEV_ROOTCROP_CFG_H
#define DEV_ROOTCROP_CFG_H

#include "hal/gpio.h"
#include "hal/i2c.h"
#include "hal/adc.h"

/* 设备标识 */
#define DEV_ID          0x01
#define DEV_TYPE        0x01

/* 传感器引脚 */
#define SOIL_DHT_PORT   0       /* PA */
#define SOIL_DHT_PIN    10      /* PA10 温湿度 */
#define LIGHT_I2C       0       /* I2C1 */
#define LIGHT_ADDR      0x23    /* BH1750地址 */
#define PH_ADC          0       /* ADC1 */
#define PH_CH           0       /* CH0 pH */

/* 执行器引脚 */
#define PUMP_PORT       1       /* PB */
#define PUMP_PIN        0       /* PB0 水泵 */
#define FAN_PORT        1       /* PB */
#define FAN_PIN         1       /* PB1 风扇 */
#define LED_PORT        1       /* PB */
#define LED_PIN         2       /* PB2 补光灯 */

#endif
