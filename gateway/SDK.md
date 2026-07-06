# TUYACLAW SDK 说明

## 获取方式

1. 访问涂鸦开发者平台：https://developer.tuya.com/
2. 注册并登录开发者账号
3. 创建T5网关项目
4. 下载TuyaOS SDK（3.x或更高版本）

## 安装

```bash
# 解压SDK到指定目录
unzip tuyaos-sdk.zip -d /path/to/sdk

# 设置环境变量
export TUYA_SDK_PATH=/path/to/sdk
```

## 使用

在CMakeLists.txt中配置SDK路径：

```cmake
cmake -DTUYA_SDK_PATH=/path/to/sdk ..
```