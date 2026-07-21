/**
 * Shennong Platform - Communication Protocol
 * 
 * 架构:
 * - 云端: 下发控制策略 + 自动化场景到 T5 网关
 * - T5网关: 转发单设备规则到子设备 / 本地执行跨设备联动
 * - 子设备: 本地执行控制规则 (断网也能跑)
 * - 云端: 远程微调参数
 */

#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <stdint.h>
#include <stdbool.h>

// ==================== 命令类型 ====================
#define CMD_HEARTBEAT       0x01    // 心跳
#define CMD_SENSOR_DATA     0x02    // 传感器数据上报
#define CMD_RULE_UPDATE     0x03    // 控制规则下发 (T5→子设备)
#define CMD_DP_UPDATE       0x04    // DP值更新 (微调)
#define CMD_ACK             0x05
#define CMD_RELAY_SET       0x06    // 继电器直接控制 (T5→子设备)
#define CMD_SCENE_SYNC      0x07    // 场景同步 (T5→子设备, 子设备本地规则)

// ==================== 设备类型 ====================
#define DEVICE_LEAFCROP     0x02    // 叶菜 (水培/气雾一体)

// ==================== 涂鸦DP定义 ====================
// 传感器 - 只上报
#define DPID_TEMP_VALUE       101   // 环境温度 (℃)
#define DPID_HUMIDITY_IN      102   // 环境湿度 (%)
#define DPID_LIGHT_VALUE      103   // 光照 (lux)
#define DPID_TDS_VALUE        104   // 营养液浓度 (ppm)
#define DPID_CO2_VALUE        105   // CO2 (ppm)
#define DPID_VOC_VALUE        106   // VOC (ppb)
#define DPID_DS18B20_TEMP     107   // 营养液温度 (℃)
#define DPID_WATER_STATUS     108   // 液位状态 (BOOL)
#define DPID_PLANT_HEIGHT     109   // 植物高度 (cm)
#define DPID_PH_VALUE         110   // pH (×10)
#define DPID_PAR_UPPER        111   // 冠层上方PAR
#define DPID_PAR_LOWER        112   // 冠层下方PAR
#define DPID_TRANSMITTANCE    113   // 透光率 (%)
#define DPID_LAI              114   // LAI (×10)

// 执行器 - 可下发可上报
#define DPID_RELAY_1          120   // 水泵
#define DPID_RELAY_2          121   // 补光灯
#define DPID_RELAY_3          122   // 排风扇
#define DPID_RELAY_4          123   // 喷雾泵
#define DPID_RELAY_5          124   // 加热片
#define DPID_RELAY_6          125   // 制冷片
#define DPID_LED_RED          126   // 红光LED亮度 (%)
#define DPID_LED_BLUE         127   // 蓝光LED亮度 (%)

// ==================== 控制规则 (云端下发，本地执行) ====================
typedef struct {
    uint8_t rule_id;                // 规则ID
    uint8_t species_id;             // 物种ID (0=自定义)
    
    // 温度控制
    int8_t temp_high;               // 温度上限 (℃) → 开风扇
    int8_t temp_low;                // 温度下限 (℃) → 开加热
    int8_t temp_fan_delay;          // 风扇启动延迟 (秒)
    int8_t temp_heat_delay;         // 加热启动延迟 (秒)
    
    // 湿度控制
    uint8_t humidity_high;          // 湿度上限 (%) → 开排风
    uint8_t humidity_low;           // 湿度下限 (%) → 开喷雾
    uint8_t humidity_fog_interval;  // 喷雾间隔 (秒)
    uint8_t humidity_fog_duration;  // 喷雾时长 (秒)
    
    // CO2控制
    uint16_t co2_high;              // CO2上限 (ppm) → 开排风
    uint16_t co2_low;               // CO2下限 (ppm) → 开补气
    
    // 营养液控制
    int8_t nutrient_temp_high;      // 营养液温度上限 (℃)
    int8_t nutrient_temp_low;       // 营养液温度下限 (℃)
    uint8_t tds_high;               // TDS上限 (×10 ppm)
    uint8_t tds_low;                // TDS下限 (×10 ppm)
    uint8_t ph_high;                // pH上限 (×10)
    uint8_t ph_low;                 // pH下限 (×10)
    
    // 光照控制
    uint16_t light_high;            // 光照上限 (lux) → 关补光
    uint16_t light_low;             // 光照下限 (lux) → 开补光
    uint8_t led_red;                // 红光亮度 (%)
    uint8_t led_blue;               // 蓝光亮度 (%)
    uint8_t light_on_hour;          // 开灯时间 (小时)
    uint8_t light_off_hour;         // 关灯时间 (小时)
    
    // 继电器直接控制 (云端手动模式)
    uint8_t relay_mask;             // 继电器掩码
    uint8_t mode;                   // 0=自动, 1=手动
} __attribute__((packed)) control_rule_t;

// ==================== 传感器数据 ====================
typedef struct {
    float temperature;              // 环境温度 (℃)
    float humidity;                 // 环境湿度 (%)
    int16_t light;                  // 光照 (lux)
    int16_t tds;                    // 营养液浓度 (ppm)
    float nutrient_temp;            // 营养液温度 (℃)
    int16_t co2;                    // CO2 (ppm)
    int16_t tvoc;                   // VOC (ppb)
    uint8_t water_status;           // 液位状态
    int16_t plant_height;           // 植物高度 (cm)
    int16_t ph;                     // pH (×10)
    int16_t par_upper;              // 冠层上方PAR
    int16_t par_lower;              // 冠层下方PAR
    int16_t transmittance;          // 透光率 (%)
    int16_t lai;                    // LAI (×10)
} __attribute__((packed)) sensor_data_t;

// ==================== 继电器位定义 ====================
#define RELAY_BIT_PUMP      0x01    // bit0: 水泵 (水培循环)
#define RELAY_BIT_LIGHT     0x02    // bit1: 补光灯
#define RELAY_BIT_FAN       0x04    // bit2: 排风扇
#define RELAY_BIT_FOG       0x08    // bit3: 喷雾泵 (气雾)
#define RELAY_BIT_HEATER    0x10    // bit4: 加热片
#define RELAY_BIT_COOLER    0x20    // bit5: 制冷片

// ==================== 液位状态位 ====================
#define WATER_BIT_LOW       0x01
#define WATER_BIT_HIGH      0x02
#define WATER_BIT_VALID     0x04

// ==================== 子设备本地自动化场景 ====================
// T5 下发到子设备，子设备本地执行
// 适用于: 单设备内的定时/阈值控制
typedef struct {
    uint8_t scene_id;               // 场景ID (1-16)
    bool enabled;                   // 是否启用

    uint8_t trigger_type;           // 0=定时, 1=传感器阈值

    // 定时触发
    struct {
        uint8_t hour;               // 小时 (0-23)
        uint8_t minute;             // 分钟 (0-59)
        uint8_t repeat;             // bit0-6=周一到周日, 0xFF=每天
    } timer;

    // 传感器阈值触发
    struct {
        uint8_t dpid;               // 数据点ID
        int32_t threshold_min;      // 下限 (含)
        int32_t threshold_max;      // 上限 (含)
    } sensor;

    // 执行动作
    uint8_t action_relay_mask;      // 继电器掩码
    uint8_t action_relay_state;     // 0=关闭, 1=开启

    uint8_t cooldown_sec;           // 冷却时间 (秒)
} __attribute__((packed)) subdev_scene_t;

#endif
