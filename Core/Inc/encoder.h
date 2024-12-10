#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f4xx_hal.h"

// Variabel Global
extern volatile int16_t encoder_value;
extern volatile int16_t last_encoder_value;

// Fungsi
void encoder_init(TIM_HandleTypeDef *htim);
void encoder_reset(void);

#endif
