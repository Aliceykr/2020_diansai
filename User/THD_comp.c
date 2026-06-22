#include "THD_comp.h"

void THD_Comp_Init(void)
{
    AD9833_Init();
    AD9833_SetFrequency(1000);
    AD9833_SetWaveform(0);
}

float THD_Calc(void)
{
    float v1 = fft_magnitude[THD_BIN_1K];
    float v2 = fft_magnitude[THD_BIN_2K];
    float v3 = fft_magnitude[THD_BIN_3K];
    float v4 = fft_magnitude[THD_BIN_4K];
    float v5 = fft_magnitude[THD_BIN_5K];

    if (v1 < 1e-6f) return 0.0f;

    float sum_sq = v2*v2 + v3*v3 + v4*v4 + v5*v5;
    
    return sqrtf(sum_sq) / v1 * 100.0f;
}
