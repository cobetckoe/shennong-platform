#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// 传感器数据结构
typedef struct {
    uint8_t device_id;       // 设备ID
    time_t timestamp;        // 时间戳
    float temperature;       // 温度
    float humidity;          // 湿度
    float light;             // 光照
    float ph;                // pH值
    float ec;                // EC值
    float dissolved_oxygen;  // 溶氧
    float co2;               // CO2浓度
} sensor_data_t;

// 控制数据结构
typedef struct {
    uint8_t device_id;       // 设备ID
    time_t timestamp;        // 时间戳
    uint8_t spray_pump;      // 喷雾泵状态
    uint8_t led_light;       // LED灯状态
    uint8_t fan;             // 风扇状态
    uint8_t valve;           // 电磁阀状态
} control_data_t;

// 数据管理初始化
bool data_manager_init(void);

// 保存传感器数据
bool data_save_sensor(const sensor_data_t *data);

// 保存控制数据
bool data_save_control(const control_data_t *data);

// 获取传感器数据
bool data_get_sensor(uint8_t device_id, sensor_data_t *data, int count);

// 获取控制数据
bool data_get_control(uint8_t device_id, control_data_t *data, int count);

// 清理旧数据
bool data_cleanup(uint32_t days_to_keep);

// 获取数据统计
bool data_get_stats(uint8_t device_id, float *avg_temp, float *avg_humidity);

#endif // DATA_MANAGER_H