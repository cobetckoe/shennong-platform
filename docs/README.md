# 神农技术文档

## 系统架构

### 设计原则

- **云端智能 + 本地自治**：云端 AI 分析决策，T5 本地执行自动化
- **多模态交互**：图像识别 + 语音控制 + APP 远程
- **分层解耦**：HAL → 驱动 → 设备，每层独立可替换
- **跨设备联动**：虚拟子设备与涂鸦生态无缝对接

### 数据流

```
用户APP → 涂鸦云 ←→ T5网关(DuckyClaw) ←→ Zigbee ←→ 子设备
                    ↕                    ↕
              本地自动化引擎         图像识别+语音交互
```

### 硬件

| 设备 | 组成 | 开发方式 | 功能 |
|------|------|---------|------|
| 网关 | T5-AI (WiFi+BLE+Zigbee+摄像头+麦克风) | TuyaOpen + DuckyClaw | 桥接+自动化+图像+语音 |
| 子设备 | STM32F103 + ZS3L | PlatformIO + STM32Cube | 采集+执行 |

### 通信

| 链路 | 协议 | 说明 |
|------|------|------|
| 云→网关 | WiFi (涂鸦IoT) | DP数据点 |
| 网关→设备 | Zigbee mesh | 115200 baud |
| 用户→网关 | 语音 (ASR/TTS) | T5-AI 麦克风+喇叭 |
| 用户→网关 | 图像 (摄像头) | T5-AI 摄像头模块 |

## T5 网关功能模块

### DuckyClaw 架构

```
gateway/t5/src/
├── tuya_app_main.c          # 入口：初始化+主循环
├── zigbee/
│   ├── zigbee_manager.c/h   # Zigbee 子设备管理
│   └── local_automation.c/h # 本地自动化 (CRON)
├── camera/
│   └── camera_module.c/h    # 摄像头/图像识别
├── voice/
│   └── voice_module.c/h     # 语音交互 (ASR/TTS)
├── memory/                   # 记忆管理 (DuckyClaw)
├── agent/                    # AI Agent (DuckyClaw)
├── tools/                    # MCP 工具 (DuckyClaw)
└── im/                       # IM 通知 (DuckyClaw)
```

### 图像识别

| 功能 | 实现方式 |
|------|---------|
| 病虫害检测 | T5-AI 摄像头 → 云端 AI 识别 → 告警 |
| 作物生长监测 | 定时拍照 → 云端分析 → 数据上报 |
| 异常检测 | 本地对比 + 云端验证 |

- 拍照间隔：5分钟（可配置）
- 识别模型：云端 plant_disease_detect
- 告警方式：语音播报 + APP 推送

### 语音交互

| 指令 | 动作 |
|------|------|
| "小农小农" | 唤醒词 |
| "开风扇/关风扇" | 控制继电器 |
| "开灯/关灯" | 控制补光灯 |
| "查温度/查湿度" | 语音播报传感器数据 |
| "状态报告" | 汇报所有设备状态 |
| "全部打开/全部关闭" | 批量控制 |

### 本地自动化

基于 TuyaOpen CRON 实现：

| 触发类型 | 说明 | 执行位置 |
|---------|------|---------|
| 定时 | 每天 X:XX 执行 | T5 CRON |
| 传感器阈值 | 温度>30°C → 开风扇 | T5 每秒检查 |
| 跨设备联动 | 设备A开 → 设备B也开 | T5 本地执行 |

### 虚拟子设备

每个 Zigbee 子设备自动注册为涂鸦虚拟设备：
- APP 中显示为独立设备
- 可与任何涂鸦设备通过智能场景联动
- 每个子设备暴露温度/湿度/水泵/风扇等 DP 点

## 通信协议

### 命令类型

| 命令 | 方向 | 说明 |
|------|------|------|
| 0x01 | 设备→网关 | 心跳 |
| 0x02 | 双向 | 传感器数据(上行) / 控制规则(下行) |
| 0x03 | 云→网关 | 控制规则下发 |
| 0x04 | 云→网关 | DP值微调 |
| 0x06 | T5→子设备 | 继电器直接控制 |
| 0x07 | T5→子设备 | 场景同步 |

### 数据结构

详见 `shared/protocols.h`

## 设备分类

| 类别 | 设备 | ID | 物种 |
|------|------|-----|------|
| 植物 | rootcrop | 0x01 | 土豆、红薯、萝卜、胡萝卜 |
| 植物 | leafcrop | 0x02 | 菠菜、生菜 |
| 植物 | stemcrop | 0x03 | 芹菜、芦笋 |
| 植物 | flowercrop | 0x04 | 花椰菜、菜花 |
| 植物 | fruitcrop | 0x05 | 番茄、黄瓜、辣椒 |
| 动物 | pollinator | 0x10 | 蜜蜂 |
| 动物 | soil | 0x11 | 蚯蚓 |
| 真菌 | edible | 0x20 | 蘑菇、银耳 |
| 藻类 | water | 0x30 | 螺旋藻、小球藻 |
| 昆虫 | feed | 0x40 | 蟋蟀、面包虫 |

## 网络方案

温室大棚推荐 **Zigbee mesh**：

| 指标 | WiFi | Zigbee | Meshtastic(LoRa) |
|------|------|--------|-------------------|
| 1000台网关数 | 20~30 | 2~5 | 2~4 |
| 传输距离 | 30~50m | 100~300m | 1~5km |
| 功耗 | 高 | 低 | 低 |
| 实时性 | 好 | 好 | 差(1~10s) |

## 代码架构

```
shared/
  hal/                          硬件抽象层
    gpio.h/c, uart.h/c, i2c.h/c, adc.h/c, delay.h/c
  drivers/
    sensor/                     传感器驱动
    actuator/                   执行器驱动
  common/
    device.h/c                  设备框架
    control_engine.h            控制引擎接口
  protocols.h                   通信协议

gateway/t5/                     T5 网关 (DuckyClaw + TuyaOpen)
  src/
    tuya_app_main.c             入口
    zigbee/zigbee_manager.c/h   Zigbee 管理
    zigbee/local_automation.c/h 本地自动化
    camera/camera_module.c/h    图像识别
    voice/voice_module.c/h      语音交互

subdevice/plant/                子设备 (PlatformIO)
  rootcrop/, leafcrop/, ...
```

## 部署

### 网关
- 硬件：T5-AI 开发板 (带摄像头+麦克风)
- 开发：TuyaOpen SDK + DuckyClaw
- 步骤：涂鸦平台创建产品 → 配置密钥 → 编译烧录

### 子设备
- 硬件：STM32F103 + ZS3L + 传感器 + 执行器
- 开发：PlatformIO + STM32Cube
- 步骤：选设备目录 → 配置引脚 → 编译烧录

```bash
cd subdevice/plant/rootcrop
pio run              # 编译
pio run -t upload    # 烧录
pio device monitor   # 监视
```
