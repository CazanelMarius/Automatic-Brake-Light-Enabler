#ifndef STM32F429I_DISC1_BOARD_API_H
#define STM32F429I_DISC1_BOARD_API_H

#include "stm32f4xx_hal.h"

#define GREEN_LED GPIO_PIN_13
#define RED_LED GPIO_PIN_14
#define USER_BUTTON GPIO_PIN_0

void writePinState(GPIO_TypeDef*, uint16_t pinAddress, short pinState);
short readPinState(GPIO_TypeDef*, uint16_t);

#endif