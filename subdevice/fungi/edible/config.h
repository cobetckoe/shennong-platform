/**
 * 食用菌培养室 - 硬件配置
 */

#ifndef DEV_EDIBLE_CFG_H
#define DEV_EDIBLE_CFG_H

#include "hal/gpio.h"
#include "hal/i2c.h"

#define DEV_ID          0x20
#define DEV_TYPE        0x20

#define ROOM_TEMP_PORT  0
#define ROOM_TEMP_PIN   10      /* DS18B20 */
#define ROOM_HUMI_PORT  0
#define ROOM_HUMI_PIN   11      /* DHT22 */
#define ROOM_LIGHT_I2C  0
#define ROOM_LIGHT_ADDR 0x23    /* BH1750 */

#define MIST_PORT       1
#define MIST_PIN        0
#define FAN_PORT        1
#define FAN_PIN         1

#endif
