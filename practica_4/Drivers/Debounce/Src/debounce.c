/*
 * debounce.c
 *
 *  Created on: Mar 29, 2026
 *      Author: luca
 */

#include "debounce.h"
#include "delay.h"
#include "stm32f4xx_hal.h"

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC

bool Button1_IsPressed() {
	HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
}

static debounce_state_t debounce_state;
static delay_t timer;

// TODO: get if button is normal up or normal down
// TODO: inject function to debounce
// TODO: pass time
void debounceInit() {
	debounce_state = BUTTON_UP;
	delayInit(&timer, 40);
}

void debounceUpdate() {
	switch (debounce_state) {
	case BUTTON_UP:
		// si boton apretado -> pasar a BUTTON_FALLING, iniciar timer
		if (Button1_IsPressed()) {
			debounce_state = BUTTON_FALLING;
			delayReset(&timer);
		}
		break;
	case BUTTON_DOWN:
		// si boton no apretado -> pasar a BUTTON_RISING, iniciar timer
		if (!Button1_IsPressed()) {
			debounce_state = BUTTON_RISING;
			delayReset(&timer);
		}
		break;
	case BUTTON_FALLING:
		if (!delayRead(&timer))
			break;

		if (Button1_IsPressed())
			debounce_state = BUTTON_DOWN;
		else
			debounce_state = BUTTON_UP;

		delayStop(&timer);
		break;
	case BUTTON_RISING:
		if (!delayRead(&timer))
			break;

		if (!Button1_IsPressed())
			debounce_state = BUTTON_UP;
		else
			debounce_state = BUTTON_DOWN;

		delayStop(&timer);
		break;

	// TODO: default
	}
}


bool buttonIsPressed() {
	return (debounce_state == BUTTON_DOWN || debounce_state == BUTTON_RISING);
}

bool buttonIsReleased() {
	return !buttonIsPressed();
}
