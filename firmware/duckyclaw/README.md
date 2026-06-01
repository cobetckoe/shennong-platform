# Duckyclaw 本地控制策略引擎

本目录包含 duckyclaw 实时调参与控制策略模块，负责设备端本地 AI 决策与执行。

## 核心职责

- 实时传感器数据采集与处理
- 本地模型推理与参数优化
- 设备间协作控制策略执行
- 故障检测与自恢复机制
- 本地缓存与决策历史记录

## 目录结构

- \src/\ - duckyclaw 引擎核心实现
- \include/\ - 引擎接口与数据结构定义
- \config/\ - 策略规则配置
- \policy/\ - 植物与环境特定的控制策略库

## 配置示例

参见 \config/duckyclaw_config.json\

## 相关文档

- docs/TUYA_T5_STANDARDS.md - 完整规范
- docs/QUICK_REFERENCE.md - 快速参考与配置模板
