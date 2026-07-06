# 神农平台（Shennong Platform）

基于城市种植的分布式AI农业生产系统。

## 架构

- 网关：涂鸦T5 + ZS3L（预装固件），管理所有养殖设备
- 养殖设备：STM32，通过网关管理
- 独立设备：不接入网关，独立运行

## 项目结构

```
shennong-platform/
├── gateway/          # 网关
├── auxiliary/        # 独立设备
├── cultivators/      # 养殖设备
├── shared/           # 共享库
└── docs/             # 文档
```

## 设备列表

### 养殖设备（按食用部位分类）

| 设备 | 类型ID | 可食用部位 | 说明 |
|------|--------|-----------|------|
| tuber-planter | 0x02 | 块茎 | 土豆等块茎作物 |
| algae-planter | 0x03 | 全株 | 螺旋藻等藻类 |
| leaf-planter | 0x04 | 叶子 | 蔬菜等叶菜类 |
| fungi-planter | 0x05 | 子实体 | 蘑菇等菌类 |

## 快速开始

### 网关开发

```bash
cd gateway/tuya-t5
mkdir -p build && cd build
cmake -DTUYA_SDK_PATH=/path/to/sdk ..
make
```

### 养殖设备开发

```bash
cd cultivators/tuber-planter
pio run
```

## 文档

- [架构文档](docs/ARCHITECTURE.md)
- [部署指南](docs/DEPLOYMENT.md)
- [SDK说明](gateway/SDK.md)