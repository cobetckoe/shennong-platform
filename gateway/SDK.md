# TUYACLAW SDK 说明

## SDK获取

TUYACLAW SDK（TuyaOS）是涂鸦智能提供的闭源开发框架，需要从官方渠道获取。

### 获取方式

1. 访问涂鸦智能开发者平台：https://developer.tuya.com/
2. 注册并登录开发者账号
3. 创建T5网关项目
4. 下载TuyaOS SDK

### SDK版本要求

- TuyaOS 3.x 或更高版本
- 支持T5模组的SDK包

## SDK安装

### 1. 解压SDK

将下载的SDK解压到指定目录。

### 2. 配置CMake

在CMakeLists.txt中设置SDK路径：

```cmake
set(TUYA_SDK_PATH "/path/to/tuyaos-sdk")
find_package(TuyaOS REQUIRED)
```

### 3. 环境变量

设置环境变量：

```bash
export TUYA_SDK_PATH=/path/to/tuyaos-sdk
```

## SDK结构

```
tuyaos-sdk/
├── include/          # 头文件
├── lib/              # 库文件
├── tools/            # 开发工具
└── docs/             # SDK文档
```

## 开发文档

SDK包含完整的开发文档和示例代码，请参考：

- TuyaOS开发指南
- T5模组API参考
- 设备接入指南

## 注意事项

1. SDK为闭源软件，仅供授权开发者使用
2. 遵守涂鸦智能的开发者协议
3. 不得将SDK用于商业授权范围外的用途