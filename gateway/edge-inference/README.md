# 边缘推理引擎（ZS3L）

本目录用于承接涂鸦 ZS3L 作为边缘推理网关的实现，负责本地模型推理、协议桥接、控制决策与缓存。

## 核心职责

- 轻量级 AI 推理与模型加载
- 与 T5 网关进行状态同步与指令下发
- 与 STM32 子设备进行实时数据交换
- 在离线场景下继续执行本地决策

## 目录结构

- `tuya-zs3l/` - ZS3L 对接与边缘网关实现
- `src/` - 推理引擎实现
- `include/` - 推理接口与协议定义
- `models/` - 预训练模型与参数文件

## 依赖

### TUYACLAW SDK（TuyaOS）

本项目依赖涂鸦智能的TuyaOS SDK，需要从官方获取。

**获取方式**：
1. 访问涂鸦智能开发者平台：https://developer.tuya.com/
2. 注册并登录开发者账号
3. 创建ZS3L边缘网关项目
4. 下载TuyaOS SDK

**版本要求**：TuyaOS 3.x 或更高

详细说明请参考：[SDK说明文档](../SDK.md)

## 编译说明

### 1. 安装SDK

按照SDK说明文档安装TuyaOS SDK。

### 2. 配置SDK路径

设置环境变量或CMake变量指向SDK目录。

### 3. 编译项目

```bash
cd gateway/edge-inference
mkdir -p build && cd build
cmake -DTUYA_SDK_PATH=/path/to/sdk ..
make
```

## 说明

- 推理模型由云端训练后下发至边缘
- 本地推理结果可直接用于控制策略
- ZS3L 作为边缘层，连接 T5 与 STM32 子设备