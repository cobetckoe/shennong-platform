# 神农平台技术文档

## 系统架构

### 设计原则

- **云端智能**：所有物种参数、控制策略、实时调控在云端完成
- **设备极简**：设备只做两件事 — 采集上报、接收执行
- **通用模板**：所有设备使用同一份代码，仅 DEVICE_ID 和 DEVICE_TYPE 不同

### 数据流

```
用户APP → 涂鸦云（物种参数+控制策略） → 网关 → Zigbee → 子设备执行
子设备（传感器数据） → Zigbee → 网关 → 涂鸦云 → 用户APP
```

### 硬件

| 设备 | 组成 | 开发方式 | 功能 |
|------|------|---------|------|
| 网关 | WBR3 + ZS3L | TuyaOS | WiFi+Zigbee桥接 |
| 子设备 | 立创地阔星(STM32F103) + ZS3L | PlatformIO | 采集+执行 |

### 通信

| 链路 | 协议 | 说明 |
|------|------|------|
| 云→网关 | WiFi (涂鸦IoT) | DP数据点 |
| 网关→设备 | Zigbee mesh | 115200 baud |

## 通信协议

### 命令类型

| 命令 | 方向 | 说明 |
|------|------|------|
| 0x01 | 设备→网关 | 心跳 (每30秒) |
| 0x02 | 双向 | 传感器数据(上行) / 控制规则(下行) |

### 数据结构

```c
// 控制规则 (云端→设备)
typedef struct {
    uint8_t rule_id;        // 规则编号
    uint8_t device_type;    // 设备类型
    uint8_t species_id;     // 物种ID (0=使用手动参数)
    float temp_high, temp_low;
    float humidity_high, humidity_low;
    uint8_t spray, fan, led;
} __attribute__((packed)) control_rule_t;

// 传感器数据 (设备→云端)
typedef struct {
    float temperature, humidity, light, ph;
} __attribute__((packed)) sensor_data_t;
```

## 设备分类

按农业功能分类，每个设备代表一类可种植/养殖的物种：

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

## 物种匹配规则

设备与物种的匹配数据存放在云端 (`species_match.h`)，包含三层：

1. **物种环境需求** — 每个物种的温湿度、光照、pH范围
2. **设备能力范围** — 每个设备能控制的参数范围
3. **兼容性表** — 哪些物种可以在哪些设备上种植

匹配逻辑：物种需求范围 ⊆ 设备能力范围，且设备具备所需执行器。

### 运行时流程

1. 设备启动，等待云端下发控制规则
2. 用户在APP选择物种
3. 云端根据 species_id 查物种表，生成控制参数
4. 下发 control_rule_t (species_id 非0)
5. 设备收到后执行控制

## 网络方案

1000台规模推荐 **Zigbee mesh**：

| 指标 | WiFi | Zigbee | Meshtastic(LoRa) |
|------|------|--------|-------------------|
| 1000台网关数 | 20~30 | 2~5 | 2~4 |
| 传输距离 | 30~50m | 100~300m | 1~5km |
| 功耗 | 高 | 低 | 低 |
| 实时性 | 好 | 好 | 差(1~10s) |
| 带宽 | 高 | 中 | 极低 |

Zigbee mesh：2~3个网关 + 10~20个路由节点覆盖1000台终端。

## 部署

### 网关
- 硬件：WBR3 + ZS3L
- 开发：Tuya Wind IDE + TuyaOS
- 步骤：涂鸦平台创建产品 → 配置密钥 → 编译烧录 → 连接ZS3L
- 引脚：ZS3L TX→GPIO4, RX→GPIO5

### 子设备
- 硬件：STM32F103 + ZS3L + 传感器 + 执行器
- 开发：PlatformIO + Arduino
- 步骤：选设备目录 → 改DEVICE_ID/DEVICE_TYPE → 实现read_sensors() → 编译烧录
- 引脚：ZS3L TX→PA3, RX→PA2 | 喷雾→PB0, 风扇→PB1, LED→PB2

```bash
cd subdevice/plant/rootcrop
pio run              # 编译
pio run -t upload    # 烧录
pio device monitor   # 监视
```
