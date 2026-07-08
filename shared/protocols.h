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

// Plant (edible parts)
#define DEVICE_TUBER        0x02  // Tuber: Potato, Sweet Potato
#define DEVICE_ROOT         0x03  // Root: Radish, Carrot
#define DEVICE_STEM         0x04  // Stem: Celery
#define DEVICE_LEAF         0x05  // Leaf: Spinach, Lettuce
#define DEVICE_FLOWER       0x06  // Flower: Broccoli, Cauliflower
#define DEVICE_FRUIT        0x07  // Fruit: Tomato, Cucumber

// Algae
#define DEVICE_SPIRAL       0x10  // Spirulina
#define DEVICE_CHLORELLA    0x11  // Chlorella

// Fungi
#define DEVICE_MUSHROOM     0x20  // Mushroom
#define DEVICE_FUNGUS       0x21  // Tremella, Auricularia

// Animal
#define DEVICE_BEE          0x30  // Bee
#define DEVICE_EARTHWORM    0x31  // Earthworm

// Insect
#define DEVICE_CRICKET      0x40  // Cricket
#define DEVICE_MEALWORM     0x41  // Mealworm

// Control rule
typedef struct {
    uint8_t rule_id;
    uint8_t device_type;
    float temp_high, temp_low;
    float humidity_high, humidity_low;
    uint8_t spray, fan, led;
} __attribute__((packed)) control_rule_t;

// Sensor data
typedef struct {
    float temperature, humidity, light, ph;
} __attribute__((packed)) sensor_data_t;

#endif
