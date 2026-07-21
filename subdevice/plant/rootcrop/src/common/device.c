/**
 * 设备公共框架 - 涂鸦协议模板 (实现)
 * 
 * 水培/气雾一体机
 * 
 * 本地控制引擎 + 涂鸦协议通信
 */

#include "device.h"
#include "control_engine.h"
#include "config.h"
#include "hal/uart.h"
#include "hal/delay.h"
#include <string.h>
#include <stdbool.h>

// ==================== 全局变量 ====================
static uint8_t device_id = 0;
static uint32_t last_heartbeat = 0;
static uint32_t last_sensor_read = 0;
static uint32_t last_control = 0;
static uint32_t last_scene_check = 0;

// ==================== 本地自动化场景 ====================
#define MAX_LOCAL_SCENES 16
static subdev_scene_t local_scenes[MAX_LOCAL_SCENES];
static uint8_t local_scene_count = 0;

static void add_local_scene(const subdev_scene_t *scene) {
    for (uint8_t i = 0; i < local_scene_count; i++) {
        if (local_scenes[i].scene_id == scene->scene_id) {
            memcpy(&local_scenes[i], scene, sizeof(subdev_scene_t));
            return;
        }
    }
    if (local_scene_count < MAX_LOCAL_SCENES) {
        memcpy(&local_scenes[local_scene_count], scene, sizeof(subdev_scene_t));
        local_scene_count++;
    }
}

static void execute_local_scenes(void) {
    uint32_t now = hal_tick_ms();
    if (now - last_scene_check < 1000) return;
    last_scene_check = now;

    const sensor_data_t *s = control_engine_get_sensor();

    for (uint8_t i = 0; i < local_scene_count; i++) {
        subdev_scene_t *sc = &local_scenes[i];
        if (!sc->enabled) continue;

        bool triggered = false;

        if (sc->trigger_type == 0) {
            /* 定时触发 - 简化: 仅比较小时和分钟 */
            /* TODO: 需要 RTC 或 NTP 获取时间 */
        } else if (sc->trigger_type == 1) {
            /* 传感器阈值触发 */
            int32_t val = 0;
            switch (sc->sensor.dpid) {
                case DPID_TEMP_VALUE:
                    val = (int32_t)(s->temperature * 10);
                    break;
                case DPID_HUMIDITY_IN:
                    val = (int32_t)(s->humidity * 10);
                    break;
                case DPID_LIGHT_VALUE:
                    val = s->light;
                    break;
                case DPID_PH_VALUE:
                    val = s->ph;
                    break;
                default:
                    break;
            }
            if (val >= sc->sensor.threshold_min &&
                val <= sc->sensor.threshold_max) {
                triggered = true;
            }
        }

        if (!triggered) continue;

        /* 冷却检查 */
        if (sc->cooldown_sec > 0 &&
            (now / 1000) - control_engine_get_last_scene_time(sc->scene_id) < sc->cooldown_sec) {
            continue;
        }

        /* 执行动作 */
        uint8_t relay = control_engine_get_relay();
        if (sc->action_relay_state) {
            relay |= sc->action_relay_mask;
        } else {
            relay &= ~sc->action_relay_mask;
        }
        control_engine_set_relay(relay);
        control_engine_update_relay_hw();
        control_engine_set_last_scene_time(sc->scene_id, now / 1000);
    }
}

// ==================== 串口通信 ====================

void device_send(uint8_t *data, uint16_t len) {
    if (len > 0 && len <= 128) {
        hal_uart_send(ZIGBEE_PORT, data, len);
    }
}

// ==================== DP上报 ====================

/**
 * 上报单个DP值
 * 格式: [CMD][device_id][dpid][type][value...]
 */
void device_report_dp(uint8_t dpid, uint32_t value, uint8_t type) {
    uint8_t buf[10];
    buf[0] = CMD_SENSOR_DATA;
    buf[1] = device_id;
    buf[2] = dpid;
    buf[3] = type;
    
    if (type == 1) {  // BOOL
        buf[4] = value ? 1 : 0;
        device_send(buf, 5);
    } else {  // VALUE (4字节)
        buf[4] = value & 0xFF;
        buf[5] = (value >> 8) & 0xFF;
        buf[6] = (value >> 16) & 0xFF;
        buf[7] = (value >> 24) & 0xFF;
        device_send(buf, 8);
    }
}

/**
 * 上报所有传感器数据
 */
void device_report_all(void) {
    const sensor_data_t *s = control_engine_get_sensor();
    uint8_t relay = control_engine_get_relay();
    uint8_t led_red, led_blue;
    control_engine_get_led(&led_red, &led_blue);
    
    // 传感器数据
    device_report_dp(DPID_TEMP_VALUE, (int32_t)(s->temperature * 10), 4);
    device_report_dp(DPID_HUMIDITY_IN, (int32_t)(s->humidity * 10), 4);
    device_report_dp(DPID_LIGHT_VALUE, s->light, 4);
    device_report_dp(DPID_TDS_VALUE, s->tds, 4);
    device_report_dp(DPID_DS18B20_TEMP, (int32_t)(s->nutrient_temp * 10), 4);
    device_report_dp(DPID_CO2_VALUE, s->co2, 4);
    device_report_dp(DPID_VOC_VALUE, s->tvoc, 4);
    device_report_dp(DPID_WATER_STATUS, s->water_status, 1);
    device_report_dp(DPID_PLANT_HEIGHT, s->plant_height, 4);
    device_report_dp(DPID_PH_VALUE, s->ph, 4);
    device_report_dp(DPID_PAR_UPPER, s->par_upper, 4);
    device_report_dp(DPID_PAR_LOWER, s->par_lower, 4);
    device_report_dp(DPID_TRANSMITTANCE, s->transmittance, 4);
    device_report_dp(DPID_LAI, s->lai, 4);
    
    // 执行器状态
    device_report_dp(DPID_RELAY_1, (relay & RELAY_BIT_PUMP) ? 1 : 0, 1);
    device_report_dp(DPID_RELAY_2, (relay & RELAY_BIT_LIGHT) ? 1 : 0, 1);
    device_report_dp(DPID_RELAY_3, (relay & RELAY_BIT_FAN) ? 1 : 0, 1);
    device_report_dp(DPID_RELAY_4, (relay & RELAY_BIT_FOG) ? 1 : 0, 1);
    device_report_dp(DPID_RELAY_5, (relay & RELAY_BIT_HEATER) ? 1 : 0, 1);
    device_report_dp(DPID_RELAY_6, (relay & RELAY_BIT_COOLER) ? 1 : 0, 1);
    device_report_dp(DPID_LED_RED, led_red, 4);
    device_report_dp(DPID_LED_BLUE, led_blue, 4);
}

// ==================== 命令处理 ====================

/**
 * 处理控制规则下发
 * 格式: [CMD_RULE_UPDATE][device_id][rule...]
 */
static void process_rule_update(uint8_t *data, uint16_t len) {
    if (len < sizeof(control_rule_t) + 2) return;
    
    control_rule_t new_rule;
    memcpy(&new_rule, &data[2], sizeof(new_rule));
    
    // 简单校验
    if (new_rule.temp_high > new_rule.temp_low &&
        new_rule.humidity_high > new_rule.humidity_low) {
        control_engine_set_rule(&new_rule);
        
        // 回复确认
        uint8_t ack[3] = {CMD_ACK, device_id, 0};
        device_send(ack, sizeof(ack));
    }
}

/**
 * 处理DP微调命令
 * 格式: [CMD_DP_UPDATE][device_id][dpid][type][value...]
 */
static void process_dp_update(uint8_t *data, uint16_t len) {
    if (len < 5) return;
    
    uint8_t dpid = data[2];
    uint8_t type = data[3];
    uint32_t value = 0;
    
    if (type == 1) {
        value = data[4];
    } else {
        value = data[4] | (data[5] << 8) | (data[6] << 16) | (data[7] << 24);
    }
    
    // 根据DPID修改规则参数
    const control_rule_t *rule = control_engine_get_rule();
    control_rule_t new_rule;
    memcpy(&new_rule, rule, sizeof(new_rule));
    
    switch (dpid) {
        case DPID_TEMP_VALUE:
            new_rule.temp_high = value / 10;
            break;
        case DPID_HUMIDITY_IN:
            new_rule.humidity_high = value / 10;
            break;
        case DPID_LED_RED:
            new_rule.led_red = value;
            control_engine_set_led(value, rule->led_blue);
            break;
        case DPID_LED_BLUE:
            new_rule.led_blue = value;
            control_engine_set_led(rule->led_red, value);
            break;
        default:
            break;
    }
    
    control_engine_set_rule(&new_rule);
}

/**
 * 处理网关下发的命令
 */
void device_process_command(uint8_t *data, uint16_t len) {
    if (len < 2 || data[1] != device_id) return;
    
    switch (data[0]) {
        case CMD_RULE_UPDATE:
            process_rule_update(data, len);
            break;
        case CMD_DP_UPDATE:
            process_dp_update(data, len);
            break;
        case CMD_SCENE_SYNC:
            /* T5 下发本地自动化场景 */
            if (len >= 2 + sizeof(subdev_scene_t)) {
                subdev_scene_t scene;
                memcpy(&scene, &data[2], sizeof(subdev_scene_t));
                add_local_scene(&scene);
            }
            break;
        case CMD_RELAY_SET:
            /* T5 直接控制继电器 */
            if (len >= 3) {
                uint8_t mask = data[2];
                uint8_t state = len > 3 ? data[3] : 0xFF;
                uint8_t relay = control_engine_get_relay();
                if (state == 0xFF) {
                    relay ^= mask;  /* 翻转 */
                } else if (state) {
                    relay |= mask;  /* 开启 */
                } else {
                    relay &= ~mask; /* 关闭 */
                }
                control_engine_set_relay(relay);
                control_engine_update_relay_hw();
            }
            break;
        default:
            break;
    }
}

// ==================== 设备接口实现 ====================

void device_init(uint8_t id) {
    device_id = id;
    
    hal_init();
    hal_uart_init(ZIGBEE_PORT, ZIGBEE_BAUD);
    
    control_engine_init();
    
    last_heartbeat = 0;
    last_sensor_read = 0;
    last_control = 0;
}

/**
 * 设备主循环
 * 本地控制引擎 + 涂鸦协议通信
 */
void device_loop(uint8_t id) {
    device_id = id;
    
    while (1) {
        uint32_t now = hal_tick_ms();

        // 心跳包 (每5秒)
        if (now - last_heartbeat >= HB_INTERVAL) {
            uint8_t hb[3] = {CMD_HEARTBEAT, device_id, 0};
            device_send(hb, sizeof(hb));
            last_heartbeat = now;
        }

        // 传感器采集 + 上报 (每2秒)
        if (now - last_sensor_read >= SENSOR_INTERVAL) {
            // 读取传感器 (由main.cpp实现)
            extern void read_sensors(void);
            read_sensors();
            
            // 上报数据
            device_report_all();
            last_sensor_read = now;
        }

        // 控制执行 (每1秒)
        if (now - last_control >= 1000) {
            // 执行控制逻辑
            uint8_t relay = control_engine_execute();
            
            // 执行本地自动化场景
            execute_local_scenes();
            
            last_control = now;
        }

        // 接收网关命令
        uint8_t rx[64];
        uint16_t rx_len = hal_uart_recv(ZIGBEE_PORT, rx, sizeof(rx), UART_TIMEOUT);
        
        if (rx_len >= 2) {
            device_process_command(rx, rx_len);
        }

        hal_delay_ms(10);
    }
}

/**
 * 设备主循环单次执行 (非阻塞)
 * 用于main.cpp自己控制主循环
 */
void device_loop_once(uint8_t id) {
    device_id = id;
    uint32_t now = hal_tick_ms();

    // 心跳包 (每5秒)
    if (now - last_heartbeat >= HB_INTERVAL) {
        uint8_t hb[3] = {CMD_HEARTBEAT, device_id, 0};
        device_send(hb, sizeof(hb));
        last_heartbeat = now;
    }

    // 传感器上报 (每2秒)
    if (now - last_sensor_read >= SENSOR_INTERVAL) {
        device_report_all();
        last_sensor_read = now;
    }

    // 接收网关命令
    uint8_t rx[64];
    uint16_t rx_len = hal_uart_recv(ZIGBEE_PORT, rx, sizeof(rx), UART_TIMEOUT);
    
    if (rx_len >= 2) {
        device_process_command(rx, rx_len);
    }
}