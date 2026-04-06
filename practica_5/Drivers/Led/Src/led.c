#include "led.h"

void led_init(void) { led_off(); }

void led_on(void) { HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET); }

void led_off(void) { HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET); }

void led_toggle(void) { HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); }

bool led_is_on(void) {
  GPIO_PinState state = HAL_GPIO_ReadPin(LD2_GPIO_Port, LD2_Pin);
  return (state == GPIO_PIN_SET);
}
