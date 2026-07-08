/**
 * 神农平台 - 共享通信协议
 * 
 * 按生物分类学组织设备类型
 */

#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <stdint.h>

// 命令类型
#define CMD_HEARTBEAT       0x01
#define CMD_SENSOR_DATA     0x02
#define CMD_CONTROL_RULE    0x02

// ========== 植物界 (Plantae) ==========
// 按可食用部位分类
#define DEVICE_TUBER        0x02  // 块茎：土豆、红薯
#define DEVICE_ROOT         0x03  // 根：萝卜、胡萝卜
#define DEVICE_STEM         0x04  // 茎：芹菜、莴笋
#define DEVICE_LEAF         0x05  // 叶：菠菜、生菜
#define DEVICE_FLOWER       0x06  // 花：花椰菜、西兰花
#define DEVICE_FRUIT        0x07  // 果实：番茄、黄瓜

// ========== 藻类 (Algae) ==========
#define DEVICE_SPIRAL       0x10  // 螺旋藻
#define DEVICE_CHLORELLA    0x11  // 小球藻

// ========== 真菌界 (Fungi) ==========
#define DEVICE_MUSHROOM     0x20  // 蘑菇
#define DEVICE_FUNGUS       0x21  // 银耳、木耳

// ========== 动物界 (Animalia) ==========
#define DEVICE_BEE          0x30  // 蜜蜂
#define DEVICE_EARTHWORM    0x31  // 蚯蚓

// ========== 昆虫纲 (Insecta) ==========
#define DEVICE_CRICKET      0x40  // 蟋蟀
#define DEVICE_MEALWORM     0x41  // 面包虫

// 控制规则
typedef struct {
    uint8_t rule_id;
    uint8_t device_type;
    float temp_high, temp_low;
    float humidity_high, humidity_low;
    uint8_t spray, fan, led;
} __attribute__((packed)) control_rule_t;

// 传感器数据
typedef struct {
    float temperature, humidity, light, ph;
} __attribute__((packed)) sensor_data_t;

#endif
