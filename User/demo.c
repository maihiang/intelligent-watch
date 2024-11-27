
#include "demo.h"
#include "./BSP/ATK_MD0280/atk_md0280.h"
#include "./BSP/led/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include <math.h>
#include "displaymode.h"

#define PI (float)(3.1415926)

/*
模式相关全局变量
mode可取0-5
0代表主菜单，1代表显示1，2代表显示2，etc
*/
int mode = 0;
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
                mode = 1;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            /*右下角的按钮，按下去后进入设置界面*/
            if ((x_scan >= 189) && (x_scan <= 239) && (y_scan >= 269) && (y_scan <= 319))
            {
                mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }

        //应用菜单的操作
        if(mode == 1)
        {
            if ((x_scan >= 0)&&(x_scan <= 60)&&(y_scan >= 0)&&(y_scan <= 50))
            {
                mode = 0;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }
        //设置界面的操作
        if(mode == 2)
        {
            //返回按钮相应
            if ((x_scan >= 0)&&(x_scan <= 60)&&(y_scan >= 0)&&(y_scan <= 50))
            {
                mode = 0;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
            //日期设置按钮相应
            if ((x_scan >= 20)&&(x_scan <= 110)&&(y_scan >= 130)&&(y_scan <= 270))
            {
                mode = 21;
                year_set = year;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }
        //设置日期界面的操作
        if(mode == 21)
        {
            //返回按钮相应
            if ((x_scan >= 0)&&(x_scan <= 60)&&(y_scan >= 0)&&(y_scan <= 50))
            {
                mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }

            //年份-1
            if ((x_scan >= 20)&&(x_scan <= 110)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                year_set -= 1;
            }

            //年份+1
            if ((x_scan >= 130)&&(x_scan <= 220)&&(y_scan >= 200)&&(y_scan <= 270))
            {
                year_set += 1;
            }

            //年份确认
            if ((x_scan >= 20)&&(x_scan <= 220)&&(y_scan >= 270)&&(y_scan <= 310))
            {
                year = year_set;
                mode = 2;
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }
    }

    //以下根据mode的值进行显示，每10ms刷新一次，显示的函数写在了displaymode.c中
    switch(mode)
    {
        case 0:
            display_main();
            break;
        
        case 1:
            display_menu(); 
            break;

        case 2:
            display_setting();
            break;
        
        case 21:
            display_setting_date();
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
    year = 2024, month = 12, day = 31, hour = 23, minute = 59, second = 55;

    while (1)
    {
        /* 进行页面刷新显示，显示完成后延时0.1秒，预计每秒刷新10次 */
        demo_show_ui();
        delay_ms(100);
    }
}
