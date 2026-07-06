#include "model_manager.h"
#include "storage.h"
#include <stdio.h>
#include <string.h>

#define MODELS_DIR "models"

// 模型管理初始化
bool model_manager_init(void) {
    // 创建模型目录
    mkdir(MODELS_DIR, 0755);
    return true;
}

// 保存模型
bool model_save(const char *name, uint32_t version, const void *data, uint32_t size) {
    char filename[128];
    snprintf(filename, sizeof(filename), "%s/%s_v%u.bin", MODELS_DIR, name, version);
    
    return storage_save(filename, data, size);
}

// 加载模型
bool model_load(const char *name, uint32_t version, void *data, uint32_t size) {
    char filename[128];
    snprintf(filename, sizeof(filename), "%s/%s_v%u.bin", MODELS_DIR, name, version);
    
    return storage_load(filename, data, size);
}

// 删除模型
bool model_delete(const char *name, uint32_t version) {
    char filename[128];
    snprintf(filename, sizeof(filename), "%s/%s_v%u.bin", MODELS_DIR, name, version);
    
    return storage_delete(filename);
}

// 获取模型信息
bool model_get_info(const char *name, uint32_t version, model_info_t *info) {
    char filename[128];
    snprintf(filename, sizeof(filename), "%s/%s_v%u.bin", MODELS_DIR, name, version);
    
    if (!storage_exists(filename)) {
        return false;
    }
    
    strncpy(info->name, name, sizeof(info->name) - 1);
    info->version = version;
    info->size = storage_size(filename);
    
    return true;
}

// 列出所有模型
int model_list(char *names[], int max_count) {
    // 简化实现
    return 0;
}

// 获取最新模型版本
uint32_t model_get_latest_version(const char *name) {
    // 简化实现，实际需要遍历模型文件
    return 1;
}