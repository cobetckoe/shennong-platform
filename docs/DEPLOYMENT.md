# 部署指南

## 环境要求

### 网关开发
- 涂鸦T5开发板
- TUYACLAW SDK（TuyaOS 3.x+）
- CMake 3.10+

### 养殖设备开发
- 立创地阔星开发板（STM32F103C8）
- PlatformIO CLI

### 独立设备开发
- K230开发板
- K230 SDK

## SDK获取

1. 访问涂鸦开发者平台：https://developer.tuya.com/
2. 注册并登录
3. 创建T5网关项目
4. 下载TuyaOS SDK

详见：[SDK说明](../gateway/SDK.md)

## 编译步骤

### 网关

```bash
cd gateway/tuya-t5
mkdir -p build && cd build
cmake -DTUYA_SDK_PATH=/path/to/sdk ..
make
```

### 养殖设备

```bash
cd cultivators/tuber-planter
pio run
```

### 独立设备

```bash
cd auxiliary/seed-planter
# 使用K230 SDK编译
```

## 烧录

### 网关
使用涂鸦烧录工具或串口烧录。

### 养殖设备
```bash
pio run -t upload
pio device monitor --baud 115200
```

### 独立设备
参考K230开发文档。