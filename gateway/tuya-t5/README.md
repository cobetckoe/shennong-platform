# T5 网关

基于涂鸦 T5 平台的网关主控程序。

## 硬件

- 涂鸦T5模组
- 涂鸦ZS3L模组（预装固件，串口连接）

## 功能

- 网关控制与设备管理
- 云端通信与App集成
- 与ZS3L串口通信
- 下发控制指令给养殖设备

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