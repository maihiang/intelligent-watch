#include "demo.h"
#include "./BSP/ATK_MD0280/atk_md0280.h"
#include "./BSP/led/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include <math.h>

extern uint16_t year, month, day, hour, minute, second;


void display_mode_0(void)
{
    atk_md0280_fill(0,150,239,169,ATK_MD0280_BLACK);
    atk_md0280_fill(110,0,129,319,ATK_MD0280_BLACK);
    atk_md0280_show_string(40, 74, ATK_MD0280_LCD_WIDTH, 12, "Show 1", ATK_MD0280_LCD_FONT_12, ATK_MD0280_BLACK);
    atk_md0280_show_string(160, 74, ATK_MD0280_LCD_WIDTH, 12, "Show 2", ATK_MD0280_LCD_FONT_12, ATK_MD0280_BLACK);
    atk_md0280_show_string(40, 234, ATK_MD0280_LCD_WIDTH, 12, "Show 3", ATK_MD0280_LCD_FONT_12, ATK_MD0280_BLACK);
    atk_md0280_show_string(160, 234, ATK_MD0280_LCD_WIDTH, 12, "Show 4", ATK_MD0280_LCD_FONT_12, ATK_MD0280_BLACK);
}

void display_mode_1(void)
{
    atk_md0280_fill(10,10,40,30,ATK_MD0280_BLACK);
    atk_md0280_show_string(12, 15, ATK_MD0280_LCD_WIDTH, 12, "BACK", ATK_MD0280_LCD_FONT_12, ATK_MD0280_WHITE);
    atk_md0280_draw_rect(1,152,32,167,ATK_MD0280_BLACK);
    atk_md0280_show_string(5, 149, ATK_MD0280_LCD_WIDTH, 16, "pre", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BLACK);
    atk_md0280_draw_rect(207,152,238,167,ATK_MD0280_BLACK);
    atk_md0280_show_string(211, 149, ATK_MD0280_LCD_WIDTH, 16, "nex", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BLACK);
    atk_md0280_fill(100, 100, 240, 116, ATK_MD0280_WHITE);
    atk_md0280_show_xnum(100, 100, second, 10, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_16, ATK_MD0280_RED);
}

void display_mode_2(void)
{
    atk_md0280_show_string(120, 160, ATK_MD0280_LCD_WIDTH, 32, "2", ATK_MD0280_LCD_FONT_32, ATK_MD0280_RED);
    atk_md0280_fill(10,10,40,30,ATK_MD0280_BLACK);
    atk_md0280_show_string(12, 15, ATK_MD0280_LCD_WIDTH, 12, "BACK", ATK_MD0280_LCD_FONT_12, ATK_MD0280_WHITE);
    atk_md0280_draw_rect(1,152,32,167,ATK_MD0280_BLACK);
    atk_md0280_show_string(5, 149, ATK_MD0280_LCD_WIDTH, 16, "pre", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BLACK);
    atk_md0280_draw_rect(207,152,238,167,ATK_MD0280_BLACK);
    atk_md0280_show_string(211, 149, ATK_MD0280_LCD_WIDTH, 16, "nex", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BLACK);
}

void display_mode_3(void)
{
    atk_md0280_show_string(120, 160, ATK_MD0280_LCD_WIDTH, 32, "3", ATK_MD0280_LCD_FONT_32, ATK_MD0280_RED);
    atk_md0280_fill(10,10,40,30,ATK_MD0280_BLACK);
    atk_md0280_show_string(12, 15, ATK_MD0280_LCD_WIDTH, 12, "BACK", ATK_MD0280_LCD_FONT_12, ATK_MD0280_WHITE);
    atk_md0280_draw_rect(1,152,32,167,ATK_MD0280_BLACK);
    atk_md0280_show_string(5, 149, ATK_MD0280_LCD_WIDTH, 16, "pre", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BLACK);
    atk_md0280_draw_rect(207,152,238,167,ATK_MD0280_BLACK);
    atk_md0280_show_string(211, 149, ATK_MD0280_LCD_WIDTH, 16, "nex", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BLACK);
}

void display_mode_4(void)
{

    atk_md0280_show_string(120, 160, ATK_MD0280_LCD_WIDTH, 32, "4", ATK_MD0280_LCD_FONT_32, ATK_MD0280_RED);
    atk_md0280_fill(10,10,40,30,ATK_MD0280_BLACK);
    atk_md0280_show_string(12, 15, ATK_MD0280_LCD_WIDTH, 12, "BACK", ATK_MD0280_LCD_FONT_12, ATK_MD0280_WHITE);
    atk_md0280_draw_rect(1,152,32,167,ATK_MD0280_BLACK);
    atk_md0280_show_string(5, 149, ATK_MD0280_LCD_WIDTH, 16, "pre", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BLACK);
    atk_md0280_draw_rect(207,152,238,167,ATK_MD0280_BLACK);
    atk_md0280_show_string(211, 149, ATK_MD0280_LCD_WIDTH, 16, "nex", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BLACK);
}
