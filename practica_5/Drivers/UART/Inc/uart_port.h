
#ifndef UART_LIB_PORT_H
#define UART_LIB_PORT_H

#include "uart.h"
#include <stdbool.h>
#include <stdint.h>

bool uart_port_init(void);

bool uart_transmit(uint8_t *data, uint16_t size);

bool uart_receive(uint8_t *buffer, uint16_t size);

bool uart_receive_nonblocking(uint8_t *buffer, uint16_t size);

bool uart_port_set_baud(uint32_t baud);

uint32_t uart_port_get_baud(void);

uart_word_length_t uart_port_get_word_length(void);

uart_stop_bits_t uart_port_get_stop_bits(void);

uart_parity_t uart_port_get_parity(void);

const char *uart_port_get_name(void);

#endif // UART_LIB_PORT_H
