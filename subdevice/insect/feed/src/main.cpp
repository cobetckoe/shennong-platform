/**
 * 饲虫养殖箱
 * 类型: 0x40 (蟋蟀、面包虫)
 *
 * 传感器: 箱体温度(DS18B20)、湿度(DHT22)
 * 执行器: 通风扇、加热片
 */

#include "device.h"
#include "config.h"
#include "drivers/sensor/ds18b20.h"
#include "drivers/sensor/dht22.h"
#include "drivers/actuator/fan.h"
#include "drivers/actuator/heater.h"

static ds18b20_t temp = {BOX_TEMP_PORT, BOX_TEMP_PIN};
static dht22_t humi = {BOX_HUMI_PORT, BOX_HUMI_PIN};
static fan_t fan = {FAN_PORT, FAN_PIN};
static heater_t heat = {HEATER_PORT, HEATER_PIN};

void read_sensors(void) {
    sensor_data_t *s = device_get_sensor();
    s->temperature = ds18b20_read(&temp);
    s->humidity = dht22_read_humi(&humi);
}

void execute(void) {
    control_rule_t *r = device_get_rule();
    sensor_data_t *s = device_get_sensor();
    fan_control(&fan, s->temperature > r->temp_high);
    heater_control(&heat, s->temperature < r->temp_low);
}

int main(void) {
    fan_init(&fan);
    heater_init(&heat);

    device_init(DEV_ID);
    device_loop(DEV_ID);
}
