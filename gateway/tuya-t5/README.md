# T5 网关

基于涂鸦 T5 平台的网关主控程序。

## 硬件

- 涂鸦T5模组
- 涂鸦ZS3L模组（预装固件，串口连接）

## 功能模块

### 本地存储模块
- 数据和模型本地保存
- 文件系统管理
- 数据持久化

### 模型管理模块
- 控制模型版本管理
- 模型保存、加载、删除
- 模型更新和部署

### 数据管理模块
- 传感器数据收集
- 控制数据记录
- 数据统计和清理

### 生物配置模块
- 不同生物参数配置
- 生长周期管理
- 环境参数优化

## 目录结构

```
tuya-t5/
├── src/              # 源代码
│   ├── main.c
│   ├── storage.c
│   ├── model_manager.c
│   ├── data_manager.c
│   └── bio_config.c
├── include/          # 头文件
│   ├── storage.h
│   ├── model_manager.h
│   ├── data_manager.h
│   └── bio_config.h
├── config/           # 配置文件
├── models/           # 模型存储
├── data/             # 数据存储
└── CMakeLists.txt
```

## 依赖

- TUYACLAW SDK（TuyaOS 3.x+）
- 从涂鸦开发者平台获取：https://developer.tuya.com/

## 编译

```bash
cd gateway/tuya-t5
mkdir -p build && cd build
cmake -DTUYA_SDK_PATH=/path/to/sdk ..
make
```