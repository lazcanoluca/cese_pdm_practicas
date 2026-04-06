
#include "stm32f4xx_hal.h"
#include "uart_port.h"
#include "main.h"

extern UART_HandleTypeDef huart2;

bool uart_port_init(void) {
    MX_USART2_UART_Init();
    return true;
}

bool uart_transmit(uint8_t *data, uint16_t size) {
    if (data == NULL || size == 0U) {
        return false;
    }

    HAL_StatusTypeDef status =
        HAL_UART_Transmit(&huart2, data, size, HAL_MAX_DELAY);
    return (status == HAL_OK);
}

bool uart_receive(uint8_t *buffer, uint16_t size) {
    if (buffer == NULL || size == 0U) {
        return false;
    }

    HAL_StatusTypeDef status =
        HAL_UART_Receive(&huart2, buffer, size, HAL_MAX_DELAY);
    return (status == HAL_OK);
}

bool uart_receive_nonblocking(uint8_t *buffer, uint16_t size) {
    if (buffer == NULL || size == 0U) {
        return false;
    }

    HAL_StatusTypeDef status = HAL_UART_Receive(&huart2, buffer, size, 0U);

    return (status == HAL_OK);
}

bool uart_port_set_baud(uint32_t baud) {
    // TODO: validate baud range before applying the new configuration
    huart2.Init.BaudRate = baud;

    if (HAL_UART_DeInit(&huart2) != HAL_OK) {
        return false;
    }

    if (HAL_UART_Init(&huart2) != HAL_OK) {
        return false;
    }

    return true;
}

uint32_t uart_port_get_baud(void) { return huart2.Init.BaudRate; }

uart_word_length_t uart_port_get_word_length(void) {
    switch (huart2.Init.WordLength) {
    case UART_WORDLENGTH_8B:
        return UART_WORD_LEN_8;
    case UART_WORDLENGTH_9B:
        return UART_WORD_LEN_9;
    default:
        return UART_WORD_LEN_UNKNOWN;
    }
}

uart_stop_bits_t uart_port_get_stop_bits(void) {
    switch (huart2.Init.StopBits) {
    case UART_STOPBITS_1:
        return UART_STOP_BITS_1;
    case UART_STOPBITS_2:
        return UART_STOP_BITS_2;
    default:
        return UART_STOP_BITS_UNKNOWN;
    }
}

uart_parity_t uart_port_get_parity(void) {
    switch (huart2.Init.Parity) {
    case UART_PARITY_NONE:
        return UART_CFG_PARITY_NONE;
    case UART_PARITY_EVEN:
        return UART_CFG_PARITY_EVEN;
    case UART_PARITY_ODD:
        return UART_CFG_PARITY_ODD;
    default:
        return UART_CFG_PARITY_UNKNOWN;
    }
}

const char *uart_port_get_name(void) { return "UART2"; }
