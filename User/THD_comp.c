#include "THD_comp.h"

void THD_Comp_Init(void)
{
    AD9833_Init();
    AD9833_SetFrequency(1000); // 设置为 1 kHz
    AD9833_SetWaveform(0); // 选择正弦波

    
}