/**
 * 叶菜种植机 - 水培/气雾一体机
 * 
 * 架构:
 * - 云端下发控制策略 (不同植物不同规则)
 * - 本地根据规则 + 实时数据 = 自动控制
 * - 断网时本地独立运行
 * - 云端可远程微调参数
 * 
 * 水培是气雾的保险备份，两个系统同时运行
 * 
 * 平台: STM32F103 + Zigbee
 */

#ifndef DEV_LEAFCROP_CFG_H
#define DEV_LEAFCROP_CFG_H

#include "hal/gpio.h"
#include "hal/i2c.h"
#include "hal/adc.h"

// ==================== 设备信息 ====================
#define DEV_ID              0x02
#define DEV_TYPE            0x02
#define DEV_FW_VERSION      "2.0.0"

// ==================== 运行模式 ====================
#define MODE_HYDROPONICS    0       // 水培模式
#define MODE_AEROPONICS     1       // 气雾模式
#define MODE_AUTO           2       // 自动切换 (默认)

// ==================== 通信配置 ====================
#define ZIGBEE_PORT         0
#define ZIGBEE_BAUD         115200
#define HB_INTERVAL         5000    // 心跳间隔 (ms)
#define SENSOR_INTERVAL     2000    // 传感器采集间隔 (ms)
#define UART_TIMEOUT        100

// ==================== 安全配置 ====================
#define WATCHDOG_TIMEOUT    3000
#define SENSOR_RETRY        3
#define SENSOR_TIMEOUT      500

// ==================== 传感器引脚配置 ====================

/* DHT11 温湿度传感器 */
#define DHT11_PORT          0
#define DHT11_PIN           10

/* DS18B20 营养液温度 (水培) */
#define DS18B20_PORT        0
#define DS18B20_PIN         12

/* TDS 水质传感器 (水培) */
#define TDS_ADC             HAL_ADC_1
#define TDS_CH              HAL_ADC_CH0

/* PH-4502C pH传感器 (水培) */
#define PH_ADC              HAL_ADC_1
#define PH_CH               HAL_ADC_CH1

/* 红外光电液位传感器 x2 (水培) - 防水型 */
#define WATER_LOW_PORT      0
#define WATER_LOW_PIN       16      // 低液位 (A2)
#define WATER_HIGH_PORT     0
#define WATER_HIGH_PIN      19      // 高液位 (A4)

/* HC-SR04 超声波传感器 */
#define ULTRASONIC_PORT     0
#define ULTRASONIC_TRIG     17
#define ULTRASONIC_ECHO     18
#define ULTRASONIC_HEIGHT   50.0f   // 安装高度(cm)

/* BH1750 光照传感器 x2 (I2C) - 冠层分析 */
#define BH1750_I2C          HAL_I2C_1
#define BH1750_UPPER_ADDR   0x23    // 冠层上方
#define BH1750_LOWER_ADDR   0x5C    // 冠层下方

/* SGP30 VOC/CO2传感器 (I2C) */
#define SGP30_I2C           HAL_I2C_1
#define SGP30_ADDR          0x58

// ==================== 执行器引脚配置 ====================

/*
 * 继电器分配 (6路)
 * bit0: 水泵 (水培循环) / 喷雾泵 (气雾)
 * bit1: 补光灯
 * bit2: 排风扇
 * bit3: 加热片 (营养液加热)
 * bit4: 制冷片 (营养液冷却)
 * bit5: 备用
 */
#define RELAY_1_PORT        1       // 水泵/喷雾泵
#define RELAY_1_PIN         0
#define RELAY_2_PORT        1       // 补光灯
#define RELAY_2_PIN         1
#define RELAY_3_PORT        1       // 排风扇
#define RELAY_3_PIN         2
#define RELAY_4_PORT        1       // 加热片
#define RELAY_4_PIN         3
#define RELAY_5_PORT        1       // 制冷片
#define RELAY_5_PIN         4
#define RELAY_6_PORT        1       // 备用
#define RELAY_6_PIN         5

/* 双路PWM LED调光 (补光灯) */
#define LED_RED_PORT        1
#define LED_RED_PIN         6       // 红光PWM
#define LED_BLUE_PORT       1
#define LED_BLUE_PIN        7       // 蓝光PWM

#endif
