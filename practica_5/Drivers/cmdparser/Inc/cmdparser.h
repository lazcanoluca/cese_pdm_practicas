
#ifndef LIB_CMDPARSER_H_
#define LIB_CMDPARSER_H_

#define CMD_MAX_LINE 64
#define CMD_MAX_TOKENS 3

typedef enum {
    CMD_COMMAND_NONE = 0,
    CMD_COMMAND_HELP,
    CMD_COMMAND_LED_ON,
    CMD_COMMAND_LED_OFF,
    CMD_COMMAND_LED_TOGGLE,
    CMD_COMMAND_STATUS,
    CMD_COMMAND_SET_BAUD,
    CMD_COMMAND_QUERY_BAUD
} cmd_command_t;

typedef enum {
    CMD_ERROR_NONE = 0,
    CMD_ERROR_OVERFLOW,
    CMD_ERROR_UNKNOWN,
    CMD_ERROR_BAD_ARGS
} cmd_error_t;

/**
 *
 */
void cmd_parser_init(void);

/**
 *
 */
void cmd_poll(void);

/**
 *
 */
void cmd_print_help(void);

#endif /* LIB_CMDPARSER_H_ */
