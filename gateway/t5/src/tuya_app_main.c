/**
 * 神农系统 T5 网关 - 主入口
 * 基于 Wukong SDK + 本地自动化
 *
 * 功能模块:
 * - Wukong SDK: AI Agent + 语音 + 记忆 + MCP 工具
 * - Zigbee 子设备管理 (zigbee/)
 * - BLE 蓝牙设备管理 (ble/)
 * - 本地自动化 (CRON + 阈值 + 联动)
 * - 云端数据同步 (cloud/)
 * - 图像识别 (camera/)
 */

#include "tuya_app_main.h"
#include "tuya_iot.h"
#include "tuya_cron.h"
#include "tuya_log.h"

/* 模块头文件 */
#include "zigbee_manager.h"
#include "ble_manager.h"
#include "local_automation.h"
#include "camera_module.h"
#include "cloud_sync.h"

/* Wukong SDK 头文件 (待集成) */
// #include "wukong.h"

#include <string.h>
#include <stdio.h>

/* 产品配置 */
#define PRODUCT_KEY     "your_product_key"
#define PRODUCT_SECRET  "your_product_secret"
#define DEVICE_NAME     "shennong_gateway"
#define DEVICE_SECRET   "your_device_secret"

/* ==================== 系统状态 ==================== */
static bool cloud_connected = false;
static bool wifi_connected = false;

/* ==================== 云端回调 ==================== */
static void on_wifi_event(TUYA_EVENT_WIFI_STATUS_E status) {
    wifi_connected = (status == TUYA_WIFI_CONNECTED);
    if (wifi_connected) {
        TUYA_LOG("WiFi 已连接");
    }
}

static void on_cloud_event(TUYA_EVENT_CLOUD_STATUS_E status) {
    cloud_connected = (status == TUYA_CLOUD_CONNECTED);
    if (cloud_connected) {
        TUYA_LOG("云端已连接");
        zigbee_manager_report_all();
        local_automation_sync_to_cloud();
    }
}

static void on_dp_cmd(uint8_t dp_id, uint8_t *data, uint16_t len) {
    TUYA_LOG("收到云端命令: DPID=%d, 长度=%d", dp_id, len);

    switch (dp_id) {
        case DPID_RULE_UPDATE:
            /* 云端下发规则 */
            local_automation_on_rule(data, len);
            break;

        case DPID_SCENE_UPDATE:
            /* 云端下发场景 */
            local_automation_on_scene(data, len);
            break;

        case DPID_MANUAL_CMD:
            /* 手动控制 */
            zigbee_manager_on_manual_cmd(data, len);
            break;

        case DPID_CONFIG_UPDATE:
            TUYA_LOG("收到配置更新");
            break;

        default:
            break;
    }
}

/* ==================== Zigbee 接收回调 ==================== */
static void on_zigbee_rx(uint8_t dev_id, uint8_t cmd, const uint8_t *data, uint16_t len) {
    switch (cmd) {
        case CMD_HEARTBEAT:
            zigbee_manager_on_heartbeat(dev_id, data, len);
            break;

        case CMD_SENSOR_DATA:
            zigbee_manager_on_sensor_data(dev_id, data, len);

            /* 触发本地自动化检查 */
            local_automation_on_sensor(dev_id, data, len);

            /* 上报到云端 (标准 DP) */
            if (cloud_connected) {
                zigbee_manager_report_sensor(dev_id);
            }
            break;

        case CMD_RELAY_STATE:
            /* 继电器状态上报 */
            break;

        default:
            break;
    }
}

/* ==================== 图像识别回调 ==================== */
static void on_image_recognized(uint8_t dev_id, const image_result_t *result) {
    if (result->has_disease) {
        TUYA_LOG("检测到病虫害: %s (置信度 %.2f)",
                 result->disease_name, result->confidence);

        /* 上报云端 */
        if (cloud_connected) {
            zigbee_manager_report_disease(dev_id, result);
        }
    }

    zigbee_manager_update_image_status(dev_id, result);
}

/* ==================== 定时任务回调 ==================== */
static void on_time_sync(uint8_t hour, uint8_t minute, uint8_t weekday) {
    /* Wukong SDK 会自动处理时间同步 */
}

/* ==================== 系统初始化 ==================== */
void tuya_app_main(void) {
    TUYA_LOG("=== 神农系统 T5 网关启动 ===");
    TUYA_LOG("版本: Wukong SDK + 本地自动化");

    /* 1. 云端同步初始化 */
    cloud_sync_init();

    /* 2. 摄像头模块初始化 */
    camera_module_init(on_image_recognized);

    /* 3. Zigbee 管理器初始化 */
    zigbee_manager_init(on_zigbee_rx);

    /* 4. BLE 蓝牙管理器初始化 */
    ble_manager_init();

    /* 5. 本地自动化初始化 */
    local_automation_init();

    /* 6. 涂鸦 IoT 初始化 */
    tuya_iot_config_t config = {
        .product_key = PRODUCT_KEY,
        .product_secret = PRODUCT_SECRET,
        .device_name = DEVICE_NAME,
        .device_secret = DEVICE_SECRET,
        .wifi_event_cb = on_wifi_event,
        .cloud_event_cb = on_cloud_event,
        .dp_cmd_cb = on_dp_cmd,
    };
    tuya_iot_init(&config);

    /* 7. 注册时间同步 CRON */
    tuya_cron_create("time_sync", "0 * * * * *",
                     (tuya_cron_callback_t)on_time_sync, NULL);

    /* 8. Wukong SDK 初始化 (待集成) */
    // wukong_init();

    TUYA_LOG("=== 所有模块初始化完成 ===");

    /* ==================== 主循环 ==================== */
    while (1) {
        /* 涂鸦 IoT 网络处理 */
        tuya_iot_loop();

        /* Zigbee 通信处理 */
        zigbee_manager_loop();

        /* BLE 蓝牙处理 */
        ble_manager_loop();

        /* 本地自动化检查 */
        local_automation_loop();

        /* 云端数据同步 */
        cloud_sync_loop();

        /* 摄像头采集 (每5分钟) */
        camera_module_loop();

        /* Wukong SDK 主循环 (待集成) */
        // wukong_loop();

        /* 主循环休眠 */
        tuya_system_sleep(10);
    }
}
