#include "ADC_samp.h"
#include "main.h"
#include "arm_math.h"

uint8_t adc_ready_flag = 0; // ADC 采样完成标志

void ADC_samp_Init(uint16_t adc_buffer[], uint16_t buffer_size)
{
    HAL_OPAMP_GetState(&hopamp1); // 确保 OPAMP 已经初始化
    HAL_TIM_Base_Start(&htim2); // 启动 TIM2
    HAL_ADC_Start_DMA(&hadc1, (uint16_t *)adc_buffer, buffer_size); // 启动 ADC DMA 传输
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        adc_ready_flag = 1; // 设置 ADC 采样完成标志
    }
}

void ADC_samp_Process(uint16_t adc_buffer[], uint16_t buffer_size, float hann_weight)
{
    float float_buffer[buffer_size];

    if (adc_ready_flag)
    {
        //汉宁窗
        for (uint16_t i = 0; i < buffer_size; i++)
        {
            float_buffer[i] = (float)adc_buffer[i] - 2048.0f; // 转换为浮点数
        }

        for (uint16_t i = 0; i < buffer_size; i++)
        {
            float_buffer[i] *= hann_weight * (1.0f - cosf(2.0f * M_PI * i / (buffer_size - 1))); // 应用汉宁窗
        }

        float32_t fft_input[buffer_size * 2]; // 复数输入数组，实部和虚部分开存储
        float32_t fft_magnitude[buffer_size / 2]; // 存储 FFT 结果的幅值

        arm_cfft_instance_f32 cfft_instance;
        arm_cfft_init_f32(&cfft_instance, buffer_size); // 初始化 FFT 实例


        adc_ready_flag = 0; // 重置标志，准备下一轮采样
    }
}