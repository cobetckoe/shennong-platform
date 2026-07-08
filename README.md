# 神农平台

基于城市种植的分布式AI农业生产系统。

## 项目结构

```
shennong-platform/
├── gateway/              # 网关设备
│   └── wbr3/
├── auxiliary/            # 独立设备
├── subdevice/            # 子设备（通过网关管理）
│   ├── plant/
│   │   ├── tuber/       # 0x02 块茎
│   │   ├── root/        # 0x03 根
│   │   ├── stem/        # 0x04 茎
│   │   ├── leaf/        # 0x05 叶
│   │   ├── flower/      # 0x06 花
│   │   └── fruit/       # 0x07 果实
│   ├── algae/
│   │   ├── spiral/      # 0x10 螺旋藻
│   │   └── chlorella/   # 0x11 小球藻
│   ├── fungi/
│   │   ├── mushroom/    # 0x20 蘑菇
│   │   └── fungus/      # 0x21 银耳
│   ├── animal/
│   │   ├── bee/         # 0x30 蜜蜂
│   │   └── earthworm/   # 0x31 蚯蚓
│   └── insect/
│       ├── cricket/     # 0x40 蟋蟀
│       └── mealworm/    # 0x41 面包虫
├── shared/
└── docs/
```

## 设备类型

| 类型 | 目录 | 连接方式 |
|------|------|---------|
| 网关 | gateway/ | WiFi + Zigbee |
| 独立设备 | auxiliary/ | 独立运行 |
| 子设备 | subdevice/ | Zigbee |

## 硬件方案

| 设备 | 组成 | 开发方式 | 成本 |
|------|------|---------|------|
| 网关 | WBR3 + ZS3L | TuyaOS | ¥25 |
| 子设备 | 立创地阔星 + ZS3L | PlatformIO | ¥20 |

## 通信链路

```
涂鸦云 ←WiFi→ 网关 ←Zigbee→ 子设备
```

## 开发

### 网关
使用Tuya Wind IDE

### 子设备
```bash
cd subdevice/plant/tuber
pio run
```
