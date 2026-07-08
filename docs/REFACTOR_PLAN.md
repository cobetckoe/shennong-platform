# 神农平台重构计划

## 重构目标

放弃本地推理功能，基于TuyaOS原生能力重构，实现工业商业级可靠性。

## 架构变更

### 重构前
```
┌─────────────────────────────────────────┐
│           自定义模块（冗余）              │
│  model_manager + data_manager + storage │
│  bio_config + 自定义协议                  │
└─────────────────────────────────────────┘
```

### 重构后
```
┌─────────────────────────────────────────┐
│           TuyaOS 原生能力                │
│  物模型(DP) + OTA + 云端规则引擎         │
│  App控制 + 设备管理                      │
├─────────────────────────────────────────┤
│           精简业务逻辑                   │
│  传感器采集 + 控制执行 + 状态上报        │
└─────────────────────────────────────────┘
```

## 模块重构清单

### 1. 删除模块（不再需要）
| 文件 | 原因 |
|------|------|
| model_manager.c/h | 放弃本地模型推理 |
| storage.c/h | 使用TuyaOS KV存储 |
| bio_config.c/h | 迁移到云端配置 |

### 2. 简化模块（保留核心）
| 文件 | 变更 |
|------|------|
| data_manager.c/h | 简化为传感器数据采集+上报 |
| main.c | 集成TuyaOS初始化和主循环 |

### 3. 新增模块（工业级可靠性）
| 文件 | 功能 |
|------|------|
| watchdog.c/h | 硬件看门狗 |
| power_manager.c/h | 断电检测与恢复 |
| error_handler.c/h | 统一错误处理 |
| tuya_dp_handler.c/h | 物模型数据点处理 |

## TuyaOS 物模型设计

### 网关 DP 点定义
| DP ID | 名称 | 类型 | 说明 |
|-------|------|------|------|
| 1 | 温度 | value | 传感器温度 |
| 2 | 湿度 | value | 传感器湿度 |
| 3 | 光照 | value | 传感器光照 |
| 4 | pH值 | value | 传感器pH |
| 5 | 喷雾泵 | bool | 控制喷雾泵 |
| 6 | LED灯 | bool | 控制LED灯 |
| 7 | 风扇 | bool | 控制风扇 |
| 8 | 设备状态 | enum | 在线/离线/故障 |

### 数据上报策略
- 传感器数据：定时上报（可配置间隔）
- 状态变化：立即上报
- 告警信息：立即上报
- 批量数据：缓存后批量上报

## 工业级可靠性设计

### 1. 看门狗保护
```c
// 硬件看门狗初始化
void watchdog_init(uint32_t timeout_ms);

// 喂狗
void watchdog_feed(void);

// 系统复位
void system_reset(void);
```

### 2. 断电恢复
```c
// 断电检测
bool power_check_loss(void);

// 保存关键数据
void power_save_critical_data(void);

// 恢复状态
void power_restore_state(void);
```

### 3. 错误处理
```c
// 错误码定义
typedef enum {
    ERR_NONE = 0,
    ERR_SENSOR_TIMEOUT,
    ERR_CONTROL_FAIL,
    ERR_NETWORK_DISCONNECT,
    ERR_STORAGE_FAIL
} error_code_t;

// 统一错误处理
void error_handler(error_code_t code);
```

### 4. 数据校验
- CRC16校验关键数据
- 双备份存储
- 版本号管理

## 编译配置

### CMakeLists.txt 变更
```cmake
# 移除不需要的模块
# add_library(t5-gateway
#   src/model_manager.c
#   src/storage.c
#   src/bio_config.c
# )

# 精简后
add_library(t5-gateway
  src/main.c
  src/data_manager.c
  src/watchdog.c
  src/power_manager.c
  src/error_handler.c
  src/tuya_dp_handler.c
)
```

## 开发步骤

1. **Phase 1: 环境准备**
   - 下载TuyaOS SDK
   - 配置Tuya Wind IDE
   - 创建新项目

2. **Phase 2: 核心重构**
   - 删除冗余模块
   - 实现TuyaOS初始化
   - 设计物模型

3. **Phase 3: 可靠性增强**
   - 实现看门狗
   - 实现断电恢复
   - 实现错误处理

4. **Phase 4: 测试验证**
   - 单元测试
   - 集成测试
   - 可靠性测试

## 预期收益

| 指标 | 重构前 | 重构后 |
|------|--------|--------|
| 代码量 | ~2000行 | ~800行 |
| Flash占用 | ~500KB | ~200KB |
| 开发周期 | 长 | 短 |
| 可靠性 | 中 | 高 |
| 维护成本 | 高 | 低 |
