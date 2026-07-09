/**
 * 蜂箱 - 硬件配置
 */

#ifndef DEV_POLLINATOR_CFG_H
#define DEV_POLLINATOR_CFG_H

#include "hal/gpio.h"

#define DEV_ID          0x10
#define DEV_TYPE        0x10

#define HIVE_TEMP_PORT  0
#define HIVE_TEMP_PIN   10      /* DS18B20 */
#define HIVE_HUMI_PORT  0
#define HIVE_HUMI_PIN   11      /* DHT22 */
#define HX711_SCK_PORT  0
#define HX711_SCK_PIN   5
#define HX711_DOUT_PORT 0
#define HX711_DOUT_PIN  6
#define IR_PORT         0
#define IR_PIN          7       /* 入口红外 */

#define FAN_PORT        1
#define FAN_PIN         1

#endif
