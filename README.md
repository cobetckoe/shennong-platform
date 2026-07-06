# 神农平台（Shennong Platform）

基于城市种植的分布式AI农业生产系统。

## 架构

- 网关：涂鸦T5 + ZS3L（预装固件），管理所有养殖设备
- 养殖设备：STM32，通过网关管理
- 独立设备：K230播种机，独立运行

## 项目结构

```
shennong-platform/
├── gateway/          # 网关
├── auxiliary/        # 独立设备（播种机）
├── cultivators/      # 养殖设备
├── shared/           # 共享库
└── docs/             # 文档
```

## 设备列表

| 设备 | 类型 | 芯片 | 说明 |
|------|------|------|------|
| seed-planter | 独立 | K230 | 播种机 |
| tuber-planter | 养殖 | STM32 | 块茎种植机 |
| algae-planter | 养殖 | STM32 | 藻类种植机 |
| leaf-planter | 养殖 | STM32 | 叶菜种植机 |
| fungi-planter | 养殖 | STM32 | 菌类种植机 |

## 快速开始

### 网关开发

```bash
# 需要TUYACLAW SDK（从涂鸦平台获取）
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

### 独立设备开发

```bash
cd auxiliary/seed-planter
# 使用K230 SDK
```

## 文档

- [架构文档](docs/ARCHITECTURE.md)
- [部署指南](docs/DEPLOYMENT.md)
- [SDK说明](gateway/SDK.md)