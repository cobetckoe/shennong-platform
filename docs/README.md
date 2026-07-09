# 神农平台技术文档

## 系统架构

### 设计原则

- **云端智能**：所有物种参数、控制策略、实时调控在云端完成
- **设备极简**：设备只做采集上报和接收执行
- **分层解耦**：HAL → 驱动 → 设备，每层独立可替换
- **配置驱动**：设备main.cpp只做硬件组装，不含底层逻辑

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

## 运行时流程

1. 设备启动，上报心跳，等待云端下发控制规则
2. 用户在APP选择物种
3. 云端匹配物种需求与设备能力，生成控制参数
4. 下发 control_rule_t (含 species_id)
5. 设备收到后直接执行控制

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

## 代码架构

```
shared/
  hal/                          硬件抽象层 (换MCU只改这层)
    gpio.h/c                    GPIO
    uart.h/c                    UART
    i2c.h/c                     I2C
    adc.h/c                     ADC
    delay.h/c                   延时
  drivers/
    sensor/                     传感器驱动 (可复用)
      dht22.h/c                 温湿度
      ds18b20.h/c               单总线温度
      bh1750.h/c                I2C光照
      hx711.h/c                 称重
      ph_sensor.h/c             pH模拟量
    actuator/                   执行器驱动 (可复用)
      fan.h/c                   风扇
      pump.h/c                  泵类
      led.h/c                   LED补光
      heater.h/c                加热片
  common/
    device.h/c                  设备框架 (主循环/通信)
  protocols.h                   通信协议
```

设备main.cpp只做硬件组装：声明驱动实例 → 实现采集/控制 → 启动。

## 部署

### 网关
- 硬件：WBR3 + ZS3L
- 开发：Tuya Wind IDE + TuyaOS
- 步骤：涂鸦平台创建产品 → 配置密钥 → 编译烧录 → 连接ZS3L
- 引脚：ZS3L TX→GPIO4, RX→GPIO5

### 子设备
- 硬件：STM32F103 + ZS3L + 传感器 + 执行器
- 开发：PlatformIO + Arduino
- 步骤：选设备目录 → 配置引脚 → 编译烧录
- 架构：HAL → 驱动 → 设备main.cpp组装

```bash
cd subdevice/plant/rootcrop
pio run              # 编译
pio run -t upload    # 烧录
pio device monitor   # 监视
```
