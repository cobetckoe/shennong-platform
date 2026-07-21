/**
 * 神农 T5 网关 - BLE 蓝牙设备管理模块
 *
 * 功能:
 * - BLE 扫描发现设备
 * - BLE 设备连接/断开
 * - BLE 数据收发 (GATT 读写)
 * - BLE 设备状态管理
 * - 与 Zigbee/云平台联动
 */

#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

/* ==================== 常量 ==================== */
#define BLE_MAX_DEVICES      16     /* 最大蓝牙设备数 */
#define BLE_MAX_SERVICES     8      /* 每设备最大服务数 */
#define BLE_MAX_CHARS        16     /* 每服务最大特征数 */
#define BLE_DEVICE_NAME_LEN  32     /* 设备名最大长度 */
#define BLE_MANUFACTURER_LEN 32     /* 厂商名最大长度 */
#define BLE_MAX_DATA_LEN     64     /* 单次收发最大字节 */

/* ==================== BLE 设备类型 ==================== */
#define BLE_TYPE_UNKNOWN     0x00
#define BLE_TYPE_LIGHT       0x01   /* 蓝牙灯 (灯泡/灯带) */
#define BLE_TYPE_SWITCH      0x02   /* 蓝牙开关 */
#define BLE_TYPE_SENSOR      0x03   /* 蓝牙传感器 (温湿度/光照) */
#define BLE_TYPE_LOCK        0x04   /* 蓝牙锁 */
#define BLE_TYPE_RELAY       0x05   /* 蓝牙继电器 */
#define BLE_TYPE_OTHER       0xFF

/* ==================== BLE 设备状态 ==================== */
typedef enum {
    BLE_STATE_IDLE = 0,
    BLE_STATE_SCANNING,
    BLE_STATE_CONNECTING,
    BLE_STATE_CONNECTED,
    BLE_STATE_DISCONNECTING,
} ble_state_t;

/* ==================== BLE 设备信息 ==================== */
typedef struct {
    uint8_t  id;                      /* 内部 ID (0-15) */
    uint8_t  type;                    /* 设备类型 */
    ble_state_t state;                /* 连接状态 */
    char     name[BLE_DEVICE_NAME_LEN]; /* 设备名 */
    char     manufacturer[BLE_MANUFACTURER_LEN]; /* 厂商 */
    uint8_t  mac[6];                  /* MAC 地址 */
    int8_t   rssi;                    /* 信号强度 */
    uint8_t  battery;                 /* 电量 (0-100) */

    /* 设备能力 */
    bool     has_switch;              /* 有开关功能 */
    bool     has_brightness;          /* 有亮度调节 */
    bool     has_color_temp;          /* 有色温调节 */
    bool     has_color_rgb;           /* 有 RGB 调色 */
    bool     has_sensor_temp;         /* 有温度传感器 */
    bool     has_sensor_humi;         /* 有湿度传感器 */
    bool     has_sensor_light;        /* 有光照传感器 */

    /* 当前状态 */
    bool     switch_on;               /* 开关状态 */
    uint8_t  brightness;              /* 亮度 0-100 */
    uint16_t color_temp;              /* 色温 2700-6500 */
    uint8_t  color_r, color_g, color_b; /* RGB */
    float    temperature;             /* 温度 ℃ */
    float    humidity;                /* 湿度 % */
    uint32_t light;                   /* 光照 lux */

    /* GATT 信息 (用于读写) */
    uint16_t service_uuid;
    uint16_t char_switch_uuid;
    uint16_t char_brightness_uuid;
    uint16_t char_color_temp_uuid;
    uint16_t char_color_rgb_uuid;
    uint16_t char_temp_uuid;
    uint16_t char_humi_uuid;
    uint16_t char_light_uuid;

    uint32_t last_seen;               /* 最后活动时间 */
    bool     registered;              /* 是否已注册到联动系统 */
} ble_device_t;

/* ==================== 回调函数类型 ==================== */
typedef void (*ble_device_cb_t)(uint8_t dev_id, bool online);
typedef void (*ble_data_cb_t)(uint8_t dev_id, uint16_t uuid, const uint8_t *data, uint16_t len);

/* ==================== 接口函数 ==================== */

/* 初始化/主循环 */
void ble_manager_init(void);
void ble_manager_loop(void);

/* 扫描控制 */
bool ble_start_scan(uint16_t duration_sec);
bool ble_stop_scan(void);
bool ble_is_scanning(void);

/* 设备连接/断开 */
bool ble_connect(uint8_t dev_id);
bool ble_disconnect(uint8_t dev_id);

/* 设备控制 */
bool ble_set_switch(uint8_t dev_id, bool on);
bool ble_set_brightness(uint8_t dev_id, uint8_t brightness);
bool ble_set_color_temp(uint8_t dev_id, uint16_t temp);
bool ble_set_color_rgb(uint8_t dev_id, uint8_t r, uint8_t g, uint8_t b);

/* 数据读写 */
bool ble_write_char(uint8_t dev_id, uint16_t uuid, const uint8_t *data, uint16_t len);
bool ble_read_char(uint8_t dev_id, uint16_t uuid);

/* 设备查询 */
const ble_device_t* ble_get_device(uint8_t dev_id);
const ble_device_t* ble_get_device_by_mac(const uint8_t *mac);
uint8_t ble_get_device_count(void);
uint8_t ble_get_online_count(void);

/* 设备注册 (用于联动) */
bool ble_register_device(uint8_t dev_id);
bool ble_unregister_device(uint8_t dev_id);

/* 回调注册 */
void ble_set_device_callback(ble_device_cb_t cb);
void ble_set_data_callback(ble_data_cb_t cb);

/* 工具函数 */
const char* ble_type_name(uint8_t type);
const char* ble_state_name(ble_state_t state);

#endif /* BLE_MANAGER_H */
