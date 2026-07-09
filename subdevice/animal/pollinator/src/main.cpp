/**
 * 蜂箱
 * 类型: 0x10 (蜜蜂)
 *
 * 传感器: 箱体温度(DS18B20)、湿度(DHT22)、重量(HX711)
 * 执行器: 通风扇
 */

#include "device.h"
#include "config.h"
#include "drivers/sensor/ds18b20.h"
#include "drivers/sensor/dht22.h"
#include "drivers/sensor/hx711.h"
#include "drivers/actuator/fan.h"

static ds18b20_t temp = {HIVE_TEMP_PORT, HIVE_TEMP_PIN};
static dht22_t humi = {HIVE_HUMI_PORT, HIVE_HUMI_PIN};
static hx711_t weight = {HX711_SCK_PORT, HX711_SCK_PIN, HX711_DOUT_PORT, HX711_DOUT_PIN, 1.0f, 0.0f};
static fan_t fan = {FAN_PORT, FAN_PIN};

void read_sensors(void) {
    sensor_data_t *s = device_get_sensor();
    s->temperature = ds18b20_read(&temp);
    s->humidity = dht22_read_humi(&humi);
}

void execute(void) {
    control_rule_t *r = device_get_rule();
    sensor_data_t *s = device_get_sensor();
    fan_control(&fan, s->temperature > r->temp_high);
}

int main(void) {
    fan_init(&fan);

    device_init(DEV_ID);
    device_loop(DEV_ID);
}
