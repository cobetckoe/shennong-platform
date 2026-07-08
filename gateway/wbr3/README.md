# WBR3 网关

## 概述

网关设备，通过WiFi连接涂鸦云，通过Zigbee管理子设备。

## 硬件

- WBR3模组 (ESP32-C3)
- ZS3L模组 (Zigbee透传)

## 开发环境

### 要求
- Tuya Wind IDE (已安装)
- TuyaOS SDK (网关开发框架)
- 涂鸦开发者账号

### 配置
1. 打开Tuya Wind IDE
2. 登录涂鸦开发者账号
3. 下载网关开发框架
4. 创建工程

## 工程结构

```
wbr3/
├── src/
│   └── main.c          # 应用代码
├── include/            # 头文件
└── README.md
```

## 配置

### 产品信息
修改 `src/main.c` 中的配置：
```c
#define PRODUCT_KEY     "your_product_key"
#define PRODUCT_SECRET  "your_product_secret"
#define DEVICE_NAME     "your_device_name"
#define DEVICE_SECRET   "your_device_secret"
```

### 引脚连接
- ZS3L TX → WBR3 GPIO4
- ZS3L RX → WBR3 GPIO5
- ZS3L VCC → 3.3V
- ZS3L GND → GND

## 编译

1. 在Tuya Wind IDE中打开工程
2. 右键工程 → Build Project
3. 输入版本号 (X.X.X)
4. 等待编译完成

## 烧录

1. USB连接WBR3
2. 选择正确的COM口
3. 烧录固件

## 调试

### 检查WiFi
- LED指示灯显示WiFi状态
- 检查涂鸦云连接

### 检查Zigbee
- 监控串口输出
- 检查ZS3L通信

## 参考

- [涂鸦网关开发指南](https://developer.tuya.com/cn/docs/iot-device-dev/Gateway-Product-Software-Development-Kit?id=Kbfjuqtma64w4)
- [Tuya Wind IDE](https://developer.tuya.com/cn/docs/iot-device-dev/tuyaos-wind-ide?id=Kbfy6kfuuqqu3)
