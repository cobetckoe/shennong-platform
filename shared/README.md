# 共享库

养殖设备共享的PlatformIO库。

## 目录结构

```
shared/lib/
├── sensors/      # 传感器驱动库
├── actuators/    # 执行器驱动库
└── protocols/    # 通信协议库
```

## 使用方法

在养殖设备的`platformio.ini`中添加：

```ini
lib_extra_dirs =
  ../../shared/lib
```

在代码中包含头文件：

```cpp
#include <shennong_sensors.h>
#include <shennong_actuators.h>
#include <shennong_protocols.h>
```

## API接口

### 传感器库
- `sensors_init()` - 初始化传感器
- `sensor_read(type)` - 读取传感器数据

### 执行器库
- `actuators_init()` - 初始化执行器
- `actuator_control(type, value)` - 控制执行器

### 通信协议库
- `protocol_init()` - 初始化通信
- `protocol_send(msg)` - 发送消息
- `protocol_receive(msg)` - 接收消息