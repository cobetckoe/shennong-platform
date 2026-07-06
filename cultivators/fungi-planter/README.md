# 菌类种植机 (fungi-planter)

## 设备信息
- 设备类型: 0x05
- 节点ID: 0x15
- 开发框架: PlatformIO
- 可食用部位: 子实体（蘑菇等）

## 目录结构
```
fungi-planter/
├── src/main.cpp       # 固件代码
├── platformio.ini     # 项目配置
├── hardware/          # 硬件设计文件
├── ai-models/         # 设备专用AI模型
├── 3d-print/          # 3D打印文件
└── README.md          # 本文件
```

## 传感器配置
- 温度传感器
- 湿度传感器
- CO2传感器
- 光照传感器
- 基质湿度传感器

## 执行器配置
- 喷雾系统
- 风扇
- 加热板
- 补水泵

## 开发说明
1. 使用PlatformIO打开项目目录
2. 选择目标环境（esp32或stm32）
3. 编译并上传固件
4. 通过串口监视器查看设备状态