# 部署指南

## 网关部署

1. 涂鸦平台创建产品
2. Tuya Wind IDE编译
3. 烧录WBR3
4. 连接ZS3L

## 子机部署

1. 复制 `shared/sub_device_template.c` 到设备目录
2. 修改 DEVICE_ID 和 DEVICE_TYPE
3. 修改传感器读取逻辑
4. STM32CubeIDE编译烧录

## 设备ID分配

| 设备类型 | ID范围 | 说明 |
|---------|--------|------|
| tuber-planter | 0x02-0x0F | 块茎种植机 |
| algae-planter | 0x10-0x1F | 藻类种植机 |
| leaf-planter | 0x20-0x2F | 叶菜种植机 |
| fungi-planter | 0x30-0x3F | 菌类种植机 |
