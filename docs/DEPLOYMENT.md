# 部署指南

## 环境要求

### 网关开发
- 涂鸦T5开发板
- TUYACLAW SDK（TuyaOS 3.x+）
- CMake 3.10+
- 交叉编译工具链

### 养殖设备开发
- 立创地阔星开发板（STM32F103C8）
- PlatformIO CLI
- STM32工具链

### 独立设备开发
- K230开发板
- K230 SDK
- 交叉编译工具链

## SDK获取

### TUYACLAW SDK

1. 访问涂鸦智能开发者平台：https://developer.tuya.com/
2. 注册并登录开发者账号
3. 创建T5网关项目
4. 下载TuyaOS SDK

详细说明请参考：[SDK说明文档](../gateway/SDK.md)

## 编译步骤

### 网关

```bash
# 1. 安装SDK
# 按照SDK说明文档安装TuyaOS SDK

# 2. 编译T5网关
cd gateway/tuya-t5
mkdir -p build && cd build
cmake -DTUYA_SDK_PATH=/path/to/sdk ..
make

# 3. 编译边缘推理引擎
cd gateway/edge-inference
mkdir -p build && cd build
cmake -DTUYA_SDK_PATH=/path/to/sdk ..
make
```

### 养殖设备

```bash
# 进入设备目录（以块茎种植机为例）
cd cultivators/tuber-planter

# 编译固件
pio run

# 上传固件
pio run -t upload
```

### 独立设备

```bash
# 进入播种机目录
cd auxiliary/seed-planter

# 使用K230 SDK编译
# 具体步骤参考K230开发文档
```

## 烧录与测试

### 网关烧录

使用涂鸦提供的烧录工具或通过串口烧录。

### 养殖设备烧录

```bash
# 连接开发板
# 上传固件
pio run -t upload

# 监视串口输出
pio device monitor --baud 115200
```

### 独立设备烧录

参考K230开发文档进行烧录。

## 故障排除

### 网关问题

- 检查SDK路径是否正确
- 确认CMake版本满足要求
- 查看编译错误信息

### 养殖设备问题

- 检查串口连接是否正常
- 验证波特率设置（通常为115200）
- 查看编译错误信息
- 检查硬件连接

### 独立设备问题

- 参考K230开发文档
- 检查SDK安装是否正确