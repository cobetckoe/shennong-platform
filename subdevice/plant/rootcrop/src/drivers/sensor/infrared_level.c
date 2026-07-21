/**
 * 传感器驱动 - 红外光电液位传感器 (实现)
 * 
 * 工业级双冗余设计
 */

#include "infrared_level.h"

void infrared_level_init(infrared_level_t *dev) {
    hal_gpio_init(dev->port, dev->pin, HAL_GPIO_INPUT_PULLUP);
    dev->last_state = 0;
    dev->status = LEVEL_OK;
}

uint8_t infrared_level_read(infrared_level_t *dev) {
    uint8_t pin_level = (hal_gpio_read(dev->port, dev->pin) == HAL_GPIO_HIGH) ? 1 : 0;
    uint8_t water_state;
    
    if (dev->active_level == 0) {
        water_state = (pin_level == 0) ? 1 : 0;
    } else {
        water_state = (pin_level == 1) ? 1 : 0;
    }
    
    dev->last_state = water_state;
    return water_state;
}

uint8_t infrared_level_is_normal(infrared_level_t *dev) {
    uint8_t water = infrared_level_read(dev);
    if (dev->status & LEVEL_SENSOR_FAULT) return 0;
    return water;
}

uint8_t infrared_level_redundancy_check(infrared_level_t *primary, infrared_level_t *backup) {
    uint8_t state_p = infrared_level_read(primary);
    uint8_t state_b = infrared_level_read(backup);
    
    uint8_t verify1 = infrared_level_read(primary);
    uint8_t verify2 = infrared_level_read(primary);
    
    if (verify1 != verify2) {
        primary->status |= LEVEL_SENSOR_FAULT;
        return LEVEL_SENSOR_FAULT;
    }
    
    if (!state_p && state_b) {
        primary->status |= LEVEL_REDUNDANCY_ERR;
        backup->status |= LEVEL_REDUNDANCY_ERR;
        return LEVEL_REDUNDANCY_ERR;
    }
    
    primary->status = LEVEL_OK;
    backup->status = LEVEL_OK;
    return LEVEL_OK;
}

uint8_t infrared_level_get_status(infrared_level_t *low, infrared_level_t *high) {
    uint8_t status = 0;
    
    uint8_t low_water = infrared_level_read(low);
    uint8_t high_water = infrared_level_read(high);
    
    if (low_water) status |= 0x01;
    if (high_water) status |= 0x02;
    
    if (!(low->status & LEVEL_SENSOR_FAULT) && !(high->status & LEVEL_SENSOR_FAULT)) {
        status |= 0x04;
    }
    
    uint8_t check = infrared_level_redundancy_check(low, high);
    if (check != LEVEL_OK) {
        status |= 0x08;
    }
    
    if (!low_water) status |= 0x10;
    if (high_water) status |= 0x20;
    
    return status;
}