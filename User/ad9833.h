#ifndef __AD9833_H__
#define __AD9833_H__

#include "main.h"

#define AD9833_MCLK   25000000UL

#define AD9833_CTRL_B28     (1 << 13)
#define AD9833_CTRL_HLB     (1 << 12)
#define AD9833_CTRL_FSEL    (1 << 11)
#define AD9833_CTRL_PSEL    (1 << 10)
#define AD9833_CTRL_RESET   (1 << 9)
#define AD9833_CTRL_SLEEP1  (1 << 8)
#define AD9833_CTRL_SLEEP12 (1 << 7)
#define AD9833_CTRL_OPBITEN (1 << 6)
#define AD9833_CTRL_DIV2    (1 << 3)
#define AD9833_CTRL_MODE    (1 << 1)

#define AD9833_CMD_WRITE_CTRL 0x0000
#define AD9833_CMD_FREQ0      0x4000
#define AD9833_CMD_FREQ1      0x8000
#define AD9833_CMD_PHASE0     0xC000
#define AD9833_CMD_PHASE1     0xE000

/* 初始化 AD9833：复位、预装载频率/相位、启动输出（默认 1kHz 正弦） */
void AD9833_Init(void);

/* 设置输出频率（Hz），无毛刺切换：写入空闲频率寄存器后翻转 FSELECT 原子切过去 */
void AD9833_SetFrequency(uint32_t freq_hz);

/* 设置相位偏移，phase 为 12 位相位寄存器值（0~4095 对应 0~2π） */
void AD9833_SetPhase(uint16_t phase);

/* 选择波形：mode=0 正弦，mode=1 三角波 */
void AD9833_SetWaveform(uint8_t mode);

/* 复位：DAC 输出停在中点、相位累加器清零（不掉电） */
void AD9833_Reset(void);

/* 解除复位，恢复输出 */
void AD9833_EnableOutput(void);

#endif
