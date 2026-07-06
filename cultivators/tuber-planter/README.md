# 块茎种植机 (tuber-planter)

## 设备信息
- 设备类型: 0x02
- 节点ID: 0x12
- 开发框架: PlatformIO
- 可食用部位: 块茎（土豆等）

## 目录结构
```
tuber-planter/
├── src/main.cpp       # 固件代码
├── platformio.ini     # 项目配置
├── hardware/          # 硬件设计文件
├── ai-models/         # 设备专用AI模型
├── 3d-print/          # 3D打印文件
└── README.md          # 本文件
```

## 传感器配置
- 根区湿度传感器
- 溶氧传感器
- 温度传感器

## 执行器配置
- 喷雾泵
- 气泵
- 加压泵

## 开发说明
1. 使用PlatformIO打开项目目录
2. 选择目标环境（esp32或stm32）
3. 编译并上传固件
4. 通过串口监视器查看设备状态