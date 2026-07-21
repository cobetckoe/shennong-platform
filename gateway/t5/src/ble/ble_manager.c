/**
 * 神农 T5 网关 - BLE 蓝牙设备管理模块
 *
 * 工作流程:
 * 1. BLE 扫描发现设备
 * 2. 自动识别设备类型 (通过厂商数据/服务UUID)
 * 3. 连接设备并订阅通知
 * 4. 控制设备 (开关/亮度/色温等)
 * 5. 接收传感器数据并同步到联动系统
 */

#include "ble_manager.h"
#include "tuya_ble.h"
#include "tuya_log.h"
#include "tuya_kv.h"
#include <string.h>

/* KV 存储键名 */
#define KV_BLE_DEVICES  "ble_devices"

/* 设备列表 */
static ble_device_t devices[BLE_MAX_DEVICES];
static uint8_t device_count = 0;

/* 回调函数 */
static ble_device_cb_t device_callback = NULL;
static ble_data_cb_t data_callback = NULL;

/* 扫描状态 */
static bool scanning = false;
static uint32_t scan_start_time = 0;
static uint16_t scan_duration = 0;

/* 已知设备 MAC 白名单 (用于自动连接) */
#define MAX_WHITELIST  16
static uint8_t whitelist[MAX_WHITELIST][6];
static uint8_t whitelist_count = 0;

/* ==================== 内部函数 ==================== */
static ble_device_t* find_or_create_device(const uint8_t *mac) {
    /* 查找已有设备 */
    for (uint8_t i = 0; i < device_count; i++) {
        if (memcmp(devices[i].mac, mac, 6) == 0) {
            return &devices[i];
        }
    }

    /* 新建设备 */
    if (device_count < BLE_MAX_DEVICES) {
        ble_device_t *dev = &devices[device_count];
        memset(dev, 0, sizeof(ble_device_t));
        dev->id = device_count;
        memcpy(dev->mac, mac, 6);
        dev->state = BLE_STATE_IDLE;
        device_count++;
        return dev;
    }

    return NULL;
}

static void identify_device_type(ble_device_t *dev, const uint8_t *adv_data, uint16_t len) {
    /* 根据广播数据识别设备类型 */

    /* TODO: 解析广播数据中的厂商特定数据和服务 UUID */

    /* 示例: 根据设备名判断 */
    if (strstr(dev->name, "灯") || strstr(dev->name, "Light") || strstr(dev->name, "Bulb")) {
        dev->type = BLE_TYPE_LIGHT;
        dev->has_switch = true;
        dev->has_brightness = true;
        dev->has_color_temp = true;
    } else if (strstr(dev->name, "开关") || strstr(dev->name, "Switch")) {
        dev->type = BLE_TYPE_SWITCH;
        dev->has_switch = true;
    } else if (strstr(dev->name, "传感器") || strstr(dev->name, "Sensor") ||
               strstr(dev->name, "温湿度")) {
        dev->type = BLE_TYPE_SENSOR;
        dev->has_sensor_temp = true;
        dev->has_sensor_humi = true;
    }
}

/* ==================== 初始化 ==================== */
void ble_manager_init(void) {
    memset(devices, 0, sizeof(devices));
    device_count = 0;
    scanning = false;
    whitelist_count = 0;

    /* 初始化 BLE 栈 */
    tuya_ble_init();

    /* 从 flash 加载已绑定设备 */
    uint32_t size = sizeof(devices);
    if (tuya_kv_get(KV_BLE_DEVICES, devices, &size) == 0) {
        device_count = size / sizeof(ble_device_t);
        TUYA_LOG("BLE 加载 %d 个已绑定设备", device_count);
    }

    TUYA_LOG("BLE 管理器初始化完成");
}

/* ==================== 主循环 ==================== */
void ble_manager_loop(void) {
    uint32_t now = tuya_system_get_millisecond();

    /* 检查扫描超时 */
    if (scanning && (now - scan_start_time >= scan_duration * 1000)) {
        ble_stop_scan();
    }

    /* 检查设备连接状态 */
    for (uint8_t i = 0; i < device_count; i++) {
        ble_device_t *dev = &devices[i];

        if (dev->state == BLE_STATE_CONNECTED) {
            /* 检查连接超时 */
            if (now - dev->last_seen > 30000) {
                TUYA_LOG("BLE 设备 %s 连接超时", dev->name);
                dev->state = BLE_STATE_DISCONNECTED;
                if (device_callback) {
                    device_callback(dev->id, false);
                }
            }
        }
    }
}

/* ==================== 扫描控制 ==================== */
bool ble_start_scan(uint16_t duration_sec) {
    if (scanning) return false;

    tuya_ble_scan_start();
    scanning = true;
    scan_start_time = tuya_system_get_millisecond();
    scan_duration = duration_sec;

    TUYA_LOG("BLE 开始扫描 (%d秒)", duration_sec);
    return true;
}

bool ble_stop_scan(void) {
    if (!scanning) return true;

    tuya_ble_scan_stop();
    scanning = false;

    TUYA_LOG("BLE 停止扫描, 发现 %d 个设备", device_count);
    return true;
}

bool ble_is_scanning(void) {
    return scanning;
}

/* ==================== 设备连接/断开 ==================== */
bool ble_connect(uint8_t dev_id) {
    if (dev_id >= device_count) return false;

    ble_device_t *dev = &devices[dev_id];
    if (dev->state == BLE_STATE_CONNECTED) return true;
    if (dev->state == BLE_STATE_CONNECTING) return false;

    dev->state = BLE_STATE_CONNECTING;
    int ret = tuya_ble_connect(dev->mac);

    if (ret == 0) {
        TUYA_LOG("BLE 连接设备: %s", dev->name);
        return true;
    } else {
        dev->state = BLE_STATE_IDLE;
        return false;
    }
}

bool ble_disconnect(uint8_t dev_id) {
    if (dev_id >= device_count) return false;

    ble_device_t *dev = &devices[dev_id];
    if (dev->state != BLE_STATE_CONNECTED) return true;

    dev->state = BLE_STATE_DISCONNECTING;
    tuya_ble_disconnect(dev->mac);
    dev->state = BLE_STATE_IDLE;

    TUYA_LOG("BLE 断开设备: %s", dev->name);
    return true;
}

/* ==================== 设备控制 ==================== */
bool ble_set_switch(uint8_t dev_id, bool on) {
    if (dev_id >= device_count) return false;

    ble_device_t *dev = &devices[dev_id];
    if (dev->state != BLE_STATE_CONNECTED || !dev->has_switch) return false;

    uint8_t data = on ? 0x01 : 0x00;
    bool ok = ble_write_char(dev_id, dev->char_switch_uuid, &data, 1);

    if (ok) {
        dev->switch_on = on;
        TUYA_LOG("BLE 设备 %s 开关: %s", dev->name, on ? "开" : "关");
    }
    return ok;
}

bool ble_set_brightness(uint8_t dev_id, uint8_t brightness) {
    if (dev_id >= device_count) return false;

    ble_device_t *dev = &devices[dev_id];
    if (dev->state != BLE_STATE_CONNECTED || !dev->has_brightness) return false;

    uint8_t data = (uint8_t)((uint16_t)brightness * 254 / 100);
    bool ok = ble_write_char(dev_id, dev->char_brightness_uuid, &data, 1);

    if (ok) {
        dev->brightness = brightness;
        TUYA_LOG("BLE 设备 %s 亮度: %d%%", dev->name, brightness);
    }
    return ok;
}

bool ble_set_color_temp(uint8_t dev_id, uint16_t temp) {
    if (dev_id >= device_count) return false;

    ble_device_t *dev = &devices[dev_id];
    if (dev->state != BLE_STATE_CONNECTED || !dev->has_color_temp) return false;

    uint8_t data[2] = {temp & 0xFF, temp >> 8};
    bool ok = ble_write_char(dev_id, dev->char_color_temp_uuid, data, 2);

    if (ok) {
        dev->color_temp = temp;
        TUYA_LOG("BLE 设备 %s 色温: %dK", dev->name, temp);
    }
    return ok;
}

bool ble_set_color_rgb(uint8_t dev_id, uint8_t r, uint8_t g, uint8_t b) {
    if (dev_id >= device_count) return false;

    ble_device_t *dev = &devices[dev_id];
    if (dev->state != BLE_STATE_CONNECTED || !dev->has_color_rgb) return false;

    uint8_t data[3] = {r, g, b};
    bool ok = ble_write_char(dev_id, dev->char_color_rgb_uuid, data, 3);

    if (ok) {
        dev->color_r = r;
        dev->color_g = g;
        dev->color_b = b;
        TUYA_LOG("BLE 设备 %s RGB: %d,%d,%d", dev->name, r, g, b);
    }
    return ok;
}

/* ==================== 数据读写 ==================== */
bool ble_write_char(uint8_t dev_id, uint16_t uuid, const uint8_t *data, uint16_t len) {
    if (dev_id >= device_count) return false;

    ble_device_t *dev = &devices[dev_id];
    if (dev->state != BLE_STATE_CONNECTED) return false;

    return tuya_ble_write_char(dev->mac, uuid, data, len) == 0;
}

bool ble_read_char(uint8_t dev_id, uint16_t uuid) {
    if (dev_id >= device_count) return false;

    ble_device_t *dev = &devices[dev_id];
    if (dev->state != BLE_STATE_CONNECTED) return false;

    return tuya_ble_read_char(dev->mac, uuid) == 0;
}

/* ==================== 设备查询 ==================== */
const ble_device_t* ble_get_device(uint8_t dev_id) {
    if (dev_id >= device_count) return NULL;
    return &devices[dev_id];
}

const ble_device_t* ble_get_device_by_mac(const uint8_t *mac) {
    for (uint8_t i = 0; i < device_count; i++) {
        if (memcmp(devices[i].mac, mac, 6) == 0) {
            return &devices[i];
        }
    }
    return NULL;
}

uint8_t ble_get_device_count(void) {
    return device_count;
}

uint8_t ble_get_online_count(void) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < device_count; i++) {
        if (devices[i].state == BLE_STATE_CONNECTED) count++;
    }
    return count;
}

/* ==================== 设备注册 ==================== */
bool ble_register_device(uint8_t dev_id) {
    if (dev_id >= device_count) return false;
    devices[dev_id].registered = true;

    /* 保存到 flash */
    tuya_kv_set(KV_BLE_DEVICES, devices, device_count * sizeof(ble_device_t));
    return true;
}

bool ble_unregister_device(uint8_t dev_id) {
    if (dev_id >= device_count) return false;
    devices[dev_id].registered = false;

    tuya_kv_set(KV_BLE_DEVICES, devices, device_count * sizeof(ble_device_t));
    return true;
}

/* ==================== 回调注册 ==================== */
void ble_set_device_callback(ble_device_cb_t cb) {
    device_callback = cb;
}

void ble_set_data_callback(ble_data_cb_t cb) {
    data_callback = cb;
}

/* ==================== 工具函数 ==================== */
const char* ble_type_name(uint8_t type) {
    switch (type) {
        case BLE_TYPE_LIGHT:   return "蓝牙灯";
        case BLE_TYPE_SWITCH:  return "蓝牙开关";
        case BLE_TYPE_SENSOR:  return "蓝牙传感器";
        case BLE_TYPE_LOCK:    return "蓝牙锁";
        case BLE_TYPE_RELAY:   return "蓝牙继电器";
        default:               return "未知设备";
    }
}

const char* ble_state_name(ble_state_t state) {
    switch (state) {
        case BLE_STATE_IDLE:         return "空闲";
        case BLE_STATE_SCANNING:     return "扫描中";
        case BLE_STATE_CONNECTING:   return "连接中";
        case BLE_STATE_CONNECTED:    return "已连接";
        case BLE_STATE_DISCONNECTING: return "断开中";
        default:                     return "未知";
    }
}

/* ==================== 涂鸦 BLE 事件回调 ==================== */
void tuya_ble_event_handler(tuya_ble_evt_t *evt) {
    switch (evt->type) {
        case TUYA_BLE_EVT_SCAN_RESULT: {
            /* 发现新设备 */
            ble_device_t *dev = find_or_create_device(evt->scan.mac);
            if (dev) {
                strncpy(dev->name, evt->scan.name, BLE_DEVICE_NAME_LEN - 1);
                dev->rssi = evt->scan.rssi;
                identify_device_type(dev, evt->scan.adv_data, evt->scan.adv_len);
                TUYA_LOG("BLE 发现设备: %s (%s, RSSI=%d)",
                         dev->name, ble_type_name(dev->type), dev->rssi);
            }
            break;
        }

        case TUYA_BLE_EVT_CONNECTED: {
            /* 设备已连接 */
            ble_device_t *dev = ble_get_device_by_mac(evt->conn.mac);
            if (dev) {
                dev->state = BLE_STATE_CONNECTED;
                dev->last_seen = tuya_system_get_millisecond();
                TUYA_LOG("BLE 已连接: %s", dev->name);
                if (device_callback) {
                    device_callback(dev->id, true);
                }
            }
            break;
        }

        case TUYA_BLE_EVT_DISCONNECTED: {
            /* 设备已断开 */
            ble_device_t *dev = ble_get_device_by_mac(evt->disconn.mac);
            if (dev) {
                dev->state = BLE_STATE_IDLE;
                TUYA_LOG("BLE 已断开: %s", dev->name);
                if (device_callback) {
                    device_callback(dev->id, false);
                }
            }
            break;
        }

        case TUYA_BLE_EVT_NOTIFY: {
            /* 收到数据通知 */
            ble_device_t *dev = ble_get_device_by_mac(evt->notify.mac);
            if (dev) {
                dev->last_seen = tuya_system_get_millisecond();

                /* 根据 UUID 解析数据 */
                if (evt->notify.uuid == dev->char_temp_uuid && dev->has_sensor_temp) {
                    dev->temperature = *(float*)evt->notify.data;
                } else if (evt->notify.uuid == dev->char_humi_uuid && dev->has_sensor_humi) {
                    dev->humidity = *(float*)evt->notify.data;
                } else if (evt->notify.uuid == dev->char_light_uuid && dev->has_sensor_light) {
                    dev->light = *(uint32_t*)evt->notify.data;
                }

                if (data_callback) {
                    data_callback(dev->id, evt->notify.uuid, evt->notify.data, evt->notify.len);
                }
            }
            break;
        }

        default:
            break;
    }
}
