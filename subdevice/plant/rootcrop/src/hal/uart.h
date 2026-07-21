/**
 * 硬件抽象层 - UART
 */

#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdint.h>

typedef enum {
    HAL_UART_1 = 0,
    HAL_UART_2 = 1,
    HAL_UART_3 = 2
} hal_uart_port_t;

void hal_uart_init(hal_uart_port_t port, uint32_t baud);
void hal_uart_send(hal_uart_port_t port, uint8_t *data, uint16_t len);
uint8_t hal_uart_recv(hal_uart_port_t port, uint8_t *buf, uint16_t len, uint32_t timeout_ms);

#endif
