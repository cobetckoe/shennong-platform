#ifndef BIO_CONFIG_H
#define BIO_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

// 生物类型枚举
typedef enum {
    BIO_TYPE_TUBER,      // 块茎类（土豆）
    BIO_TYPE_ALGAE,      // 藻类（螺旋藻）
    BIO_TYPE_LEAF,       // 叶菜类（蔬菜）
    BIO_TYPE_FUNGI       // 菌类（蘑菇）
} bio_type_t;

// 生物配置结构
typedef struct {
    bio_type_t type;             // 生物类型
    char name[64];               // 生物名称
    float optimal_temp_min;      // 最适温度下限
    float optimal_temp_max;      // 最适温度上限
    float optimal_humidity_min;  // 最适湿度下限
    float optimal_humidity_max;  // 最适湿度上限
    float optimal_ph_min;        // 最适pH下限
    float optimal_ph_max;        // 最适pH上限
    uint32_t growth_cycle_days;  // 生长周期（天）
} bio_config_t;

// 生物配置管理初始化
bool bio_config_init(void);

// 保存生物配置
bool bio_config_save(const bio_config_t *config);

// 加载生物配置
bool bio_config_load(bio_type_t type, bio_config_t *config);

// 删除生物配置
bool bio_config_delete(bio_type_t type);

// 列出所有生物配置
int bio_config_list(bio_type_t *types, int max_count);

// 获取默认配置
bool bio_config_get_default(bio_type_t type, bio_config_t *config);

#endif // BIO_CONFIG_H