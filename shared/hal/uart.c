/**
 * 硬件抽象层 - UART实现 (STM32F103)
 */

#include "uart.h"
#include "stm32f1xx_hal.h"

static UART_HandleTypeDef huart_list[3];

void hal_uart_init(hal_uart_port_t port, uint32_t baud) {
    UART_HandleTypeDef *h = &huart_list[port];

    if (port == HAL_UART_1) {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef g = {0};
        g.Pin = GPIO_PIN_9; g.Mode = GPIO_MODE_AF_PP; g.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &g);
        g.Pin = GPIO_PIN_10; g.Mode = GPIO_MODE_INPUT; g.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOA, &g);
        h->Instance = USART1;
    } else if (port == HAL_UART_2) {
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef g = {0};
        g.Pin = GPIO_PIN_2; g.Mode = GPIO_MODE_AF_PP; g.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &g);
        g.Pin = GPIO_PIN_3; g.Mode = GPIO_MODE_INPUT; g.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOA, &g);
        h->Instance = USART2;
    } else if (port == HAL_UART_3) {
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitTypeDef g = {0};
        g.Pin = GPIO_PIN_10; g.Mode = GPIO_MODE_AF_PP; g.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &g);
        g.Pin = GPIO_PIN_11; g.Mode = GPIO_MODE_INPUT; g.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOB, &g);
        h->Instance = USART3;
    }

    h->Init.BaudRate = baud;
    h->Init.WordLength = UART_WORDLENGTH_8B;
    h->Init.StopBits = UART_STOPBITS_1;
    h->Init.Parity = UART_PARITY_NONE;
    h->Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(h);
}

void hal_uart_send(hal_uart_port_t port, uint8_t *data, uint16_t len) {
    HAL_UART_Transmit(&huart_list[port], data, len, 100);
}

uint8_t hal_uart_recv(hal_uart_port_t port, uint8_t *buf, uint16_t len, uint32_t timeout_ms) {
    return HAL_UART_Receive(&huart_list[port], buf, len, timeout_ms) == HAL_OK ? 1 : 0;
}
