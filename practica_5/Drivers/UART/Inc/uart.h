#ifndef UART_LIB_H_
#define UART_LIB_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define UART_MIN_TRANSFER_SIZE 1U
#define UART_MAX_TRANSFER_SIZE 256U

/**
 * @brief Initialize the UART peripheral via the port layer.
 *
 * @return true if the hardware init succeeds, false otherwise.
 */
bool uart_init(void);

/**
 * @brief Reconfigure the UART baud rate.
 *
 * @param baud Desired baud rate in bits per second.
 *
 * @return true on success, false if the HAL reinit fails.
 */
bool uart_set_baud(uint32_t baud);

/**
 * @brief Retrieve the current baud rate.
 *
 * @return Active baud rate in bits per second.
 */
uint32_t uart_get_baud(void);

typedef enum {
    UART_WORD_LEN_UNKNOWN = 0,
    UART_WORD_LEN_8,
    UART_WORD_LEN_9
} uart_word_length_t;

typedef enum {
    UART_STOP_BITS_UNKNOWN = 0,
    UART_STOP_BITS_1,
    UART_STOP_BITS_2,
} uart_stop_bits_t;

typedef enum {
    UART_CFG_PARITY_UNKNOWN = 0,
    UART_CFG_PARITY_NONE,
    UART_CFG_PARITY_EVEN,
    UART_CFG_PARITY_ODD
} uart_parity_t;

uart_word_length_t uart_get_word_length(void);

uart_stop_bits_t uart_get_stop_bits(void);

uart_parity_t uart_get_parity(void);

/**
 * @brief Human-readable identifier for the active UART (e.g., "USART2").
 */
const char *uart_get_name(void);

/**
 * @brief Send a raw byte buffer.
 *
 * Guards against NULL pointers and enforces the transfer size limits.
 *
 * @param data Buffer containing bytes to transmit.
 * @param length Number of bytes to send (between UART_MIN_TRANSFER_SIZE and
 * UART_MAX_TRANSFER_SIZE).
 */
void uart_send_string_size(const char *data, size_t length);

/**
 * @brief Convenience wrapper that transmits a NULL-terminated string.
 *
 * @param str Pointer to the NUL-terminated string to send.
 */
void uart_send_string(const char *str);

/**
 * @brief Blocking helper to receive a line of text.
 *
 * Reads bytes until '\n' or '\r', truncating if the buffer fills up.
 *
 * @param buf Destination buffer for the received characters.
 * @param max_len Total capacity of @p buf.
 */
void uart_receive_string_size(char *buf, size_t max_len);

/**
 * @brief Attempt to read a single byte without blocking.
 *
 * @param byte Storage for the received byte (must not be NULL).
 *
 * @return true if a byte was read, false if no data was available.
 */
bool uart_receive_byte_nonblocking(uint8_t *byte);

#endif
