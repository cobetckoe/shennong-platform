#include "shennong_sensors.h"

// 初始化所有传感器
void sensors_init(void) {
    // 初始化I2C、ADC等接口
    // 初始化各个传感器
}

// 读取指定类型传感器
float sensor_read(sensor_type_t type) {
    switch (type) {
        case SENSOR_TEMPERATURE:
            // 读取温度传感器
            return 25.0;
        case SENSOR_HUMIDITY:
            // 读取湿度传感器
            return 60.0;
        case SENSOR_LIGHT:
            // 读取光照传感器
            return 1000.0;
        case SENSOR_PH:
            // 读取pH传感器
            return 7.0;
        case SENSOR_EC:
            // 读取EC传感器
            return 1.5;
        case SENSOR_DISSOLVED_OXYGEN:
            // 读取溶氧传感器
            return 8.0;
        case SENSOR_CO2:
            // 读取CO2传感器
            return 400.0;
        case SENSOR_SOIL_MOISTURE:
            // 读取土壤湿度传感器
            return 50.0;
        default:
            return 0.0;
    }
}

// 读取所有传感器数据
void sensor_read_all(sensor_data_t *data, int *count) {
    int i = 0;
    
    data[i].type = SENSOR_TEMPERATURE;
    data[i].value = sensor_read(SENSOR_TEMPERATURE);
    data[i].timestamp = millis();
    i++;
    
    data[i].type = SENSOR_HUMIDITY;
    data[i].value = sensor_read(SENSOR_HUMIDITY);
    data[i].timestamp = millis();
    i++;
    
    data[i].type = SENSOR_LIGHT;
    data[i].value = sensor_read(SENSOR_LIGHT);
    data[i].timestamp = millis();
    i++;
    
    *count = i;
}