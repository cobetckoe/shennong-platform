/**
 * 蚯蚓养殖箱 - 硬件配置
 */

#ifndef DEV_SOIL_CFG_H
#define DEV_SOIL_CFG_H

#include "hal/gpio.h"
#include "hal/adc.h"

#define DEV_ID          0x11
#define DEV_TYPE        0x11

#define SUB_TEMP_PORT   0
#define SUB_TEMP_PIN    10      /* DS18B20 */
#define SUB_PH_ADC      0
#define SUB_PH_CH       2       /* pH ADC */

#define SPRAY_PORT      1
#define SPRAY_PIN       0
#define FAN_PORT        1
#define FAN_PIN         1

#endif
