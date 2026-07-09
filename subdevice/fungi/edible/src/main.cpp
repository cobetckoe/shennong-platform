/**
 * 食用菌培养室
 * 类型: 0x20 (蘑菇、银耳)
 *
 * 传感器: 室内温度(DS18B20)、空气湿度(DHT22)、光照(BH1750)
 * 执行器: 雾化器、排风扇
 */

#include "device.h"
#include "config.h"
#include "drivers/sensor/ds18b20.h"
#include "drivers/sensor/dht22.h"
#include "drivers/sensor/bh1750.h"
#include "drivers/actuator/pump.h"
#include "drivers/actuator/fan.h"

static ds18b20_t temp = {ROOM_TEMP_PORT, ROOM_TEMP_PIN};
static dht22_t humi = {ROOM_HUMI_PORT, ROOM_HUMI_PIN};
static bh1750_t light = {ROOM_LIGHT_I2C, ROOM_LIGHT_ADDR};
static pump_t mist = {MIST_PORT, MIST_PIN};
static fan_t fan = {FAN_PORT, FAN_PIN};

void read_sensors(void) {
    sensor_data_t *s = device_get_sensor();
    s->temperature = ds18b20_read(&temp);
    s->humidity = dht22_read_humi(&humi);
    s->light = bh1750_read(&light);
}

void execute(void) {
    control_rule_t *r = device_get_rule();
    sensor_data_t *s = device_get_sensor();
    fan_control(&fan, s->temperature > r->temp_high);
    pump_control(&mist, s->humidity < r->humidity_low && r->spray);
}

int main(void) {
    fan_init(&fan);
    pump_init(&mist);
    bh1750_init(&light);

    device_init(DEV_ID);
    device_loop(DEV_ID);
}
