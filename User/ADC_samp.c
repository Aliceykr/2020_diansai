#include "ADC_samp.h"
#include "arm_math.h"
#include <math.h>

#define BUFFER_SIZE 256

volatile uint8_t adc_ready_flag = 0;

static float32_t fft_input[ADC_SAMP_MAX_SIZE * 2];
static float32_t fft_magnitude[ADC_SAMP_MAX_SIZE / 2];
static uint16_t adc_buffer[BUFFER_SIZE] = {0};

void ADC_samp_Init(uint16_t adc_buffer[], uint16_t buffer_size)
{
    HAL_OPAMP_GetState(&hopamp1);
    HAL_TIM_Base_Start(&htim2);
    HAL_ADC_Start_DMA(&hadc1, (uint16_t *)adc_buffer, buffer_size);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        HAL_ADC_Stop_DMA(&hadc1);
        HAL_TIM_Base_Stop(&htim2);
        adc_ready_flag = 1;
    }
}

void ADC_samp_Stop(void)
{
    HAL_ADC_Stop_DMA(&hadc1);
    HAL_TIM_Base_Stop(&htim2);
}

void ADC_samp_Process()
{
    if (!adc_ready_flag || BUFFER_SIZE > ADC_SAMP_MAX_SIZE)
        return;

    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        float hann = 0.5f * (1.0f - cosf(2.0f * (float)M_PI * i / (BUFFER_SIZE - 1)));
        fft_input[2 * i] = ((float)adc_buffer[i] - 2048.0f) * hann;
        fft_input[2 * i + 1] = 0.0f;
    }

    arm_cfft_instance_f32 cfft_instance;
    arm_cfft_init_f32(&cfft_instance, BUFFER_SIZE);
    arm_cfft_f32(&cfft_instance, fft_input, 0, 1);

    arm_cmplx_mag_f32(fft_input, fft_magnitude, BUFFER_SIZE / 2);

    for (uint16_t i = 0; i < BUFFER_SIZE / 2; i++)
        fft_magnitude[i] /= BUFFER_SIZE;

    adc_ready_flag = 0;
}
