/**
 * 传感器驱动 - 红外光电液位传感器 (防水型)
 * 
 * 工业级双冗余设计
 */

#ifndef INFRARED_LEVEL_H
#define INFRARED_LEVEL_H

#include "hal/gpio.h"

// ==================== 状态定义 ====================
#define LEVEL_OK            0x00
#define LEVEL_LOW_ALARM     0x01
#define LEVEL_HIGH_ALARM    0x02
#define LEVEL_SENSOR_FAULT  0x04
#define LEVEL_REDUNDANCY_ERR 0x08

// ==================== 传感器实例 ====================
typedef struct {
    uint8_t port;
    uint8_t pin;
    uint8_t active_level;   // 0=低电平有效, 1=高电平有效
    uint8_t last_state;
    uint8_t status;
} infrared_level_t;

// ==================== API函数 ====================
void infrared_level_init(infrared_level_t *dev);
uint8_t infrared_level_read(infrared_level_t *dev);
uint8_t infrared_level_is_normal(infrared_level_t *dev);
uint8_t infrared_level_redundancy_check(infrared_level_t *primary, infrared_level_t *backup);
uint8_t infrared_level_get_status(infrared_level_t *low, infrared_level_t *high);

#endif