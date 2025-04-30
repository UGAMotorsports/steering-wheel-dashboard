/*
 * shiftLights.h
 *
 *  Created on: Apr 21, 2025
 *      Author: cathy
 */

#ifndef SHIFTLIGHTS_H_
#define SHIFTLIGHTS_H_

#include "stm32f4xx_hal.h"

#define LED_COUNT 16    // Number of WS2812B LEDs
#define BITS_PER_LED 24  // 8 Red + 8 Green + 8 Blue
#define PWM_BUFFER_SIZE (LED_COUNT * BITS_PER_LED)

void setColor(TIM_HandleTypeDef *htim, uint32_t Channel, uint8_t green, uint8_t red,
		uint8_t blue, uint8_t *ledcolors, uint16_t *ledbytes, int ledIndex);

void shiftLightsInit(TIM_HandleTypeDef *htim, uint32_t Channel, uint8_t *ledcolors, uint16_t *ledbytes);

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);

void UpdateShiftLights(TIM_HandleTypeDef *htim, uint32_t Channel, uint8_t *ledcolors,
		uint16_t *ledbytes, int current_rpm, int * RPM_thresholds);

void lightOn(TIM_HandleTypeDef *htim, uint32_t Channel, uint8_t *ledcolors,
		uint16_t *ledbytes,int index);

void startUp(TIM_HandleTypeDef *htim, uint32_t Channel, uint8_t *ledcolors, uint16_t *ledbytes);

#endif
