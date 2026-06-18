#ifndef __OLED_H__
#define __OLED_H__

#include "main.h"

#define OLED_ADDR    0x3C
#define OLED_WIDTH   128
#define OLED_HEIGHT  64
#define OLED_PAGES   (OLED_HEIGHT / 8)

/* 初始化 SSD1306 OLED（128x64，I2C），完成上电配置并清屏点亮 */
void OLED_Init(void);

/* 把显存逐页刷到屏幕（按页寻址，兼容性好） */
void OLED_Display(void);

/* 把整屏显存一次性刷出（水平寻址，速度更快） */
void OLED_DisplayFast(void);

/* 清空显存（全灭），自动刷屏 */
void OLED_Clear(void);

/* 显存全亮（点亮所有像素），自动刷屏 */
void OLED_Fill(void);

/* 在 (x,y) 画一个像素，color=1 点亮、0 熄灭；越界自动忽略 */
void OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t color);

/* 在 (x,y) 显示一个 6x8 ASCII 字符（左上角坐标） */
void OLED_ShowChar(uint8_t x, uint8_t y, char ch);

/* 在 (x,y) 显示字符串，自动换行、自动刷屏 */
void OLED_ShowString(uint8_t x, uint8_t y, const char *str);

/* 在 (x,y) 格式化输出（用法同 printf），自动刷屏 */
void OLED_Printf(uint8_t x, uint8_t y, const char *fmt, ...);

#endif
