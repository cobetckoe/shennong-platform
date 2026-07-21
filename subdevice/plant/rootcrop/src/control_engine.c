/**
 * 根菜种植机 - 控制引擎实现
 */

#include "control_engine.h"
#include "hw_config.h"
#include "hal/gpio.h"
#include <string.h>

static sensor_data_t sensor;
static control_rule_t rule;
static uint8_t relay_state = 0;
static uint8_t led_red_val = 0;
static uint8_t led_blue_val = 0;

void control_engine_init(void) {
    memset(&sensor, 0, sizeof(sensor));
    memset(&rule, 0, sizeof(rule));
    relay_state = 0;

    rule.temp_high = 30;
    rule.temp_low = 5;
    rule.humidity_high = 80;
    rule.humidity_low = 40;
    rule.light_high = 20000;
    rule.light_low = 5000;
    rule.ph_high = 75;
    rule.ph_low = 55;
}

const sensor_data_t *control_engine_get_sensor(void) {
    return &sensor;
}

sensor_data_t *control_engine_sensor_mut(void) {
    return &sensor;
}

const control_rule_t *control_engine_get_rule(void) {
    return &rule;
}

void control_engine_set_rule(const control_rule_t *r) {
    memcpy(&rule, r, sizeof(rule));
}

uint8_t control_engine_get_relay(void) {
    return relay_state;
}

void control_engine_get_led(uint8_t *red, uint8_t *blue) {
    if (red) *red = led_red_val;
    if (blue) *blue = led_blue_val;
}

void control_engine_set_led(uint8_t red, uint8_t blue) {
    led_red_val = red;
    led_blue_val = blue;
}

uint8_t control_engine_execute(void) {
    uint8_t new_relay = 0;
    float temp = sensor.temperature;
    float humi = sensor.humidity;

    if (temp > rule.temp_high) new_relay |= RELAY_BIT_FAN;
    if (humi < rule.humidity_low) new_relay |= RELAY_BIT_PUMP;
    if (sensor.light < rule.light_low) new_relay |= RELAY_BIT_LIGHT;

    relay_state = new_relay;
    control_engine_update_relay_hw();

    return new_relay;
}

void control_engine_set_relay(uint8_t relay) {
    relay_state = relay;
}

void control_engine_update_relay_hw(void) {
    hal_gpio_write(PUMP_PORT, PUMP_PIN,
        (relay_state & RELAY_BIT_PUMP) ? HAL_GPIO_HIGH : HAL_GPIO_LOW);
    hal_gpio_write(FAN_PORT, FAN_PIN,
        (relay_state & RELAY_BIT_FAN) ? HAL_GPIO_HIGH : HAL_GPIO_LOW);
    hal_gpio_write(LED_PORT, LED_PIN,
        (relay_state & RELAY_BIT_LIGHT) ? HAL_GPIO_HIGH : HAL_GPIO_LOW);
}

static uint32_t scene_times[16] = {0};

uint32_t control_engine_get_last_scene_time(uint8_t scene_id) {
    if (scene_id > 0 && scene_id <= 16) return scene_times[scene_id - 1];
    return 0;
}

void control_engine_set_last_scene_time(uint8_t scene_id, uint32_t time) {
    if (scene_id > 0 && scene_id <= 16) scene_times[scene_id - 1] = time;
}
