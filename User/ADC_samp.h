#ifndef __ADC_SAMP_H__
#define __ADC_SAMP_H__

#include "main.h"
#include <stdint.h>

#define ADC_SAMP_MAX_SIZE 1024

#define THD_BIN_1K  32
#define THD_BIN_2K  64
#define THD_BIN_3K  96
#define THD_BIN_4K  128
#define THD_BIN_5K  160

extern volatile uint8_t adc_ready_flag;
extern float32_t fft_magnitude[ADC_SAMP_MAX_SIZE / 2];

void ADC_samp_Init(uint16_t adc_buffer[], uint16_t buffer_size);
void ADC_samp_Process(uint16_t adc_buffer[], uint16_t buffer_size);
void ADC_samp_Stop(void);

#endif
