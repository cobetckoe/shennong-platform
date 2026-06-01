# 涂鸦 T5 平台开发规范 - 快速参考指南

## 一、项目初始化清单

### 1. 创建标准目录结构

```bash
firmware/
├── edge/
│   ├── src/
│   │   ├── core/
│   │   ├── duckyclaw/
│   │   ├── models/
│   │   ├── sensors/
│   │   ├── controllers/
│   │   ├── storage/
│   │   ├── communication/
│   │   └── ui/
│   ├── include/
│   ├── config/
│   ├── tests/
│   ├── tuya_interface/
│   └── CMakeLists.txt
├── meshtastic/
│   ├── src/
│   ├── include/
│   ├── config/
│   └── CMakeLists.txt
├── storage/
│   ├── src/
│   ├── include/
│   └── CMakeLists.txt
├── control/
│   ├── src/
│   └── CMakeLists.txt
├── ota/
│   ├── src/
│   ├── include/
│   ├── config/
│   └── CMakeLists.txt
├── version.txt
└── README.md
```

### 2. 初始化脚本

```bash
#!/bin/bash
# init_project.sh - 初始化项目结构

mkdir -p firmware/edge/{src/{core,duckyclaw,models,sensors,controllers,storage,communication,ui},include,config,tests/{unit,integration,mock},tuya_interface}
mkdir -p firmware/meshtastic/{src,include,config}
mkdir -p firmware/storage/{src,include}
mkdir -p firmware/control/{src,include}
mkdir -p firmware/ota/{src,include,config,certs}

# 初始化 Git 子模块
git submodule add https://github.com/meshtastic/python firmware/meshtastic/libs/python

# 创建版本文件
echo "FIRMWARE_VERSION=1.0.0" > firmware/version.txt
echo "BUILD_TIMESTAMP=$(date +%Y%m%d_%H%M%S)" >> firmware/version.txt

echo "项目初始化完成"
```

---

## 二、核心配置文件模板

### 1. duckyclaw_config.json

```json
{
  "version": "1.0",
  "device_id": "gateway_001",
  "device_type": "seed-planting",
  "tuya_config": {
    "product_id": "xxxxxxxxx",
    "device_token": "device_token_string",
    "region": "cn",
    "mqtt_host": "mqtt.tuyacn.com",
    "mqtt_port": 8883,
    "use_tls": true
  },
  "local_mode": {
    "enabled": true,
    "fallback_timeout": 300,
    "offline_control": true,
    "local_api_port": 8080
  },
  "model": {
    "type": "edge_model",
    "version": "v1.0.0",
    "update_interval": 3600,
    "inference_engine": "tflite",
    "model_path": "/sdcard/models/edge_model_v1.0.tflite",
    "input_size": 10,
    "output_size": 4
  },
  "sensors": {
    "sample_interval": 60,
    "filter_type": "kalman",
    "calibration_data": "/sdcard/config/sensors/calibration.bin",
    "sensor_list": {
      "temperature": {"pin": "ADC0", "enabled": true},
      "humidity": {"pin": "ADC1", "enabled": true},
      "soil_moisture": {"pin": "ADC2", "enabled": true},
      "light_intensity": {"pin": "GPIO_3", "enabled": true}
    }
  },
  "storage": {
    "backend": "tf_card",
    "mount_point": "/sdcard",
    "buffer_size": 1000,
    "sync_interval": 300,
    "retention_days": 90,
    "data_format": "binary"
  },
  "meshtastic": {
    "enabled": true,
    "radio_port": "/dev/ttyUSB0",
    "baud_rate": 115200,
    "device_serial": "S0000000",
    "node_num": 1
  },
  "logging": {
    "level": "info",
    "output": "both",
    "file_path": "/sdcard/logs/duckyclaw.log",
    "max_file_size": 10485760,
    "backup_count": 5
  },
  "performance": {
    "watchdog_enabled": true,
    "watchdog_timeout": 60,
    "cpu_throttle": true,
    "memory_limit_mb": 512
  }
}
```

### 2. policy_rules.json

```json
{
  "version": "1.0",
  "policies": [
    {
      "id": "auto_irrigation_v1",
      "name": "自动灌溉 - 蔬菜",
      "priority": 10,
      "enabled": true,
      "crop_type": "vegetable",
      "growth_stage": "all",
      "conditions": {
        "soil_moisture": {
          "operator": "<",
          "value": 50,
          "duration_sec": 30
        },
        "temperature": {
          "operator": ">",
          "value": 15
        },
        "time_since_last_watering": {
          "operator": ">",
          "value": 300
        }
      },
      "actions": [
        {
          "type": "actuator_control",
          "actuator_id": "irrigation_pump",
          "action": "on",
          "duration": 300,
          "power_level": 100
        }
      ],
      "timeout": 600
    },
    {
      "id": "humidity_control_v1",
      "name": "湿度控制 - 喷雾",
      "priority": 8,
      "enabled": true,
      "crop_type": "vegetable",
      "conditions": {
        "humidity": {
          "operator": "<",
          "value": 60
        }
      },
      "actions": [
        {
          "type": "actuator_control",
          "actuator_id": "mist_pump",
          "action": "pulse",
          "duration": 10,
          "interval": 60
        }
      ]
    },
    {
      "id": "temperature_warning",
      "name": "温度告警",
      "priority": 50,
      "enabled": true,
      "conditions": {
        "or": [
          {
            "temperature": {
              "operator": "<",
              "value": 10
            }
          },
          {
            "temperature": {
              "operator": ">",
              "value": 35
            }
          }
        ]
      },
      "actions": [
        {
          "type": "trigger_alert",
          "alert_type": "warning",
          "message": "环境温度异常，请检查"
        },
        {
          "type": "log_event",
          "log_level": "warning"
        }
      ]
    },
    {
      "id": "manual_mode_override",
      "name": "手动模式",
      "priority": 100,
      "enabled": true,
      "trigger_source": "local_button",
      "actions": [
        {
          "type": "set_device_mode",
          "mode": "manual"
        }
      ]
    }
  ]
}
```

### 3. thing_model.json

```json
{
  "version": "1.0",
  "schema": "schema_version",
  "productId": "xxxxxxxxx",
  "properties": [
    {
      "id": "soil_moisture",
      "name": "土壤湿度",
      "desc": "当前土壤湿度百分比",
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
      "id": "temperature",
      "name": "温度",
      "desc": "环境温度",
      "accessMode": "r",
      "type": "number",
      "specs": {
        "min": -20,
        "max": 60,
        "step": 0.1,
        "unitName": "℃"
      }
    },
    {
      "id": "humidity",
      "name": "湿度",
      "desc": "环境相对湿度",
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
      "id": "light_intensity",
      "name": "光照强度",
      "desc": "光照强度（勒克斯）",
      "accessMode": "r",
      "type": "number",
      "specs": {
        "min": 0,
        "max": 100000,
        "step": 1,
        "unitName": "lux"
      }
    },
    {
      "id": "irrigation_pump",
      "name": "灌溉泵",
      "desc": "灌溉泵开关控制",
      "accessMode": "rw",
      "type": "bool",
      "specs": {}
    },
    {
      "id": "mist_pump",
      "name": "雾化泵",
      "desc": "雾化喷洒泵控制",
      "accessMode": "rw",
      "type": "bool",
      "specs": {}
    },
    {
      "id": "grow_light",
      "name": "生长灯",
      "desc": "LED 生长灯开关与亮度",
      "accessMode": "rw",
      "type": "integer",
      "specs": {
        "min": 0,
        "max": 100,
        "step": 1,
        "unitName": "%"
      }
    },
    {
      "id": "device_mode",
      "name": "设备模式",
      "desc": "设备工作模式选择",
      "accessMode": "rw",
      "type": "enum",
      "specs": {
        "enumList": [
          {
            "value": 0,
            "desc": "手动"
          },
          {
            "value": 1,
            "desc": "自动"
          },
          {
            "value": 2,
            "desc": "定时"
          },
          {
            "value": 3,
            "desc": "离线"
          }
        ]
      }
    },
    {
      "id": "crop_type",
      "name": "作物类型",
      "desc": "当前种植作物类型",
      "accessMode": "rw",
      "type": "enum",
      "specs": {
        "enumList": [
          {
            "value": 1,
            "desc": "蔬菜"
          },
          {
            "value": 2,
            "desc": "土豆"
          },
          {
            "value": 3,
            "desc": "螺旋藻"
          }
        ]
      }
    },
    {
      "id": "network_status",
      "name": "网络状态",
      "desc": "当前网络连接状态",
      "accessMode": "r",
      "type": "enum",
      "specs": {
        "enumList": [
          {
            "value": 0,
            "desc": "离线"
          },
          {
            "value": 1,
            "desc": "云连接"
          },
          {
            "value": 2,
            "desc": "Mesh 连接"
          },
          {
            "value": 3,
            "desc": "云 + Mesh"
          }
        ]
      }
    }
  ],
  "events": [
    {
      "id": "sensor_alarm",
      "name": "传感器告警",
      "desc": "传感器异常或离线",
      "outputData": [
        {
          "id": "alarm_code",
          "name": "告警代码",
          "type": "string",
          "specs": {}
        },
        {
          "id": "sensor_name",
          "name": "传感器名称",
          "type": "string",
          "specs": {}
        }
      ]
    },
    {
      "id": "device_alarm",
      "name": "设备告警",
      "desc": "设备运行异常",
      "outputData": [
        {
          "id": "alarm_type",
          "name": "告警类型",
          "type": "string",
          "specs": {}
        }
      ]
    },
    {
      "id": "policy_executed",
      "name": "策略执行完成",
      "desc": "本地控制策略执行完成",
      "outputData": [
        {
          "id": "policy_id",
          "name": "策略 ID",
          "type": "string",
          "specs": {}
        },
        {
          "id": "result",
          "name": "执行结果",
          "type": "enum",
          "specs": {
            "enumList": [
              {"value": 0, "desc": "成功"},
              {"value": 1, "desc": "失败"}
            ]
          }
        }
      ]
    }
  ],
  "services": [
    {
      "id": "manual_watering",
      "name": "手动浇水",
      "desc": "立即执行灌溉，指定持续时间",
      "inputData": [
        {
          "id": "duration",
          "name": "浇水时长",
          "type": "integer",
          "specs": {
            "min": 1,
            "max": 3600,
            "step": 1,
            "unitName": "秒"
          }
        }
      ],
      "outputData": [
        {
          "id": "result",
          "name": "执行结果",
          "type": "bool",
          "specs": {}
        },
        {
          "id": "message",
          "name": "执行消息",
          "type": "string",
          "specs": {}
        }
      ]
    },
    {
      "id": "reset_device",
      "name": "重置设备",
      "desc": "恢复出厂设置",
      "inputData": [],
      "outputData": [
        {
          "id": "result",
          "name": "执行结果",
          "type": "bool",
          "specs": {}
        }
      ]
    },
    {
      "id": "query_logs",
      "name": "查询日志",
      "desc": "查询本地存储的运行日志",
      "inputData": [
        {
          "id": "log_type",
          "name": "日志类型",
          "type": "enum",
          "specs": {
            "enumList": [
              {"value": 0, "desc": "系统日志"},
              {"value": 1, "desc": "传感器日志"},
              {"value": 2, "desc": "控制日志"}
            ]
          }
        },
        {
          "id": "limit",
          "name": "返回条数",
          "type": "integer",
          "specs": {"min": 1, "max": 1000}
        }
      ],
      "outputData": [
        {
          "id": "logs",
          "name": "日志数据",
          "type": "string",
          "specs": {}
        }
      ]
    }
  ]
}
```

### 4. meshtastic_config.json

```json
{
  "version": "1.0",
  "radio_config": {
    "frequency": 470000000,
    "bandwidth": 62500,
    "spreading_factor": 9,
    "coding_rate": "4/8",
    "tx_power": 17,
    "mode_preset": "longFast"
  },
  "mesh_config": {
    "channel": 0,
    "psk": "0x1a1a1a1a2b2b2b2b3c3c3c3c4d4d4d4d",
    "node_delay": 100,
    "position_broadcast_secs": 900,
    "region": "CN"
  },
  "device_config": {
    "node_num": 1,
    "long_name": "Shennong_Gateway_01",
    "short_name": "GW01",
    "hardware_model": "T5",
    "firmware_version": "2.0.0"
  },
  "serial_config": {
    "port": "/dev/ttyUSB0",
    "baud_rate": 115200,
    "timeout": 10
  },
  "data_sync": {
    "batch_size": 10,
    "sync_interval": 300,
    "retry_count": 3,
    "timeout": 30,
    "compression": true
  },
  "filtering": {
    "ignore_incoming": false,
    "sanitize_ascii": true,
    "want_all_nodelist": true
  }
}
```

### 5. ota_config.json

```json
{
  "version": "1.0",
  "ota_enabled": true,
  "update_server": "https://ota.shennong.cloud/firmware",
  "check_interval": 86400,
  "update_strategy": "scheduled",
  "scheduled_window": {
    "hour_start": 2,
    "hour_end": 6,
    "day_of_week": [0, 1, 2, 3, 4, 5, 6]
  },
  "download_config": {
    "max_concurrent_downloads": 1,
    "chunk_size": 8192,
    "timeout": 3600,
    "verify_checksum": true
  },
  "rollback": {
    "max_retries": 3,
    "auto_rollback": true,
    "health_check_timeout": 300,
    "health_check_interval": 10
  },
  "signature": {
    "enabled": true,
    "algorithm": "RSA-2048",
    "public_key_file": "firmware/ota/certs/public_key.pem",
    "verify_chain": true
  },
  "notification": {
    "send_start": true,
    "send_progress": false,
    "send_complete": true,
    "notify_via": ["mqtt", "local_log"]
  }
}
```

---

## 三、目录结构速查表

### A. 固件（firmware/）

```
firmware/
├── edge/                  ← T5 核心网关逻辑
│   ├── duckyclaw/        ← 本地策略执行引擎 ⭐
│   ├── tuya_interface/   ← 涂鸦 SDK 适配层
│   └── config/           ← 配置文件
├── meshtastic/           ← Mesh 网络模块
├── storage/              ← TF 卡存储管理
├── control/              ← 本地控制逻辑
├── ota/                  ← 固件升级机制
└── [device-type]/        ← 设备子固件
    ├── seed-planting/
    ├── vegetable-planting/
    ├── potato-planting/
    └── spirulina-culture/
```

### B. 配置文件清单

```
config/ 目录下应包含：
├── duckyclaw_config.json      ✓ 必需
├── policy_rules.json           ✓ 必需
├── thing_model.json            ✓ 必需
├── meshtastic_config.json      ✓ 必需
├── ota_config.json             ✓ 必需
└── device_config.json          (可选)
```

### C. TF 卡存储约定

```
/sdcard/
├── data/sensors/              ← 传感器二进制数据
├── data/events/               ← 事件日志（JSONL）
├── models/                    ← 本地 TFLite 模型
├── config/                    ← 配置备份
├── logs/                      ← 运行日志
└── firmware/                  ← OTA 升级包
```

---

## 四、关键代码片段

### 1. duckyclaw 初始化示例

```c
#include "duckyclaw.h"

int main(void) {
    // 初始化 duckyclaw
    int ret = duckyclaw_init("config/duckyclaw_config.json");
    if (ret != 0) {
        fprintf(stderr, "Failed to init duckyclaw: %d\n", ret);
        return -1;
    }
    
    // 加载策略规则
    ret = duckyclaw_load_policies("config/policy_rules.json");
    if (ret != 0) {
        fprintf(stderr, "Failed to load policies: %d\n", ret);
        return -1;
    }
    
    // 主循环
    while (1) {
        sensor_data_t sensor_data;
        
        // 更新传感器数据
        duckyclaw_update_sensor_data(&sensor_data);
        
        // 评估策略
        policy_context_t ctx = {.sensor_state = sensor_data};
        duckyclaw_evaluate_policies(&ctx);
        
        sleep(1);
    }
    
    duckyclaw_shutdown();
    return 0;
}
```

### 2. Meshtastic 消息处理示例

```c
#include "meshtastic.h"

void on_receive_message(mesh_message_t* msg) {
    if (msg->message_type == MSG_TYPE_SENSOR_DATA) {
        // 处理传感器数据
        sensor_data_t* sensor_data = (sensor_data_t*)msg->payload;
        
        // 保存到 TF 卡
        storage_save_sensor_data(msg->from_id, sensor_data);
        
        // 转发给 duckyclaw
        duckyclaw_update_sensor_data(sensor_data);
    }
}

int main(void) {
    meshtastic_init("/dev/ttyUSB0", 115200);
    meshtastic_register_rx_callback(on_receive_message);
    
    while (1) {
        meshtastic_process_events();
        sleep(1);
    }
    
    return 0;
}
```

### 3. MQTT 属性上报示例

```c
#include "mqtt_adapter.h"

void report_sensor_data(sensor_data_t* data) {
    cJSON* json = cJSON_CreateObject();
    
    cJSON_AddNumberToObject(json, "soil_moisture", data->soil_moisture);
    cJSON_AddNumberToObject(json, "temperature", data->temperature);
    cJSON_AddNumberToObject(json, "humidity", data->humidity);
    
    char* json_str = cJSON_Print(json);
    
    // 发送到 Tuya 云端
    mqtt_publish("tylink://device_001/property/report", json_str);
    
    cJSON_Delete(json);
}
```

---

## 五、编译与构建

### CMakeLists.txt 标准模板

```cmake
cmake_minimum_required(VERSION 3.10)
project(shennong_edge_gateway)

# 版本信息
set(PROJECT_VERSION "1.0.0")
set(CMAKE_C_STANDARD 11)

# Tuya SDK
set(TUYA_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tuya_sdk")
link_directories(${TUYA_SDK_PATH}/libs)
include_directories(${TUYA_SDK_PATH}/include)

# 项目源码
file(GLOB SOURCES "src/**/*.c")
file(GLOB_RECURSE HEADERS "include/**/*.h")

# 编译标志
add_compile_options(-Wall -Wextra -Werror)
add_compile_options($<$<CONFIG:Debug>:-g -O0>)
add_compile_options($<$<CONFIG:Release>:-O2 -DNDEBUG>)

# 可执行文件
add_executable(shennong_gateway ${SOURCES})

# 链接库
target_link_libraries(shennong_gateway
    tuya_sdk
    pthread
    ssl
    crypto
    m
)

# 安装
install(TARGETS shennong_gateway DESTINATION bin)
install(FILES config/thing_model.json DESTINATION etc)
install(FILES config/duckyclaw_config.json DESTINATION etc)

# 测试
enable_testing()
add_subdirectory(tests)
```

### 编译命令

```bash
# 创建编译目录
mkdir build && cd build

# 调试版本
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# 发布版本
cmake -DCMAKE_BUILD_TYPE=Release ..
make
make install DESTDIR=./install

# 清理
make clean
rm -rf *
```

---

## 六、命名规范速查

| 类型 | 规范 | 示例 |
|------|------|------|
| **C 源文件** | `module_name.c` | `mqtt_adapter.c` |
| **头文件** | `module_name.h` | `sensor_manager.h` |
| **JSON 配置** | `<purpose>_config.json` | `duckyclaw_config.json` |
| **二进制数据** | `data_YYYYMMDD.bin` | `sensor_data_20240501.bin` |
| **日志文件** | `YYYY_MM_<type>.log` | `2024_05_system.log` |
| **函数** | `snake_case` | `duckyclaw_init()` |
| **宏常量** | `UPPER_CASE` | `MSG_TYPE_SENSOR_DATA` |
| **变量** | `snake_case` | `sensor_data` |
| **类型** | `name_t` | `sensor_data_t` |

---

## 七、验证清单

部署前检查：

- [ ] 所有 JSON 配置文件语法正确
- [ ] 物模型 (thing_model.json) 已定义所有属性
- [ ] duckyclaw 策略规则已配置
- [ ] Meshtastic 射频参数与硬件匹配
- [ ] TF 卡挂载点正确（/sdcard）
- [ ] OTA 签名证书已配置
- [ ] CMakeLists.txt 编译无错误
- [ ] 单元测试覆盖率 ≥ 85%
- [ ] 代码审查已完成
- [ ] 版本号已更新 (firmware/version.txt)

---

## 八、常见问题快速查阅

| 问题 | 解决方案 |
|------|--------|
| duckyclaw 策略不执行 | 检查 `policy_rules.json` 条件配置，确保 `enabled: true` |
| Meshtastic 无法连接 | 验证 `meshtastic_config.json` 中的串口配置与物理连接 |
| TF 卡数据写入失败 | 检查 `/sdcard` 挂载点和磁盘空间 |
| MQTT 连接超时 | 确认网络连接，检查 `tuya_config` 中的服务器地址 |
| OTA 升级失败 | 验证签名文件，检查 `ota_config.json` 中的签名配置 |
| 传感器数据异常 | 运行传感器校准，检查 `calibration.bin` 文件 |

