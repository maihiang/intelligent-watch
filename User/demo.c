
#include "demo.h"
#include "./BSP/ATK_MD0280/atk_md0280.h"
#include "./BSP/led/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include <math.h>

#define PI (float)(3.1415926)

/*
模式相关全局变量
mode可取0-5
0代表主菜单，1代表显示1，2代表显示2，etc
*/
int mode = 0;

/**
 * @brief       演示立方体3D旋转
 * @param       无
 * @retval      无
 */
static void demo_show_cube(void)
{
    uint16_t x_scan;
    uint16_t y_scan;

    //以下进行扫描，并在合适的时候进行模式切换
    if (atk_md0280_touch_scan(&x_scan, &y_scan) == 0)
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
                atk_md0280_clear(ATK_MD0280_BLACK);
            }
        }
        
    }

    switch(mode)
    {
        case 0:
            atk_md0280_fill(0,0,109,149,ATK_MD0280_WHITE);
            atk_md0280_fill(130,0,239,149,ATK_MD0280_WHITE);
            atk_md0280_fill(0,170,109,319,ATK_MD0280_WHITE);
            atk_md0280_fill(130,170,239,319,ATK_MD0280_WHITE);
            break;
        
        case 1:
            atk_md0280_show_string(120, 160, ATK_MD0280_LCD_WIDTH, 32, "1", ATK_MD0280_LCD_FONT_32, ATK_MD0280_RED);
            atk_md0280_fill(10,10,40,30,ATK_MD0280_BLACK);
            atk_md0280_show_string(12, 15, ATK_MD0280_LCD_WIDTH, 12, "BACK", ATK_MD0280_LCD_FONT_12, ATK_MD0280_WHITE);
            break;

        case 2:
            atk_md0280_show_string(120, 160, ATK_MD0280_LCD_WIDTH, 32, "2", ATK_MD0280_LCD_FONT_32, ATK_MD0280_RED);
            atk_md0280_fill(10,10,40,30,ATK_MD0280_BLACK);
            atk_md0280_show_string(12, 15, ATK_MD0280_LCD_WIDTH, 12, "BACK", ATK_MD0280_LCD_FONT_12, ATK_MD0280_WHITE);
            break;
            
        case 3:
            atk_md0280_show_string(120, 160, ATK_MD0280_LCD_WIDTH, 32, "3", ATK_MD0280_LCD_FONT_32, ATK_MD0280_RED);
            atk_md0280_fill(10,10,40,30,ATK_MD0280_BLACK);
            atk_md0280_show_string(12, 15, ATK_MD0280_LCD_WIDTH, 12, "BACK", ATK_MD0280_LCD_FONT_12, ATK_MD0280_WHITE);
            break;

        case 4:
            atk_md0280_show_string(120, 160, ATK_MD0280_LCD_WIDTH, 32, "4", ATK_MD0280_LCD_FONT_32, ATK_MD0280_RED);
            atk_md0280_fill(10,10,40,30,ATK_MD0280_BLACK);
            atk_md0280_show_string(12, 15, ATK_MD0280_LCD_WIDTH, 12, "BACK", ATK_MD0280_LCD_FONT_12, ATK_MD0280_WHITE);
            break;
    }

    
    delay_ms(100);
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
    
    while (1)
    {
        /* 演示立方体3D旋转 */
        demo_show_cube();
    }
}
