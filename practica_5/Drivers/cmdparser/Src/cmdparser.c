
#include "cmdparser.h"
#include "led.h"
#include "uart.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_ERROR_OVERFLOW_MSG "ERROR: line too long\r\n"
#define CMD_ERROR_UNKNOWN_MSG "ERROR: unknown command\r\n"
#define CMD_ERROR_BAD_ARGS_MSG "ERROR: bad arguments\r\n"
#define CMD_HELP_TEXT                                                          \
    "Commands:\r\n- HELP\r\n- LED ON\r\n- LED OFF\r\n- LED TOGGLE\r\n- "       \
    "STATUS\r\n- BAUD=XXXX\r\n- BAUD?\r\n"
#define CMD_STATUS_LED_ON_MSG "LED is ON\r\n"
#define CMD_STATUS_LED_OFF_MSG "LED is OFF\r\n"

typedef enum {
    CMD_STATE_IDLE,
    CMD_STATE_RECEIVING,
    CMD_STATE_PROCESS,
    CMD_STATE_EXEC,
    CMD_STATE_ERROR
} cmd_state_t;

typedef struct {
    char line[CMD_MAX_LINE];
    size_t len;
    bool overflow;
} cmd_line_state_t;

typedef struct {
    cmd_command_t type;
    union {
        uint32_t baud;
    } data;
} cmd_command_payload_t;

/*
 * cmd_poll drives a simple finite state machine:
 *   - CMD_STATE_IDLE: reset line buffer and transition to RECEIVING.
 *   - CMD_STATE_RECEIVING: non-blocking UART read, accumulate characters, set
 * overflow flag when buffer is full, switch to PROCESS on newline.
 *   - CMD_STATE_PROCESS: interpret the buffered line, determine command or
 * error, and transition to EXEC or ERROR respectively.
 *   - CMD_STATE_EXEC: execute the parsed command (LED, STATUS, BAUD, etc.),
 * then go back to IDLE.
 *   - CMD_STATE_ERROR: emit the corresponding error string and reset to IDLE.
 */
static cmd_state_t parser_state;
static cmd_line_state_t line_state;
static cmd_command_payload_t pending_command;
static cmd_error_t pending_error;

static void cmd_reset_line(void);
static void cmd_process_line(void);
static void cmd_handle_command(void);
static void cmd_handle_error(void);
static bool cmd_should_ignore_line(const char *line);

void cmd_parser_init(void) {
    parser_state = CMD_STATE_IDLE;
    cmd_reset_line();
    pending_command.type = CMD_COMMAND_NONE;
    pending_error = CMD_ERROR_NONE;
}

void cmd_poll(void) {
    switch (parser_state) {
    case CMD_STATE_IDLE:
        /* Reset buffer and start listening for a new line. */
        cmd_reset_line();
        parser_state = CMD_STATE_RECEIVING;
        break;
    case CMD_STATE_RECEIVING: {
        /* Poll UART without blocking; collect characters until newline. */
        uint8_t byte = 0U;
        if (!uart_receive_byte_nonblocking(&byte)) {
            break;
        }

        if (byte == '\n' || byte == '\r') {
            parser_state = CMD_STATE_PROCESS;
            break;
        }

        if (line_state.len < (CMD_MAX_LINE - 1U)) {
            line_state.line[line_state.len++] = (char)byte;
            line_state.line[line_state.len] = '\0';
        } else {
            line_state.overflow = true;
        }
        break;
    }
    case CMD_STATE_PROCESS:
        /* Interpret the buffered line and decide next state. */
        cmd_process_line();
        break;
    case CMD_STATE_EXEC:
        /* Execute the pending command, then return to idle. */
        cmd_handle_command();
        pending_command.type = CMD_COMMAND_NONE;
        parser_state = CMD_STATE_IDLE;
        break;
    case CMD_STATE_ERROR:
        /* Emit the error message and reset to idle. */
        cmd_handle_error();
        pending_error = CMD_ERROR_NONE;
        parser_state = CMD_STATE_IDLE;
        break;
    default:
        parser_state = CMD_STATE_IDLE;
        break;
    }
}

void cmd_print_help(void) { uart_send_string(CMD_HELP_TEXT); }

static void cmd_reset_line(void) {
    line_state.len = 0U;
    line_state.line[0] = '\0';
    line_state.overflow = false;
}

static void cmd_process_line(void) {
    if (line_state.overflow) {
        pending_error = CMD_ERROR_OVERFLOW;
        parser_state = CMD_STATE_ERROR;
        return;
    }

    if (line_state.len == 0U) {
        parser_state = CMD_STATE_IDLE;
        return;
    }

    if (cmd_should_ignore_line(line_state.line)) {
        parser_state = CMD_STATE_IDLE;
        return;
    }

    if (strcmp(line_state.line, "HELP") == 0) {
        pending_command.type = CMD_COMMAND_HELP;
        parser_state = CMD_STATE_EXEC;
        return;
    }

    if (strcmp(line_state.line, "LED ON") == 0) {
        pending_command.type = CMD_COMMAND_LED_ON;
        parser_state = CMD_STATE_EXEC;
        return;
    }

    if (strcmp(line_state.line, "LED OFF") == 0) {
        pending_command.type = CMD_COMMAND_LED_OFF;
        parser_state = CMD_STATE_EXEC;
        return;
    }

    if (strcmp(line_state.line, "LED TOGGLE") == 0) {
        pending_command.type = CMD_COMMAND_LED_TOGGLE;
        parser_state = CMD_STATE_EXEC;
        return;
    }

    if (strcmp(line_state.line, "STATUS") == 0) {
        pending_command.type = CMD_COMMAND_STATUS;
        parser_state = CMD_STATE_EXEC;
        return;
    }

    if (strncmp(line_state.line, "BAUD=", 5) == 0) {
        const char *value_str = line_state.line + 5;
        char *end_ptr = NULL;
        unsigned long parsed = strtoul(value_str, &end_ptr, 10);

        if (value_str == end_ptr || *end_ptr != '\0') {
            pending_error = CMD_ERROR_BAD_ARGS;
            parser_state = CMD_STATE_ERROR;
            return;
        }

        pending_command.type = CMD_COMMAND_SET_BAUD;
        pending_command.data.baud = (uint32_t)parsed;
        parser_state = CMD_STATE_EXEC;
        return;
    }

    if (strcmp(line_state.line, "BAUD?") == 0) {
        pending_command.type = CMD_COMMAND_QUERY_BAUD;
        parser_state = CMD_STATE_EXEC;
        return;
    }

    pending_error = CMD_ERROR_UNKNOWN;
    parser_state = CMD_STATE_ERROR;
}

static void cmd_handle_command(void) {
    switch (pending_command.type) {
    case CMD_COMMAND_HELP:
        cmd_print_help();
        break;
    case CMD_COMMAND_LED_ON:
        led_on();
        break;
    case CMD_COMMAND_LED_OFF:
        led_off();
        break;
    case CMD_COMMAND_LED_TOGGLE:
        led_toggle();
        break;
    case CMD_COMMAND_STATUS:
        if (led_is_on()) {
            uart_send_string(CMD_STATUS_LED_ON_MSG);
        } else {
            uart_send_string(CMD_STATUS_LED_OFF_MSG);
        }
        break;
    case CMD_COMMAND_SET_BAUD:
        (void)uart_set_baud(pending_command.data.baud);
        break;
    case CMD_COMMAND_QUERY_BAUD: {
        uint32_t baud = uart_get_baud();
        char response[32];
        snprintf(response, sizeof(response), "BAUD %lu\r\n",
                 (unsigned long)baud);
        uart_send_string(response);
        break;
    }
    case CMD_COMMAND_NONE:
    default:
        break;
    }
}

static void cmd_handle_error(void) {
    switch (pending_error) {
    case CMD_ERROR_OVERFLOW:
        uart_send_string(CMD_ERROR_OVERFLOW_MSG);
        break;
    case CMD_ERROR_UNKNOWN:
        uart_send_string(CMD_ERROR_UNKNOWN_MSG);
        break;
    case CMD_ERROR_BAD_ARGS:
        uart_send_string(CMD_ERROR_BAD_ARGS_MSG);
        break;
    case CMD_ERROR_NONE:
    default:
        break;
    }
}

static bool cmd_should_ignore_line(const char *line) {
    if (line == NULL || line[0] == '\0') {
        return false;
    }

    if (line[0] == '#') {
        return true;
    }

    if (line[0] == '/' && line[1] == '/') {
        return true;
    }

    return false;
}
