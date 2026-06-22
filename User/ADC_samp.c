#include "ADC_samp.h"

void ADC_samp_Init(uint16_t adc_buffer[], uint16_t buffer_size)
{
    HAL_OPAMP_GetState(&hopamp1);
    HAL_TIM_Base_Start(&htim2);
    HAL_ADC_Start_DMA(&hadc1, (uint16_t *)adc_buffer, buffer_size);
}
