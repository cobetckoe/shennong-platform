#ifndef SHENNONG_SENSORS_H
#define SHENNONG_SENSORS_H

#include <Arduino.h>

// 传感器类型定义
typedef enum {
    SENSOR_TEMPERATURE,
    SENSOR_HUMIDITY,
    SENSOR_LIGHT,
    SENSOR_PH,
    SENSOR_EC,
    SENSOR_DISSOLVED_OXYGEN,
    SENSOR_CO2,
    SENSOR_SOIL_MOISTURE
} sensor_type_t;

// 传感器数据结构
typedef struct {
    sensor_type_t type;
    float value;
    unsigned long timestamp;
} sensor_data_t;

// 初始化所有传感器
void sensors_init(void);

// 读取指定类型传感器
float sensor_read(sensor_type_t type);

// 读取所有传感器数据
void sensor_read_all(sensor_data_t *data, int *count);

#endif // SHENNONG_SENSORS_H