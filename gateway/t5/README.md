# 神农 T5 网关

## 概述

T5 一体化网关，基于 Wukong SDK + 本地自动化架构。
通过 WiFi 连接涂鸦云，通过 Zigbee/BLE 管理子设备。

## 硬件

- T5-AI 开发板 (WiFi + BLE + Zigbee + 摄像头 + 麦克风)

## 开发环境

- Tuya Wind IDE
- Wukong SDK (AI 能力)
- TuyaOS (IoT 连接)

## 功能模块

```
gateway/t5/src/
├── tuya_app_main.c/h       # 主入口: 初始化 + 主循环
├── zigbee/                 # Zigbee 通信
│   ├── zigbee_manager.c/h  # 子设备管理
│   └── local_automation.c/h # 本地自动化 (CRON + 阈值 + 联动)
├── ble/                    # BLE 蓝牙
│   └── ble_manager.c/h     # 蓝牙设备管理
├── camera/                 # 图像识别
│   └── camera_module.c/h   # T5-AI 摄像头
└── cloud/                  # 云端数据同步
    └── cloud_sync.c/h      # 标准 DP 上报/下发
```

## Wukong SDK (待集成)

Wukong SDK 提供以下能力，无需自写：

| 功能 | 说明 |
|------|------|
| AI Agent | 智能决策、自然语言处理 |
| 语音交互 | ASR 语音识别 + TTS 语音合成 |
| 记忆系统 | 对话历史、用户偏好 |
| MCP 工具 | 闹钟、提醒、番茄钟等 |
| 本地工具 | 端侧 MCP 工具集 |

## 本地自动化

断网时 T5 继续运行已下发的规则：

- **定时触发**: 每天 X:XX 执行
- **阈值触发**: 温度>30°C → 开风扇
- **联动触发**: 设备A状态变化 → 触发设备B
- **CRON 任务**: 基于 tuya_cron API

## 数据流

```
在线时:
涂鸦云 → cloud(DP下发) → local_automation(执行) → zigbee/ble → 设备

断网时:
local_automation(本地规则) → zigbee/ble → 设备

恢复后:
zigbee/ble(数据) → cloud(DP上报) → 涂鸦云
```

## 配置

1. 在 Tuya Wind IDE 中打开工程
2. 修改 `tuya_app_main.c` 中的产品配置:
   - PRODUCT_KEY
   - PRODUCT_SECRET
   - DEVICE_NAME
   - DEVICE_SECRET
3. 在涂鸦平台配置产品和 DP 点

## 编译烧录

1. 在 Tuya Wind IDE 中 Build Project
2. USB 连接 T5
3. 烧录固件

## Wukong SDK 集成步骤

1. 申请 Wukong SDK 权限 (Tuya Wind IDE → 资源中心)
2. 创建 Wukong SDK 工程
3. 将 zigbee/ble/cloud/local_automation 复制到新工程
4. 在 tuya_app_main.c 中调用 wukong_init() 和 wukong_loop()
5. 编译烧录

## 待办事项

- [ ] 申请 Wukong SDK 权限
- [ ] 创建 Wukong SDK 工程
- [ ] 集成 Wukong SDK API
- [ ] 在涂鸦平台配置 AI Agent + 自控指令
