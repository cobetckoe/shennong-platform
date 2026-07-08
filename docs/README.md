# 神农平台文档

## 概述

基于涂鸦IoT平台的分布式AI农业生产系统。

## 架构

### 系统拓扑
```
涂鸦云 ←WiFi→ 网关 ←Zigbee→ 子设备
                           ←Zigbee→ 子设备
                           ←Zigbee→ 子设备
```

### 设备类型
| 类型 | 目录 | 连接方式 | 功能 |
|------|------|---------|------|
| 网关 | gateway/ | WiFi + Zigbee | 云端桥接 |
| 独立设备 | auxiliary/ | 独立运行 | 独立控制 |
| 子设备 | subdevice/ | Zigbee | 本地控制 |

### 硬件
| 设备 | 组成 | 开发方式 | 成本 |
|------|------|---------|------|
| 网关 | WBR3 + ZS3L | TuyaOS | ¥25 |
| 子设备 | 立创地阔星 + ZS3L | PlatformIO | ¥20 |

## 开发

### 网关 (TuyaOS)
1. 安装Tuya Wind IDE
2. 下载网关开发框架
3. 配置产品信息
4. 编译烧录

### 子设备 (PlatformIO)
1. 安装PlatformIO
2. 配置设备ID
3. 实现传感器读取
4. 编译烧录

## 通信协议

### 命令类型
| 命令 | 方向 | 说明 |
|------|------|------|
| 0x01 | 子设备→网关 | 心跳 |
| 0x02 | 双向 | 传感器数据/控制规则 |

### 数据结构
```c
// 控制规则
typedef struct {
    uint8_t rule_id;
    uint8_t device_type;
    float temp_high, temp_low;
    float humidity_high, humidity_low;
    uint8_t spray, fan, led;
} __attribute__((packed)) control_rule_t;

// 传感器数据
typedef struct {
    float temperature, humidity, light, ph;
} __attribute__((packed)) sensor_data_t;
```

## 设备列表

### 植物 (按可食用部位)
| 设备 | ID | 部位 | 作物 |
|------|-----|------|------|
| tuber | 0x02 | 块茎 | 土豆 |
| root | 0x03 | 根 | 萝卜 |
| stem | 0x04 | 茎 | 芹菜 |
| leaf | 0x05 | 叶 | 菠菜 |
| flower | 0x06 | 花 | 花椰菜 |
| fruit | 0x07 | 果实 | 番茄 |

### 藻类
| 设备 | ID | 名称 |
|------|-----|------|
| spiral | 0x10 | 螺旋藻 |
| chlorella | 0x11 | 小球藻 |

### 真菌
| 设备 | ID | 名称 |
|------|-----|------|
| mushroom | 0x20 | 蘑菇 |
| fungus | 0x21 | 银耳 |

### 动物
| 设备 | ID | 名称 |
|------|-----|------|
| bee | 0x30 | 蜜蜂 |
| earthworm | 0x31 | 蚯蚓 |

### 昆虫
| 设备 | ID | 名称 |
|------|-----|------|
| cricket | 0x40 | 蟋蟀 |
| mealworm | 0x41 | 面包虫 |

## 参考

- [涂鸦开发者平台](https://developer.tuya.com/)
- [涂鸦IoT文档](https://developer.tuya.com/cn/docs/iot-device-dev)
- [PlatformIO](https://platformio.org/)
