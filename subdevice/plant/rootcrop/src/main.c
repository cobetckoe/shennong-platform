/**
 * 根菜种植机
 * 类型: 0x01 (土豆、红薯、萝卜、胡萝卜)
 *
 * 传感器: 土壤温湿度(DHT22)、光照(BH1750)、土壤pH(ADC)
 * 执行器: 水泵、风扇、补光灯
 */

#include "device.h"
#include "hw_config.h"
#include "control_engine.h"
#include "drivers/sensor/dht22.h"
#include "drivers/sensor/bh1750.h"
#include "drivers/sensor/ph_sensor.h"
#include "drivers/actuator/pump.h"
#include "drivers/actuator/fan.h"
#include "drivers/actuator/led.h"
#include "hal/gpio.h"
#include "hal/i2c.h"
#include "hal/adc.h"

static dht22_t soil = {SOIL_DHT_PORT, SOIL_DHT_PIN};
static bh1750_t light_dev = {LIGHT_I2C, LIGHT_ADDR};
static ph_sensor_t ph = {PH_ADC, PH_CH, 0.0f, 1.0f};
static pump_t pump_dev = {PUMP_PORT, PUMP_PIN};
static fan_t fan_dev = {FAN_PORT, FAN_PIN};
static led_t led_dev = {LED_PORT, LED_PIN};

void read_sensors(void) {
    sensor_data_t *s = control_engine_sensor_mut();
    s->temperature = dht22_read_temp(&soil);
    s->humidity = dht22_read_humi(&soil);
    s->light = (int16_t)bh1750_read(&light_dev);
    s->ph = (int16_t)(ph_read(&ph) * 100);
}

int main(void) {
    hal_gpio_init(PUMP_PORT, PUMP_PIN, HAL_GPIO_OUTPUT_PP);
    hal_gpio_init(FAN_PORT, FAN_PIN, HAL_GPIO_OUTPUT_PP);
    hal_gpio_init(LED_PORT, LED_PIN, HAL_GPIO_OUTPUT_PP);

    hal_i2c_init(LIGHT_I2C, 100000);
    hal_adc_init(PH_ADC);

    pump_init(&pump_dev);
    fan_init(&fan_dev);
    led_init(&led_dev);

    device_init(DEVICE_ID);
    device_loop(DEVICE_ID);
}
