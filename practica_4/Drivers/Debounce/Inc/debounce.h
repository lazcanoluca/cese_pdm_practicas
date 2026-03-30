/*
 * debounce.h
 *
 *  Created on: Mar 29, 2026
 *      Author: luca
 */

#ifndef DEBOUNCE_INC_DEBOUNCE_H_
#define DEBOUNCE_INC_DEBOUNCE_H_

#include <stdbool.h>

typedef enum {
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RISING,
} debounce_state_t;

void debounceInit();

void debounceUpdate();

bool buttonIsPressed();

bool buttonIsReleased();

#endif /* DEBOUNCE_INC_DEBOUNCE_H_ */
