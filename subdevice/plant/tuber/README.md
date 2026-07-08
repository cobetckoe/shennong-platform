# 块茎种植机

## 概述

子设备，用于块茎类作物（土豆、红薯）。

## 硬件

- 立创地阔星 (STM32F103C8)
- ZS3L模组 (Zigbee)
- 传感器：温度、湿度、光照、pH
- 执行器：喷雾泵、风扇、LED

## 开发环境

### 要求
- PlatformIO
- ST-Link调试器
- Arduino框架

### 配置
1. 安装PlatformIO
2. 打开工程文件夹
3. 编译烧录

## 配置

### 设备ID
修改 `src/main.cpp` 中的配置：
```c
#define DEVICE_ID       0x02
#define DEVICE_TYPE     0x02
```

### 引脚连接
- ZS3L TX → PA3 (USART2_RX)
- ZS3L RX → PA2 (USART2_TX)
- ZS3L VCC → 3.3V
- ZS3L GND → GND
- 喷雾泵 → PB0
- 风扇 → PB1
- LED → PB2

## 编译

```bash
pio run
```

## 烧录

```bash
pio run -t upload
```

## 调试

```bash
pio device monitor --baud 115200
```

## 控制参数

| 参数 | 默认值 | 范围 |
|------|--------|------|
| 温度上限 | 28°C | 20-40°C |
| 温度下限 | 15°C | 5-25°C |
| 湿度上限 | 75% | 50-100% |
| 湿度下限 | 50% | 30-80% |

## 参考

- [PlatformIO](https://platformio.org/)
- [STM32 Arduino](https://docs.platformio.org/en/latest/boards/ststm32/genericSTM32F103C8.html)
