#include "demo.h"
#include "./BSP/ATK_MD0280/atk_md0280.h"
#include "./BSP/led/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "displaymode.h"
#include <math.h>
#include "img.h"

extern uint16_t year, month, day, hour, minute, second;

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


/*
以下函数的功能是在屏幕上显示日期，占用像素为64*56
*/
void draw_date(uint8_t x, uint8_t y)
{
    atk_md0280_show_xnum(x, y, year, 4, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_32, ATK_MD0280_RED);
    atk_md0280_show_xnum(x + 1, y + 32, month, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_24, ATK_MD0280_RED);
    atk_md0280_show_char(x + 1 + 2 * 12, y + 32, '/', ATK_MD0280_LCD_FONT_24, ATK_MD0280_RED);
    atk_md0280_show_xnum(x + 1 + 3 * 12, y + 32, day, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_24, ATK_MD0280_RED);
}

/*
以下函数的功能是在屏幕上显示日期，占用像素为66*58，显示的日期会加粗
对应的x，y坐标为日期的左上角坐标
*/
void draw_date_bold(uint8_t x, uint8_t y)
{
    draw_date(x, y);
    draw_date(x - 1, y);
    draw_date(x + 1, y);
    draw_date(x, y - 1);
    draw_date(x, y + 1);
}

/*
以下函数的功能是绘制一个有厚度的正方形框
其中x1，y1为左上角坐标，x2，y2为右下角坐标
*/
void draw_rect_thick(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t thick)
{
    uint8_t i;
    for(i = 0; i < thick; i++)
    {
        atk_md0280_draw_rect(x1 + i, y1 + i, x2 - i, y2 - i, color);
    }
}

void display_main(void)
{
    static uint16_t year_tmp, month_tmp, day_tmp, hour_tmp, minute_tmp, second_tmp;

    if(year_tmp != year || month_tmp != month || day_tmp != day)
    {
        atk_md0280_fill(86, 100, 152, 158, ATK_MD0280_BLACK);
        year_tmp = year;
        month_tmp = month;
        day_tmp = day;
    }

    if(hour_tmp != hour || minute_tmp != minute)
    {
        atk_md0280_fill(71, 165, 167, 189, ATK_MD0280_BLACK);
        hour_tmp = hour;
        minute_tmp = minute;
    }

    if(second_tmp != second)
    {
        atk_md0280_fill(144, 165, 167, 189, ATK_MD0280_BLACK);
        second_tmp = second;
    }

    draw_date_bold(86, 100);

    draw_rect_thick(2, 267, 52, 317, ATK_MD0280_WHITE, 6);
    atk_md0280_fill(15, 280, 23, 288, ATK_MD0280_YELLOW);
    atk_md0280_fill(31, 296, 39, 304, ATK_MD0280_GREEN);
    atk_md0280_fill(15, 296, 23, 304, ATK_MD0280_BLUE);
    atk_md0280_fill(31, 280, 39, 288, ATK_MD0280_RED);

    atk_md0280_show_pic(187, 267, 50, 50, setting_logo);

    atk_md0280_show_xnum(71, 165, hour, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_24, ATK_MD0280_RED);
    atk_md0280_show_char(71 + 2 * 12, 165, ':', ATK_MD0280_LCD_FONT_24, ATK_MD0280_RED);
    atk_md0280_show_xnum(71 + 3 * 12, 165, minute, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_24, ATK_MD0280_RED);
    atk_md0280_show_char(71 + 5 * 12, 165, ':', ATK_MD0280_LCD_FONT_24, ATK_MD0280_RED);
    atk_md0280_show_xnum(71 + 6 * 12, 165, second, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_24, ATK_MD0280_RED);
}
