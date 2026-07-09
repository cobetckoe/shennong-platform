/**
 * 果菜种植机 - 硬件配置
 */

#ifndef DEV_FRUITCROP_CFG_H
#define DEV_FRUITCROP_CFG_H

#include "hal/gpio.h"
#include "hal/i2c.h"
#include "hal/adc.h"

#define DEV_ID          0x05
#define DEV_TYPE        0x05

#define SOIL_DHT_PORT   0
#define SOIL_DHT_PIN    10
#define LIGHT_I2C       0
#define LIGHT_ADDR      0x23
#define PH_ADC          0
#define PH_CH           0

#define PUMP_PORT       1
#define PUMP_PIN        0
#define FAN_PORT        1
#define FAN_PIN         1
#define LED_PORT        1
#define LED_PIN         2

#endif
