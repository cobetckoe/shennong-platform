/**
 * 设备公共框架 - 涂鸦协议模板
 * 
 * 水培/气雾一体机
 * 
 * 架构:
 * - 云端下发控制策略 (不同植物不同规则)
 * - 本地根据规则 + 实时数据 = 自动控制
 * - 断网时本地独立运行
 * - 云端可远程微调参数
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>
#include "protocols.h"

// ==================== 设备API ====================

// ==================== 设备API ====================

/**
 * 设备初始化
 * @param device_id 设备ID
 */
void device_init(uint8_t device_id);

/**
 * 设备主循环
 * @param device_id 设备ID
 */
void device_loop(uint8_t device_id);

/**
 * 发送数据到网关
 * @param data 数据指针
 * @param len 数据长度
 */
void device_send(uint8_t *data, uint16_t len);

/**
 * 上报单个DP值
 * @param dpid 数据点ID
 * @param value 数据值
 * @param type 数据类型 (1=BOOL, 4=VALUE)
 */
void device_report_dp(uint8_t dpid, uint32_t value, uint8_t type);

/**
 * 上报所有传感器数据
 */
void device_report_all(void);

/**
 * 处理网关下发的命令
 * @param data 命令数据
 * @param len 数据长度
 */
void device_process_command(uint8_t *data, uint16_t len);

/**
 * 设备主循环单次执行 (非阻塞)
 * @param device_id 设备ID
 */
void device_loop_once(uint8_t device_id);

#endif