
#include "demo.h"
#include "./BSP/ATK_MD0280/atk_md0280.h"
#include "./BSP/led/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include <math.h>
#include "displaymode.h"
#include "./SYSTEM/date/date.h"

#define PI (float)(3.1415926)

/*
模式相关全局变量
mode可取0-5
0代表主菜单，1代表显示1，2代表显示2，etc
*/
int mode = 0;
int next_mode = 0;
//定义全局变量，表示日期时间
uint16_t year = 2024, month = 12, day = 25, hour = 23, minute = 59, second = 50;
//定义全局变量，表示设置界面中的日期和时间
uint16_t year_set, month_set, day_set, hour_set, minute_set, second_set;

/**
 * @brief       演示各个界面
 * @param       无
 * @retval      无
 */
static void demo_show_ui(void)
{
    uint16_t x_scan;
    uint16_t y_scan;

    //以下进行扫描，并在合适的时候进行模式切换
    //在该周期内，触摸屏有触摸，进行相应操作，否则mode不变；同时在切换mode时清屏
    if (atk_md0280_touch_scan(&x_scan, &y_scan) == 0)
    {
        //主界面的操作
        if(mode == 0)
        {
            /*左下角的按钮，按下去后进入选择各个功能的界面*/
            if ((x_scan >= 0) && (x_scan <= 50) && (y_scan >= 269) && (y_scan <= 319))
            {
                next_mode = 1;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            /*右下角的按钮，按下去后进入设置界面*/
            if ((x_scan >= 189) && (x_scan <= 239) && (y_scan >= 269) && (y_scan <= 319))
            {
                next_mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }

        //应用菜单的操作
        if(mode == 1)
        {
            //返回按钮相应
            if ((x_scan >= 0)&&(x_scan <= 60)&&(y_scan >= 0)&&(y_scan <= 50))
            {
                next_mode = 0;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
            //DHT11按钮相应
            if ((x_scan >= 20)&&(x_scan <= 110)&&(y_scan >= 130)&&(y_scan <= 270))
            {
                next_mode = 11;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }
        
        //应用1：DHT11温湿度传感器
        if(mode == 11)
        {
            //返回按钮相应
            if ((x_scan >= 0)&&(x_scan <= 60)&&(y_scan >= 0)&&(y_scan <= 50))
            {
                next_mode = 1;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }

        //设置界面的操作
        if(mode == 2)
        {
            //返回按钮相应
            if ((x_scan >= 0)&&(x_scan <= 60)&&(y_scan >= 0)&&(y_scan <= 50))
            {
                next_mode = 0;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
            //日期设置按钮相应
            if ((x_scan >= 20)&&(x_scan <= 110)&&(y_scan >= 130)&&(y_scan <= 270))
            {
                next_mode = 21;
                year_set = year;
                month_set = month;
                day_set = day;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
            //时间设置按钮相应
            if ((x_scan >= 129)&&(x_scan <= 219)&&(y_scan >= 130)&&(y_scan <= 270))
            {
                next_mode = 24;
                hour_set = hour;
                minute_set = minute;
                second_set = second;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }

        //设置年份界面的操作
        if(mode == 21)
        {
            //返回按钮相应
            if ((x_scan >= 0)&&(x_scan <= 60)&&(y_scan >= 0)&&(y_scan <= 50))
            {
                next_mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //年份确认
            if ((x_scan >= 20)&&(x_scan <= 220)&&(y_scan >= 270)&&(y_scan <= 310))
            {
                year = year_set;
                next_mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //年份-1
            if ((x_scan >= 20)&&(x_scan <= 110)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                if(year_set > 1980)
                {
                    year_set -= 1;
                }
            }

            //年份+1
            if ((x_scan >= 130)&&(x_scan <= 220)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                if(year_set < 9999)
                {
                    year_set += 1;
                }
            }

            //切换成月份设置界面
            if ((x_scan >= 179)&&(x_scan <= 239)&&(y_scan >= 80)&&(y_scan <= 180))
            {
                next_mode = 22;
                month_set = month;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }

        //设置月份界面的操作
        if(mode == 22)
        {
            //返回按钮相应
            if ((x_scan >= 0)&&(x_scan <= 60)&&(y_scan >= 0)&&(y_scan <= 50))
            {
                next_mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //月份确认
            if ((x_scan >= 20)&&(x_scan <= 220)&&(y_scan >= 270)&&(y_scan <= 310))
            {
                month = month_set;
                next_mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //月份-1
            if ((x_scan >= 20)&&(x_scan <= 110)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                month_set -= 1;
                if(month_set <= 0)
                {
                    month_set = 12;
                }
            }

            //月份+1
            if ((x_scan >= 130)&&(x_scan <= 220)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                month_set += 1;
                if(month_set >= 13)
                {
                    month_set = 1;
                }
            }

            //切换成日期设置界面
            if ((x_scan >= 179)&&(x_scan <= 239)&&(y_scan >= 80)&&(y_scan <= 180))
            {
                next_mode = 23;
                day_set = day;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //切换成年份设置界面
            if ((x_scan >= 0)&&(x_scan <= 60)&&(y_scan >= 80)&&(y_scan <= 180))
            {
                next_mode = 21;
                year_set = year;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }

        //设置日期界面的操作
        if(mode == 23)
        {
            //返回按钮相应
            if ((x_scan >= 0)&&(x_scan <= 60)&&(y_scan >= 0)&&(y_scan <= 50))
            {
                next_mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //日期确认
            if ((x_scan >= 20)&&(x_scan <= 220)&&(y_scan >= 270)&&(y_scan <= 310))
            {
                day = day_set;
                next_mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //日期-1
            if ((x_scan >= 20)&&(x_scan <= 110)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                day_set -= 1;
                if (day_set <= 0)
                {
                    day_set = daysInMonth(year, month);
                }
            }

            //日期+1
            if ((x_scan >= 130)&&(x_scan <= 220)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                day_set += 1;
                if (day_set > daysInMonth(year, month))
                {
                    day_set = 1;
                }
            }

            //切换成月份设置界面
            if ((x_scan >= 0)&&(x_scan <= 60)&&(y_scan >= 80)&&(y_scan <= 180))
            {
                next_mode = 22;
                month_set = month;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }
    
        //设置小时界面的操作
        if(mode == 24)
        {
            //返回按钮相应
            if ((x_scan >= 0)&&(x_scan <= 50)&&(y_scan >= 0)&&(y_scan <= 50))
            {
                next_mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //时间确认
            if ((x_scan >= 20)&&(x_scan <= 220)&&(y_scan >= 270)&&(y_scan <= 310))
            {
                hour = hour_set;
                next_mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //小时-1
            if ((x_scan >= 20)&&(x_scan <= 110)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                if(hour_set <= 0 || hour_set > 23)
                {
                    hour_set = 23;
                }
                else
                {
                    hour_set -= 1;
                }
            }

            //小时+1
            if ((x_scan >= 130)&&(x_scan <= 220)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                hour_set += 1;
                if (hour_set > 23)
                {
                    hour_set = 0;
                }
            }

            //切换成分钟设置界面
            if ((x_scan >= 179)&&(x_scan <= 239)&&(y_scan >= 80)&&(y_scan <= 180))
            {
                next_mode = 25;
                minute_set = minute;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }

        //设置分钟界面的操作
        if(mode == 25)
        {
            //返回按钮相应
            if ((x_scan >= 0)&&(x_scan <= 60)&&(y_scan >= 0)&&(y_scan <= 50))
            {
                next_mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //分钟确认
            if ((x_scan >= 20)&&(x_scan <= 220)&&(y_scan >= 270)&&(y_scan <= 310))
            {
                minute = minute_set;
                next_mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //分钟-1
            if ((x_scan >= 20)&&(x_scan <= 110)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                if (minute_set <= 0 || minute_set > 59)
                {
                    minute_set = 59;
                }
                else
                {
                    minute_set -= 1;
                }
            }

            //分钟+1
            if ((x_scan >= 130)&&(x_scan <= 220)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                minute_set += 1;
                if (minute_set > 59)
                {
                    minute_set = 0;
                }
            }

            //切换成小时设置界面
            if ((x_scan >= 0)&&(x_scan <= 50)&&(y_scan >= 80)&&(y_scan <= 180))
            {
                next_mode = 24;
                hour_set = hour;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //切换成秒设置界面
            if ((x_scan >= 189)&&(x_scan <= 239)&&(y_scan >= 80)&&(y_scan <= 180))
            {
                next_mode = 26;
                second_set = second;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }

        //设置秒界面的操作
        if(mode == 26)
        {
            //返回按钮相应
            if ((x_scan >= 0)&&(x_scan <= 60)&&(y_scan >= 0)&&(y_scan <= 50))
            {
                next_mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //秒确认
            if ((x_scan >= 20)&&(x_scan <= 220)&&(y_scan >= 270)&&(y_scan <= 310))
            {
                second = second_set;
                next_mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //秒-1
            if ((x_scan >= 20)&&(x_scan <= 110)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                if (second_set <= 0 || second_set > 59)
                {
                    second_set = 59;
                }
                else
                {
                    second_set -= 1;
                }
            }

            //秒+1
            if ((x_scan >= 130)&&(x_scan <= 220)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                second_set += 1;
                if (second_set > 59)
                {
                    second_set = 0;
                }
            }

            //切换成分钟设置界面
            if ((x_scan >= 0)&&(x_scan <= 50)&&(y_scan >= 80)&&(y_scan <= 180))
            {
                next_mode = 25;
                minute_set = minute;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }
    }

    mode = next_mode;

    //以下根据mode的值进行显示，每10ms刷新一次，显示的函数写在了displaymode.c中
    switch(mode)
    {
        //显示主界面
        case 0:
            display_main();
            break;
        
        //显示应用菜单
        case 1:
            display_menu(); 
            break;

        //显示DHT11界面
        case 11:
            display_DHT11();
            break;

        //显示设置界面
        case 2:
            display_setting();
            break;
        
        //显示设置年份界面
        case 21:
            display_setting_year();
            break;
        
        //显示设置月份界面
        case 22:
            display_setting_month();
            break;
        
        //显示设置日期界面
        case 23:
            display_setting_day();
            break;
        
        //显示设置小时界面
        case 24:
            display_setting_hour();
            break;
        
        //显示设置分钟界面
        case 25:
            display_setting_minute();
            break;
        
        //显示设置秒界面
        case 26:
            display_setting_second();
            break;
            
        case 3:
            break;

        case 4:
            break;
    }
}

/**
 * @brief       例程演示入口函数
 * @param       无
 * @retval      无
 */
void demo_run(void)
{
    uint8_t ret;
    
    /* 初始化ATK-MD0280模块 */
    ret = atk_md0280_init();
    if (ret != 0)
    {
        printf("ATK-MD0280 init failed!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
    
    /* ATK-MD0280模块LCD清屏 */
    atk_md0280_clear(ATK_MD0280_BLACK);

    /* 初始化日期 */
    year = 2024, month = 12, day = 31, hour = 23, minute = 59, second = 40;

    while (1)
    {
        /* 进行页面刷新显示，显示完成后延时0.1秒，预计每秒刷新10次 */
        demo_show_ui();
        delay_ms(100);
    }
}
