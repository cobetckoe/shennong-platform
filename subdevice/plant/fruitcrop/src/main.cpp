/**
 * 果菜种植机
 * 类型: 0x05 (番茄、黄瓜、辣椒)
 *
 * 传感器: 土壤温湿度(DHT22)、光照(BH1750)、土壤pH(ADC)
 * 执行器: 水泵、风扇、补光灯
 */

#include "device.h"
#include "config.h"
#include "drivers/sensor/dht22.h"
#include "drivers/sensor/bh1750.h"
#include "drivers/sensor/ph_sensor.h"
#include "drivers/actuator/pump.h"
#include "drivers/actuator/fan.h"
#include "drivers/actuator/led.h"

static dht22_t soil = {SOIL_DHT_PORT, SOIL_DHT_PIN};
static bh1750_t light = {LIGHT_I2C, LIGHT_ADDR};
static ph_sensor_t ph = {PH_ADC, PH_CH, 0.0f, 1.0f};
static pump_t pump = {PUMP_PORT, PUMP_PIN};
static fan_t fan = {FAN_PORT, FAN_PIN};
static led_t led = {LED_PORT, LED_PIN};

void read_sensors(void) {
    sensor_data_t *s = device_get_sensor();
    s->temperature = dht22_read_temp(&soil);
    s->humidity = dht22_read_humi(&soil);
    s->light = bh1750_read(&light);
    s->ph = ph_read(&ph);
}

void execute(void) {
    control_rule_t *r = device_get_rule();
    sensor_data_t *s = device_get_sensor();
    fan_control(&fan, s->temperature > r->temp_high);
    pump_control(&pump, s->humidity < r->humidity_low && r->spray);
    led_control(&led, r->led);
}

int main(void) {
    fan_init(&fan);
    pump_init(&pump);
    led_init(&led);
    bh1750_init(&light);

    device_init(DEV_ID);
    device_loop(DEV_ID);
}
