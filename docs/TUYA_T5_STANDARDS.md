# 涂鸦 T5 网关平台开发规范

## 1. T5 平台项目结构规范

### 1.1 顶级目录结构

```
project-root/
├── firmware/                    # 固件层
│   ├── edge/                   # T5 边缘网关核心逻辑
│   ├── meshtastic/             # Mesh 网络客户端
│   ├── storage/                # TF 卡数据管理
│   ├── control/                # 本地控制策略
│   ├── ota/                    # 固件升级机制
│   └── [device-type]/          # 设备类型子目录
│       ├── src/                # 源码
│       ├── include/            # 头文件
│       ├── config/             # 配置文件
│       └── README.md
├── ai-models/                  # AI 模型层
│   ├── edge-model/            # 边缘本地调参模型
│   ├── cloud-model/           # 云端训练模型
│   ├── data-pipeline/         # 数据处理流程
│   └── [device-type]/         # 设备类型模型
├── cloud/                      # 云端服务
│   ├── api/                    # REST/GraphQL API
│   ├── training/               # 模型训练服务
│   ├── storage/                # 数据存储方案
│   └── marketplace/            # 交易平台
├── app/                        # 应用层
│   ├── mobile/                 # 移动 APP
│   ├── dashboard/              # 云端仪表盘
│   ├── local-ui/               # 本地界面
│   └── marketplace/            # 商城模块
├── hardware/                   # 硬件设计
│   └── [device-type]/          # 设备硬件规范
├── 3d-print/                   # 3D 打印部件
│   ├── parts/                  # 部件文件
│   ├── material/               # 材料参数
│   └── assembly/               # 装配说明
├── docs/                       # 文档
│   ├── ARCHITECTURE.md         # 架构总览
│   ├── TUYA_T5_STANDARDS.md   # 本规范
│   ├── API.md                  # API 文档
│   └── DEPLOYMENT.md           # 部署指南
└── .gitignore, LICENSE, README.md
```

### 1.2 设备类型约定

对每类种植机，按照以下约定创建子目录：

| 设备类型 | 目录名 | 说明 |
|---------|--------|------|
| 种子种植机 | `seed-planting` | 精确播种与定位 |
| 蔬菜种植机 | `vegetable-planting` | 蔬菜类种植机 |
| 土豆种植机 | `potato-planting` | 块茎类种植机 |
| 螺旋藻种植机 | `spirulina-culture` | 微藻培养 |
| 蘑菇种植机 | `mushroom-culture` | 真菌类栽培 |

---

## 2. duckyclaw 本地控制策略引擎规范

### 2.1 duckyclaw 目录结构

duckyclaw 是 T5 网关上的本地策略执行引擎。推荐目录组织如下：

```
firmware/edge/duckyclaw/
├── src/
│   ├── core/                     # 核心策略执行器
│   │   ├── policy_engine.c       # 策略评估与执行
│   │   ├── state_machine.c       # 设备状态机
│   │   └── event_handler.c       # 事件处理
│   ├── models/                   # 本地调参模型接口
│   │   ├── model_loader.c        # 模型加载与版本管理
│   │   ├── model_inference.c     # 模型推理（TFLite/ONNX）
│   │   └── param_optimizer.c     # 参数优化算法
│   ├── sensors/                  # 传感器数据采集
│   │   ├── sensor_manager.c      # 传感器管理器
│   │   ├── data_filter.c         # 数据滤波与去噪
│   │   └── sensor_drivers/       # 各传感器驱动
│   ├── controllers/              # 执行器控制
│   │   ├── actuator_manager.c    # 执行器管理
│   │   ├── pwm_controller.c      # PWM 控制
│   │   └── solenoid_valve.c      # 电磁阀控制
│   ├── storage/                  # 本地数据存储接口
│   │   ├── data_logger.c         # 数据日志记录
│   │   └── config_manager.c      # 配置参数管理
│   ├── communication/            # 通信接口
│   │   ├── mqtt_client.c         # MQTT（涂鸦）
│   │   ├── meshtastic_adapter.c  # Meshtastic 适配层
│   │   └── local_api.c           # 本地 HTTP API
│   ├── ui/                       # 本地 UI 交互
│   │   ├── display_driver.c      # 屏幕驱动
│   │   ├── button_handler.c      # 按键处理
│   │   └── ui_state.c            # UI 状态管理
│   └── main.c                    # 主程序入口
├── include/
│   ├── duckyclaw.h               # 公共接口定义
│   ├── types.h                   # 通用数据结构
│   ├── config.h                  # 配置常量
│   └── [module_name].h           # 各模块头文件
├── config/
│   ├── duckyclaw_config.json     # 默认配置
│   ├── policy_rules.json         # 策略规则定义
│   ├── model_config.json         # 模型配置
│   └── device_config.json        # 设备级配置
├── tests/
│   ├── unit/                     # 单元测试
│   ├── integration/              # 集成测试
│   └── test_runner.c             # 测试框架
├── CMakeLists.txt                # 编译配置
└── README.md
```

### 2.2 duckyclaw 配置文件规范

**duckyclaw_config.json**

```json
{
  "version": "1.0",
  "device_id": "gateway_001",
  "device_type": "seed-planting",
  "tuya_config": {
    "product_id": "xxxxx",
    "device_token": "****",
    "region": "cn"
  },
  "local_mode": {
    "enabled": true,
    "fallback_timeout": 300,
    "offline_control": true
  },
  "model": {
    "type": "edge_model",
    "version": "v1.0.0",
    "update_interval": 3600,
    "inference_engine": "tflite"
  },
  "sensors": {
    "sample_interval": 60,
    "filter_type": "kalman",
    "calibration_data": "sensors/calibration.bin"
  },
  "storage": {
    "backend": "tf_card",
    "buffer_size": 1000,
    "sync_interval": 300,
    "retention_days": 90
  },
  "meshtastic": {
    "enabled": true,
    "radio_port": "/dev/ttyUSB0",
    "baud_rate": 115200
  }
}
```

**policy_rules.json**

```json
{
  "version": "1.0",
  "policies": [
    {
      "id": "auto_irrigation",
      "name": "自动灌溉策略",
      "priority": 10,
      "enabled": true,
      "conditions": {
        "soil_moisture": { "operator": "<", "value": 50 },
        "temperature": { "operator": ">", "value": 15 }
      },
      "actions": [
        {
          "type": "actuator_control",
          "actuator_id": "irrigation_pump",
          "action": "on",
          "duration": 300
        }
      ]
    },
    {
      "id": "manual_mode",
      "name": "手动模式",
      "priority": 100,
      "enabled": true,
      "trigger_source": "local_button"
    }
  ]
}
```

### 2.3 duckyclaw 关键接口定义

```c
// duckyclaw.h
typedef struct {
  char device_id[32];
  char device_type[32];
  uint32_t timestamp;
  float temperature;
  float humidity;
  float soil_moisture;
  // ... 其他传感器数据
} sensor_data_t;

typedef struct {
  char policy_id[32];
  sensor_data_t sensor_state;
  void (*execute_callback)(const char* action_id);
} policy_context_t;

// 核心函数接口
int duckyclaw_init(const char* config_file);
int duckyclaw_load_policies(const char* policy_file);
int duckyclaw_update_sensor_data(sensor_data_t* data);
int duckyclaw_evaluate_policies(policy_context_t* ctx);
int duckyclaw_execute_action(const char* action_id);
int duckyclaw_shutdown(void);
```

---

## 3. Meshtastic 集成方案

### 3.1 Meshtastic 模块目录结构

```
firmware/meshtastic/
├── src/
│   ├── meshtastic_client.c       # Meshtastic 客户端
│   ├── radio_interface.c         # 无线接口适配
│   ├── message_protocol.c        # 消息协议处理
│   ├── topology_manager.c        # 网格拓扑管理
│   ├── data_sync.c               # 数据同步策略
│   └── error_handling.c          # 错误处理与重连
├── include/
│   ├── meshtastic.h              # 公共接口
│   └── message_types.h           # 消息类型定义
├── config/
│   ├── meshtastic_config.json    # 射频配置
│   └── mesh_topology.json        # 拓扑配置
└── README.md
```

### 3.2 Meshtastic 消息协议规范

**消息类型定义** (message_types.h)

```c
#define MSG_TYPE_SENSOR_DATA      0x01   // 传感器数据
#define MSG_TYPE_CONTROL_CMD      0x02   // 控制指令
#define MSG_TYPE_STATUS_HEARTBEAT 0x03   // 心跳状态
#define MSG_TYPE_ALERT            0x04   // 告警消息
#define MSG_TYPE_OTA_TRIGGER      0x05   // OTA 升级触发
#define MSG_TYPE_CONFIG_UPDATE    0x06   // 配置更新

// Meshtastic 消息格式
typedef struct {
  uint32_t from_id;               // 来源设备 ID
  uint32_t to_id;                 // 目的设备 ID
  uint8_t message_type;           // 消息类型
  uint16_t sequence_id;           // 序列号
  uint32_t timestamp;             // 时间戳
  uint16_t payload_length;        // 负载长度
  uint8_t payload[256];           // 消息负载
  uint8_t want_ack;               // 是否需要 ACK
} mesh_message_t;
```

### 3.3 Meshtastic 配置文件规范

**meshtastic_config.json**

```json
{
  "version": "1.0",
  "radio_config": {
    "frequency": 470000000,
    "bandwidth": 62500,
    "power": 17,
    "spreading_factor": 9,
    "coding_rate": 8
  },
  "mesh_config": {
    "channel": 0,
    "psk": "0x1a1a1a1a2b2b2b2b3c3c3c3c4d4d4d4d",
    "node_delay": 100,
    "position_broadcast_secs": 900
  },
  "device_config": {
    "node_num": 1,
    "long_name": "Shennong_Gateway_01",
    "short_name": "GW01",
    "region": "CN"
  },
  "data_sync": {
    "batch_size": 10,
    "sync_interval": 300,
    "retry_count": 3,
    "timeout": 30
  }
}
```

### 3.4 Meshtastic 数据同步流程

```
设备端 Mesh 采集
     ↓
Meshtastic 无线传输
     ↓
T5 网关接收与解析
     ↓
数据缓冲与去重（duckyclaw 处理）
     ↓
本地存储（TF 卡）
     ↓
duckyclaw 策略评估
     ↓
控制指令通过 Mesh 下发
     ↓
设备端执行
```

---

## 4. 本地存储与 OTA 升级标准

### 4.1 TF 卡存储架构

```
/sdcard/
├── data/                         # 原始数据存储
│   ├── sensors/
│   │   ├── [device_id]/
│   │   │   ├── 2024_05/
│   │   │   │   ├── sensor_data_20240501.bin
│   │   │   │   └── sensor_data_20240502.bin
│   │   │   └── latest_sensor_data.bin
│   │   └── index.json            # 数据索引
│   ├── events/
│   │   ├── [device_id]/
│   │   │   └── event_log_20240501.jsonl
│   │   └── index.json
│   └── control/
│       └── control_history.jsonl # 控制命令历史
├── models/                       # 模型存储
│   ├── edge_model_v1.0.tflite
│   ├── edge_model_v1.1.tflite
│   └── model_metadata.json       # 模型元数据
├── config/                       # 设备配置备份
│   ├── device_config.json
│   ├── policy_rules.json
│   └── sensor_calibration.bin
├── logs/                         # 运行日志
│   ├── system/
│   │   └── 2024_05_system.log
│   ├── duckyclaw/
│   │   └── 2024_05_duckyclaw.log
│   └── meshtastic/
│       └── 2024_05_mesh.log
├── firmware/                     # 固件升级包
│   ├── firmware_v2.0.bin         # 待升级固件
│   ├── firmware_v1.9.bin         # 备份固件
│   └── firmware_manifest.json    # 固件清单
└── metadata.json                 # 存储元数据
```

### 4.2 数据存储规范

**sensors 数据格式** (二进制)

```
结构：[8字节时间戳] [4字节设备ID] [传感器数据记录] [校验和]

二进制格式定义（C结构）：
typedef struct {
  uint64_t timestamp;             // Unix 时间戳（ms）
  uint32_t device_id;             // 设备 ID
  struct {
    float temperature;            // 温度（°C）
    float humidity;               // 湿度（%）
    float soil_moisture;          // 土壤湿度（%）
    float light_intensity;        // 光照强度（lux）
    float co2_level;              // CO2 浓度（ppm）
    uint8_t status_flags;         // 状态标志
  } data;
  uint32_t checksum;              // CRC32 校验和
} sensor_record_t;
```

**events 数据格式** (JSONL)

```jsonl
{"timestamp":"2024-05-01T12:00:00Z","device_id":"dev_001","event_type":"policy_triggered","policy_id":"auto_irrigation","details":{"soil_moisture":45.2,"threshold":50},"status":"success"}
{"timestamp":"2024-05-01T12:05:00Z","device_id":"dev_001","event_type":"actuator_action","actuator_id":"pump_01","action":"on","duration":300,"status":"success"}
```

**metadata.json**

```json
{
  "version": "1.0",
  "device_id": "gateway_001",
  "storage_info": {
    "total_capacity": 2000000000,
    "used_capacity": 1234567890,
    "last_sync": "2024-05-01T12:30:00Z"
  },
  "data_index": {
    "sensors": {
      "latest_timestamp": "2024-05-01T12:30:00Z",
      "record_count": 12345,
      "devices": ["dev_001", "dev_002"]
    },
    "events": {
      "latest_timestamp": "2024-05-01T12:30:00Z",
      "record_count": 5678
    }
  },
  "retention_policy": {
    "data_retention_days": 90,
    "log_retention_days": 30,
    "auto_cleanup": true
  }
}
```

### 4.3 OTA 升级流程与规范

#### 4.3.1 OTA 升级目录结构

```
firmware/ota/
├── src/
│   ├── ota_manager.c             # OTA 管理器
│   ├── firmware_download.c       # 固件下载
│   ├── signature_verify.c        # 签名验证
│   ├── rollback_handler.c        # 回滚处理
│   └── ota_protocol.c            # OTA 协议实现
├── include/
│   ├── ota_manager.h
│   └── ota_protocol.h
├── config/
│   ├── ota_config.json           # OTA 配置
│   └── firmware_manifest.json    # 固件清单
└── README.md
```

#### 4.3.2 OTA 配置规范

**ota_config.json**

```json
{
  "version": "1.0",
  "ota_enabled": true,
  "update_server": "https://ota.tuya.com/firmware",
  "check_interval": 86400,
  "update_strategy": "scheduled",
  "scheduled_window": {
    "hour_start": 22,
    "hour_end": 6,
    "day_of_week": [0, 1, 2, 3, 4, 5, 6]
  },
  "rollback": {
    "max_retries": 3,
    "auto_rollback": true,
    "health_check_timeout": 300
  },
  "signature": {
    "enabled": true,
    "algorithm": "RSA-2048",
    "public_key_file": "firmware/ota/certs/public_key.pem"
  }
}
```

**firmware_manifest.json**

```json
{
  "version": "1.0",
  "manifests": [
    {
      "firmware_version": "2.0.0",
      "release_date": "2024-05-01",
      "device_type": "seed-planting",
      "file_name": "firmware_v2.0.0.bin",
      "file_size": 2097152,
      "md5": "abc123def456...",
      "sha256": "abc123def456...",
      "release_notes": "新增 Meshtastic 支持，优化传感器采样",
      "min_prev_version": "1.9.0",
      "critical": false,
      "signature": "firmware_v2.0.0.sig"
    }
  ]
}
```

#### 4.3.3 OTA 升级流程

```
┌──────────────────────────────────┐
│ 云端推送更新通知 (MQTT/Meshtastic) │
└──────────┬───────────────────────┘
           ↓
┌──────────────────────────────────┐
│ 检查本地升级窗口与健康状态         │
└──────────┬───────────────────────┘
           ↓
   ┌───────────────────────┐
   │ 符合条件？              │
   └───┬─────────┬──────────┘
       │         │
      否        是
       ↓        ↓
┌─────────┐ ┌──────────────────────────┐
│ 延迟    │ │ 从云端下载固件（断点续传）  │
└─────────┘ └──────────┬───────────────┘
                        ↓
             ┌──────────────────────────┐
             │ 验证签名与校验和          │
             └──────────┬───────────────┘
                        ↓
             ┌──────────────────────────┐
             │ 备份当前固件到 TF 卡       │
             └──────────┬───────────────┘
                        ↓
             ┌──────────────────────────┐
             │ 刷写新固件                │
             └──────────┬───────────────┘
                        ↓
             ┌──────────────────────────┐
             │ 重启设备                  │
             └──────────┬───────────────┘
                        ↓
             ┌──────────────────────────┐
             │ 运行健康检查（5分钟内）   │
             └──────────┬───────────────┘
                        ↓
        ┌────────┬──────────┬──────────┐
        │        │          │          │
       通过    失败       异常       报告
        ↓       ↓          ↓         ↓
     ┌───┐  ┌───────┐  ┌───────┐  ┌─────┐
     │成功│ │自动   │ │人工    │ │状态 │
     │   │ │回滚   │ │干预    │ │记录 │
     └───┘ └───────┘ └───────┘ └─────┘
```

#### 4.3.4 OTA 关键接口

```c
// ota_manager.h
typedef enum {
  OTA_STATE_IDLE,
  OTA_STATE_DOWNLOADING,
  OTA_STATE_VERIFYING,
  OTA_STATE_FLASHING,
  OTA_STATE_VERIFYING_NEW,
  OTA_STATE_DONE,
  OTA_STATE_ERROR
} ota_state_t;

int ota_init(const char* config_file);
int ota_check_update(void);
int ota_download_firmware(const char* url, const char* dest);
int ota_verify_firmware(const char* firmware_path);
int ota_flash_firmware(const char* firmware_path);
int ota_rollback(void);
ota_state_t ota_get_state(void);
```

---

## 5. TuyaOS 与固件项目的约定俗成

### 5.1 涂鸦 SDK 集成规范

#### 5.1.1 目录结构约定

```
firmware/edge/
├── tuya_sdk/                     # 涂鸦 SDK（版本锁定）
│   ├── include/
│   ├── libs/
│   └── version.txt               # SDK 版本文件
├── tuya_interface/               # 涂鸦接口适配层
│   ├── mqtt_adapter.c            # MQTT 协议适配
│   ├── thing_model.c             # 物模型解析与处理
│   ├── property_handler.c        # 属性处理
│   ├── event_handler.c           # 事件处理
│   ├── service_handler.c         # 服务处理
│   └── oem_config.c              # OEM 配置
├── duckyclaw/                    # duckyclaw 引擎
├── CMakeLists.txt
└── README.md
```

#### 5.1.2 涂鸦物模型规范

**thing_model.json**（物模型定义）

```json
{
  "version": "1.0",
  "schema": "schema_version",
  "properties": [
    {
      "id": "soil_moisture",
      "name": "土壤湿度",
      "accessMode": "r",
      "type": "number",
      "specs": {
        "min": 0,
        "max": 100,
        "step": 0.1,
        "unitName": "%"
      }
    },
    {
      "id": "irrigation_pump",
      "name": "灌溉泵",
      "accessMode": "rw",
      "type": "bool",
      "specs": {}
    },
    {
      "id": "auto_mode",
      "name": "自动模式",
      "accessMode": "rw",
      "type": "enum",
      "specs": {
        "enumList": [
          {"value": 0, "desc": "手动"},
          {"value": 1, "desc": "自动"},
          {"value": 2, "desc": "定时"}
        ]
      }
    }
  ],
  "events": [
    {
      "id": "sensor_alarm",
      "name": "传感器告警",
      "outputData": [
        {
          "id": "alarm_code",
          "type": "string"
        }
      ]
    }
  ],
  "services": [
    {
      "id": "manual_watering",
      "name": "手动浇水",
      "inputData": [
        {
          "id": "duration",
          "name": "持续时间",
          "type": "number",
          "specs": {"min": 1, "max": 600}
        }
      ],
      "outputData": [
        {
          "id": "result",
          "type": "bool"
        }
      ]
    }
  ]
}
```

### 5.2 TuyaOS 编译约定

#### 5.2.1 CMakeLists.txt 规范

```cmake
# firmware/edge/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(shennong_edge_gateway)

# 设置 Tuya SDK
set(TUYA_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tuya_sdk")
set(TUYA_SDK_VERSION "2.5.0")

# 编译标志
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

# 源码目录
set(SOURCES
  src/main.c
  src/tuya_interface/mqtt_adapter.c
  src/duckyclaw/duckyclaw.c
  # ...
)

# 包含目录
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${TUYA_SDK_PATH}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/tuya_interface
)

# 链接库
link_directories(
  ${TUYA_SDK_PATH}/libs
)

# 生成可执行文件
add_executable(shennong_gateway ${SOURCES})

target_link_libraries(shennong_gateway
  tuya_sdk
  pthread
  ssl
  crypto
  m
)

# 安装规则
install(TARGETS shennong_gateway DESTINATION bin)
install(FILES config/thing_model.json DESTINATION etc)
install(FILES config/duckyclaw_config.json DESTINATION etc)
```

#### 5.2.2 编译命令约定

```bash
# 调试版本编译
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# 发布版本编译
mkdir -p build_release && cd build_release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
make install DESTDIR=./install

# 清理
make clean
```

### 5.3 文件命名规范

#### 5.3.1 源代码文件

| 类型 | 命名规范 | 示例 |
|------|--------|------|
| 模块源文件 | `module_name.c` | `mqtt_adapter.c`, `sensor_manager.c` |
| 模块头文件 | `module_name.h` | `mqtt_adapter.h` |
| 公共头文件 | `duckyclaw.h`, `types.h` | - |
| 平台相关 | `platform_<name>.c` | `platform_timer.c`, `platform_gpio.c` |
| 私有文件 | `module_name_private.h` | `mqtt_adapter_private.h` |

#### 5.3.2 配置文件

| 类型 | 命名规范 | 示例 |
|------|--------|------|
| JSON 配置 | `<module>_config.json` | `duckyclaw_config.json` |
| 物模型 | `thing_model.json` | - |
| 固件清单 | `firmware_manifest.json` | - |
| 策略规则 | `policy_rules.json` | - |
| 校准数据 | `<sensor>_calibration.bin` | `temperature_calibration.bin` |

#### 5.3.3 数据文件

| 类型 | 命名规范 | 示例 |
|------|--------|------|
| 传感器数据 | `sensor_data_YYYYMMDD.bin` | `sensor_data_20240501.bin` |
| 事件日志 | `event_log_YYYYMMDD.jsonl` | `event_log_20240501.jsonl` |
| 系统日志 | `YYYY_MM_system.log` | `2024_05_system.log` |
| 备份数据 | `backup_YYYYMMDD_HHMMSS.tar.gz` | `backup_20240501_120000.tar.gz` |

### 5.4 涂鸦消息协议约定

#### 5.4.1 MQTT 主题规范

```
# 属性上报
tylink://[device_id]/property/report

# 属性设置请求
tylink://[device_id]/property/set

# 事件上报
tylink://[device_id]/event/report

# 服务调用
tylink://[device_id]/service/invoke

# 远程配置推送
tylink://[device_id]/config/push

# OTA 升级通知
tylink://[device_id]/ota/update
```

#### 5.4.2 消息格式约定

**属性上报示例**

```json
{
  "protocol": 4,
  "timestamp": 1651478400000,
  "data": {
    "soil_moisture": 65.5,
    "temperature": 25.3,
    "irrigation_pump": false
  }
}
```

**事件上报示例**

```json
{
  "protocol": 4,
  "timestamp": 1651478400000,
  "events": [
    {
      "eventId": "sensor_alarm",
      "data": {
        "alarm_code": "SENSOR_DISCONNECTED"
      }
    }
  ]
}
```

---

## 6. 版本管理与依赖约定

### 6.1 版本号规范 (Semantic Versioning)

```
固件版本：MAJOR.MINOR.PATCH
示例：2.0.1

MAJOR：主版本号（不兼容的 API 变更）
MINOR：次版本号（向后兼容的功能新增）
PATCH：修订版本号（向后兼容的问题修复）

版本号文件：firmware/version.txt
格式：
FIRMWARE_VERSION=2.0.1
BUILD_TIMESTAMP=20240501_120000
BUILD_HASH=abc123def456
```

### 6.2 依赖管理

**requirements.txt**（Python 工具依赖）

```
tuya-iot-device-sdk==1.2.0
paho-mqtt==1.6.1
numpy==1.24.0
tensorflow==2.12.0
```

**submodules.txt**（外部库依赖）

```
meshtastic https://github.com/meshtastic/python v2.2.27
tflite-micro https://github.com/tensorflow/tflite-micro commit=abc123
```

---

## 7. 测试与质量保证规范

### 7.1 单元测试结构

```
firmware/edge/tests/
├── unit/
│   ├── test_duckyclaw.c
│   ├── test_sensor_manager.c
│   ├── test_mqtt_adapter.c
│   └── test_utils.h
├── integration/
│   ├── test_edge_gateway.c
│   ├── test_meshtastic_integration.c
│   └── test_ota.c
├── mock/
│   ├── mock_tuya_sdk.c
│   ├── mock_meshtastic.c
│   └── mock_storage.c
└── CMakeLists.txt
```

### 7.2 测试覆盖率目标

- 核心模块（duckyclaw）：≥ 90%
- 通信模块（MQTT、Meshtastic）：≥ 85%
- 存储模块：≥ 80%
- 总体覆盖率：≥ 85%

---

## 8. 文档编写规范

### 8.1 README.md 模板

```markdown
# [模块名称]

## 概述

[模块说明]

## 功能特性

- 功能1
- 功能2

## 目录结构

```
module/
├── src/
├── include/
└── config/
```

## 编译方法

```bash
make
```

## 配置说明

参考 `config/xxx_config.json`

## API 文档

### 函数名 (function_name)

**原型**：`int function_name(void);`

**参数**：无

**返回值**：成功返回 0，失败返回 -1

**说明**：函数功能描述

## 贡献指南

[贡献说明]
```

### 8.2 注释规范

```c
/**
 * @brief 函数简要说明
 * 
 * @param param1 参数1说明
 * @param param2 参数2说明
 * @return 返回值说明
 * 
 * @note 注意事项
 * @warning 警告信息
 * @example 使用示例
 */
int my_function(int param1, const char* param2);
```

---

## 9. 快速参考

### 9.1 常用配置清单

- [ ] 创建 `firmware/edge/config/duckyclaw_config.json`
- [ ] 创建 `firmware/edge/config/policy_rules.json`
- [ ] 创建 `firmware/edge/config/thing_model.json`
- [ ] 创建 `firmware/meshtastic/config/meshtastic_config.json`
- [ ] 创建 `firmware/ota/config/ota_config.json`
- [ ] 创建 `firmware/version.txt`
- [ ] 创建 `.gitignore`（排除 TF 卡映像、二进制文件等）

### 9.2 推荐工具链

- **编译**：GCC 9.0+, CMake 3.10+
- **版本控制**：Git
- **CI/CD**：GitHub Actions, GitLab CI
- **文档**：Markdown, Doxygen
- **测试**：CUnit, CMocka
- **代码分析**：cppcheck, clang-static-analyzer

### 9.3 关键文件清单

| 文件 | 位置 | 用途 |
|-----|-----|------|
| thing_model.json | `firmware/edge/config/` | 涂鸦物模型定义 |
| duckyclaw_config.json | `firmware/edge/config/` | duckyclaw 配置 |
| policy_rules.json | `firmware/edge/config/` | 本地控制策略 |
| meshtastic_config.json | `firmware/meshtastic/config/` | Meshtastic 射频配置 |
| ota_config.json | `firmware/ota/config/` | OTA 升级配置 |
| CMakeLists.txt | 各模块根目录 | 编译配置 |

---

## 10. 参考资源

- **涂鸦官方文档**：https://docs.tuya.com
- **Meshtastic 文档**：https://meshtastic.org/docs/
- **TensorFlow Lite**：https://www.tensorflow.org/lite
- **本项目架构文档**：`docs/ARCHITECTURE.md`


## �������ģ��˵��

��ϵͳ����������෽ʽ��֯ģ�飬��ǰ����ṹ������

- Kingdom_Plantae: ����ֲ�����豸�����Ӳ��ֻ����߲���������������������
- Kingdom_Bacteria: ���������豸����������������
- Kingdom_Fungi: ������豸��Ģ����ֲ����

�������ݴ���� io/ Ŀ¼�У��� Kingdom > Phylum > Device ��ʽ��֯��
## �������ģ��˵��

��ϵͳ����������෽ʽ��֯ģ�飬��ǰ����ṹ������

- Kingdom_Plantae: ����ֲ�����豸�����Ӳ��ֻ����߲�������ֲ��������������ֲ����
- Kingdom_Bacteria: ���������豸����������������
- Kingdom_Fungi: ������豸��Ģ����ֲ����

�������ݴ���� `bio/` Ŀ¼�У��� `Kingdom > Phylum > Device` ��ʽ��֯��
