# T5 网关主程序

本目录包含基于涂鸦 T5 平台的网关主控程序、系统初始化、任务调度和设备管理逻辑。

## 功能说明

- T5 平台初始化与 TuyaOS 系统集成
- 多设备通信协调与调度
- 本地事件驱动与策略执行
- 云端与本地双向通信接口

## 目录结构

- `src/` - 主程序源代码
- `include/` - 头文件与接口定义
- `config/` - 配置文件与参数

## 依赖

### TUYACLAW SDK（TuyaOS）

本项目依赖涂鸦智能的TuyaOS SDK，需要从官方获取。

**获取方式**：
1. 访问涂鸦智能开发者平台：https://developer.tuya.com/
2. 注册并登录开发者账号
3. 创建T5网关项目
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
cd gateway/tuya-t5
mkdir -p build && cd build
cmake -DTUYA_SDK_PATH=/path/to/sdk ..
make
```