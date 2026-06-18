#include "ad9833.h"
#include "spi.h"
#include "gpio.h"

static uint16_t ad9833_ctrl = AD9833_CTRL_B28;
static uint8_t  ad9833_fsel = 0;   /* 当前生效的频率寄存器: 0=FREQ0, 1=FREQ1 */

static void AD9833_Write16(uint16_t data)
{
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&data, 1, 100);
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

void AD9833_Init(void)
{
    /* 复位期间预装载 FREQ0/PHASE0，选定 FREQ0 为初始输出 */
    ad9833_fsel = 0;
    ad9833_ctrl &= ~AD9833_CTRL_FSEL;           /* FSELECT=0 → FREQ0 */

    AD9833_Write16(AD9833_CMD_WRITE_CTRL | ad9833_ctrl | AD9833_CTRL_RESET);

    /* 初始频率写入 FREQ0（当前选中的寄存器，复位期间允许写） */
    uint32_t freq_reg = (uint32_t)((uint64_t)1000 * 268435456ULL / AD9833_MCLK);
    AD9833_Write16(AD9833_CMD_FREQ0 | (freq_reg & 0x3FFF));
    AD9833_Write16(AD9833_CMD_FREQ0 | ((freq_reg >> 14) & 0x3FFF));

    AD9833_Write16(AD9833_CMD_PHASE0 | 0);      /* 相位 0 */

    /* 清除 RESET，显式关闭 SLEEP1/SLEEP12/OPBITEN */
    AD9833_Write16(AD9833_CMD_WRITE_CTRL | ad9833_ctrl
                   & ~(AD9833_CTRL_SLEEP1 | AD9833_CTRL_SLEEP12 | AD9833_CTRL_OPBITEN));
    HAL_Delay(1);
}

void AD9833_SetFrequency(uint32_t freq_hz)
{
    uint32_t freq_reg = (uint32_t)((uint64_t)freq_hz * 268435456ULL / AD9833_MCLK);
    uint16_t lower_14 = freq_reg & 0x3FFF;
    uint16_t upper_14 = (freq_reg >> 14) & 0x3FFF;

    if (ad9833_fsel == 0) {
        /* 当前 FREQ0 输出 → 新频率写入空闲的 FREQ1，然后翻转 FSELECT */
        AD9833_Write16(AD9833_CMD_FREQ1 | lower_14);
        AD9833_Write16(AD9833_CMD_FREQ1 | upper_14);
        ad9833_ctrl |= AD9833_CTRL_FSEL;         /* FSELECT=1 → FREQ1 */
        ad9833_fsel = 1;
    } else {
        /* 当前 FREQ1 输出 → 新频率写入空闲的 FREQ0，然后翻转 FSELECT */
        AD9833_Write16(AD9833_CMD_FREQ0 | lower_14);
        AD9833_Write16(AD9833_CMD_FREQ0 | upper_14);
        ad9833_ctrl &= ~AD9833_CTRL_FSEL;        /* FSELECT=0 → FREQ0 */
        ad9833_fsel = 0;
    }

    /* 单次控制字写入，原子翻转 FSELECT —— 无毛刺切换 */
    AD9833_Write16(AD9833_CMD_WRITE_CTRL | ad9833_ctrl);
}

void AD9833_SetPhase(uint16_t phase)
{
    /* 相位寄存器同样 ping-pong：写当前未使用的，再原子切 PSEL */
    if (ad9833_ctrl & AD9833_CTRL_PSEL) {
        AD9833_Write16(AD9833_CMD_PHASE0 | (phase & 0x0FFF));
        ad9833_ctrl &= ~AD9833_CTRL_PSEL;        /* PSEL=0 → PHASE0 */
    } else {
        AD9833_Write16(AD9833_CMD_PHASE1 | (phase & 0x0FFF));
        ad9833_ctrl |= AD9833_CTRL_PSEL;         /* PSEL=1 → PHASE1 */
    }
    AD9833_Write16(AD9833_CMD_WRITE_CTRL | ad9833_ctrl);
}

void AD9833_SetWaveform(uint8_t mode)
{
    ad9833_ctrl &= ~AD9833_CTRL_MODE;
    if (mode)
        ad9833_ctrl |= AD9833_CTRL_MODE;
    AD9833_Write16(AD9833_CMD_WRITE_CTRL | ad9833_ctrl);
}

void AD9833_Reset(void)
{
    AD9833_Write16(AD9833_CMD_WRITE_CTRL | ad9833_ctrl | AD9833_CTRL_RESET);
}

void AD9833_EnableOutput(void)
{
    AD9833_Write16(AD9833_CMD_WRITE_CTRL | ad9833_ctrl);
}
