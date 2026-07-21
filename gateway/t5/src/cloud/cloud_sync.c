/**
 * 神农系统 T5 网关 - 云端数据同步模块
 *
 * 同步策略:
 * - 实时上报: 传感器数据每2秒上报一次
 * - 批量上报: 历史数据每5分钟批量同步
 * - 断网缓存: 网络断开时缓存数据, 恢复后自动同步
 * - 优先级: 告警 > 状态 > 传感器 > 历史
 */

#include "cloud_sync.h"
#include "memory_manager.h"
#include "tuya_iot.h"
#include "tuya_log.h"
#include "tuya_rtc.h"
#include <string.h>
#include <stdio.h>

/* 缓存队列 */
#define MAX_CACHED_REPORTS  128
static cloud_report_t cached_reports[MAX_CACHED_REPORTS];
static uint16_t cache_head = 0;
static uint16_t cache_tail = 0;

/* 命令队列 */
#define MAX_PENDING_CMDS  16
static cloud_cmd_t pending_cmds[MAX_PENDING_CMDS];
static uint8_t cmd_head = 0;
static uint8_t cmd_tail = 0;

/* 同步状态 */
static cloud_sync_status_t sync_status;

/* 配置 */
static char product_key[32] = "your_product_key";
static char device_secret[64] = "your_device_secret";

/* 同步间隔 */
#define SYNC_INTERVAL_SENSOR_MS    2000     /* 2秒 */
#define SYNC_INTERVAL_HISTORY_MS   300000   /* 5分钟 */
#define SYNC_INTERVAL_STATUS_MS    60000    /* 1分钟 */

static uint32_t last_sensor_sync = 0;
static uint32_t last_history_sync = 0;
static uint32_t last_status_sync = 0;

/* ==================== 初始化 ==================== */
void cloud_sync_init(void) {
    memset(cached_reports, 0, sizeof(cached_reports));
    memset(pending_cmds, 0, sizeof(pending_cmds));
    memset(&sync_status, 0, sizeof(sync_status));

    cache_head = 0;
    cache_tail = 0;
    cmd_head = 0;
    cmd_tail = 0;
    last_sensor_sync = 0;
    last_history_sync = 0;
    last_status_sync = 0;

    TUYA_LOG("云端同步模块初始化完成");
}

/* ==================== 主循环 ==================== */
void cloud_sync_loop(void) {
    uint32_t now = tuya_system_get_millisecond();

    /* 更新连接状态 */
    sync_status.cloud_connected = tuya_iot_cloud_is_connected();
    sync_status.wifi_connected = tuya_iot_wifi_is_connected();

    if (!sync_status.cloud_connected) return;

    /* 缓存同步 */
    if (cache_head != cache_tail) {
        cloud_sync_flush_cache();
    }

    /* 定期同步历史数据 */
    if (now - last_history_sync >= SYNC_INTERVAL_HISTORY_MS) {
        cloud_sync_sync_history();
        last_history_sync = now;
    }

    /* 定期同步状态 */
    if (now - last_status_sync >= SYNC_INTERVAL_STATUS_MS) {
        cloud_sync_sync_status();
        last_status_sync = now;
    }
}

/* ==================== 数据上报 ==================== */
bool cloud_sync_report(cloud_report_type_t type, uint8_t device_id,
                       const void *data, uint16_t len) {
    if (!data || len > 512) return false;

    /* 如果网络断开, 缓存数据 */
    if (!sync_status.cloud_connected) {
        return cloud_sync_cache_report(type, device_id, data, len);
    }

    /* 直接上报 */
    cloud_report_t report;
    memset(&report, 0, sizeof(report));
    report.type = type;
    report.device_id = device_id;
    report.timestamp = tuya_rtc_get_timestamp();
    report.data_len = len;
    memcpy(report.data, data, len);

    /* 转换为涂鸦 DP 格式上报 */
    bool success = cloud_sync_send_dp(&report);

    if (success) {
        sync_status.total_synced++;
        sync_status.last_sync_time = tuya_rtc_get_timestamp();
    } else {
        sync_status.sync_errors++;
        /* 上报失败, 缓存 */
        cloud_sync_cache_report(type, device_id, data, len);
    }

    return success;
}

bool cloud_sync_report_sensor(uint8_t device_id, const void *sensor_data) {
    return cloud_sync_report(CLOUD_REPORT_SENSOR, device_id,
                             sensor_data, sizeof(sensor_history_t));
}

bool cloud_sync_report_history(uint8_t device_id, const void *history_data, uint16_t count) {
    uint16_t len = count * sizeof(sensor_history_t);
    return cloud_sync_report(CLOUD_REPORT_HISTORY, device_id,
                             history_data, len);
}

bool cloud_sync_report_alert(uint8_t device_id, const char *title, const char *content) {
    char data[320];
    int len = snprintf(data, sizeof(data), "{\"title\":\"%s\",\"content\":\"%s\"}",
                       title ? title : "", content ? content : "");
    return cloud_sync_report(CLOUD_REPORT_ALERT, device_id, data, len);
}

/* ==================== 命令接收 ==================== */
bool cloud_sync_get_command(cloud_cmd_t *cmd) {
    if (!cmd || cmd_head == cmd_tail) return false;
    memcpy(cmd, &pending_cmds[cmd_head], sizeof(cloud_cmd_t));
    cmd_head = (cmd_head + 1) % MAX_PENDING_CMDS;
    return true;
}

bool cloud_sync_confirm_command(uint8_t cmd_id, bool success) {
    /* TODO: 向云端确认命令执行结果 */
    TUYA_LOG("命令%d确认: %s", cmd_id, success ? "成功" : "失败");
    return true;
}

/* ==================== 状态查询 ==================== */
bool cloud_sync_get_status(cloud_sync_status_t *status) {
    if (!status) return false;
    memcpy(status, &sync_status, sizeof(cloud_sync_status_t));
    return true;
}

bool cloud_sync_is_connected(void) {
    return sync_status.cloud_connected;
}

/* ==================== 断网缓存 ==================== */
uint32_t cloud_sync_get_cached_count(void) {
    if (cache_tail >= cache_head) return cache_tail - cache_head;
    return MAX_CACHED_REPORTS - cache_head + cache_tail;
}

bool cloud_sync_flush_cache(void) {
    if (!sync_status.cloud_connected) return false;

    uint16_t flushed = 0;
    while (cache_head != cache_tail) {
        cloud_report_t *report = &cached_reports[cache_head];

        if (cloud_sync_send_dp(report)) {
            cache_head = (cache_head + 1) % MAX_CACHED_REPORTS;
            flushed++;
            sync_status.total_synced++;
        } else {
            break;  /* 发送失败, 等待下次重试 */
        }
    }

    sync_status.pending_reports = cloud_sync_get_cached_count();

    if (flushed > 0) {
        TUYA_LOG("云端缓存同步: %d 条已发送, %d 条待发送",
                 flushed, sync_status.pending_reports);
    }

    return flushed > 0;
}

/* ==================== 配置 ==================== */
void cloud_sync_set_product_key(const char *key) {
    if (key) strncpy(product_key, key, sizeof(product_key) - 1);
}

void cloud_sync_set_device_secret(const char *secret) {
    if (secret) strncpy(device_secret, secret, sizeof(device_secret) - 1);
}

/* ==================== 内部函数 ==================== */
static bool cloud_sync_cache_report(cloud_report_type_t type, uint8_t device_id,
                                    const void *data, uint16_t len) {
    uint16_t next = (cache_tail + 1) % MAX_CACHED_REPORTS;
    if (next == cache_head) return false;  /* 缓存满 */

    cloud_report_t *report = &cached_reports[cache_tail];
    memset(report, 0, sizeof(cloud_report_t));
    report->type = type;
    report->device_id = device_id;
    report->timestamp = tuya_rtc_get_timestamp();
    report->data_len = len;
    if (len > 512) len = 512;
    memcpy(report->data, data, len);

    cache_tail = next;
    sync_status.pending_reports = cloud_sync_get_cached_count();
    return true;
}

static bool cloud_sync_send_dp(const cloud_report_t *report) {
    if (!report) return false;

    /* 根据类型选择 DP ID */
    uint8_t dp_id;
    switch (report->type) {
        case CLOUD_REPORT_SENSOR:
            dp_id = 2;   /* DPID_SENSOR_ALL */
            break;
        case CLOUD_REPORT_HISTORY:
            dp_id = 2;   /* 批量数据也用传感器 DP */
            break;
        case CLOUD_REPORT_STATUS:
            dp_id = 6;   /* DPID_GW_STATUS */
            break;
        case CLOUD_REPORT_ALERT:
            dp_id = 7;   /* 自定义告警 DP */
            break;
        default:
            dp_id = 2;
            break;
    }

    /* 调用涂鸦 DP 上报 API */
    TUYA_DP_DATA data = {
        .id = dp_id,
        .type = TUYA_DP_TYPE_STRING,
        .value.string = report->data,
        .len = report->data_len,
    };

    int ret = tuya_iot_dp_report(TUYA_ACCESS_TYPE_LOCAL, &data, 1);
    return (ret == 0);
}

static void cloud_sync_sync_history(void) {
    /* 同步最近1小时的历史数据到云端 */
    sensor_history_t history[12];  /* 每5分钟一条, 1小时12条 */
    uint8_t count = 0;

    for (uint8_t dev = 0; dev < 8; dev++) {
        if (memory_get_sensor_record(dev, history, &count, 12) && count > 0) {
            cloud_sync_report_history(dev, history, count);
        }
    }
}

static void cloud_sync_sync_status(void) {
    /* 同步网关状态 */
    char status_json[256];
    snprintf(status_json, sizeof(status_json),
             "{\"device_count\":%d,\"uptime\":%lu,\"free_heap\":%lu}",
             /* TODO: 获取实际值 */ 0,
             (unsigned long)(tuya_system_get_millisecond() / 1000),
             (unsigned long)esp_get_free_heap_size());

    cloud_sync_report(CLOUD_REPORT_STATUS, 0xFF, status_json, strlen(status_json));
}
