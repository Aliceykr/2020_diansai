#ifndef __ADC_SAMP_H__
#define __ADC_SAMP_H__

#include "main.h"
#include <stdint.h>

#define ADC_SAMP_MAX_SIZE 1024
#define BUFFER_SIZE 256

#define THD_BIN_1K  8
#define THD_BIN_2K  16
#define THD_BIN_3K  24
#define THD_BIN_4K  32
#define THD_BIN_5K  40

extern volatile uint8_t adc_ready_flag;
extern float32_t fft_magnitude[ADC_SAMP_MAX_SIZE / 2];

void ADC_samp_Init(void);
void ADC_samp_Process(void);
void ADC_samp_Stop(void);

#endif
