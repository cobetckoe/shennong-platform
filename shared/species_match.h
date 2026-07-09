/**
 * 神农平台 - 设备与物种匹配规则
 *
 * 每个物种有环境需求参数，每个设备有能力范围。
 * 匹配规则：物种需求必须在设备能力范围内。
 */

#ifndef SPECIES_MATCH_H
#define SPECIES_MATCH_H

#include <stdint.h>

/* 设备类型编号 (protocols.h) */
#define DEVICE_ROOTCROP     0x01
#define DEVICE_LEAFCROP     0x02
#define DEVICE_STEMCROP     0x03
#define DEVICE_FLOWERCROP   0x04
#define DEVICE_FRUITCROP    0x05
#define DEVICE_POLLINATOR   0x10
#define DEVICE_SOIL         0x11
#define DEVICE_EDIBLE_FUNGI 0x20
#define DEVICE_WATER_ALGAE  0x30
#define DEVICE_FEED_INSECT  0x40

/* 物种编号 */
#define SPEC_POTATO       0x01
#define SPEC_SWEET_POTATO 0x02
#define SPEC_RADISH       0x03
#define SPEC_CARROT       0x04
#define SPEC_SPINACH      0x05
#define SPEC_LETTUCE      0x06
#define SPEC_CELERY       0x07
#define SPEC_ASPARAGUS    0x08
#define SPEC_BROCCOLI     0x09
#define SPEC_CAULIFLOWER  0x0A
#define SPEC_TOMATO       0x0B
#define SPEC_CUCUMBER     0x0C
#define SPEC_PEPPER       0x0D
#define SPEC_HONEYBEE     0x10
#define SPECBUMBLEBEE     0x11
#define SPEC_EARTHWORM    0x12
#define SPEC_MUSHROOM     0x20
#define SPEC_TREMELLA     0x21
#define SPEC_SPIRULINA    0x30
#define SPEC_CHLORELLA    0x31
#define SPEC_CRICKET      0x40
#define SPEC_MEALWORM     0x41

/* 执行器位掩码 */
#define ACT_SPRAY  (1 << 0)
#define ACT_FAN    (1 << 1)
#define ACT_LED    (1 << 2)

/**
 * 物种环境需求
 */
typedef struct {
    uint8_t species_id;
    float temp_min, temp_max;       // 温度范围 (°C)
    float humidity_min, humidity_max; // 湿度范围 (%)
    float light_min, light_max;     // 光照范围 (lux), 动物为0
    float ph_min, ph_max;           // pH范围, 动物为0
    uint8_t actuators;              // 需要的执行器 (ACT_xxx位掩码)
} species_profile_t;

/**
 * 设备能力范围
 */
typedef struct {
    uint8_t device_type;
    float temp_min, temp_max;
    float humidity_min, humidity_max;
    float light_min, light_max;
    float ph_min, ph_max;
    uint8_t actuators;              // 设备拥有的执行器
} device_capability_t;

/* ========== 物种环境需求表 ========== */

static const species_profile_t species_table[] = {
    /* 植物 - 根菜 */
    { SPEC_POTATO,       15, 28,  50, 75,  3000, 8000,  5.5, 7.0, ACT_SPRAY | ACT_FAN | ACT_LED },
    { SPEC_SWEET_POTATO, 18, 30,  60, 80,  4000, 9000,  5.5, 6.5, ACT_SPRAY | ACT_FAN | ACT_LED },
    { SPEC_RADISH,        8, 25,  45, 70,  3000, 7000,  6.0, 7.5, ACT_SPRAY | ACT_FAN | ACT_LED },
    { SPEC_CARROT,       10, 25,  50, 70,  3000, 7000,  6.0, 6.8, ACT_SPRAY | ACT_FAN | ACT_LED },

    /* 植物 - 叶菜 */
    { SPEC_SPINACH,      10, 25,  55, 80,  4000, 8000,  6.0, 7.5, ACT_SPRAY | ACT_FAN | ACT_LED },
    { SPEC_LETTUCE,       8, 22,  50, 75,  3000, 7000,  6.0, 7.0, ACT_SPRAY | ACT_FAN | ACT_LED },

    /* 植物 - 茎菜 */
    { SPEC_CELERY,       12, 26,  50, 75,  3000, 7000,  6.0, 7.0, ACT_SPRAY | ACT_FAN | ACT_LED },
    { SPEC_ASPARAGUS,    15, 30,  50, 70,  4000, 8000,  6.5, 7.5, ACT_SPRAY | ACT_FAN | ACT_LED },

    /* 植物 - 花菜 */
    { SPEC_BROCCOLI,     10, 24,  50, 70,  4000, 7000,  6.0, 7.0, ACT_SPRAY | ACT_FAN | ACT_LED },
    { SPEC_CAULIFLOWER,  10, 24,  50, 70,  4000, 7000,  6.0, 7.0, ACT_SPRAY | ACT_FAN | ACT_LED },

    /* 植物 - 果菜 */
    { SPEC_TOMATO,       18, 30,  50, 75,  5000, 9000,  6.0, 6.8, ACT_SPRAY | ACT_FAN | ACT_LED },
    { SPEC_CUCUMBER,     15, 30,  55, 80,  5000, 8000,  5.5, 7.0, ACT_SPRAY | ACT_FAN | ACT_LED },
    { SPEC_PEPPER,       18, 30,  50, 70,  5000, 9000,  6.0, 6.8, ACT_SPRAY | ACT_FAN | ACT_LED },

    /* 动物 - 授粉 */
    { SPEC_HONEYBEE,     15, 35,  40, 80,   0,    0,    0,   0,   ACT_FAN },
    { SPECBUMBLEBEE,     12, 32,  40, 80,   0,    0,    0,   0,   ACT_FAN },

    /* 动物 - 沃土 */
    { SPEC_EARTHWORM,     5, 28,  60, 85,   0,    0,    0,   0,   ACT_SPRAY },

    /* 真菌 */
    { SPEC_MUSHROOM,     12, 25,  70, 90,  100,  500,  5.5, 6.5, ACT_SPRAY | ACT_FAN },
    { SPEC_TREMELLA,     15, 28,  65, 85,   50,  300,  5.0, 6.0, ACT_SPRAY | ACT_FAN },

    /* 藻类 */
    { SPEC_SPIRULINA,    25, 38,  80, 100, 5000, 10000, 8.0, 10.0, ACT_FAN | ACT_LED },
    { SPEC_CHLORELLA,    20, 35,  80, 100, 4000, 9000,  6.5, 7.5, ACT_FAN | ACT_LED },

    /* 昆虫 */
    { SPEC_CRICKET,      20, 32,  40, 70,   0,    0,    0,   0,   ACT_FAN },
    { SPEC_MEALWORM,     18, 30,  35, 65,   0,    0,    0,   0,   ACT_FAN },
};

#define SPECIES_COUNT (sizeof(species_table) / sizeof(species_table[0]))

/* ========== 设备能力范围表 ========== */

static const device_capability_t device_table[] = {
    /* 植物设备: 全传感器 + 全执行器 */
    { DEVICE_ROOTCROP,    5, 40,  30, 100,  0, 12000,  4.0, 9.0,  ACT_SPRAY | ACT_FAN | ACT_LED },
    { DEVICE_LEAFCROP,    5, 40,  30, 100,  0, 12000,  4.0, 9.0,  ACT_SPRAY | ACT_FAN | ACT_LED },
    { DEVICE_STEMCROP,    5, 40,  30, 100,  0, 12000,  4.0, 9.0,  ACT_SPRAY | ACT_FAN | ACT_LED },
    { DEVICE_FLOWERCROP,  5, 40,  30, 100,  0, 12000,  4.0, 9.0,  ACT_SPRAY | ACT_FAN | ACT_LED },
    { DEVICE_FRUITCROP,   5, 40,  30, 100,  0, 12000,  4.0, 9.0,  ACT_SPRAY | ACT_FAN | ACT_LED },

    /* 动物设备: 温湿度 + 风扇 */
    { DEVICE_POLLINATOR,  0, 45,  30, 100,  0,    0,    0,   0,   ACT_FAN },
    { DEVICE_SOIL,        0, 40,  40, 100,  0,    0,    0,   0,   ACT_SPRAY },

    /* 真菌设备: 温湿度 + 风扇 + 喷雾 */
    { DEVICE_EDIBLE_FUNGI, 5, 40,  50, 100,  0,  1000,  4.0, 8.0, ACT_SPRAY | ACT_FAN },

    /* 藻类设备: 全传感器 + 风扇 + LED */
    { DEVICE_WATER_ALGAE, 10, 45,  50, 100, 2000, 12000, 5.0, 11.0, ACT_FAN | ACT_LED },

    /* 昆虫设备: 温湿度 + 风扇 */
    { DEVICE_FEED_INSECT, 10, 40,  25, 80,   0,    0,    0,   0,   ACT_FAN },
};

#define DEVICE_COUNT (sizeof(device_table) / sizeof(device_table[0]))

/* ========== 设备-物种兼容性表 ========== */

typedef struct {
    uint8_t device_type;
    uint8_t species_id;
} device_species_pair_t;

static const device_species_pair_t compatibility_table[] = {
    /* 根菜设备 */
    { DEVICE_ROOTCROP, SPEC_POTATO },
    { DEVICE_ROOTCROP, SPEC_SWEET_POTATO },
    { DEVICE_ROOTCROP, SPEC_RADISH },
    { DEVICE_ROOTCROP, SPEC_CARROT },

    /* 叶菜设备 */
    { DEVICE_LEAFCROP, SPEC_SPINACH },
    { DEVICE_LEAFCROP, SPEC_LETTUCE },

    /* 茎菜设备 */
    { DEVICE_STEMCROP, SPEC_CELERY },
    { DEVICE_STEMCROP, SPEC_ASPARAGUS },

    /* 花菜设备 */
    { DEVICE_FLOWERCROP, SPEC_BROCCOLI },
    { DEVICE_FLOWERCROP, SPEC_CAULIFLOWER },

    /* 果菜设备 */
    { DEVICE_FRUITCROP, SPEC_TOMATO },
    { DEVICE_FRUITCROP, SPEC_CUCUMBER },
    { DEVICE_FRUITCROP, SPEC_PEPPER },

    /* 授粉设备 */
    { DEVICE_POLLINATOR, SPEC_HONEYBEE },
    { DEVICE_POLLINATOR, SPECBUMBLEBEE },

    /* 沃土设备 */
    { DEVICE_SOIL, SPEC_EARTHWORM },

    /* 食用菌设备 */
    { DEVICE_EDIBLE_FUNGI, SPEC_MUSHROOM },
    { DEVICE_EDIBLE_FUNGI, SPEC_TREMELLA },

    /* 水藻设备 */
    { DEVICE_WATER_ALGAE, SPEC_SPIRULINA },
    { DEVICE_WATER_ALGAE, SPEC_CHLORELLA },

    /* 饲虫设备 */
    { DEVICE_FEED_INSECT, SPEC_CRICKET },
    { DEVICE_FEED_INSECT, SPEC_MEALWORM },
};

#define COMPAT_COUNT (sizeof(compatibility_table) / sizeof(compatibility_table[0]))

/* ========== 匹配函数 ========== */

/**
 * 查找物种环境需求
 */
static inline const species_profile_t* find_species(uint8_t species_id) {
    for (int i = 0; i < SPECIES_COUNT; i++) {
        if (species_table[i].species_id == species_id) return &species_table[i];
    }
    return 0;
}

/**
 * 查找设备能力范围
 */
static inline const device_capability_t* find_device(uint8_t device_type) {
    for (int i = 0; i < DEVICE_COUNT; i++) {
        if (device_table[i].device_type == device_type) return &device_table[i];
    }
    return 0;
}

/**
 * 检查物种是否兼容设备
 * 返回: 1=兼容, 0=不兼容
 */
static inline int is_compatible(uint8_t device_type, uint8_t species_id) {
    for (int i = 0; i < COMPAT_COUNT; i++) {
        if (compatibility_table[i].device_type == device_type &&
            compatibility_table[i].species_id == species_id) {
            return 1;
        }
    }
    return 0;
}

/**
 * 检查物种需求是否在设备能力范围内
 * 返回: 1=满足, 0=不满足
 */
static inline int check_capability(uint8_t device_type, uint8_t species_id) {
    const device_capability_t* dev = find_device(device_type);
    const species_profile_t* spec = find_species(species_id);
    if (!dev || !spec) return 0;

    if (spec->temp_min < dev->temp_min || spec->temp_max > dev->temp_max) return 0;
    if (spec->humidity_min < dev->humidity_min || spec->humidity_max > dev->humidity_max) return 0;
    if (spec->light_min < dev->light_min || spec->light_max > dev->light_max) return 0;
    if (spec->ph_min < dev->ph_min || spec->ph_max > dev->ph_max) return 0;
    if ((spec->actuators & dev->actuators) != spec->actuators) return 0;

    return 1;
}

/**
 * 获取物种的默认控制规则参数
 */
static inline void get_default_rule(uint8_t device_type, uint8_t species_id,
                                     float* temp_high, float* temp_low,
                                     float* humidity_high, float* humidity_low,
                                     uint8_t* spray, uint8_t* fan, uint8_t* led) {
    const species_profile_t* spec = find_species(species_id);
    if (!spec) return;

    *temp_high = spec->temp_max;
    *temp_low = spec->temp_min;
    *humidity_high = spec->humidity_max;
    *humidity_low = spec->humidity_min;
    *spray = (spec->actuators & ACT_SPRAY) ? 1 : 0;
    *fan = (spec->actuators & ACT_FAN) ? 1 : 0;
    *led = (spec->actuators & ACT_LED) ? 1 : 0;
}

#endif
