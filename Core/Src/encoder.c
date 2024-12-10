#include "encoder.h"

volatile int16_t encoder_value = 0;
volatile int16_t last_encoder_value = 0;

void encoder_init(TIM_HandleTypeDef *htim) {
    HAL_TIM_Encoder_Start_IT(htim, TIM_CHANNEL_ALL); // Aktifkan mode encoder
}

void encoder_reset(void) {
    encoder_value = 0;  // Reset nilai encoder
}
