# T5 网关主程序

本目录包含基于涂鸦 T5 平台的网关主控程序、系统初始化、任务调度和设备管理逻辑。

## 功能说明

- T5 平台初始化与 TuyaOS 系统集成
- 多设备通信协调与调度
- 本地事件驱动与策略执行
- 云端与本地双向通信接口

## 目录结构

- \src/\ - 主程序源代码
- \include/\ - 头文件与接口定义
- \config/\ - 配置文件与参数
- \uild/\ - 编译输出目录

## 编译说明

\\\ash
cd firmware/t5-gateway
mkdir -p build && cd build
cmake ..
make
\\\

## 涂鸦 T5 SDK 依赖

- TuyaOS 3.x 或更高
- Tuya IoT Development Kit
