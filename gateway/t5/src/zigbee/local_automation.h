/**
 * 神农系统 T5 网关 - 本地自动化模块
 */

#ifndef LOCAL_AUTOMATION_H
#define LOCAL_AUTOMATION_H

#include "protocols.h"
#include <stdint.h>
#include <stdbool.h>

#define CROSS_TRIGGER_RELAY   0
#define CROSS_TRIGGER_SENSOR  1

typedef struct {
    uint8_t scene_id;
    bool enabled;
    uint8_t trigger_type;
    uint8_t target_dev;
    uint8_t trigger_dev;
    uint8_t trigger_relay;
    uint8_t trigger_state;
    uint8_t trigger_dpid;
    int32_t trigger_min;
    int32_t trigger_max;
    uint8_t action_relay;
    uint8_t action_state;
    uint8_t cooldown_sec;
    uint32_t last_triggered;
} cross_scene_t;

void local_automation_init(void);
void local_automation_loop(void);

void local_automation_on_rule(const uint8_t *data, uint16_t len);
void local_automation_on_scene(const uint8_t *data, uint16_t len);
void local_automation_on_sensor(uint8_t dev_id, const uint8_t *data, uint16_t len);
void local_automation_sync_to_cloud(void);

void local_automation_save(void);
void local_automation_load(void);
void local_automation_factory_reset(void);

#endif
