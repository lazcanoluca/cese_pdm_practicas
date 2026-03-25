/*
 * delay.h
 *
 *  Created on: Mar 25, 2026
 *      Author: luca
 */

#ifndef DELAY_INC_DELAY_H_
#define DELAY_INC_DELAY_H_

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t tick_t;

typedef bool bool_t;

typedef struct {
  tick_t startTime;
  tick_t duration;
  bool_t running;
} delay_t;


/**
 * @brief Initializes a non-blocking delay.
 * @param delay Pointer to the delay structure.
 * @param duration Delay duration in ticks.
 * @note Does nothing if @p delay is NULL or @p duration is 0.
 */
void delayInit(delay_t *delay, tick_t duration);

/**
 * @brief Checks whether the delay has expired.
 * @param delay Pointer to the delay structure.
 * @retval true The configured duration has elapsed.
 * @retval false The configured duration has not elapsed yet.
 * @note Returns false immediately if @p delay is NULL.
 */
bool_t delayRead(delay_t *delay);

/**
 * @brief Updates the delay duration.
 * @param delay Pointer to the delay structure.
 * @param duration New delay duration in ticks.
 * @note Does nothing if @p delay is NULL or @p duration is 0.
 */
void delayWrite(delay_t *delay, tick_t duration);

/**
 * @brief Checks if the delay is running.
 * @param delay Pointer to the delay structure.
 * @retval true The delay is running.
 * @retval false the delay is not running.
 * @note Returns false immediately if @p delay is NULL.
 */
bool_t delayIsRunning(delay_t * delay);

#endif /* DELAY_INC_DELAY_H_ */
