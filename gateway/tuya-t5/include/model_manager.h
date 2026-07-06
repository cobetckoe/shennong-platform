#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

// 模型信息结构
typedef struct {
    char name[64];           // 模型名称
    uint32_t version;        // 模型版本
    uint32_t size;           // 模型大小
    char type[32];           // 模型类型（控制、分类等）
    char生物类型[32];        // 适用的生物类型
} model_info_t;

// 模型管理初始化
bool model_manager_init(void);

// 保存模型
bool model_save(const char *name, uint32_t version, const void *data, uint32_t size);

// 加载模型
bool model_load(const char *name, uint32_t version, void *data, uint32_t size);

// 删除模型
bool model_delete(const char *name, uint32_t version);

// 获取模型信息
bool model_get_info(const char *name, uint32_t version, model_info_t *info);

// 列出所有模型
int model_list(char *names[], int max_count);

// 获取最新模型版本
uint32_t model_get_latest_version(const char *name);

#endif // MODEL_MANAGER_H