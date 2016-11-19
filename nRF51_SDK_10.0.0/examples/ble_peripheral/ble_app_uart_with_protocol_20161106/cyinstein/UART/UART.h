#ifndef UART_H__
#define UART_H__
#include "nrf.h"

#include "app_uart.h"
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 1                           /**< UART RX buffer size. */
void uart_error_handle(app_uart_evt_t * p_event);
void uart_init(void);
#endif //UART_H__
