# 涂鸦 T5 网关部署指南

## 已完成的目录结构

本项目已按照涂鸦规范创建了完整的 T5 网关和本地 AI 控制架构：

### 核心模块

1. **t5-gateway/** - T5 网关主程序
   - 包含 T5 平台初始化、多设备管理、任务调度
   - 构建配置：CMakeLists.txt

2. **duckyclaw/** - 本地 AI 控制策略引擎
   - 实时传感器数据采集与处理
   - 本地模型推理与参数自适应优化
   - 配置文件：config/duckyclaw_config.json

3. **meshtastic-client/** - Mesh 网络通信
   - 数百台设备的低功耗 Mesh 通信
   - 自组网与路由管理
   - 配置文件：config/meshtastic_config.json

4. **local-control/** - 本地控制界面
   - 触摸屏/LCD 驱动与显示
   - 物理按键事件处理
   - 脱网自主操作支持

5. **edge-inference/** - 边缘推理引擎
   - TensorFlow Lite / ONNX 推理框架
   - 作物识别与生长阶段预测
   - 轻量级模型执行

6. **storage/** - TF卡存储管理
   - 原始数据与日志持久化
   - 云端同步策略
   - 配置文件：config/storage_config.json

7. **ota/** - 固件升级管理
   - 版本控制与增量升级
   - 分布式 Mesh 升级协调
   - 配置文件：config/ota_config.json

### 编译与构建

\\\ash
cd firmware
mkdir -p build && cd build
cmake ..
make
\\\

### 配置说明

各模块配置文件已在对应的 config/ 目录下，参考格式：

- duckyclaw_config.json - 设置推理间隔、设备参数、学习率
- meshtastic_config.json - 无线射频、消息类型、重试策略
- storage_config.json - 存储分区、数据保留策略、同步间隔
- ota_config.json - 固件版本、升级策略、完整性检查

### 后续步骤

1. 根据涂鸦 SDK 文档填充各模块源代码
2. 完成传感器驱动与执行器控制接口
3. 编译测试与单元测试
4. 集成云端 API 与用户 App 接口

## 参考文档

详见 docs/ 目录：
- TUYA_T5_STANDARDS.md - 完整涂鸦规范
- QUICK_REFERENCE.md - 快速参考与配置模板
- DEPLOYMENT_GUIDE.md - 部署最佳实践
