# Arduino PlatformIO 设备固件

本目录包含使用 PlatformIO 开发的各类农业设备固件。

## 项目结构

\
arduino/
├── common/              # 公共库与通信协议
│   ├── lib/
│   │   ├── mesh_protocol.h    # Mesh 通信协议
│   │   ├── sensor_types.h     # 传感器定义
│   │   └── control_types.h    # 控制命令定义
│   └── src/
│       └── mesh_protocol.c    # 协议实现
├── Kingdom_Plantae/
│   └── Phylum_Magnoliophyta/
│       ├── seed-planting/        # 种子种植机
│       ├── vegetable-planting/   # 蔬菜种植机
│       └── potato-planting/      # 土豆种植机
├── Kingdom_Bacteria/
│   └── Phylum_Cyanobacteria/
│       └── spirulina-culture/    # 螺旋藻种植机
└── Kingdom_Fungi/
  └── Phylum_Basidiomycota/
    └── mushroom-culture/     # 蘑菇种植机
\

## 快速开始

### 安装 PlatformIO

\\\ash
# 通过 pip 安装
pip install platformio

# 或在 VS Code 中安装 PlatformIO Extension
\\\

### 编译特定设备

\\\ash
# 进入设备目录
cd firmware/arduino/Kingdom_Plantae/Phylum_Magnoliophyta/seed-planting

# 编译（ESP32 环境为例）
pio run -e esp32

# 上传固件
pio run -e esp32 -t upload

# 监视串口输出
pio device monitor --baud 115200
\\\

### 编译所有设备

进入各设备目录后执行 `pio run` 编译。例如：

```bash
# 示例：编译种子种植机
cd firmware/arduino/Kingdom_Plantae/Phylum_Magnoliophyta/seed-planting
pio run -e esp32

# 编译蘑菇种植机
cd ../../Kingdom_Fungi/Phylum_Basidiomycota/mushroom-culture
pio run -e esp32
```

或在脚本中递归查找 `platformio.ini` 并按目录编译（根据你的环境选择合适方法）。

## 通信协议

所有 Arduino 设备通过 UART 与 Meshtastic 模块通信。

详见 \docs/PROTOCOL.md\

### 基本流程

1. **初始化**: Arduino 启动后自动连接 Meshtastic
2. **数据上传**: 周期性采集传感器数据，打包后发送
3. **命令接收**: 接收来自 T5 网关的控制命令
4. **本地执行**: 根据命令控制执行器

## 每个设备的特定说明

### seed-planting (种子种植机)
- 主微控制器: ESP32 或 STM32
- 传感器: 土壤湿度、温度、光照
- 执行器: 步进电机、电磁阀

### vegetable-planting (蔬菜种植机)
- 传感器: 温度、湿度、光照、pH、EC
- 执行器: 喷雾泵、LED 生长灯、风扇

### potato-planting (土豆种植机)
- 传感器: 根区湿度、溶氧、温度
- 执行器: 喷雾泵、气泵、加压泵

### spirulina-culture (螺旋藻培养机)
- 传感器: 温度、光照、pH、溶氧、浊度
- 执行器: LED 生长灯、搅拌电机、气泵

### mushroom-culture (蘑菇种植机)
- 传感器: 温度、湿度、CO2、光照、基质湿度
- 执行器: 喷雾系统、风扇、加热板、补水泵

## 开发指南

### 添加新设备

1. 在 \irmware/arduino/\ 下创建新目录
2. 复制 \platformio.ini\ 和 \src/main.cpp\ 模板
3. 在 \main.cpp\ 中实现:
   - \setup()\ - 初始化传感器和执行器
   - \loop()\ - 周期采集和发送数据
4. 使用 \common/lib/\ 中的通信函数

### 编译选项

在 \platformio.ini\ 中配置:
- 目标平台 (ESP32, STM32 等)
- 波特率 (通常 115200)
- 监视端口 (USB 或 UART)
- 必要的库

## 故障排查

- 检查串口连接是否正常
- 验证波特率设置是否为 115200
- 查看设备日志判断数据是否发送
- 使用 \pio device monitor\ 实时查看输出
