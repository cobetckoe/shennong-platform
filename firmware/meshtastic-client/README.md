# Meshtastic 通信客户端

本目录包含 Meshtastic Mesh 网络客户端，实现数百台设备的低功耗、长距离通信网络。

## 网络功能

- Mesh 网络初始化与拓扑维护
- 设备发现与路由管理
- 传感器数据上行采集
- 控制指令下行广播或单播
- 网络故障检测与自愈

## 目录结构

- \src/\ - Meshtastic 协议栈与消息处理
- \include/\ - 网络接口定义
- \config/\ - 射频与网络配置

## 消息协议

参见 \config/meshtastic_config.json\ 和规范文档
