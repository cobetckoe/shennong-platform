# 涂鸦 T5 平台开发 - 部署与最佳实践指南

## 一、本地开发环境搭建

### 1.1 系统要求

| 组件 | 要求版本 | 说明 |
|------|---------|------|
| Linux 内核 | 4.4+ | Ubuntu 18.04/20.04 推荐 |
| GCC | 9.0+ | 支持 C11 标准 |
| CMake | 3.10+ | 构建系统 |
| Git | 2.20+ | 版本控制 |
| Python | 3.8+ | 工具脚本语言 |
| OpenSSL | 1.1.1+ | 加密库 |

### 1.2 依赖安装

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y \
  build-essential cmake git \
  libssl-dev libcjson-dev libcurl4-openssl-dev \
  python3 python3-pip

# 安装 Python 依赖
pip3 install tuya-iot-device-sdk paho-mqtt cjson
```

### 1.3 项目克隆与初始化

```bash
# 克隆项目
git clone https://github.com/kanch-ai/shennong-platform.git
cd shennong-platform

# 初始化子模块
git submodule update --init --recursive

# 创建编译目录
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

---

## 二、集成开发工作流

### 2.1 基础工作流程

```
需求分析
    ↓
创建分支 (feature/xxx)
    ↓
本地开发
    ↓
单元测试 (≥ 90% 覆盖率)
    ↓
集成测试
    ↓
代码审查
    ↓
Merge 到 main
    ↓
自动化部署 (CI/CD)
    ↓
线上验证
```

### 2.2 Git 分支策略

```
main (生产版本)
  ↑
release/v1.x (发布分支)
  ↑
develop (开发分支)
  ↑
feature/* (功能分支)
hotfix/* (热修复分支)
```

**分支命名规范**

```
feature/duckyclaw-policy-engine
feature/meshtastic-integration
bugfix/ota-rollback-issue
hotfix/mqtt-connection-timeout
```

---

## 三、模块集成最佳实践

### 3.1 duckyclaw 集成步骤

#### 步骤 1：配置基础参数

```bash
# 编辑 firmware/edge/config/duckyclaw_config.json
# 关键参数：
#   - device_id: 网关唯一标识
#   - device_type: 设备类型
#   - model_path: TFLite 模型路径
#   - sample_interval: 传感器采样间隔（秒）
```

#### 步骤 2：定义控制策略

```bash
# 编辑 firmware/edge/config/policy_rules.json
# 定义各业务场景的控制规则
# 注意：priority 越高优先级越高，100 为最高优先级
```

#### 步骤 3：编译与测试

```bash
cd firmware/edge
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# 运行单元测试
ctest --verbose

# 集成测试（需要硬件）
./shennong_gateway --config ../config/duckyclaw_config.json --test-mode
```

#### 步骤 4：部署到 T5 网关

```bash
# 交叉编译用于 T5 硬件
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_TOOLCHAIN_FILE=../toolchain/T5.cmake ..
make

# 上传到设备
scp shennong_gateway root@192.168.1.100:/opt/shennong/
scp ../config/*.json root@192.168.1.100:/etc/shennong/

# SSH 连接到设备
ssh root@192.168.1.100
/opt/shennong/shennong_gateway --daemon
```

### 3.2 Meshtastic 集成步骤

#### 步骤 1：硬件连接

```bash
# 连接 Meshtastic 无线模块（通过 USB）
lsusb  # 验证 USB 设备

# 查看串口
ls -la /dev/ttyUSB*
```

#### 步骤 2：配置参数

```json
{
  "radio_config": {
    "frequency": 470000000,      // 470MHz 中国频段
    "spreading_factor": 9,        // LoRa 扩频因子
    "bandwidth": 62500            // 62.5kHz 带宽
  },
  "mesh_config": {
    "psk": "0x1a1a1a1a...",      // 网络加密密钥（各节点相同）
    "region": "CN"                // 区域设置
  }
}
```

#### 步骤 3：消息处理

```c
// 实现消息回调
void handle_mesh_message(mesh_message_t* msg) {
    // 1. 验证消息完整性
    if (!verify_message_integrity(msg)) {
        log_error("Invalid message CRC");
        return;
    }
    
    // 2. 根据消息类型处理
    switch (msg->message_type) {
        case MSG_TYPE_SENSOR_DATA:
            process_sensor_data(msg);
            break;
        case MSG_TYPE_CONTROL_CMD:
            process_control_command(msg);
            break;
        // ...
    }
    
    // 3. 存储到 TF 卡
    storage_log_message(msg);
}
```

#### 步骤 4：网络拓扑管理

```c
// 监控网络拓扑变化
void update_mesh_topology(void) {
    mesh_node_t* nodes = meshtastic_get_nodes();
    int node_count = meshtastic_get_node_count();
    
    for (int i = 0; i < node_count; i++) {
        if (nodes[i].signal_quality < 50) {
            log_warning("Node %d signal quality low: %d%%",
                       nodes[i].id, nodes[i].signal_quality);
        }
    }
    
    // 保存拓扑信息
    storage_save_topology(nodes, node_count);
}
```

### 3.3 TF 卡存储集成

#### 步骤 1：初始化存储

```c
int storage_init(const char* config_file) {
    // 1. 解析配置
    storage_config_t config;
    load_storage_config(config_file, &config);
    
    // 2. 挂载 TF 卡
    if (mount("/dev/mmcblk0p1", config->mount_point, "ext4", 0, "") != 0) {
        log_error("Failed to mount TF card");
        return -1;
    }
    
    // 3. 初始化索引
    init_storage_index();
    
    // 4. 清理过期数据
    cleanup_expired_data(config->retention_days);
    
    return 0;
}
```

#### 步骤 2：数据写入

```c
int storage_save_sensor_data(uint32_t device_id, sensor_data_t* data) {
    // 1. 构建时间戳目录
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char path[256];
    snprintf(path, sizeof(path),
            "/sdcard/data/sensors/%u/%04d_%02d/",
            device_id,
            tm_info->tm_year + 1900,
            tm_info->tm_mon + 1);
    
    // 2. 创建目录
    mkdirat_recursive(path);
    
    // 3. 二进制序列化
    sensor_record_t record = {
        .timestamp = now * 1000,  // 毫秒
        .device_id = device_id,
        .data = *data,
        .checksum = crc32_calculate(&data, sizeof(sensor_data_t))
    };
    
    // 4. 追加到日志文件
    FILE* fp = fopen(path, "ab");
    if (!fp) return -1;
    fwrite(&record, sizeof(record), 1, fp);
    fclose(fp);
    
    return 0;
}
```

#### 步骤 3：数据同步

```c
void storage_sync_to_cloud(void) {
    // 1. 扫描未同步数据
    storage_index_t* index = load_storage_index();
    
    for (int i = 0; i < index->file_count; i++) {
        if (!index->files[i].synced) {
            // 2. 读取文件
            char* data = read_file(index->files[i].path);
            
            // 3. 上传到云端
            int ret = cloud_upload_data(data, index->files[i].path);
            
            // 4. 更新索引
            if (ret == 0) {
                index->files[i].synced = true;
                save_storage_index(index);
            }
        }
    }
}
```

### 3.4 OTA 升级流程

#### 步骤 1：检查更新

```c
void check_firmware_update(void) {
    // 1. 构建检查请求
    ota_check_request_t req = {
        .current_version = FIRMWARE_VERSION,
        .device_id = get_device_id(),
        .device_type = get_device_type()
    };
    
    // 2. 向云端查询
    ota_manifest_t* manifest = ota_query_manifest(&req);
    
    // 3. 比较版本
    if (manifest && version_compare(manifest->version, FIRMWARE_VERSION) > 0) {
        log_info("New firmware available: %s", manifest->version);
        schedule_ota_download(manifest);
    }
}
```

#### 步骤 2：下载与验证

```c
int ota_download_and_verify(ota_manifest_t* manifest) {
    // 1. 检查升级窗口
    if (!in_update_window()) {
        log_info("Not in update window, scheduling later");
        return 0;
    }
    
    // 2. 下载固件
    const char* dest_path = "/sdcard/firmware/firmware_new.bin";
    if (download_file(manifest->url, dest_path) != 0) {
        log_error("Failed to download firmware");
        return -1;
    }
    
    // 3. 验证签名
    if (!verify_firmware_signature(dest_path, manifest->signature)) {
        log_error("Firmware signature verification failed");
        remove(dest_path);
        return -1;
    }
    
    // 4. 验证校验和
    char* checksum = calculate_file_hash(dest_path, "sha256");
    if (strcmp(checksum, manifest->sha256) != 0) {
        log_error("Firmware checksum mismatch");
        remove(dest_path);
        return -1;
    }
    
    log_info("Firmware download and verification successful");
    return 0;
}
```

#### 步骤 3：刷写与回滚

```c
int ota_flash_firmware(const char* firmware_path) {
    // 1. 备份当前固件
    if (backup_current_firmware() != 0) {
        log_error("Failed to backup current firmware");
        return -1;
    }
    
    // 2. 刷写新固件
    if (flash_firmware(firmware_path) != 0) {
        log_error("Failed to flash firmware");
        return -1;
    }
    
    // 3. 重启并验证
    system_reboot();
    sleep(30);
    
    // 4. 运行健康检查
    if (!run_health_check()) {
        log_error("Health check failed, initiating rollback");
        ota_rollback_firmware();
        return -1;
    }
    
    log_info("OTA upgrade completed successfully");
    return 0;
}
```

---

## 四、测试策略

### 4.1 单元测试框架

```bash
# 使用 CMocka 进行单元测试
# tests/unit/test_duckyclaw.c

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

// Mock 函数
int __wrap_policy_execute(const char* policy_id) {
    return mock_type(int);
}

// 测试用例
static void test_policy_evaluation(void** state) {
    // 准备数据
    sensor_data_t data = {
        .soil_moisture = 45.0,
        .temperature = 25.0
    };
    
    // 设置期望
    will_return(__wrap_policy_execute, 0);
    
    // 执行
    policy_context_t ctx = {.sensor_state = data};
    int ret = duckyclaw_evaluate_policies(&ctx);
    
    // 验证
    assert_int_equal(ret, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_policy_evaluation),
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}
```

### 4.2 集成测试

```bash
# tests/integration/test_edge_gateway.c
# 测试完整的边缘网关功能链

void test_sensor_to_storage_pipeline(void) {
    // 1. 初始化所有模块
    duckyclaw_init("config/duckyclaw_config.json");
    meshtastic_init("/dev/ttyUSB0", 115200);
    storage_init("config/storage_config.json");
    
    // 2. 模拟传感器数据
    sensor_data_t data = generate_test_data();
    
    // 3. 验证数据流
    duckyclaw_update_sensor_data(&data);
    storage_save_sensor_data(1, &data);
    
    // 4. 检查存储结果
    sensor_data_t* retrieved = storage_read_sensor_data(1, 1);
    assert_memory_equal(&data, retrieved, sizeof(sensor_data_t));
}
```

### 4.3 性能测试

```bash
# 测试 duckyclaw 推理性能
# tests/performance/test_inference_latency.c

void benchmark_policy_evaluation(void) {
    const int ITERATIONS = 1000;
    clock_t start, end;
    double cpu_time;
    
    start = clock();
    
    for (int i = 0; i < ITERATIONS; i++) {
        sensor_data_t data = generate_random_data();
        policy_context_t ctx = {.sensor_state = data};
        duckyclaw_evaluate_policies(&ctx);
    }
    
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Average latency: %.2f ms\n", (cpu_time / ITERATIONS) * 1000);
    
    // 验证性能指标
    assert_true(cpu_time / ITERATIONS < 0.010);  // < 10ms
}
```

---

## 五、监控与故障排查

### 5.1 日志配置

```json
{
  "logging": {
    "level": "info",           // debug, info, warn, error
    "output": "both",          // console, file, both
    "file_path": "/sdcard/logs/duckyclaw.log",
    "max_file_size": 10485760, // 10MB
    "backup_count": 5,
    "format": "[%(timestamp)s] [%(level)s] %(module)s: %(message)s"
  }
}
```

### 5.2 常见故障排查

#### 问题：Meshtastic 无法接收消息

```bash
# 1. 检查串口连接
dmesg | grep ttyUSB

# 2. 测试串口通信
minicom -D /dev/ttyUSB0 -b 115200

# 3. 检查 Meshtastic 固件版本
./meshtastic --info

# 4. 重置 Meshtastic 设备
./meshtastic --reboot
```

#### 问题：TF 卡写入失败

```bash
# 1. 检查挂载状态
mount | grep /sdcard

# 2. 检查磁盘空间
df -h /sdcard

# 3. 检查文件权限
ls -la /sdcard/data/sensors/

# 4. 运行文件系统检查
fsck.ext4 -n /dev/mmcblk0p1

# 5. 清理过期数据
find /sdcard/data -mtime +90 -delete
```

#### 问题：duckyclaw 策略不执行

```bash
# 1. 验证配置文件语法
python3 -m json.tool config/policy_rules.json

# 2. 检查日志
tail -f /sdcard/logs/duckyclaw.log | grep "policy"

# 3. 运行调试模式
./shennong_gateway --config config/duckyclaw_config.json --debug-policy

# 4. 验证传感器数据
cat /sdcard/data/sensors/*/latest_sensor_data.bin | hexdump -C
```

#### 问题：MQTT 连接超时

```bash
# 1. 检查网络连接
ping mqtt.tuyacn.com

# 2. 验证 MQTT 凭证
cat config/duckyclaw_config.json | grep -A5 tuya_config

# 3. 测试 MQTT 连接
mosquitto_pub -h mqtt.tuyacn.com -p 8883 --cafile ca.crt -t test/topic -m "hello"

# 4. 查看连接日志
grep -i mqtt /sdcard/logs/duckyclaw.log
```

---

## 六、性能优化建议

### 6.1 duckyclaw 优化

| 优化点 | 方法 | 预期效果 |
|--------|------|--------|
| 策略评估 | 缓存条件结果，减少重复计算 | -30% CPU |
| 模型推理 | 使用 INT8 量化模型 | -60% 内存，+20% 速度 |
| 传感器采样 | 增加采样间隔，使用滑动平均 | -50% I/O |
| 日志写入 | 批量写入，异步刷盘 | -40% I/O |

### 6.2 Meshtastic 优化

```json
{
  "optimization": {
    "batch_messages": true,          // 批量发送消息
    "batch_size": 20,
    "compression": true,              // 启用压缩
    "skip_hello_on_startup": true,    // 跳过启动问候消息
    "want_all_nodelist": false        // 不请求完整节点列表
  }
}
```

### 6.3 存储优化

```c
// 使用循环缓冲区减少磁盘写入
typedef struct {
    sensor_record_t buffer[1000];
    int write_index;
    int flush_threshold;  // 缓冲区达到此大小时刷盘
} storage_buffer_t;

void storage_flush_buffer(storage_buffer_t* buf) {
    if (buf->write_index >= buf->flush_threshold) {
        // 批量写入
        write_records_to_disk(buf->buffer, buf->write_index);
        buf->write_index = 0;
    }
}
```

---

## 七、生产部署清单

部署前必须完成：

- [ ] **代码审查**：所有代码改动已通过审查
- [ ] **测试覆盖率**：≥ 85% 代码覆盖率
- [ ] **性能基准**：关键路径 < 10ms
- [ ] **安全审计**：无已知安全漏洞
- [ ] **文档完整**：API 文档、部署指南完整
- [ ] **版本号**：更新 `firmware/version.txt`
- [ ] **配置验证**：所有 JSON 配置文件有效
- [ ] **硬件兼容性**：在目标硬件上验证
- [ ] **网络测试**：MQTT、Meshtastic、TF 卡 OK
- [ ] **冒烟测试**：完整功能端到端测试通过
- [ ] **回滚方案**：OTA 回滚机制已验证
- [ ] **监控就位**：日志、告警系统已配置

---

## 八、相关资源链接

- **涂鸦开发文档**：https://docs.tuya.com
- **Meshtastic 官方**：https://meshtastic.org
- **TensorFlow Lite 优化**：https://www.tensorflow.org/lite/guide/optimization
- **CMake 教程**：https://cmake.org/cmake/help/latest/
- **Git 最佳实践**：https://git-scm.com/book/en/v2

