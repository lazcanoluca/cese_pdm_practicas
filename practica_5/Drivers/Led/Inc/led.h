#ifndef LED_LED_H_
#define LED_LED_H_

#include "main.h"
#include <stdbool.h>

void led_init(void);
void led_on(void);
void led_off(void);
void led_toggle(void);
bool led_is_on(void);

#endif /* LED_LED_H_ */
