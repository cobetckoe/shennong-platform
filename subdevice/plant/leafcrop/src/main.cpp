/**
 * 叶菜种植机 - 水培/气雾一体机 (STM32 + Zigbee)
 * 类型: 0x02
 * 
 * 架构:
 * - 云端下发控制策略 (不同植物不同规则)
 * - 本地根据规则 + 实时数据 = 自动控制 (断网也能跑)
 * - 云端可远程微调参数
 *
 * 传感器:
 *   - DHT11: 环境温湿度
 *   - DS18B20: 营养液温度
 *   - TDS: 营养液浓度
 *   - pH: 营养液酸碱度
 *   - 红外光电液位 x2: 液位检测
 *   - SGP30: CO2/VOC
 *   - BH1750 x2: 冠层光照分析
 *   - HC-SR04: 植物高度
 *
 * 执行器:
 *   - 6路继电器: 水泵、补光灯、排风扇、喷雾泵、加热片、制冷片
 *   - 双路PWM LED: 红光+蓝光调光
 */

#include "device.h"
#include "config.h"
#include "control_engine.h"
#include "hal/gpio.h"
#include "hal/adc.h"
#include "hal/delay.h"
#include "sensor/dht22.h"
#include "sensor/bh1750.h"
#include "sensor/sgp30.h"
#include "sensor/ultrasonic.h"
#include "sensor/ds18b20.h"
#include "sensor/tds.h"
#include "sensor/ph_sensor.h"
#include "sensor/infrared_level.h"

// ==================== 传感器实例 ====================
static dht22_t dht11 = {DHT11_PORT, DHT11_PIN};
static bh1750_t bh1750_upper = {BH1750_I2C, BH1750_UPPER_ADDR};
static bh1750_t bh1750_lower = {BH1750_I2C, BH1750_LOWER_ADDR};
static sgp30_t sgp30_dev = {SGP30_I2C, SGP30_ADDR, 0, 0};
static ultrasonic_t ultrasonic = {
    ULTRASONIC_PORT, ULTRASONIC_TRIG, ULTRASONIC_ECHO, ULTRASONIC_HEIGHT
};
static ds18b20_t ds18b20 = {DS18B20_PORT, DS18B20_PIN};
static tds_t tds = {TDS_ADC, TDS_CH, 25.0f};
static ph_sensor_t ph = {PH_ADC, PH_CH, 0.0f, 1.0f};
static infrared_level_t level_low = {WATER_LOW_PORT, WATER_LOW_PIN, 0, 0, 0};
static infrared_level_t level_high = {WATER_HIGH_PORT, WATER_HIGH_PIN, 0, 0, 0};

// ==================== PAR计算 ====================

static int16_t calculate_par_from_lux(int16_t lux) {
    int32_t par = (int32_t)(lux * 0.0185f);
    if (par < 0) par = 0;
    if (par > 2500) par = 2500;
    return (int16_t)par;
}

static int16_t calculate_transmittance(int16_t par_upper, int16_t par_lower) {
    if (par_upper <= 0) return 0;
    int32_t trans = (int32_t)par_lower * 100 / par_upper;
    if (trans > 100) trans = 100;
    return (int16_t)trans;
}

static int16_t calculate_lai(int16_t transmittance) {
    if (transmittance <= 0 || transmittance >= 100) return 0;
    float lai = -__log((float)transmittance / 100.0f) / 0.6f;
    if (lai < 0) lai = 0;
    if (lai > 10) lai = 10;
    return (int16_t)(lai * 10);
}

// ==================== 继电器控制 ====================

static void relay_init_all(void) {
    hal_gpio_init(RELAY_1_PORT, RELAY_1_PIN, HAL_GPIO_OUTPUT_PP);
    hal_gpio_init(RELAY_2_PORT, RELAY_2_PIN, HAL_GPIO_OUTPUT_PP);
    hal_gpio_init(RELAY_3_PORT, RELAY_3_PIN, HAL_GPIO_OUTPUT_PP);
    hal_gpio_init(RELAY_4_PORT, RELAY_4_PIN, HAL_GPIO_OUTPUT_PP);
    hal_gpio_init(RELAY_5_PORT, RELAY_5_PIN, HAL_GPIO_OUTPUT_PP);
    hal_gpio_init(RELAY_6_PORT, RELAY_6_PIN, HAL_GPIO_OUTPUT_PP);
    
    hal_gpio_write(RELAY_1_PORT, RELAY_1_PIN, HAL_GPIO_LOW);
    hal_gpio_write(RELAY_2_PORT, RELAY_2_PIN, HAL_GPIO_LOW);
    hal_gpio_write(RELAY_3_PORT, RELAY_3_PIN, HAL_GPIO_LOW);
    hal_gpio_write(RELAY_4_PORT, RELAY_4_PIN, HAL_GPIO_LOW);
    hal_gpio_write(RELAY_5_PORT, RELAY_5_PIN, HAL_GPIO_LOW);
    hal_gpio_write(RELAY_6_PORT, RELAY_6_PIN, HAL_GPIO_LOW);
}

static void relay_update(uint8_t states) {
    hal_gpio_write(RELAY_1_PORT, RELAY_1_PIN, (states & 0x01) ? HAL_GPIO_HIGH : HAL_GPIO_LOW);
    hal_gpio_write(RELAY_2_PORT, RELAY_2_PIN, (states & 0x02) ? HAL_GPIO_HIGH : HAL_GPIO_LOW);
    hal_gpio_write(RELAY_3_PORT, RELAY_3_PIN, (states & 0x04) ? HAL_GPIO_HIGH : HAL_GPIO_LOW);
    hal_gpio_write(RELAY_4_PORT, RELAY_4_PIN, (states & 0x08) ? HAL_GPIO_HIGH : HAL_GPIO_LOW);
    hal_gpio_write(RELAY_5_PORT, RELAY_5_PIN, (states & 0x10) ? HAL_GPIO_HIGH : HAL_GPIO_LOW);
    hal_gpio_write(RELAY_6_PORT, RELAY_6_PIN, (states & 0x20) ? HAL_GPIO_HIGH : HAL_GPIO_LOW);
}

// ==================== PWM LED控制 ====================

static void led_pwm_init(void) {
    hal_gpio_init(LED_RED_PORT, LED_RED_PIN, HAL_GPIO_OUTPUT_PP);
    hal_gpio_init(LED_BLUE_PORT, LED_BLUE_PIN, HAL_GPIO_OUTPUT_PP);
    // TODO: 配置定时器PWM输出
}

static void led_update(uint8_t red, uint8_t blue) {
    // TODO: 更新PWM占空比
    (void)red;
    (void)blue;
}

// ==================== 液位检测 ====================

static void water_init(void) {
    infrared_level_init(&level_low);
    infrared_level_init(&level_high);
}

static uint8_t water_read(void) {
    return infrared_level_get_status(&level_low, &level_high);
}

// ==================== 执行器更新 ====================

static void execute(void) {
    // 获取控制引擎计算的继电器状态
    uint8_t relay = control_engine_execute();
    
    // 安全保护：液位过低时强制关闭水泵
    sensor_data_t *s = (sensor_data_t*)control_engine_get_sensor();
    if (s->water_status & WATER_BIT_LOW) {
        relay &= ~RELAY_BIT_PUMP;
    }
    
    // 更新继电器硬件
    relay_update(relay);
    
    // 更新LED亮度
    uint8_t red, blue;
    control_engine_get_led(&red, &blue);
    led_update(red, blue);
}

// ==================== 传感器读取 ====================

void read_sensors(void) {
    sensor_data_t *s = (sensor_data_t*)control_engine_get_sensor();
    
    // DHT11 温湿度
    s->temperature = dht22_read_temp(&dht11);
    s->humidity = dht22_read_humi(&dht11);
    
    // DS18B20 营养液温度
    s->nutrient_temp = ds18b20_read(&ds18b20);
    
    // TDS 营养液浓度
    tds.temperature = s->nutrient_temp;
    s->tds = tds_read_ppm(&tds);
    
    // pH
    s->ph = (int16_t)(ph_read(&ph) * 10);
    
    // SGP30 CO2/VOC
    if (sgp30_measure(&sgp30_dev)) {
        s->co2 = sgp30_dev.co2;
        s->tvoc = sgp30_dev.tvoc;
    }
    
    // 液位
    s->water_status = water_read();
    
    // HC-SR04 植物高度
    s->plant_height = (int16_t)ultrasonic_read_cm(&ultrasonic);
    
    // BH1750 双光照 + 冠层分析
    float lux_upper = bh1750_read(&bh1750_upper);
    float lux_lower = bh1750_read(&bh1750_lower);
    
    s->light = (int16_t)lux_upper;
    s->par_upper = calculate_par_from_lux((int16_t)lux_upper);
    s->par_lower = calculate_par_from_lux((int16_t)lux_lower);
    s->transmittance = calculate_transmittance(s->par_upper, s->par_lower);
    s->lai = calculate_lai(s->transmittance);
    
    // 更新控制引擎的传感器数据
    control_engine_update_sensor(s);
}

// ==================== 主函数 ====================

int main(void) {
    // 初始化执行器
    relay_init_all();
    led_pwm_init();
    
    // 初始化传感器
    bh1750_init(&bh1750_upper);
    bh1750_init(&bh1750_lower);
    sgp30_init(&sgp30_dev);
    water_init();
    hal_adc_init(TDS_ADC);
    
    // 初始化控制引擎
    control_engine_init();
    
    // 初始化设备框架
    device_init(DEV_ID);
    
    // 主循环
    uint32_t last_sensor_time = 0;
    uint32_t last_control_time = 0;
    
    while (1) {
        uint32_t now = hal_tick_ms();
        
        // 传感器采集 (每2秒)
        if (now - last_sensor_time >= 2000) {
            read_sensors();
            last_sensor_time = now;
        }
        
        // 控制执行 (每1秒)
        if (now - last_control_time >= 1000) {
            execute();
            last_control_time = now;
        }
        
        // 接收网关命令
        device_loop_once(DEV_ID);
        
        hal_delay_ms(10);
    }
}