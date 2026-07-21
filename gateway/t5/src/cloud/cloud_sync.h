/**
 * 神农 T5 网关 - 云端数据同步模块
 * 负责:
 * - 传感器数据批量上报
 * - 历史数据同步
 * - 智能决策下发接收
 * - 设备状态同步
 * - 断网缓存 & 恢复同步
 */

#ifndef CLOUD_SYNC_H
#define CLOUD_SYNC_H

#include <stdint.h>
#include <stdbool.h>

/* ==================== 数据上报类型 ==================== */
typedef enum {
    CLOUD_REPORT_SENSOR,           /* 传感器数据 */
    CLOUD_REPORT_HISTORY,          /* 历史数据 */
    CLOUD_REPORT_STATUS,           /* 设备状态 */
    CLOUD_REPORT_ALERT,            /* 告警信息 */
    CLOUD_REPORT_DECISION_LOG,     /* 决策日志 */
    CLOUD_REPORT_STATISTICS,       /* 统计数据 */
} cloud_report_type_t;

/* ==================== 上报数据包 ==================== */
typedef struct {
    cloud_report_type_t type;
    uint8_t device_id;
    uint32_t timestamp;
    uint16_t data_len;
    uint8_t data[512];
} cloud_report_t;

/* ==================== 下发命令 ==================== */
typedef enum {
    CLOUD_CMD_RULE_UPDATE,          /* 规则更新 */
    CLOUD_CMD_SCENE_UPDATE,         /* 场景更新 */
    CLOUD_CMD_MANUAL_CMD,           /* 手动控制 */
    CLOUD_CMD_CONFIG_UPDATE,        /* 配置更新 */
    CLOUD_CMD_FIRMWARE_UPGRADE,     /* 固件升级 */
    CLOUD_CMD_AI_ANALYSIS,          /* AI 分析结果 */
} cloud_cmd_type_t;

typedef struct {
    cloud_cmd_type_t type;
    uint8_t device_id;
    uint16_t data_len;
    uint8_t data[512];
} cloud_cmd_t;

/* ==================== 同步状态 ==================== */
typedef struct {
    bool cloud_connected;
    bool wifi_connected;
    uint32_t last_sync_time;
    uint32_t pending_reports;
    uint32_t total_synced;
    uint32_t sync_errors;
} cloud_sync_status_t;

/* ==================== 接口函数 ==================== */

/* 初始化/主循环 */
void cloud_sync_init(void);
void cloud_sync_loop(void);

/* 数据上报 */
bool cloud_sync_report(cloud_report_type_t type, uint8_t device_id,
                       const void *data, uint16_t len);
bool cloud_sync_report_sensor(uint8_t device_id, const void *sensor_data);
bool cloud_sync_report_history(uint8_t device_id, const void *history_data, uint16_t count);
bool cloud_sync_report_alert(uint8_t device_id, const char *title, const char *content);

/* 命令接收 */
bool cloud_sync_get_command(cloud_cmd_t *cmd);
bool cloud_sync_confirm_command(uint8_t cmd_id, bool success);

/* 状态查询 */
bool cloud_sync_get_status(cloud_sync_status_t *status);
bool cloud_sync_is_connected(void);

/* 断网缓存 */
uint32_t cloud_sync_get_cached_count(void);
bool cloud_sync_flush_cache(void);

/* 配置 */
void cloud_sync_set_product_key(const char *key);
void cloud_sync_set_device_secret(const char *secret);

#endif /* CLOUD_SYNC_H */
