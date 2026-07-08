/**
 * 神农平台 - 子机通用模板
 * 
 * 使用方法：
 * 1. 复制此文件到具体设备目录
 * 2. 修改 DEVICE_ID 和 DEVICE_TYPE
 * 3. 修改传感器读取和控制逻辑
 */

#include "stm32f1xx_hal.h"
#include <string.h>
#include "protocols.h"

// 修改这两个配置
#define DEVICE_ID       0x01    // 每台设备唯一
#define DEVICE_TYPE     DEVICE_TUBER  // DEVICE_TUBER/DEVICE_ALGAE/DEVICE_LEAF/DEVICE_FUNGI

// 串口配置
#define ZIGBEE_BAUD     115200

// GPIO定义（根据实际硬件修改）
#define SPRAY_PIN       GPIO_PIN_0
#define FAN_PIN         GPIO_PIN_1
#define LED_PIN         GPIO_PIN_2
#define GPIO_PORT       GPIOA

static control_rule_t rule = {1, DEVICE_TYPE, 30, 20, 80, 50, 1, 1, 1};
static sensor_data_t sensor;
static UART_HandleTypeDef huart2;

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

// 根据设备类型修改传感器读取
static void read_sensors(void) {
    // TODO: 实现实际传感器读取
    sensor = (sensor_data_t){25.0f, 60.0f, 5000.0f, 7.0f};
}

static void execute(void) {
    HAL_GPIO_WritePin(GPIO_PORT, FAN_PIN,
        sensor.temperature > rule.temp_high ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_PORT, SPRAY_PIN,
        sensor.humidity < rule.humidity_low && rule.spray ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_PORT, LED_PIN,
        rule.led ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

int main(void) {
    HAL_Init();
    uart_init();
    gpio_init();
    
    uint32_t last_heartbeat = 0, last_sensor = 0;
    
    while (1) {
        uint32_t now = HAL_GetTick();
        
        if (now - last_heartbeat >= 30000) {
            uint8_t hb[9] = {CMD_HEARTBEAT, DEVICE_ID};
            send(hb, 9);
            last_heartbeat = now;
        }
        
        if (now - last_sensor >= 5000) {
            read_sensors();
            uint8_t data[17] = {CMD_SENSOR_DATA, DEVICE_ID};
            memcpy(&data[2], &sensor, sizeof(sensor));
            send(data, sizeof(sensor) + 2);
            execute();
            last_sensor = now;
        }
        
        uint8_t rx[64];
        if (HAL_UART_Receive(&huart2, rx, sizeof(rx), 10) == HAL_OK && rx[0] == CMD_CONTROL_RULE) {
            memcpy(&rule, &rx[1], sizeof(rule));
            execute();
        }
        
        HAL_Delay(10);
    }
}
