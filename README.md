# 神农平台（Shennong Platform）

基于城市种植的分布式AI农业生产系统。

## 架构概述

采用"网关 + 独立设备 + 养殖设备"架构：

- 网关：涂鸦T5 + ZS3L，统一管理所有养殖设备
- 独立设备：K230播种机，独立运行
- 养殖设备：STM32 + ZS3L，通过网关管理

## 项目结构

```
shennong-platform/
├── gateway/          # 网关（T5 + ZS3L）
│   ├── tuya-t5/      # T5网关固件
│   └── edge-inference/ # ZS3L边缘推理
├── auxiliary/        # 独立设备（播种机）
├── cultivators/      # 养殖设备（按食用部位分类）
├── shared/           # 共享PlatformIO库
├── docs/             # 文档
└── .vscode/          # VS Code配置
```

## 设备列表

### 独立设备
| 设备 | 芯片 | 说明 |
|------|------|------|
| seed-planter | K230 | 播种机，图像识别 |

### 养殖设备（按食用部位分类）
| 设备 | 类型ID | 可食用部位 | 说明 |
|------|--------|-----------|------|
| tuber-planter | 0x02 | 块茎 | 土豆等块茎作物 |
| algae-planter | 0x03 | 全株 | 螺旋藻等藻类 |
| leaf-planter | 0x04 | 叶子 | 蔬菜等叶菜类 |
| fungi-planter | 0x05 | 子实体 | 蘑菇等菌类 |

## 技术栈

- 网关：TUYACLAW框架（涂鸦T5）
- 养殖设备：PlatformIO（STM32 + ZS3L）
- 独立设备：K230 SDK
- 通信：Meshtastic mesh网络

## 依赖获取

### TUYACLAW SDK（网关开发）

网关开发需要涂鸦智能的TuyaOS SDK，需要从官方获取：

1. 访问涂鸦智能开发者平台：https://developer.tuya.com/
2. 注册并登录开发者账号
3. 创建T5网关项目
4. 下载TuyaOS SDK

详细说明请参考：[SDK说明文档](gateway/SDK.md)

### K230 SDK（独立设备开发）

独立设备开发需要K230 SDK，请参考K230官方文档获取。

## 快速开始

### 编译网关

```bash
# 安装TuyaOS SDK后
cd gateway/tuya-t5
mkdir -p build && cd build
cmake -DTUYA_SDK_PATH=/path/to/sdk ..
make
```

### 编译养殖设备

```bash
cd cultivators/tuber-planter
pio run
```

### 编译独立设备

```bash
cd auxiliary/seed-planter
# 使用K230 SDK编译
```

## 文档

- [架构文档](docs/ARCHITECTURE.md)
- [部署指南](docs/DEPLOYMENT.md)
- [SDK说明文档](gateway/SDK.md)