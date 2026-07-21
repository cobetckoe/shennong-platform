/**
 * 神农系统 T5 网关 - 本地自动化模块
 * 基于 TuyaOpen CRON 实现本地自动化
 * 支持: 定时触发、传感器阈值触发、跨设备联动
 * 支持设备: 内部子设备 + 外部 Zigbee 设备 + 蓝牙设备 + WiFi 设备(仅在线)
 */

#include "local_automation.h"
#include "zigbee_manager.h"
#include "ble_manager.h"
#include "tuya_cron.h"
#include "tuya_kv.h"
#include <string.h>

#define MAX_LOCAL_SCENES    16
#define MAX_CROSS_SCENES    16
#define CHECK_INTERVAL_MS   1000

/* 本地单设备场景 (转发到子设备执行) */
static subdev_scene_t local_scenes[MAX_LOCAL_SCENES];
static uint8_t local_scene_count = 0;

/* 跨设备场景 (T5 本地执行) */
static cross_scene_t cross_scenes[MAX_CROSS_SCENES];
static uint8_t cross_scene_count = 0;

static uint32_t last_check_time = 0;

/* ==================== 初始化 ==================== */
void local_automation_init(void) {
    local_scene_count = 0;
    cross_scene_count = 0;
    last_check_time = 0;

    /* 从持久化存储加载场景 */
    local_automation_load();

    /* 注册 CRON 任务: 每分钟检查定时场景 */
    tuya_cron_create("scene_check", "0 * * * * *", local_automation_cron_check, NULL);
}

/* ==================== 主循环 ==================== */
void local_automation_loop(void) {
    uint32_t now = tuya_system_get_millisecond();
    if (now - last_check_time < CHECK_INTERVAL_MS) return;
    last_check_time = now;

    /* 检查传感器阈值触发的场景 */
    local_automation_check_sensor_scenes();

    /* 检查跨设备联动场景 */
    local_automation_check_cross_scenes();
}

/* ==================== 场景管理 ==================== */
void local_automation_add_local_scene(const subdev_scene_t *scene) {
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
    local_automation_save();
}

void local_automation_add_cross_scene(const cross_scene_t *scene) {
    for (uint8_t i = 0; i < cross_scene_count; i++) {
        if (cross_scenes[i].scene_id == scene->scene_id) {
            memcpy(&cross_scenes[i], scene, sizeof(cross_scene_t));
            return;
        }
    }
    if (cross_scene_count < MAX_CROSS_SCENES) {
        memcpy(&cross_scenes[cross_scene_count], scene, sizeof(cross_scene_t));
        cross_scene_count++;
    }
    local_automation_save();
}

void local_automation_remove_scene(uint8_t scene_id) {
    for (uint8_t i = 0; i < local_scene_count; i++) {
        if (local_scenes[i].scene_id == scene_id) {
            if (i < local_scene_count - 1) {
                memmove(&local_scenes[i], &local_scenes[i + 1],
                        (local_scene_count - i - 1) * sizeof(subdev_scene_t));
            }
            local_scene_count--;
            local_automation_save();
            return;
        }
    }
    for (uint8_t i = 0; i < cross_scene_count; i++) {
        if (cross_scenes[i].scene_id == scene_id) {
            if (i < cross_scene_count - 1) {
                memmove(&cross_scenes[i], &cross_scenes[i + 1],
                        (cross_scene_count - i - 1) * sizeof(cross_scene_t));
            }
            cross_scene_count--;
            local_automation_save();
            return;
        }
    }
}

/* ==================== 云端交互 ==================== */
void local_automation_on_rule(const uint8_t *data, uint16_t len) {
    if (len < 2) return;
    uint8_t dev_id = data[0];

    if (dev_id == 0xFF) {
        /* 广播规则到所有子设备 */
        for (uint8_t i = 0; i < zigbee_manager_get_total_count(); i++) {
            const device_info_t *dev = zigbee_manager_get_device(i);
            if (dev && dev->online) {
                control_rule_t rule;
                memcpy(&rule, &data[1], sizeof(control_rule_t));
                zigbee_manager_send_rule(dev->zigbee_id, &rule);
            }
        }
    } else {
        /* 单播到指定子设备 */
        control_rule_t rule;
        memcpy(&rule, &data[1], sizeof(control_rule_t));
        zigbee_manager_send_rule(dev_id, &rule);
    }
}

void local_automation_on_scene(const uint8_t *data, uint16_t len) {
    if (len < 2) return;
    uint8_t scene_type = data[0];
    uint8_t target_dev = data[1];

    if (scene_type == 0 && target_dev != 0xFF) {
        /* 单设备场景 → 转发到子设备 */
        subdev_scene_t scene;
        memcpy(&scene, &data[2], len - 2);
        local_automation_add_local_scene(&scene);
        zigbee_manager_send_scene(target_dev, &scene);
    } else {
        /* 跨设备场景 → T5 本地执行 */
        cross_scene_t scene;
        memset(&scene, 0, sizeof(scene));
        memcpy(&scene, &data[2], len - 2);
        local_automation_add_cross_scene(&scene);
    }
}

void local_automation_on_sensor(uint8_t dev_id, const uint8_t *data, uint16_t len) {
    /* 传感器数据更新 → 检查阈值触发场景 */
    /* 已在 loop 中处理 */
}

void local_automation_sync_to_cloud(void) {
    /* 将本地场景同步到云端 (通过 DP 上报) */
    /* 简化实现: 云端会在连接时拉取 */
}

/* ==================== 场景检查 ==================== */
static void local_automation_check_sensor_scenes(void) {
    uint32_t now = tuya_system_get_millisecond() / 1000;

    for (uint8_t i = 0; i < local_scene_count; i++) {
        subdev_scene_t *sc = &local_scenes[i];
        if (!sc->enabled) continue;

        if (sc->trigger_type != 1) continue;  /* 只检查传感器阈值触发 */

        const sensor_data_t *s = zigbee_manager_get_latest_sensor(0xFF);
        if (!s) continue;

        int32_t val = 0;
        switch (sc->sensor.dpid) {
            case 1: val = (int32_t)(s->temperature * 10); break;
            case 2: val = (int32_t)(s->humidity * 10); break;
            case 3: val = s->light; break;
            default: break;
        }

        if (val >= sc->sensor.threshold_min && val <= sc->sensor.threshold_max) {
            if (sc->cooldown_sec == 0 || (now - sc->last_triggered) >= sc->cooldown_sec) {
                /* 转发场景到子设备 */
                for (uint8_t j = 0; j < zigbee_manager_get_total_count(); j++) {
                    const device_info_t *dev = zigbee_manager_get_device(j);
                    if (dev && dev->online) {
                        zigbee_manager_send_scene(dev->zigbee_id, sc);
                    }
                }
                sc->last_triggered = now;
            }
        }
    }
}

/**
 * 跨设备联动场景检查
 * 支持: 内部子设备 + 外部涂鸦 Zigbee 设备
 */
static void local_automation_check_cross_scenes(void) {
    uint32_t now = tuya_system_get_millisecond() / 1000;

    for (uint8_t i = 0; i < cross_scene_count; i++) {
        cross_scene_t *sc = &cross_scenes[i];
        if (!sc->enabled) continue;

        bool triggered = false;

        /* 检查触发条件 */
        switch (sc->trigger_type) {
            case CROSS_TRIGGER_RELAY:
            {
                const sensor_data_t *s = zigbee_manager_get_latest_sensor(sc->trigger_dev);
                if (s) {
                    uint8_t relay = zigbee_manager_get_relay(sc->trigger_dev);
                    bool is_on = (relay & sc->trigger.relay.relay_bit) != 0;
                    if (is_on == (sc->trigger.relay.state != 0)) triggered = true;
                }
                break;
            }
            case CROSS_TRIGGER_SENSOR:
            {
                const sensor_data_t *s = zigbee_manager_get_latest_sensor(sc->trigger_dev);
                if (s) {
                    int32_t val = 0;
                    switch (sc->trigger_dpid) {
                        case 1: val = (int32_t)(s->temperature * 10); break;
                        case 2: val = (int32_t)(s->humidity * 10); break;
                        case 3: val = s->light; break;
                        default: break;
                    }
                    if (val >= sc->trigger.sensor.threshold_min &&
                        val <= sc->trigger.sensor.threshold_max) {
                        triggered = true;
                    }
                }
                break;
            }
        }

        if (!triggered) continue;
        if (sc->cooldown_sec > 0 && (now - sc->last_triggered) < sc->cooldown_sec) continue;

        /* 执行动作: 根据目标设备类型分发 */
        switch (sc->target_type) {
            case TARGET_TYPE_SUBDEV:
                /* 内部子设备: 直接 Zigbee 控制 */
                zigbee_manager_send_relay(sc->target_dev,
                                          sc->action_relay_mask,
                                          sc->action_relay_state);
                break;

            case TARGET_TYPE_ZIGBEE_EXT:
                /* 外部涂鸦 Zigbee 设备 (灯具/开关) */
                zigbee_manager_send_ext_dp(sc->target_ext_addr,
                                           sc->target_ep,
                                           sc->action_dpid,
                                           sc->action_value_type,
                                           sc->action_value);
                break;

            case TARGET_TYPE_BLE:
                /* 蓝牙设备 (蓝牙灯/开关/传感器) */
                switch (sc->action_dpid) {
                    case 1:  /* 开关 */
                        ble_set_switch(sc->target_dev, sc->action_value);
                        break;
                    case 2:  /* 亮度 */
                        ble_set_brightness(sc->target_dev, sc->action_value);
                        break;
                    case 3:  /* 色温 */
                        ble_set_color_temp(sc->target_dev, sc->action_value);
                        break;
                    case 4:  /* RGB (value 打包为 R<<16|G<<8|B) */
                        ble_set_color_rgb(sc->target_dev,
                                          (sc->action_value >> 16) & 0xFF,
                                          (sc->action_value >> 8) & 0xFF,
                                          sc->action_value & 0xFF);
                        break;
                    default:
                        break;
                }
                break;

            case TARGET_TYPE_WIFI_CLOUD:
                /* WiFi 涂鸦设备: 仅在线时通过云端控制 */
                /* TODO: 通过 cloud_sync 发送云端命令 */
                break;

            default:
                break;
        }

        sc->last_triggered = now;
    }
}

/* ==================== CRON 回调 ==================== */
static void local_automation_cron_check(void *arg) {
    uint32_t now = tuya_system_get_millisecond() / 1000;
    uint8_t hour, minute, second;
    tuya_rtc_get_time(&hour, &minute, &second);

    /* 检查本地定时场景 */
    for (uint8_t i = 0; i < local_scene_count; i++) {
        subdev_scene_t *sc = &local_scenes[i];
        if (!sc->enabled || sc->trigger_type != 0) continue;

        if (sc->timer.hour == hour && sc->timer.minute == minute) {
            for (uint8_t j = 0; j < zigbee_manager_get_total_count(); j++) {
                const device_info_t *dev = zigbee_manager_get_device(j);
                if (dev && dev->online) {
                    zigbee_manager_send_scene(dev->zigbee_id, sc);
                }
            }
        }
    }
}

/* ==================== 持久化 ==================== */
void local_automation_save(void) {
    tuya_kv_set("local_scenes", local_scenes, local_scene_count * sizeof(subdev_scene_t));
    tuya_kv_set("cross_scenes", cross_scenes, cross_scene_count * sizeof(cross_scene_t));
}

void local_automation_load(void) {
    uint32_t size = MAX_LOCAL_SCENES * sizeof(subdev_scene_t);
    if (tuya_kv_get("local_scenes", local_scenes, &size) == 0) {
        local_scene_count = size / sizeof(subdev_scene_t);
    }
    size = MAX_CROSS_SCENES * sizeof(cross_scene_t);
    if (tuya_kv_get("cross_scenes", cross_scenes, &size) == 0) {
        cross_scene_count = size / sizeof(cross_scene_t);
    }
}

void local_automation_factory_reset(void) {
    tuya_kv_delete("local_scenes");
    tuya_kv_delete("cross_scenes");
    local_scene_count = 0;
    cross_scene_count = 0;
}
