# 神农平台（Shennong Platform）

基于城市种植的分布式AI农业生产系统。

## 架构

- 网关设备：WBR3 + ZS3L（Zigbee透传）
- 养殖设备：立创地阔星 + ZS3L，通过网关管理
- 独立设备：不接入网关，独立运行

## 项目结构

```
shennong-platform/
├── gateway-device/                # 网关设备
│   └── wbr3/
├── auxiliary/                     # 独立设备
├── cultivators/                   # 养殖设备
│   ├── plants/                  # 植物
│   │   ├── tuber-planter/       # 块茎(0x02)
│   │   ├── root-planter/        # 根(0x03)
│   │   ├── stem-planter/        # 茎(0x04)
│   │   ├── leaf-planter/        # 叶(0x05)
│   │   ├── flower-planter/      # 花(0x06)
│   │   └── fruit-planter/       # 果实(0x07)
│   ├── algae-farm/               # 藻类
│   │   ├── spiral-algae/        # 螺旋藻(0x10)
│   │   └── chlorella-algae/     # 小球藻(0x11)
│   ├── fungi-farm/               # 真菌
│   │   ├── mushroom/            # 蘑菇(0x20)
│   │   └── fungus/              # 银耳/木耳(0x21)
│   ├── animal-farm/              # 动物
│   │   ├── bee/                 # 蜜蜂(0x30)
│   │   └── earthworm/           # 蚯蚓(0x31)
│   └── insect-farm/              # 昆虫
│       ├── cricket/             # 蟋蟀(0x40)
│       └── mealworm/            # 面包虫(0x41)
├── shared/
└── docs/
```

## 设备列表

### 植物 - 按可食用部位

| 设备 | 类型ID | 可食用部位 | 代表作物 |
|------|--------|-----------|---------|
| tuber-planter | 0x02 | 块茎 | 土豆、红薯 |
| root-planter | 0x03 | 根 | 萝卜、胡萝卜 |
| stem-planter | 0x04 | 茎 | 芹菜、莴笋 |
| leaf-planter | 0x05 | 叶 | 菠菜、生菜 |
| flower-planter | 0x06 | 花 | 花椰菜、西兰花 |
| fruit-planter | 0x07 | 果实 | 番茄、黄瓜 |

### 藻类

| 设备 | 类型ID | 说明 |
|------|--------|------|
| spiral-algae | 0x10 | 螺旋藻 |
| chlorella-algae | 0x11 | 小球藻 |

### 真菌

| 设备 | 类型ID | 说明 |
|------|--------|------|
| mushroom | 0x20 | 蘑菇 |
| fungus | 0x21 | 银耳、木耳 |

### 动物

| 设备 | 类型ID | 说明 |
|------|--------|------|
| bee | 0x30 | 蜜蜂养殖 |
| earthworm | 0x31 | 蚯蚓养殖 |

### 昆虫

| 设备 | 类型ID | 说明 |
|------|--------|------|
| cricket | 0x40 | 蟋蟀养殖 |
| mealworm | 0x41 | 面包虫养殖 |

## 硬件方案

| 设备 | 组成 | 开发方式 | 成本 |
|------|------|---------|------|
| 网关设备 | WBR3 + ZS3L | TuyaOS | ¥25 |
| 养殖设备 | 立创地阔星 + ZS3L | PlatformIO | ¥20 |

## 通信链路

```
涂鸦云 ←WiFi→ WBR3网关设备 ←Zigbee→ 养殖设备
```

## 开发

```bash
cd cultivators/plants/tuber-planter
pio run
```

## 文档

- [架构文档](docs/ARCHITECTURE.md)
- [部署指南](docs/DEPLOYMENT.md)
