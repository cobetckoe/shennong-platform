/**
 * 神农系统 T5 网关 - Zigbee 子设备管理器
 * 管理子设备注册、数据收集、命令下发
 */

#include "zigbee_manager.h"
#include "tuya_iot_subdev.h"
#include "tuya_zigbee.h"
#include <string.h>

#define MAX_DEVICES     64
#define HEARTBEAT_TIMEOUT_MS   60000

typedef struct {
    uint8_t zigbee_id;
    uint8_t dev_type;
    char name[32];
    uint8_t subdev_id;          /* 涂鸦虚拟子设备 ID */
    bool online;
    bool registered;
    sensor_data_t last_sensor;
    uint8_t last_relay;
    uint32_t last_seen;
} device_entry_t;

static device_entry_t devices[MAX_DEVICES];
static uint8_t device_count = 0;
static zigbee_rx_cb_t rx_callback = NULL;

static const char* get_type_name(uint8_t type) {
    switch (type) {
        case 0x01: return "rootcrop";
        case 0x02: return "leafcrop";
        case 0x03: return "stemcrop";
        case 0x04: return "flowercrop";
        case 0x05: return "fruitcrop";
        default:   return "device";
    }
}

void zigbee_manager_init(zigbee_rx_cb_t cb) {
    rx_callback = cb;
    device_count = 0;
    memset(devices, 0, sizeof(devices));
    tuya_zigbee_init();
}

void zigbee_manager_loop(void) {
    uint32_t now = tuya_system_get_millisecond();
    for (uint8_t i = 0; i < device_count; i++) {
        if (devices[i].online && (now - devices[i].last_seen > HEARTBEAT_TIMEOUT_MS)) {
            devices[i].online = false;
            tuya_iot_subdev_offline(devices[i].subdev_id);
        }
    }
}

void zigbee_manager_on_heartbeat(uint8_t dev_id, const uint8_t *data, uint16_t len) {
    device_entry_t *dev = find_or_create(dev_id);
    if (!dev) return;

    dev->last_seen = tuya_system_get_millisecond();
    if (!dev->online) {
        dev->online = true;
        dev->dev_type = data[0];
        snprintf(dev->name, sizeof(dev->name), "%s_%02d",
                 get_type_name(dev->dev_type), dev_id);
        tuya_iot_subdev_online(dev->subdev_id);
    }
}

void zigbee_manager_on_sensor_data(uint8_t dev_id, const uint8_t *data, uint16_t len) {
    device_entry_t *dev = find_or_create(dev_id);
    if (!dev || len < sizeof(sensor_data_t)) return;

    memcpy(&dev->last_sensor, data, sizeof(sensor_data_t));
    dev->last_seen = tuya_system_get_millisecond();

    /* 同步到涂鸦虚拟子设备 */
    tuya_iot_subdev_dp_report(dev->subdev_id, 1, DP_TYPE_VALUE,
        (uint8_t[]){LOWORD((int32_t)(dev->last_sensor.temperature * 10)),
                    HIWORD((int32_t)(dev->last_sensor.temperature * 10))}, 4);
    tuya_iot_subdev_dp_report(dev->subdev_id, 2, DP_TYPE_VALUE,
        (uint8_t[]){LOWORD((int32_t)(dev->last_sensor.humidity * 10)),
                    HIWORD((int32_t)(dev->last_sensor.humidity * 10))}, 4);
}

void zigbee_manager_on_manual_cmd(const uint8_t *data, uint16_t len) {
    if (len < 3) return;
    uint8_t dev_id = data[0];
    uint8_t mask = data[1];
    uint8_t state = data[2];
    zigbee_manager_send_relay(dev_id, mask, state);
}

void zigbee_manager_send_relay(uint8_t dev_id, uint8_t mask, uint8_t state) {
    uint8_t tx[4] = {CMD_RELAY_SET, dev_id, mask, state};
    tuya_zigbee_send(dev_id, tx, sizeof(tx));

    /* 更新本地状态 */
    device_entry_t *dev = find_by_id(dev_id);
    if (dev) {
        if (state) dev->last_relay |= mask;
        else dev->last_relay &= ~mask;
        tuya_iot_subdev_dp_report(dev->subdev_id, 5, DP_TYPE_BOOL,
            (uint8_t[]){(dev->last_relay & RELAY_BIT_PUMP) ? 1 : 0}, 1);
        tuya_iot_subdev_dp_report(dev->subdev_id, 6, DP_TYPE_BOOL,
            (uint8_t[]){(dev->last_relay & RELAY_BIT_FAN) ? 1 : 0}, 1);
        tuya_iot_subdev_dp_report(dev->subdev_id, 7, DP_TYPE_BOOL,
            (uint8_t[]){(dev->last_relay & RELAY_BIT_LIGHT) ? 1 : 0}, 1);
    }
}

void zigbee_manager_send_rule(uint8_t dev_id, const control_rule_t *rule) {
    uint8_t tx[32];
    tx[0] = CMD_RULE_UPDATE;
    tx[1] = dev_id;
    memcpy(&tx[2], rule, sizeof(control_rule_t));
    tuya_zigbee_send(dev_id, tx, sizeof(control_rule_t) + 2);
}

void zigbee_manager_send_scene(uint8_t dev_id, const subdev_scene_t *scene) {
    uint8_t tx[32];
    tx[0] = CMD_SCENE_SYNC;
    tx[1] = dev_id;
    memcpy(&tx[2], scene, sizeof(subdev_scene_t));
    tuya_zigbee_send(dev_id, tx, sizeof(subdev_scene_t) + 2);
}

void zigbee_manager_report_all(void) {
    for (uint8_t i = 0; i < device_count; i++) {
        if (devices[i].online) {
            zigbee_manager_report_sensor(devices[i].zigbee_id);
        }
    }
}

void zigbee_manager_report_sensor(uint8_t dev_id) {
    device_entry_t *dev = find_by_id(dev_id);
    if (dev) {
        zigbee_manager_on_sensor_data(dev_id, (uint8_t*)&dev->last_sensor, sizeof(sensor_data_t));
    }
}

const sensor_data_t* zigbee_manager_get_latest_sensor(uint8_t dev_id) {
    if (dev_id == 0xFF) {
        /* 返回任意在线设备的传感器数据 */
        for (uint8_t i = 0; i < device_count; i++) {
            if (devices[i].online) return &devices[i].last_sensor;
        }
        return NULL;
    }
    device_entry_t *dev = find_by_id(dev_id);
    return dev ? &dev->last_sensor : NULL;
}

uint8_t zigbee_manager_get_online_count(void) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < device_count; i++) {
        if (devices[i].online) count++;
    }
    return count;
}

uint8_t zigbee_manager_get_total_count(void) {
    return device_count;
}

static device_entry_t* find_or_create(uint8_t id) {
    for (uint8_t i = 0; i < device_count; i++) {
        if (devices[i].zigbee_id == id) return &devices[i];
    }
    if (device_count < MAX_DEVICES) {
        memset(&devices[device_count], 0, sizeof(device_entry_t));
        devices[device_count].zigbee_id = id;
        devices[device_count].subdev_id = device_count + 1;
        return &devices[device_count++];
    }
    return NULL;
}

static device_entry_t* find_by_id(uint8_t id) {
    for (uint8_t i = 0; i < device_count; i++) {
        if (devices[i].zigbee_id == id) return &devices[i];
    }
    return NULL;
}

/* ==================== 外部 Zigbee 设备控制 ==================== */

/**
 * 向外部涂鸦 Zigbee 设备发送 DP 命令
 * 用于控制灯具、开关、传感器等涂鸦 Zigbee 设备
 */
bool zigbee_manager_send_ext_dp(uint16_t ext_addr, uint8_t ep,
                                 uint8_t dpid, uint8_t type, uint32_t value) {
    /*
     * 涂鸦 Zigbee DP 命令格式:
     * [0x00][cluster_id][ext_addr_lo][ext_addr_hi][ep][dpid][type][len][value...]
     *
     * cluster_id: 0x0006 (OnOff), 0x0008 (Level), 0x0300 (Color)
     */
    uint8_t tx[16];
    uint16_t cluster_id;

    /* 根据 DPID 选择 Cluster */
    switch (dpid) {
        case 1:  cluster_id = 0x0006; break;  /* OnOff */
        case 2:  cluster_id = 0x0008; break;  /* Level Control */
        case 3:  cluster_id = 0x0300; break;  /* Color Control */
        default: cluster_id = 0x0006; break;
    }

    uint8_t len = 0;
    tx[len++] = 0x00;                        /* 命令类型 */
    tx[len++] = (uint8_t)(cluster_id & 0xFF);
    tx[len++] = (uint8_t)(cluster_id >> 8);
    tx[len++] = (uint8_t)(ext_addr & 0xFF);
    tx[len++] = (uint8_t)(ext_addr >> 8);
    tx[len++] = ep;
    tx[len++] = dpid;
    tx[len++] = type;

    /* 值 */
    if (type == DP_TYPE_BOOL) {
        tx[len++] = 1;
        tx[len++] = value ? 1 : 0;
    } else if (type == DP_TYPE_VALUE) {
        tx[len++] = 4;
        tx[len++] = value & 0xFF;
        tx[len++] = (value >> 8) & 0xFF;
        tx[len++] = (value >> 16) & 0xFF;
        tx[len++] = (value >> 24) & 0xFF;
    }

    /* 通过 Zigbee 协调器发送 */
    tuya_zigbee_send_ext(ext_addr, ep, tx, len);
    return true;
}

/**
 * 控制外部开关设备
 */
bool zigbee_manager_control_ext_switch(uint16_t ext_addr, uint8_t ep, bool on) {
    return zigbee_manager_send_ext_dp(ext_addr, ep, 1, DP_TYPE_BOOL, on ? 1 : 0);
}

/**
 * 控制外部灯具亮度
 */
bool zigbee_manager_control_ext_brightness(uint16_t ext_addr, uint8_t ep, uint8_t brightness) {
    /* 亮度 0-100 → 0-254 */
    uint8_t level = (uint8_t)((uint16_t)brightness * 254 / 100);
    return zigbee_manager_send_ext_dp(ext_addr, ep, 2, DP_TYPE_VALUE, level);
}

/**
 * 控制外部灯具色温
 */
bool zigbee_manager_control_ext_color_temp(uint16_t ext_addr, uint8_t ep, uint16_t temp) {
    return zigbee_manager_send_ext_dp(ext_addr, ep, 3, DP_TYPE_VALUE, temp);
}

/* ==================== 外部设备信息查询 ==================== */
const device_info_t* zigbee_manager_get_device(uint8_t index) {
    if (index >= device_count) return NULL;
    return (const device_info_t*)&devices[index];
}

uint8_t zigbee_manager_get_relay(uint8_t dev_id) {
    device_entry_t *dev = find_by_id(dev_id);
    return dev ? dev->last_relay : 0;
}
