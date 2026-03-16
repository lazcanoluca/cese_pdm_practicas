/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdbool.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef uint32_t tick_t;

typedef bool bool_t;

typedef struct {
  tick_t startTime;
  tick_t duration;
  bool_t running;
} delay_t;

typedef struct {
  const tick_t *durations;
  uint8_t durations_len;
  uint8_t repeats_per_step;
  uint8_t duty_cycle;
  uint8_t counter;
  uint8_t position;
  bool_t high;
  tick_t millis_high;
} blink_pattern_t;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

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
 * @brief Initializes the blink pattern generator.
 * @param bp Pointer to the pattern structure.
 * @param durations Array of base periods in ticks.
 * @param durations_len Number of elements in @p durations.
 * @param repeats_per_step Number of repeats for each period.
 * @param duty_cycle Duty cycle percentage (0..100).
 * @code
 * const tick_t durations[] = {1000, 1500, 500};
 * blink_pattern_t blink_pattern;
 * blinkPatternInit(&blink_pattern,
 *                  durations,
 *                  sizeof(durations) / sizeof(durations[0]),
 *                  5,
 *                  50);
 * @endcode
 */
void blinkPatternInit(blink_pattern_t *bp, const tick_t durations[], uint8_t durations_len,
                      uint8_t repeats_per_step, uint8_t duty_cycle);

/**
 * @brief Advances one pattern step and returns the next wait time.
 * @param bp Pointer to the pattern structure.
 * @return Time in ticks until the next state change.
 */
tick_t blinkPatternStep(blink_pattern_t *bp);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
