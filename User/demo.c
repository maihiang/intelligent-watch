
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
uint16_t year = 2024, month = 11, day = 25, hour = 11, minute = 45, second = 14;

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
    if (atk_md0280_touch_scan(&x_scan, &y_scan) == 0)//在该周期内，触摸屏有触摸，进行相应操作
    {
        if(mode == 0)
        {
            if ((x_scan >= 0) && (x_scan <= 109) && (y_scan >= 0 ) && (y_scan <= 149))
            {
                mode = 1;
                atk_md0280_clear(ATK_MD0280_WHITE);
            }

            if ((x_scan >= 130) && (x_scan <= 239) && (y_scan >= 0 ) && (y_scan <= 149))
            {
                mode = 2;
                atk_md0280_clear(ATK_MD0280_WHITE);
            }

            if ((x_scan >= 0) && (x_scan <= 109) && (y_scan >= 170 ) && (y_scan <= 319))
            {
                mode = 3;
                atk_md0280_clear(ATK_MD0280_WHITE);
            }

            if ((x_scan >= 130) && (x_scan <= 239) && (y_scan >= 170 ) && (y_scan <= 319))
            {
                mode = 4;
                atk_md0280_clear(ATK_MD0280_WHITE);
            }
        }

        if(mode == 1 || mode == 2 || mode == 3 || mode == 4)
        {
            if ((x_scan >= 10)&&(x_scan <= 40)&&(y_scan >= 10)&&(y_scan <= 30))
            {
                mode = 0;
                atk_md0280_clear(ATK_MD0280_WHITE);
            }

            if ((x_scan >= 1)&&(x_scan <= 32)&&(y_scan >= 152)&&(y_scan <= 167))
            {
                mode--;
                mode=mode?mode:4;  
                atk_md0280_clear(ATK_MD0280_WHITE);
            }
            
            if ((x_scan >= 207)&&(x_scan <= 238)&&(y_scan >= 152)&&(y_scan <= 167))
            {
                mode=(mode+1)%4;
                mode=mode?mode:4;
                atk_md0280_clear(ATK_MD0280_WHITE);
            }

        }
        
    }

    //以下根据mode的值进行显示，每10ms刷新一次
    switch(mode)
    {
        case 0:
            display_mode_0();
            break;
        
        case 1:
            display_mode_1();
            break;

        case 2:
            display_mode_2();
            break;
            
        case 3:
            display_mode_3();
            break;

        case 4:
            display_mode_4();
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
    atk_md0280_clear(ATK_MD0280_WHITE);

    /* 初始化日期 */
    year = 2024, month = 11, day = 25, hour = 11, minute = 45, second = 14;
    
    while (1)
    {
        /* 演示立方体3D旋转 */
        demo_show_ui();
        delay_ms(100);
    }
}
