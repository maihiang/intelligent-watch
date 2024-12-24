#include "demo.h"
#include "./BSP/ATK_MD0280/atk_md0280.h"
#include "./BSP/led/led.h"
#include "./BSP/DHT11/dht11.h"
#include "./BSP/CSA/csa.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "displaymode.h"
#include <math.h>
#include "img.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/sys/sys.h"

extern uint16_t year, month, day, hour, minute, second, solar_term;
extern uint16_t year_set, month_set, day_set, hour_set, minute_set, second_set;

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
    switch (solar_term)
    {
    case 1:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "Lesser Cold", ATK_MD0280_LCD_FONT_16, ATK_MD0280_GBLUE);
        break;
    case 2:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "Greater Cold", ATK_MD0280_LCD_FONT_16, ATK_MD0280_GBLUE);
        break;
    case 3:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "the Beginning of Spring", ATK_MD0280_LCD_FONT_16, ATK_MD0280_GREEN);
        break;
    case 4:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "Rain Water", ATK_MD0280_LCD_FONT_16, ATK_MD0280_GREEN);
        break;
    case 5:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "the Waking of Insects", ATK_MD0280_LCD_FONT_16, ATK_MD0280_GREEN);
        break;
    case 6:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "the Spring Equinox", ATK_MD0280_LCD_FONT_16, ATK_MD0280_GREEN);
        break;
    case 7:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "Pure Brightness", ATK_MD0280_LCD_FONT_16, ATK_MD0280_GREEN);
        break;
    case 8:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "the Grain Rain", ATK_MD0280_LCD_FONT_16, ATK_MD0280_GREEN);
        break;
    case 9:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "the Beginning of Summer", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BRRED);
        break;
    case 10:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "Lesser Fullness of Grain", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BRRED);
        break;
    case 11:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "Grain in Beard", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BRRED);
        break;
    case 12:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "the Summer Solstice", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BRRED);
        break;
    case 13:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "Lesser Heat", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BRRED);
        break;
    case 14:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "Greater Heat", ATK_MD0280_LCD_FONT_16, ATK_MD0280_BRRED);
        break;
    case 15:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "the Beginning of Autumn", ATK_MD0280_LCD_FONT_16, ATK_MD0280_YELLOW);
        break;
    case 16:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "the End of Heat", ATK_MD0280_LCD_FONT_16, ATK_MD0280_YELLOW);
        break;
    case 17:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "White Dew", ATK_MD0280_LCD_FONT_16, ATK_MD0280_YELLOW);
        break;
    case 18:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "the Autumn Equinox", ATK_MD0280_LCD_FONT_16, ATK_MD0280_YELLOW);
        break;
    case 19:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "Cold Dew", ATK_MD0280_LCD_FONT_16, ATK_MD0280_YELLOW);
        break;
    case 20:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "Frost's Descent", ATK_MD0280_LCD_FONT_16, ATK_MD0280_YELLOW);
        break;
    case 21:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "the Beginning of Winter", ATK_MD0280_LCD_FONT_16, ATK_MD0280_GBLUE);
        break;
    case 22:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "Lesser Snow", ATK_MD0280_LCD_FONT_16, ATK_MD0280_GBLUE);
        break;
    case 23:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "Greater Snow", ATK_MD0280_LCD_FONT_16, ATK_MD0280_GBLUE);
        break;
    case 24:
        atk_md0280_show_string(x - 40, y - 20, ATK_MD0280_LCD_WIDTH, 16, "the Winter Solstice", ATK_MD0280_LCD_FONT_16, ATK_MD0280_GBLUE);
        break;
    }
}

/*
以下函数的功能是绘制一个有厚度的正方形框
其中x1，y1为左上角坐标，x2，y2为右下角坐标
*/
void draw_rect_thick(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t thick)
{
    uint8_t i;
    for (i = 0; i < thick; i++)
    {
        atk_md0280_draw_rect(x1 + i, y1 + i, x2 - i, y2 - i, color);
    }
}

/*
以下函数绘制应用菜单logo，占用像素为50*50
*/
void draw_menu_logo(uint16_t x, uint16_t y)
{
    draw_rect_thick(x, y, x + 50, y + 50, ATK_MD0280_WHITE, 6);
    atk_md0280_fill(x + 13, y + 13, x + 21, y + 21, ATK_MD0280_YELLOW);
    atk_md0280_fill(x + 29, y + 29, x + 37, y + 37, ATK_MD0280_GREEN);
    atk_md0280_fill(x + 13, y + 29, x + 21, y + 37, ATK_MD0280_BLUE);
    atk_md0280_fill(x + 29, y + 13, x + 37, y + 21, ATK_MD0280_RED);
}
/*
以下函数的功能是在屏幕上显示主界面
*/
void display_main(void)
{
    static uint16_t year_tmp, month_tmp, day_tmp, hour_tmp, minute_tmp, second_tmp;

    // 如果日期发生变化，清除原来的日期
    if (year_tmp != year || month_tmp != month || day_tmp != day)
    {
        atk_md0280_fill(20, 50, 160, 158, ATK_MD0280_BLACK);
        year_tmp = year;
        month_tmp = month;
        day_tmp = day;
    }
    // 如果小时/分钟发生变化，清除原来的小时分钟
    if (hour_tmp != hour || minute_tmp != minute)
    {
        atk_md0280_fill(71, 165, 167, 189, ATK_MD0280_BLACK);
        hour_tmp = hour;
        minute_tmp = minute;
    }
    // 如果秒发生变化，清除原来的秒
    if (second_tmp != second)
    {
        atk_md0280_fill(144, 165, 167, 189, ATK_MD0280_BLACK);
        second_tmp = second;
    }

    // 在屏幕上显示日期
    draw_date_bold(86, 100);

    // 在左下角显示应用菜单按钮，右下角显示设置按钮
    draw_menu_logo(2, 267);
    atk_md0280_show_pic(187, 267, 50, 50, (uint8_t *)setting_logo);

    // 在屏幕上显示时间
    atk_md0280_show_xnum(71, 165, hour, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_24, ATK_MD0280_RED);
    atk_md0280_show_char(71 + 2 * 12, 165, ':', ATK_MD0280_LCD_FONT_24, ATK_MD0280_RED);
    atk_md0280_show_xnum(71 + 3 * 12, 165, minute, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_24, ATK_MD0280_RED);
    atk_md0280_show_char(71 + 5 * 12, 165, ':', ATK_MD0280_LCD_FONT_24, ATK_MD0280_RED);
    atk_md0280_show_xnum(71 + 6 * 12, 165, second, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_24, ATK_MD0280_RED);
}

/*
以下函数的功能是在屏幕上显示应用菜单界面
*/
void display_menu(void)
{
    atk_md0280_show_pic(5, 5, 50, 40, (uint8_t *)back_logo);
    draw_menu_logo(95, 60);
    // 在屏幕上显示DHT11功能按钮
    atk_md0280_draw_rect(20, 130, 219, 170, ATK_MD0280_WHITE);
    atk_md0280_show_string(30, 135, ATK_MD0280_LCD_WIDTH, 32, "Temp/Humid", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 在屏幕上显示WIFI模块功能按钮
    atk_md0280_draw_rect(20, 180, 219, 220, ATK_MD0280_WHITE);
    atk_md0280_show_string(30, 185, ATK_MD0280_LCD_WIDTH, 32, "Weather", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 在屏幕上显示核心算法功能按钮
    atk_md0280_draw_rect(20, 230, 219, 270, ATK_MD0280_WHITE);
    atk_md0280_show_string(30, 235, ATK_MD0280_LCD_WIDTH, 32, "Radar Image", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
}

/*
以下函数的功能是在屏幕上显示DHT11界面
*/
void display_DHT11(void)
{
    // 定义DHT11的温湿度变量
    static uint8_t temperature, humidity;
    static uint8_t temperature_tmp, humidity_tmp;
    atk_md0280_show_pic(5, 5, 50, 40, (uint8_t *)back_logo);                                                      // 左上角返回按钮
    atk_md0280_show_string(20, 130, ATK_MD0280_LCD_WIDTH, 32, "temp:", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE); // 在屏幕上显示温度
    atk_md0280_show_string(20, 180, ATK_MD0280_LCD_WIDTH, 32, "humi:", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE); // 在屏幕上显示湿度
    delay_ms(100);
    DHT11_Read_Data(&temperature, &humidity);
    if (temperature_tmp != temperature || humidity_tmp != humidity)
    {
        atk_md0280_fill(100, 130, 132, 212, ATK_MD0280_BLACK);
        temperature_tmp = temperature;
        humidity_tmp = humidity;
    }
    atk_md0280_show_xnum(100, 130, temperature_tmp, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    atk_md0280_show_xnum(100, 180, humidity_tmp, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
}

/*
以下函数的功能是在屏幕上显示核心算法得出来的深度图像
*/
void display_CORE(void)
{
    // process_CSA1();//跑一遍核心算法
    // 核心算法并不是在这里跑，因为只需要跑一次
    atk_md0280_show_pic(5, 5, 50, 40, (uint8_t *)back_logo); // 左上角返回按钮
    static uint8_t t;
    if (t == 0)
    {
        process_CSA1();
        t++;
    }
    // 此时跑完后，echo_use[16][512]中存储的就是计算完成的结果
    // todo：把echo_use显示出来
    atk_md0280_show_core(70, 30);
}

/*
以下函数的功能是在屏幕上显示设置界面
*/
void display_setting(void)
{
    atk_md0280_show_pic(5, 5, 50, 40, (uint8_t *)back_logo);      // 左上角返回按钮
    atk_md0280_show_pic(95, 60, 50, 50, (uint8_t *)setting_logo); // 中上方设置按钮
    // 在屏幕上显示设置日期按钮
    atk_md0280_draw_rect(20, 130, 110, 270, ATK_MD0280_WHITE);
    atk_md0280_show_string(35, 175, ATK_MD0280_LCD_WIDTH, 32, "date", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 在屏幕上显示设置时间按钮
    atk_md0280_draw_rect(129, 130, 219, 270, ATK_MD0280_WHITE);
    atk_md0280_show_string(144, 175, ATK_MD0280_LCD_WIDTH, 32, "time", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
}

/*
以下函数的功能是在屏幕上显示设置年份界面
*/
void display_setting_year(void)
{
    static uint16_t year_set_tmp;

    // 左上角返回按钮
    atk_md0280_show_pic(5, 5, 50, 40, (uint8_t *)back_logo);

    // 如果日期发生变化，清除原来的日期
    if (year_set_tmp != year_set)
    {
        atk_md0280_fill(88, 145, 152, 177, ATK_MD0280_BLACK);
        year_set_tmp = year_set;
    }
    // 切换成月份设置界面
    atk_md0280_draw_rect(179, 80, 229, 180, ATK_MD0280_WHITE);
    atk_md0280_show_string(189, 85, ATK_MD0280_LCD_WIDTH, 16, "month", ATK_MD0280_LCD_FONT_16, ATK_MD0280_WHITE);
    // 年份-1
    atk_md0280_draw_rect(20, 200, 110, 270, ATK_MD0280_WHITE);
    atk_md0280_show_char(55, 220, '-', ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 年份+1
    atk_md0280_draw_rect(130, 200, 220, 270, ATK_MD0280_WHITE);
    atk_md0280_show_char(165, 220, '+', ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 确认按钮
    atk_md0280_draw_rect(20, 270, 220, 310, ATK_MD0280_WHITE);
    atk_md0280_show_string(100, 275, ATK_MD0280_LCD_WIDTH, 32, "OK", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 显示进入设置年份界面一瞬间的年份
    atk_md0280_show_xnum(88, 145, year_set, 4, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
}

/*
以下函数的功能是在屏幕上显示设置月份界面
*/
void display_setting_month(void)
{
    static uint16_t month_set_tmp;

    // 左上角返回按钮
    atk_md0280_show_pic(5, 5, 50, 40, (uint8_t *)back_logo);

    // 如果日期发生变化，清除原来的日期
    if (month_set_tmp != month_set)
    {
        atk_md0280_fill(88, 145, 152, 177, ATK_MD0280_BLACK);
        month_set_tmp = month_set;
    }
    // 切换成年份设置界面
    atk_md0280_draw_rect(10, 80, 60, 180, ATK_MD0280_WHITE);
    atk_md0280_show_string(20, 85, ATK_MD0280_LCD_WIDTH, 16, "year", ATK_MD0280_LCD_FONT_16, ATK_MD0280_WHITE);
    // 切换成日期设置界面
    atk_md0280_draw_rect(179, 80, 229, 180, ATK_MD0280_WHITE);
    atk_md0280_show_string(189, 85, ATK_MD0280_LCD_WIDTH, 16, "day", ATK_MD0280_LCD_FONT_16, ATK_MD0280_WHITE);
    // 月份-1
    atk_md0280_draw_rect(20, 200, 110, 270, ATK_MD0280_WHITE);
    atk_md0280_show_char(55, 220, '-', ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 月份+1
    atk_md0280_draw_rect(130, 200, 220, 270, ATK_MD0280_WHITE);
    atk_md0280_show_char(165, 220, '+', ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 确认按钮
    atk_md0280_draw_rect(20, 270, 220, 310, ATK_MD0280_WHITE);
    atk_md0280_show_string(100, 275, ATK_MD0280_LCD_WIDTH, 32, "OK", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 显示进入设置月份界面一瞬间的月份
    atk_md0280_show_xnum(104, 145, month_set, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
}

/*
以下函数的功能是在屏幕上显示设置日期界面
*/
void display_setting_day(void)
{
    static uint16_t day_set_tmp;

    // 左上角返回按钮
    atk_md0280_show_pic(5, 5, 50, 40, (uint8_t *)back_logo);

    // 如果日期发生变化，清除原来的日期
    if (day_set_tmp != day_set)
    {
        atk_md0280_fill(88, 145, 152, 177, ATK_MD0280_BLACK);
        day_set_tmp = day_set;
    }
    // 切换成月份设置界面
    atk_md0280_draw_rect(10, 80, 60, 180, ATK_MD0280_WHITE);
    atk_md0280_show_string(20, 85, ATK_MD0280_LCD_WIDTH, 16, "month", ATK_MD0280_LCD_FONT_16, ATK_MD0280_WHITE);
    // 日期-1
    atk_md0280_draw_rect(20, 200, 110, 270, ATK_MD0280_WHITE);
    atk_md0280_show_char(55, 220, '-', ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 日期+1
    atk_md0280_draw_rect(130, 200, 220, 270, ATK_MD0280_WHITE);
    atk_md0280_show_char(165, 220, '+', ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 确认按钮
    atk_md0280_draw_rect(20, 270, 220, 310, ATK_MD0280_WHITE);
    atk_md0280_show_string(100, 275, ATK_MD0280_LCD_WIDTH, 32, "OK", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 显示进入设置日期界面一瞬间的日期
    atk_md0280_show_xnum(104, 145, day_set, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
}

/*
以下函数的功能是在屏幕上显示设置小时界面
*/
void display_setting_hour(void)
{
    static uint16_t hour_set_tmp;

    // 左上角返回按钮
    atk_md0280_show_pic(5, 5, 50, 40, (uint8_t *)back_logo);

    // 如果小时发生变化，清除原来的小时
    if (hour_set_tmp != hour_set)
    {
        atk_md0280_fill(88, 145, 152, 177, ATK_MD0280_BLACK);
        hour_set_tmp = hour_set;
    }
    // 切换成分钟设置界面
    atk_md0280_draw_rect(179, 80, 229, 180, ATK_MD0280_WHITE);
    atk_md0280_show_string(189, 85, ATK_MD0280_LCD_WIDTH, 16, "minute", ATK_MD0280_LCD_FONT_16, ATK_MD0280_WHITE);
    // 小时-1
    atk_md0280_draw_rect(20, 200, 110, 270, ATK_MD0280_WHITE);
    atk_md0280_show_char(55, 220, '-', ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 小时+1
    atk_md0280_draw_rect(130, 200, 220, 270, ATK_MD0280_WHITE);
    atk_md0280_show_char(165, 220, '+', ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 确认按钮
    atk_md0280_draw_rect(20, 270, 220, 310, ATK_MD0280_WHITE);
    atk_md0280_show_string(100, 275, ATK_MD0280_LCD_WIDTH, 32, "OK", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 显示进入设置小时界面一瞬间的小时
    atk_md0280_show_xnum(104, 145, hour_set, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
}

/*
以下函数的功能是在屏幕上显示设置分钟界面
*/
void display_setting_minute(void)
{
    static uint16_t minute_set_tmp;

    // 左上角返回按钮
    atk_md0280_show_pic(5, 5, 50, 40, (uint8_t *)back_logo);

    // 如果分钟发生变化，清除原来的分钟
    if (minute_set_tmp != minute_set)
    {
        atk_md0280_fill(88, 145, 152, 177, ATK_MD0280_BLACK);
        minute_set_tmp = minute_set;
    }
    // 切换成秒设置界面
    atk_md0280_draw_rect(179, 80, 229, 180, ATK_MD0280_WHITE);
    atk_md0280_show_string(189, 85, ATK_MD0280_LCD_WIDTH, 16, "second", ATK_MD0280_LCD_FONT_16, ATK_MD0280_WHITE);
    // 切换成小时设置界面
    atk_md0280_draw_rect(10, 80, 60, 180, ATK_MD0280_WHITE);
    atk_md0280_show_string(20, 85, ATK_MD0280_LCD_WIDTH, 16, "hour", ATK_MD0280_LCD_FONT_16, ATK_MD0280_WHITE);
    // 分钟-1
    atk_md0280_draw_rect(20, 200, 110, 270, ATK_MD0280_WHITE);
    atk_md0280_show_char(55, 220, '-', ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 分钟+1
    atk_md0280_draw_rect(130, 200, 220, 270, ATK_MD0280_WHITE);
    atk_md0280_show_char(165, 220, '+', ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 确认按钮
    atk_md0280_draw_rect(20, 270, 220, 310, ATK_MD0280_WHITE);
    atk_md0280_show_string(100, 275, ATK_MD0280_LCD_WIDTH, 32, "OK", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 显示进入设置分钟界面一瞬间的分钟
    atk_md0280_show_xnum(104, 145, minute_set, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
}

/*
以下函数的功能是在屏幕上显示设置秒界面
*/
void display_setting_second(void)
{
    static uint16_t second_set_tmp;

    // 左上角返回按钮
    atk_md0280_show_pic(5, 5, 50, 40, (uint8_t *)back_logo);

    // 如果秒发生变化，清除原来的秒
    if (second_set_tmp != second_set)
    {
        atk_md0280_fill(88, 145, 152, 177, ATK_MD0280_BLACK);
        second_set_tmp = second_set;
    }
    // 切换成分钟设置界面
    atk_md0280_draw_rect(10, 80, 60, 180, ATK_MD0280_WHITE);
    atk_md0280_show_string(20, 85, ATK_MD0280_LCD_WIDTH, 16, "minute", ATK_MD0280_LCD_FONT_16, ATK_MD0280_WHITE);
    // 秒-1
    atk_md0280_draw_rect(20, 200, 110, 270, ATK_MD0280_WHITE);
    atk_md0280_show_char(55, 220, '-', ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 秒+1
    atk_md0280_draw_rect(130, 200, 220, 270, ATK_MD0280_WHITE);
    atk_md0280_show_char(165, 220, '+', ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 确认按钮
    atk_md0280_draw_rect(20, 270, 220, 310, ATK_MD0280_WHITE);
    atk_md0280_show_string(100, 275, ATK_MD0280_LCD_WIDTH, 32, "OK", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
    // 显示进入设置秒界面一瞬间的秒
    atk_md0280_show_xnum(104, 145, second_set, 2, ATK_MD0280_NUM_SHOW_ZERO, ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);
}
