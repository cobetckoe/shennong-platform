/**
 * 控制引擎 - 水培/气雾一体机
 * 
 * 按照涂鸦协议规范设计
 * 
 * 架构:
 * - 云端下发控制规则 (不同植物不同策略)
 * - 本地根据规则 + 实时数据 = 自动控制
 * - 断网时本地独立运行
 * - 云端可远程微调参数
 */

#ifndef CONTROL_ENGINE_H
#define CONTROL_ENGINE_H

#include <stdint.h>

// ==================== 控制规则结构 ====================
// 云端下发的控制策略
typedef struct {
    uint8_t rule_id;                    // 规则ID
    uint8_t species_id;                 // 物种ID (0=自定义)
    
    // 温度控制
    int8_t temp_high;                   // 温度上限 (℃) → 开风扇
    int8_t temp_low;                    // 温度下限 (℃) → 开加热
    uint8_t temp_fan_delay;             // 风扇启动延迟 (秒)
    uint8_t temp_heat_delay;            // 加热启动延迟 (秒)
    
    // 湿度控制
    uint8_t humidity_high;              // 湿度上限 (%) → 开排风
    uint8_t humidity_low;               // 湿度下限 (%) → 开喷雾
    uint8_t humidity_fog_interval;      // 喷雾间隔 (秒)
    uint8_t humidity_fog_duration;      // 喷雾时长 (秒)
    
    // CO2控制
    uint16_t co2_high;                  // CO2上限 (ppm) → 开排风
    uint16_t co2_low;                   // CO2下限 (ppm) → 开补气
    
    // 营养液控制
    int8_t nutrient_temp_high;          // 营养液温度上限 (℃)
    int8_t nutrient_temp_low;           // 营养液温度下限 (℃)
    uint8_t tds_high;                   // TDS上限 (×10 ppm)
    uint8_t tds_low;                    // TDS下限 (×10 ppm)
    uint8_t ph_high;                    // pH上限 (×10)
    uint8_t ph_low;                     // pH下限 (×10)
    
    // 光照控制
    uint16_t light_high;                // 光照上限 (lux) → 关补光
    uint16_t light_low;                 // 光照下限 (lux) → 开补光
    uint8_t led_red;                    // 红光亮度 (%)
    uint8_t led_blue;                   // 蓝光亮度 (%)
    
    // 模式控制
    uint8_t mode;                       // 0=自动, 1=手动
    uint8_t relay_mask;                 // 手动模式继电器状态
} __attribute__((packed)) control_rule_t;

// ==================== 传感器数据结构 ====================
typedef struct {
    float temperature;                  // 环境温度 (℃)
    float humidity;                     // 环境湿度 (%)
    int16_t light;                      // 光照 (lux)
    int16_t tds;                        // 营养液浓度 (ppm)
    float nutrient_temp;                // 营养液温度 (℃)
    int16_t co2;                        // CO2 (ppm)
    int16_t tvoc;                       // VOC (ppb)
    uint8_t water_status;               // 液位状态
    int16_t plant_height;               // 植物高度 (cm)
    int16_t ph;                         // pH (×10)
    int16_t par_upper;                  // 冠层上方PAR
    int16_t par_lower;                  // 冠层下方PAR
    int16_t transmittance;              // 透光率 (%)
    int16_t lai;                        // LAI (×10)
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

// ==================== 控制引擎API ====================

/**
 * 初始化控制引擎
 */
void control_engine_init(void);

/**
 * 更新控制规则 (云端下发)
 * @param new_rule 新的控制规则
 */
void control_engine_set_rule(const control_rule_t *new_rule);

/**
 * 获取当前控制规则
 * @return 当前规则指针
 */
const control_rule_t* control_engine_get_rule(void);

/**
 * 获取当前传感器数据
 * @return 传感器数据指针
 */
const sensor_data_t* control_engine_get_sensor(void);

/**
 * 更新传感器数据
 * @param data 新的传感器数据
 */
void control_engine_update_sensor(const sensor_data_t *data);

/**
 * 执行控制逻辑 (每秒调用)
 * @return 当前继电器状态
 */
uint8_t control_engine_execute(void);

/**
 * 获取当前继电器状态
 * @return 继电器状态位掩码
 */
uint8_t control_engine_get_relay(void);

/**
 * 获取当前LED亮度
 * @param red 红光亮度指针
 * @param blue 蓝光亮度指针
 */
void control_engine_get_led(uint8_t *red, uint8_t *blue);

/**
 * 设置LED亮度 (云端微调)
 * @param red 红光亮度
 * @param blue 蓝光亮度
 */
void control_engine_set_led(uint8_t red, uint8_t blue);

/**
 * 获取控制模式
 * @return 0=自动, 1=手动
 */
uint8_t control_engine_get_mode(void);

/**
 * 设置控制模式
 * @param mode 0=自动, 1=手动
 */
void control_engine_set_mode(uint8_t mode);

/**
 * 获取手动模式继电器状态
 * @return 继电器状态
 */
uint8_t control_engine_get_manual_relay(void);

/**
 * 设置手动模式继电器状态
 * @param relay 继电器状态
 */
void control_engine_set_manual_relay(uint8_t relay);

/**
 * 获取喷雾阶段 (调试用)
 * @return 0=停止, 1=喷雾中, 2=间隔中
 */
uint8_t control_engine_get_fog_phase(void);

/**
 * 获取喷雾阶段已运行时间 (调试用)
 * @return 已运行秒数
 */
uint32_t control_engine_get_fog_elapsed(void);

#endif