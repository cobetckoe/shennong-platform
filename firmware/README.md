# 神农平台固件

## 架构说明

本固件采用**双平行开发架构**，T5 网关和 Arduino 子设备是独立的两个开发环境：

### 1. T5 网关 (TuyaOS) - 独立环境
- **目录**: `firmware/t5-gateway/`
- **平台**: 涂鸦 T5 + TuyaOS
- **编译工具**: CMake + make
- **职责**:
  - 主控管理与协调
  - Mesh 网络网关（通过串口接收 Meshtastic 数据）
  - 本地 AI 决策（duckyclaw）
  - 云端通信与设备控制
  - TF卡存储与数据备份

### 2. Arduino 子设备 (PlatformIO) - 独立环境
- **目录**: `firmware/arduino/`
- **平台**: Arduino / STM32 等微控制器
- **编译工具**: PlatformIO
- **设备按生物分类组织**:
  - `Kingdom_Plantae/Phylum_Magnoliophyta/seed-planting/` - 种子种植机
  - `Kingdom_Plantae/Phylum_Magnoliophyta/vegetable-planting/` - 蔬菜种植机
  - `Kingdom_Plantae/Phylum_Magnoliophyta/potato-planting/` - 土豆种植机
  - `Kingdom_Bacteria/Phylum_Cyanobacteria/spirulina-culture/` - 螺旋藻种植机
  - `Kingdom_Fungi/Phylum_Basidiomycota/mushroom-culture/` - 蘑菇种植机
- **职责**:
  - 传感器数据采集
  - 本地执行器控制
  - 通过串口发送数据到 Meshtastic 模块

### 3. Meshtastic Mesh 网络
- **设备**: 成品 Meshtastic 模块（无需自己开发）
- **通信**: 所有设备通过 UART/Serial 连接到 Meshtastic 模块
- **网络**: 自动 Mesh 组网，T5 网关作为网关节点

## 编译说明

### T5 网关 (TuyaOS)
```bash
cd firmware/t5-gateway
mkdir -p build && cd build
cmake ..
make
```

### Arduino 设备 (PlatformIO)
```bash
# 编译种子种植机
cd firmware/arduino/Kingdom_Plantae/Phylum_Magnoliophyta/seed-planting
pio run -e esp32  # 或对应的环境

# 上传固件
pio run -e esp32 -t upload
```

## 通信协议

详见 `docs/PROTOCOL.md`

串口通信格式：
- 波特率: 115200
- 帧格式: [START] [TYPE] [LEN] [DATA] [CHECKSUM] [END]
- Mesh 网络通过串口转发到 T5 网关

## 项目文件说明

### 独立开发环境

#### T5 网关 (TuyaOS) - 环境1
- **目录**: `firmware/t5-gateway/`
- **编译**: `cmake` + `make`
- **结构**:
  - `src/` - 源代码
  - `include/` - 头文件
  - `config/` - 配置文件
  - `libs/duckyclaw/` - 本地 AI 决策引擎
  - `libs/mesh-serial/` - 串口 Mesh 通信接口
  - `libs/storage/` - 存储与数据同步

#### Arduino 子设备 (PlatformIO) - 环境2
- **目录**: `firmware/arduino/`
- **编译**: `PlatformIO`
- **结构**:
  - `common/` - PlatformIO 公共库与协议
  - `Kingdom_Plantae/Phylum_Magnoliophyta/seed-planting/` - 种子种植机
  - `Kingdom_Plantae/Phylum_Magnoliophyta/vegetable-planting/` - 蔬菜种植机
  - `Kingdom_Plantae/Phylum_Magnoliophyta/potato-planting/` - 土豆种植机
  - `Kingdom_Bacteria/Phylum_Cyanobacteria/spirulina-culture/` - 螺旋藻种植机
  - `Kingdom_Fungi/Phylum_Basidiomycota/mushroom-culture/` - 蘑菇种植机

### 共享基础设施模块

- `duckyclaw/` - 本地 AI 决策引擎（被 T5 网关集成）
- `edge-inference/` - 边缘推理库
- `local-control/` - 本地控制协议与实现
- `meshtastic-client/` - Meshtastic 通信客户端
- `ota/` - 固件 OTA 更新
- `storage/` - 数据存储与备份

## 开发流程

### 平行开发模式

**T5 网关开发** (独立于 Arduino 开发)
- 在 `firmware/t5-gateway/` 中开发 TuyaOS 应用
- 编译命令: `cd firmware/t5-gateway && mkdir -p build && cd build && cmake .. && make`
- 负责主控、Mesh 网关、本地决策

**Arduino 设备开发** (独立于 T5 开发)
- 各设备在 `firmware/arduino/Kingdom_*/` 中独立开发
- 编译命令: `cd firmware/arduino/Kingdom_*/.../device && pio run`
- 负责传感器采集与执行器控制

### 集成通信

1. **Arduino 设备上传**
   - 每个 Arduino 设备通过串口连接 Meshtastic 模块
   - 发送传感器数据到 Meshtastic

2. **Meshtastic Mesh 网络**
   - 自动组网，T5 网关作为网络中心节点
   - 转发 Arduino 设备数据到 T5 网关

3. **T5 网关处理**
   - 接收 Arduino 设备的传感器数据
   - 本地运行 duckyclaw 做出决策
   - 将控制指令通过 Meshtastic 发送回设备
   - 定期上传数据到云端

## 参考

- `docs/INTEGRATION.md` - 系统集成指南
- `docs/PROTOCOL.md` - 通信协议详解
- `docs/TUYA_T5_STANDARDS.md` - T5 开发规范
