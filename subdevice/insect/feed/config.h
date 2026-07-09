/**
 * 饲虫养殖箱 - 硬件配置
 */

#ifndef DEV_FEED_CFG_H
#define DEV_FEED_CFG_H

#include "hal/gpio.h"

#define DEV_ID          0x40
#define DEV_TYPE        0x40

#define BOX_TEMP_PORT   0
#define BOX_TEMP_PIN    10      /* DS18B20 */
#define BOX_HUMI_PORT   0
#define BOX_HUMI_PIN    11      /* DHT22 */

#define FAN_PORT        1
#define FAN_PIN         1
#define HEATER_PORT     1
#define HEATER_PIN      2

#endif
