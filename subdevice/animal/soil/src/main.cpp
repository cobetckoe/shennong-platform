/**
 * 蚯蚓养殖箱
 * 类型: 0x11 (蚯蚓)
 *
 * 传感器: 基质温度(DS18B20)、基质pH(ADC)
 * 执行器: 喷淋泵、通风扇
 */

#include "device.h"
#include "config.h"
#include "drivers/sensor/ds18b20.h"
#include "drivers/sensor/ph_sensor.h"
#include "drivers/actuator/pump.h"
#include "drivers/actuator/fan.h"

static ds18b20_t temp = {SUB_TEMP_PORT, SUB_TEMP_PIN};
static ph_sensor_t ph = {SUB_PH_ADC, SUB_PH_CH, 0.0f, 1.0f};
static pump_t spray = {SPRAY_PORT, SPRAY_PIN};
static fan_t fan = {FAN_PORT, FAN_PIN};

void read_sensors(void) {
    sensor_data_t *s = device_get_sensor();
    s->temperature = ds18b20_read(&temp);
    s->ph = ph_read(&ph);
}

void execute(void) {
    control_rule_t *r = device_get_rule();
    sensor_data_t *s = device_get_sensor();
    fan_control(&fan, s->temperature > r->temp_high);
    pump_control(&spray, s->humidity < r->humidity_low && r->spray);
}

int main(void) {
    fan_init(&fan);
    pump_init(&spray);

    device_init(DEV_ID);
    device_loop(DEV_ID);
}
