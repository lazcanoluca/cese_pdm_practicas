#include "uart.h"
#include "uart_port.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static const char *word_length_to_string(uart_word_length_t word_length) {
  switch (word_length) {
  case UART_WORD_LEN_8:
    return "8";
  case UART_WORD_LEN_9:
    return "9";
  default:
    return "?";
  }
}

static const char *stop_bits_to_string(uart_stop_bits_t stop_bits) {
  switch (stop_bits) {
  case UART_STOP_BITS_1:
    return "1";
  case UART_STOP_BITS_2:
    return "2";
  default:
    return "?";
  }
}

static const char *parity_to_string(uart_parity_t parity) {
  switch (parity) {
  case UART_CFG_PARITY_NONE:
    return "none";
  case UART_CFG_PARITY_EVEN:
    return "even";
  case UART_CFG_PARITY_ODD:
    return "odd";
  default:
    return "?";
  }
}

bool uart_init() {
  if (!uart_port_init()) {
    return false;
  }

  char config_buffer[80];
  const uint32_t baud = uart_get_baud();
  const char *word_len = word_length_to_string(uart_get_word_length());
  const char *stop_bits = stop_bits_to_string(uart_get_stop_bits());
  const char *parity = parity_to_string(uart_get_parity());

  snprintf(config_buffer, sizeof(config_buffer), "%s: baud=%lu word=%s stop=%s parity=%s\r\n",
           uart_get_name(), (unsigned long)baud, word_len, stop_bits, parity);
  uart_send_string(config_buffer);

  return true;
}

void uart_send_string_size(const char *data, size_t length) {
  if (data == NULL)
    return;

  if (length < UART_MIN_TRANSFER_SIZE || length > UART_MAX_TRANSFER_SIZE)
    return;

  (void)uart_transmit((uint8_t *)data, (uint16_t)length);
}

void uart_send_string(const char *str) {
  if (str == NULL)
    return;

  size_t len = strlen(str);
  if (len < UART_MIN_TRANSFER_SIZE || len > UART_MAX_TRANSFER_SIZE)
    return;

  uart_send_string_size(str, len);
}

void uart_receive_string_size(char *buf, size_t max_len) {
  if (buf == NULL)
    return;

  if (max_len < UART_MIN_TRANSFER_SIZE || max_len > UART_MAX_TRANSFER_SIZE)
    return;

  size_t idx = 0U;

  while (1) {
    uint8_t byte = 0U;

    if (!uart_receive(&byte, 1U)) {
      break;
    }

    if (byte == '\n' || byte == '\r') {
      break;
    }

    if (idx < (max_len - 1U)) {
      buf[idx++] = (char)byte;
    }
  }

  buf[idx] = '\0';
}

bool uart_receive_byte_nonblocking(uint8_t *byte) {
  if (byte == NULL) {
    return false;
  }

  return uart_receive_nonblocking(byte, 1U);
}

bool uart_set_baud(uint32_t baud) {
  // TODO: validate baud range before reinitializing hardware
  return uart_port_set_baud(baud);
}

uint32_t uart_get_baud(void) { return uart_port_get_baud(); }

uart_word_length_t uart_get_word_length(void) { return uart_port_get_word_length(); }

uart_stop_bits_t uart_get_stop_bits(void) { return uart_port_get_stop_bits(); }

uart_parity_t uart_get_parity(void) { return uart_port_get_parity(); }

const char *uart_get_name(void) { return uart_port_get_name(); }
