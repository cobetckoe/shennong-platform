/**
 * 控制引擎 - 水培/气雾一体机 (实现)
 * 
 * 按照涂鸦协议规范设计
 * 本地控制逻辑，断网也能独立运行
 */

#include "control_engine.h"
#include "hal/delay.h"
#include <string.h>

// ==================== 全局变量 ====================
static control_rule_t rule;
static sensor_data_t sensor;
static uint8_t relay_states = 0;
static uint8_t led_red_brightness = 50;
static uint8_t led_blue_brightness = 50;

// ==================== 控制标志 ====================
static uint8_t temp_fan_active = 0;
static uint8_t temp_heat_active = 0;
static uint8_t humidity_fog_active = 0;
static uint32_t fog_start_time = 0;     // 喷雾开始时间
static uint8_t fog_phase = 0;           // 0=停止, 1=喷雾中, 2=间隔中

// ==================== 初始化 ====================

void control_engine_init(void) {
    memset(&rule, 0, sizeof(rule));
    memset(&sensor, 0, sizeof(sensor));
    relay_states = 0;
    
    // 默认规则 (适用于大多数叶菜)
    rule.rule_id = 1;
    rule.species_id = 0;
    
    // 温度控制
    rule.temp_high = 35;            // 35℃开风扇
    rule.temp_low = 15;             // 15℃开加热
    rule.temp_fan_delay = 0;
    rule.temp_heat_delay = 0;
    
    // 湿度控制
    rule.humidity_high = 85;        // 85%开排风
    rule.humidity_low = 50;         // 50%开喷雾
    rule.humidity_fog_interval = 30; // 30秒喷雾间隔
    rule.humidity_fog_duration = 5;  // 5秒喷雾时长
    
    // CO2控制
    rule.co2_high = 2000;           // 2000ppm开排风
    rule.co2_low = 400;             // 400ppm开补气
    
    // 营养液控制
    rule.nutrient_temp_high = 30;   // 30℃开制冷
    rule.nutrient_temp_low = 18;    // 18℃开加热
    rule.tds_high = 15;             // 1500ppm
    rule.tds_low = 5;               // 500ppm
    rule.ph_high = 75;              // pH 7.5
    rule.ph_low = 55;               // pH 5.5
    
    // 光照控制
    rule.light_high = 25000;        // 25000lux关补光
    rule.light_low = 5000;          // 5000lux开补光
    rule.led_red = 70;              // 红光70%
    rule.led_blue = 30;             // 蓝光30%
    
    // 模式
    rule.mode = 0;                  // 自动模式
    rule.relay_mask = 0;
}

// ==================== 规则管理 ====================

void control_engine_set_rule(const control_rule_t *new_rule) {
    if (new_rule != NULL) {
        memcpy(&rule, new_rule, sizeof(control_rule_t));
        // 规则变更时重置喷雾计时
        fog_phase = 0;
        fog_start_time = 0;
    }
}

const control_rule_t* control_engine_get_rule(void) {
    return &rule;
}

// ==================== 传感器数据 ====================

const sensor_data_t* control_engine_get_sensor(void) {
    return &sensor;
}

void control_engine_update_sensor(const sensor_data_t *data) {
    if (data != NULL) {
        memcpy(&sensor, data, sizeof(sensor_data_t));
    }
}

// ==================== 温度控制逻辑 ====================
// 温度 > 上限 → 开风扇 (带回差)
// 温度 < 下限 → 开加热 (带回差)

static void control_temperature(void) {
    // 温度过高 → 开风扇
    if (sensor.temperature > rule.temp_high) {
        if (!temp_fan_active) {
            temp_fan_active = 1;
            relay_states |= RELAY_BIT_FAN;
        }
    } else if (sensor.temperature < rule.temp_high - 2) {
        // 回差2℃关风扇
        temp_fan_active = 0;
        relay_states &= ~RELAY_BIT_FAN;
    }
    
    // 温度过低 → 开加热
    if (sensor.temperature < rule.temp_low) {
        if (!temp_heat_active) {
            temp_heat_active = 1;
            relay_states |= RELAY_BIT_HEATER;
        }
    } else if (sensor.temperature > rule.temp_low + 2) {
        // 回差2℃关加热
        temp_heat_active = 0;
        relay_states &= ~RELAY_BIT_HEATER;
    }
}

// ==================== 湿度控制逻辑 ====================
// 湿度 < 下限 → 间歇喷雾 (开duration秒 → 关interval秒 → 循环)
// 湿度 > 上限 → 开排风

static void control_humidity(void) {
    uint32_t now = hal_tick_ms() / 1000;  // 转换为秒
    
    // 湿度过低 → 启动间歇喷雾
    if (sensor.humidity < rule.humidity_low) {
        if (!humidity_fog_active) {
            humidity_fog_active = 1;
            fog_phase = 1;  // 开始喷雾
            fog_start_time = now;
            relay_states |= RELAY_BIT_FOG;
        }
    } else if (sensor.humidity > rule.humidity_low + 5) {
        // 回差5%关喷雾
        humidity_fog_active = 0;
        fog_phase = 0;
        relay_states &= ~RELAY_BIT_FOG;
    }
    
    // 间歇喷雾时间控制
    if (humidity_fog_active) {
        uint32_t elapsed = now - fog_start_time;
        
        if (fog_phase == 1) {
            // 喷雾中，检查是否到达喷雾时长
            if (elapsed >= rule.humidity_fog_duration) {
                fog_phase = 2;  // 切换到间隔
                fog_start_time = now;
                relay_states &= ~RELAY_BIT_FOG;  // 关闭喷雾
            }
        } else if (fog_phase == 2) {
            // 间隔中，检查是否到达间隔时间
            if (elapsed >= rule.humidity_fog_interval) {
                fog_phase = 1;  // 切换到喷雾
                fog_start_time = now;
                relay_states |= RELAY_BIT_FOG;  // 开启喷雾
            }
        }
    }
    
    // 湿度过高 → 排风
    if (sensor.humidity > rule.humidity_high) {
        relay_states |= RELAY_BIT_FAN;
    }
}

// ==================== CO2控制逻辑 ====================
// CO2 > 上限 → 开排风
// CO2 < 下限 → 开补气

static void control_co2(void) {
    if (sensor.co2 > rule.co2_high) {
        relay_states |= RELAY_BIT_FAN;
    } else if (sensor.co2 < rule.co2_low) {
        // CO2过低，关闭排风
        if (!(relay_states & RELAY_BIT_FAN)) {
            // 可以添加补气泵控制
        }
    }
}

// ==================== 光照控制逻辑 ====================
// 光照 < 下限 → 开补光
// 光照 > 上限 → 关补光

static void control_light(void) {
    if (sensor.light < rule.light_low) {
        relay_states |= RELAY_BIT_LIGHT;
        led_red_brightness = rule.led_red;
        led_blue_brightness = rule.led_blue;
    } else if (sensor.light > rule.light_high) {
        relay_states &= ~RELAY_BIT_LIGHT;
    }
}

// ==================== 营养液控制逻辑 ====================

static void control_nutrient(void) {
    // 营养液温度控制 (带回差)
    if (sensor.nutrient_temp > rule.nutrient_temp_high) {
        relay_states |= RELAY_BIT_COOLER;
        relay_states &= ~RELAY_BIT_HEATER;
    } else if (sensor.nutrient_temp < rule.nutrient_temp_high - 2) {
        relay_states &= ~RELAY_BIT_COOLER;
    }
    
    if (sensor.nutrient_temp < rule.nutrient_temp_low) {
        relay_states |= RELAY_BIT_HEATER;
        relay_states &= ~RELAY_BIT_COOLER;
    } else if (sensor.nutrient_temp > rule.nutrient_temp_low + 2) {
        relay_states &= ~RELAY_BIT_HEATER;
    }
    
    // TDS过高 → 报警 (可扩展)
    // pH异常 → 报警 (可扩展)
}

// ==================== 液位保护 ====================
// 液位过低 → 关水泵 (安全保护)

static void control_water(void) {
    if (!(sensor.water_status & WATER_BIT_VALID) || 
        (sensor.water_status & WATER_BIT_LOW)) {
        // 液位异常，关闭水泵保护
        relay_states &= ~RELAY_BIT_PUMP;
    }
}

// ==================== 主控制引擎 ====================

uint8_t control_engine_execute(void) {
    // 手动模式：直接使用规则中的继电器状态
    if (rule.mode == 1) {
        relay_states = rule.relay_mask;
        return relay_states;
    }
    
    // 自动模式：依次执行各控制逻辑
    control_temperature();
    control_humidity();
    control_co2();
    control_light();
    control_nutrient();
    control_water();
    
    return relay_states;
}

// ==================== 状态查询 ====================

uint8_t control_engine_get_relay(void) {
    return relay_states;
}

void control_engine_get_led(uint8_t *red, uint8_t *blue) {
    if (red != NULL) *red = led_red_brightness;
    if (blue != NULL) *blue = led_blue_brightness;
}

void control_engine_set_led(uint8_t red, uint8_t blue) {
    led_red_brightness = red;
    led_blue_brightness = blue;
}

uint8_t control_engine_get_mode(void) {
    return rule.mode;
}

void control_engine_set_mode(uint8_t mode) {
    rule.mode = mode;
}

uint8_t control_engine_get_manual_relay(void) {
    return rule.relay_mask;
}

void control_engine_set_manual_relay(uint8_t relay) {
    rule.relay_mask = relay;
    if (rule.mode == 1) {
        relay_states = relay;
    }
}

// ==================== 调试接口 ====================

uint8_t control_engine_get_fog_phase(void) {
    return fog_phase;
}

uint32_t control_engine_get_fog_elapsed(void) {
    uint32_t now = hal_tick_ms() / 1000;
    return now - fog_start_time;
}