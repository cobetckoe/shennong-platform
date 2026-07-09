/**
 * 藻类培养池
 * 类型: 0x30 (螺旋藻、小球藻)
 *
 * 传感器: 水温(DS18B20防水)、光照(BH1750)、pH(ADC)
 * 执行器: 曝气泵、循环泵、LED补光
 */

#include "device.h"
#include "config.h"
#include "drivers/sensor/ds18b20.h"
#include "drivers/sensor/bh1750.h"
#include "drivers/sensor/ph_sensor.h"
#include "drivers/actuator/pump.h"
#include "drivers/actuator/led.h"

static ds18b20_t temp = {WATER_TEMP_PORT, WATER_TEMP_PIN};
static bh1750_t light = {WATER_LIGHT_I2C, WATER_LIGHT_ADDR};
static ph_sensor_t ph = {WATER_PH_ADC, WATER_PH_CH, 0.0f, 1.0f};
static pump_t air = {AIR_PORT, AIR_PIN};
static pump_t circ = {PUMP_PORT, PUMP_PIN};
static led_t led = {LED_PORT, LED_PIN};

void read_sensors(void) {
    sensor_data_t *s = device_get_sensor();
    s->temperature = ds18b20_read(&temp);
    s->light = bh1750_read(&light);
    s->ph = ph_read(&ph);
}

void execute(void) {
    control_rule_t *r = device_get_rule();
    sensor_data_t *s = device_get_sensor();
    pump_control(&air, s->temperature > r->temp_high);
    led_control(&led, r->led);
    pump_control(&circ, r->spray);
}

int main(void) {
    pump_init(&air);
    pump_init(&circ);
    led_init(&led);
    bh1750_init(&light);

    device_init(DEV_ID);
    device_loop(DEV_ID);
}
