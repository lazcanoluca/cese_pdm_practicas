/*
 * delay.c
 *
 *  Created on: Mar 25, 2026
 *      Author: luca
 */

#include "delay.h"
#include "stm32f4xx_hal.h"

void delayInit(delay_t *delay, tick_t duration) {
  if (delay == NULL || duration == 0) {
    return;
  }

  delay->startTime = 0;
  delay->duration = duration;
  delay->running = false;
}

void delayReset(delay_t *delay) {
	if (!delay)
		return;

	delay->startTime = HAL_GetTick();
	delay->running = true;
}

void delayStart(delay_t *delay) {
	if (!delay || delay->running)
		return;

	delay->startTime = HAL_GetTick();
	delay->running = true;
}

void delayStop(delay_t *delay) {
	if (!delay || !delay->running)
		return;

	delay->startTime = 0;
	delay->running = false;
}

bool_t delayRead(delay_t *delay) {
  if (delay == NULL) {
    return false;
  }

  // If the delay is not running, start it and return false immediately
  if (!delay->running) {
    delay->startTime = HAL_GetTick();
    delay->running = true;

    return false;
  }

  // If the delay is running, check if the duration has elapsed
  if (HAL_GetTick() - delay->startTime >= delay->duration) {
    delay->running = false;

    return true;
  }

  return false;
}

void delayWrite(delay_t *delay, tick_t duration) {
  if (delay == NULL || duration == 0) {
    return;
  }

  delay->duration = duration;
}

bool_t delayIsRunning(delay_t * delay) {
  if (delay == NULL) {
    return false;
  }

  return delay->running;
}
