/**
 * Shennong Platform - Communication Protocol
 */

#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <stdint.h>

// Command types
#define CMD_HEARTBEAT       0x01
#define CMD_SENSOR_DATA     0x02
#define CMD_CONTROL_RULE    0x02

// 植物
#define DEVICE_ROOTCROP     0x01  // 根菜: 土豆、红薯、萝卜、胡萝卜
#define DEVICE_LEAFCROP     0x02  // 叶菜: 菠菜、生菜
#define DEVICE_STEMCROP     0x03  // 茎菜: 芹菜、芦笋
#define DEVICE_FLOWERCROP   0x04  // 花菜: 花椰菜、菜花
#define DEVICE_FRUITCROP    0x05  // 果菜: 番茄、黄瓜、辣椒

// 动物
#define DEVICE_POLLINATOR   0x10  // 授粉: 蜜蜂
#define DEVICE_SOIL         0x11  // 沃土: 蚯蚓

// 真菌
#define DEVICE_EDIBLE_FUNGI 0x20  // 食用菌: 蘑菇、银耳

// 藻类
#define DEVICE_WATER_ALGAE  0x30  // 水藻: 螺旋藻、小球藻

// 昆虫
#define DEVICE_FEED_INSECT  0x40  // 饲虫: 蟋蟀、面包虫

// Control rule
typedef struct {
    uint8_t rule_id;
    uint8_t device_type;
    uint8_t species_id;             // 当前种植物种 (0=未设置)
    float temp_high, temp_low;
    float humidity_high, humidity_low;
    uint8_t spray, fan, led;
} __attribute__((packed)) control_rule_t;

// Sensor data
typedef struct {
    float temperature, humidity, light, ph;
} __attribute__((packed)) sensor_data_t;

#endif
