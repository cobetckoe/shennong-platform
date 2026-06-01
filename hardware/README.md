# 硬件模块

本模块描述硬件设计、传感器布置、执行单元标准和设备装配规范。

## 核心内容

- 传感器清单：温度、湿度、光照、CO2、pH、EC、溶氧等
- 执行机构：喷雾泵、LED 生长灯、风扇、电磁阀、步进电机
- 电源与接口：电源管理、供电方案、接口标准、屏蔽设计
- 机械结构：模块化外壳、支架、托盘、挂架设计
- 连接规范：I2C、SPI、UART、PWM、ADC、GPIO

## 设备子目录

本模块为各类子设备提供独立硬件设计目录，按生物分类组织：

- `hardware/Kingdom_Plantae/Phylum_Magnoliophyta/seed-planting/` - 种子种植机硬件
- `hardware/Kingdom_Plantae/Phylum_Magnoliophyta/vegetable-planting/` - 蔬菜种植机硬件
- `hardware/Kingdom_Plantae/Phylum_Magnoliophyta/potato-planting/` - 土豆种植机硬件
- `hardware/Kingdom_Bacteria/Phylum_Cyanobacteria/spirulina-culture/` - 螺旋藻培养机硬件
- `hardware/Kingdom_Fungi/Phylum_Basidiomycota/mushroom-culture/` - 蘑菇种植机硬件

## 设计原则

- 模块化：硬件模块应可独立替换与升级
- 易维护：传感器可更换，管线与排水可快速拆装
- 安全性：防水防尘、漏电保护、过载保护、报警机制
- 兼容性：与现有 Tuya/T5 平台、Meshtastic 模块、3D 打印结构兼容
