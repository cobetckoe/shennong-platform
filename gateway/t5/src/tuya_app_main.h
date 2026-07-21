/**
 * 神农系统 T5 网关 - 主入口头文件
 */

#ifndef TUYA_APP_MAIN_H
#define TUYA_APP_MAIN_H

#include "protocols.h"

/* ==================== 涂鸦 DP 定义 ==================== */
#define DPID_DEV_LIST        1    /* 子设备列表 (STRING/JSON) */
#define DPID_SENSOR_ALL      2    /* 全部传感器数据 (STRING/JSON) */
#define DPID_RULE_UPDATE     3    /* 规则下发 (STRING/JSON) */
#define DPID_SCENE_UPDATE    4    /* 自动化场景下发 (STRING/JSON) */
#define DPID_MANUAL_CMD      5    /* 手动控制命令 (STRING/JSON) */
#define DPID_GW_STATUS       6    /* 网关状态 (STRING/JSON) */
#define DPID_VOICE_CMD       7    /* 语音指令 (STRING/JSON) */
#define DPID_CONFIG_UPDATE   8    /* 配置更新 (STRING/JSON) */
#define DPID_AI_ANALYSIS     9    /* AI 分析结果 (STRING/JSON) */
#define DPID_ALERT           10   /* 告警信息 (STRING/JSON) */

/* ==================== 继电器位定义 ==================== */
#define RELAY_BIT_PUMP       0x01  /* 水泵 */
#define RELAY_BIT_LIGHT      0x02  /* 补光灯 */
#define RELAY_BIT_FAN        0x04  /* 排风扇 */
#define RELAY_BIT_FOG        0x08  /* 喷雾泵 */
#define RELAY_BIT_HEATER     0x10  /* 加热片 */
#define RELAY_BIT_COOLER     0x20  /* 制冷片 */

/* ==================== 设备类型定义 ==================== */
#define DEVICE_ROOTCROP      0x01  /* 根菜 */
#define DEVICE_LEAFCROP      0x02  /* 叶菜 */
#define DEVICE_STEMCROP      0x03  /* 茎菜 */
#define DEVICE_FLOWERCROP    0x04  /* 花菜 */
#define DEVICE_FRUITCROP     0x05  /* 果菜 */
#define DEVICE_POLLINATOR    0x10  /* 蜜蜂 */
#define DEVICE_SOIL          0x11  /* 蚯蚓 */
#define DEVICE_EDIBLE_FUNGI  0x20  /* 食用菌 */
#define DEVICE_WATER_ALGAE   0x30  /* 水藻 */
#define DEVICE_FEED_INSECT   0x40  /* 饲虫 */

/* ==================== 函数声明 ==================== */
void tuya_app_main(void);

#endif /* TUYA_APP_MAIN_H */
