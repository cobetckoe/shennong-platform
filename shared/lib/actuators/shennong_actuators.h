#ifndef SHENNONG_ACTUATORS_H
#define SHENNONG_ACTUATORS_H

#include <Arduino.h>

// 执行器类型定义
typedef enum {
    ACTUATOR_SPRAY_PUMP,
    ACTUATOR_LED_LIGHT,
    ACTUATOR_FAN,
    ACTUATOR_SOLENOID_VALVE,
    ACTUATOR_STEPPER_MOTOR
} actuator_type_t;

// 初始化所有执行器
void actuators_init(void);

// 控制指定执行器
void actuator_control(actuator_type_t type, uint8_t value);

// 获取执行器状态
uint8_t actuator_get_status(actuator_type_t type);

#endif // SHENNONG_ACTUATORS_H