#include "data_manager.h"
#include "storage.h"
#include <stdio.h>
#include <string.h>

#define DATA_DIR "data"

// 数据管理初始化
bool data_manager_init(void) {
    // 创建数据目录
    mkdir(DATA_DIR, 0755);
    return true;
}

// 保存传感器数据
bool data_save_sensor(const sensor_data_t *data) {
    char filename[128];
    snprintf(filename, sizeof(filename), "%s/sensor_%u_%ld.bin", DATA_DIR, data->device_id, data->timestamp);
    
    return storage_save(filename, data, sizeof(sensor_data_t));
}

// 保存控制数据
bool data_save_control(const control_data_t *data) {
    char filename[128];
    snprintf(filename, sizeof(filename), "%s/control_%u_%ld.bin", DATA_DIR, data->device_id, data->timestamp);
    
    return storage_save(filename, data, sizeof(control_data_t));
}

// 获取传感器数据
bool data_get_sensor(uint8_t device_id, sensor_data_t *data, int count) {
    // 简化实现，实际需要遍历数据文件
    return false;
}

// 获取控制数据
bool data_get_control(uint8_t device_id, control_data_t *data, int count) {
    // 简化实现
    return false;
}

// 清理旧数据
bool data_cleanup(uint32_t days_to_keep) {
    // 简化实现
    return true;
}

// 获取数据统计
bool data_get_stats(uint8_t device_id, float *avg_temp, float *avg_humidity) {
    // 简化实现
    *avg_temp = 25.0;
    *avg_humidity = 60.0;
    return true;
}