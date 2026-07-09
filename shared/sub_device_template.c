/**
 * 神农平台 - 通用子设备模板
 *
 * 设备职责：
 * 1. 采集传感器数据，定时上报云端
 * 2. 接收云端控制参数，执行控制
 *
 * 所有智能逻辑在云端，设备只做执行器。
 * 修改 DEVICE_ID 和 DEVICE_TYPE 即可适配不同设备。
 */

#include "stm32f1xx_hal.h"
#include <string.h>
#include "protocols.h"

// ============ 设备配置 ============
#define DEVICE_ID       0x01
#define DEVICE_TYPE     0x01

// 通信配置
#define ZIGBEE_BAUD     115200
#define HB_INTERVAL     30000   // 心跳间隔 ms
#define SENSOR_INTERVAL 5000    // 采集间隔 ms

// GPIO（根据实际硬件修改）
#define SPRAY_PIN       GPIO_PIN_0
#define FAN_PIN         GPIO_PIN_1
#define LED_PIN         GPIO_PIN_2
#define GPIO_PORT       GPIOA

// ============ 全局状态 ============
static control_rule_t rule = {0};
static sensor_data_t sensor;
static UART_HandleTypeDef huart2;

// ============ 底层驱动 ============

static void uart_init(void) {
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef g = {0};
    g.Pin = GPIO_PIN_2;
    g.Mode = GPIO_MODE_AF_PP;
    g.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &g);

    g.Pin = GPIO_PIN_3;
    g.Mode = GPIO_MODE_INPUT;
    g.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &g);

    huart2.Instance = USART2;
    huart2.Init.BaudRate = ZIGBEE_BAUD;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&huart2);
}

static void gpio_init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef g = {0};
    g.Pin = SPRAY_PIN | FAN_PIN | LED_PIN;
    g.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIO_PORT, &g);
}

static void send(uint8_t *data, uint16_t len) {
    HAL_UART_Transmit(&huart2, data, len, 100);
}

// ============ 业务逻辑 ============

// TODO: 根据实际硬件替换传感器读取
static void read_sensors(void) {
    sensor = (sensor_data_t){25.0f, 60.0f, 5000.0f, 7.0f};
}

// 根据云端下发的参数控制执行器
static void execute(void) {
    HAL_GPIO_WritePin(GPIO_PORT, FAN_PIN,
        sensor.temperature > rule.temp_high ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_PORT, SPRAY_PIN,
        sensor.humidity < rule.humidity_low && rule.spray ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_PORT, LED_PIN,
        rule.led ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// ============ 主循环 ============

int main(void) {
    HAL_Init();
    uart_init();
    gpio_init();

    uint32_t last_heartbeat = 0, last_sensor = 0;

    while (1) {
        uint32_t now = HAL_GetTick();

        // 心跳
        if (now - last_heartbeat >= HB_INTERVAL) {
            uint8_t hb[9] = {CMD_HEARTBEAT, DEVICE_ID};
            send(hb, 9);
            last_heartbeat = now;
        }

        // 采集 + 上报 + 执行
        if (now - last_sensor >= SENSOR_INTERVAL) {
            read_sensors();
            uint8_t data[17] = {CMD_SENSOR_DATA, DEVICE_ID};
            memcpy(&data[2], &sensor, sizeof(sensor));
            send(data, sizeof(sensor) + 2);
            execute();
            last_sensor = now;
        }

        // 接收云端控制指令
        uint8_t rx[64];
        if (HAL_UART_Receive(&huart2, rx, sizeof(rx), 10) == HAL_OK && rx[0] == CMD_CONTROL_RULE) {
            memcpy(&rule, &rx[1], sizeof(rule));
            execute();
        }

        HAL_Delay(10);
    }
}
