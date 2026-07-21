/**
 * 控制引擎接口
 * 
 * 每个子设备必须实现这些函数。
 * device.c 通过此头文件调用控制引擎。
 */

#ifndef CONTROL_ENGINE_H
#define CONTROL_ENGINE_H

#include "protocols.h"
#include <stdint.h>

/* 初始化控制引擎 */
void control_engine_init(void);

/* 获取传感器数据 (只读) */
const sensor_data_t *control_engine_get_sensor(void);

/* 获取传感器数据 (可写，供 read_sensors 使用) */
sensor_data_t *control_engine_sensor_mut(void);

/* 获取当前控制规则 (只读) */
const control_rule_t *control_engine_get_rule(void);

/* 设置控制规则 */
void control_engine_set_rule(const control_rule_t *rule);

/* 执行控制逻辑，返回继电器状态 */
uint8_t control_engine_execute(void);

/* 获取继电器状态 */
uint8_t control_engine_get_relay(void);

/* 获取 LED 亮度 */
void control_engine_get_led(uint8_t *red, uint8_t *blue);

/* 设置 LED 亮度 */
void control_engine_set_led(uint8_t red, uint8_t blue);

/* 直接设置继电器状态 */
void control_engine_set_relay(uint8_t relay);

/* 更新继电器硬件输出 */
void control_engine_update_relay_hw(void);

/* 场景冷却时间管理 */
uint32_t control_engine_get_last_scene_time(uint8_t scene_id);
void control_engine_set_last_scene_time(uint8_t scene_id, uint32_t time);

#endif
