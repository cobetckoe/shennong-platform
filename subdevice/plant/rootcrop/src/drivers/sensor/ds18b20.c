/**
 * 传感器驱动 - DS18B20 单总线温度 (实现)
 */

#include "ds18b20.h"
#include "hal/delay.h"

/* TODO: 实现DS18B20单总线协议 (复位、跳过ROM、转换、读暂存器) */
float ds18b20_read(ds18b20_t *dev) {
    (void)dev;
    return 25.0f;
}
