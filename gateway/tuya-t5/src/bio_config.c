#include "bio_config.h"
#include "storage.h"
#include <stdio.h>
#include <string.h>

#define CONFIG_DIR "config"

// 默认配置
static const bio_config_t default_configs[] = {
    // 块茎类（土豆）
    {
        .type = BIO_TYPE_TUBER,
        .name = "土豆",
        .optimal_temp_min = 15.0,
        .optimal_temp_max = 22.0,
        .optimal_humidity_min = 60.0,
        .optimal_humidity_max = 80.0,
        .optimal_ph_min = 5.5,
        .optimal_ph_max = 6.5,
        .growth_cycle_days = 90
    },
    // 藻类（螺旋藻）
    {
        .type = BIO_TYPE_ALGAE,
        .name = "螺旋藻",
        .optimal_temp_min = 20.0,
        .optimal_temp_max = 35.0,
        .optimal_humidity_min = 0.0,
        .optimal_humidity_max = 0.0,
        .optimal_ph_min = 8.0,
        .optimal_ph_max = 10.0,
        .growth_cycle_days = 7
    },
    // 叶菜类（蔬菜）
    {
        .type = BIO_TYPE_LEAF,
        .name = "蔬菜",
        .optimal_temp_min = 18.0,
        .optimal_temp_max = 25.0,
        .optimal_humidity_min = 60.0,
        .optimal_humidity_max = 80.0,
        .optimal_ph_min = 6.0,
        .optimal_ph_max = 7.0,
        .growth_cycle_days = 30
    },
    // 菌类（蘑菇）
    {
        .type = BIO_TYPE_FUNGI,
        .name = "蘑菇",
        .optimal_temp_min = 15.0,
        .optimal_temp_max = 20.0,
        .optimal_humidity_min = 80.0,
        .optimal_humidity_max = 95.0,
        .optimal_ph_min = 6.0,
        .optimal_ph_max = 7.0,
        .growth_cycle_days = 14
    }
};

// 生物配置管理初始化
bool bio_config_init(void) {
    // 创建配置目录
    mkdir(CONFIG_DIR, 0755);
    return true;
}

// 保存生物配置
bool bio_config_save(const bio_config_t *config) {
    char filename[128];
    snprintf(filename, sizeof(filename), "%s/bio_%d.bin", CONFIG_DIR, config->type);
    
    return storage_save(filename, config, sizeof(bio_config_t));
}

// 加载生物配置
bool bio_config_load(bio_type_t type, bio_config_t *config) {
    char filename[128];
    snprintf(filename, sizeof(filename), "%s/bio_%d.bin", CONFIG_DIR, type);
    
    if (storage_exists(filename)) {
        return storage_load(filename, config, sizeof(bio_config_t));
    }
    
    // 返回默认配置
    return bio_config_get_default(type, config);
}

// 删除生物配置
bool bio_config_delete(bio_type_t type) {
    char filename[128];
    snprintf(filename, sizeof(filename), "%s/bio_%d.bin", CONFIG_DIR, type);
    
    return storage_delete(filename);
}

// 列出所有生物配置
int bio_config_list(bio_type_t *types, int max_count) {
    int count = 0;
    for (int i = 0; i < sizeof(default_configs) / sizeof(default_configs[0]); i++) {
        if (count < max_count) {
            types[count++] = default_configs[i].type;
        }
    }
    return count;
}

// 获取默认配置
bool bio_config_get_default(bio_type_t type, bio_config_t *config) {
    for (int i = 0; i < sizeof(default_configs) / sizeof(default_configs[0]); i++) {
        if (default_configs[i].type == type) {
            *config = default_configs[i];
            return true;
        }
    }
    return false;
}