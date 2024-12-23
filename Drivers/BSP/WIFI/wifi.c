#include "wifi.h"
#include "img.h"
#include "./BSP/ATK_MD0280/atk_md0280.h"
#include "./BSP/ATK_MW8266D/atk_mw8266d.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

// 定义WIFI的SSID，密码，TCP服务器的IP和端口
#define DEMO_WIFI_SSID "MH160"
#define DEMO_WIFI_PWD "88888898"
#define DEMO_TCP_SERVER_IP "172.20.10.5"
#define DEMO_TCP_SERVER_PORT "8008"

// 声明display_WIFI函数中需要的函数
static void demo_show_ip(char *buf);
static void demo_key0_fun(uint8_t is_unvarnished);
static void demo_key1_fun(uint8_t *is_unvarnished);
static void demo_upload_data(uint8_t is_unvarnished);

// 定义静态全局变量，标志是否已经连接WIFI
static int t = 0;
// 定义天气的状态
static int weather = 0;
/*
以下函数的功能是在屏幕上显示WIFI界面
*/
void display_WIFI(void)
{
    static uint8_t ret;
    static char ip_buf[16];
    static uint8_t key;
    static uint8_t is_unvarnished;
    if (t == 0)
    {
        is_unvarnished = 0;
        /* 初始化ATK-MW8266D模块 */
        ret = atk_mw8266d_init(115200);
        if (ret != 0)
        {
            printf("ATK-MW8266D init failed!\r\n");
            while (1)
            {
                LED0_TOGGLE();
                delay_ms(200);
            }
        }

        /* 连接WIFI */
        printf("Joining to AP...\r\n");
        ret = atk_mw8266d_restore();                               /* 恢复出厂设置 */
        delay_ms(1000);
        ret += atk_mw8266d_at_test();                              /* AT测试 */
        delay_ms(1000);
        ret += atk_mw8266d_set_mode(1);                            /* station模式 */
        delay_ms(1000);
        ret += atk_mw8266d_sw_reset();                             /* 软件复位 */
        delay_ms(1000);
        ret += atk_mw8266d_ate_config(0);                          /* 关闭回显功能 */
        delay_ms(1000);
        ret += atk_mw8266d_join_ap(DEMO_WIFI_SSID, DEMO_WIFI_PWD); /* 连接WIFI */
        delay_ms(1000);
        ret += atk_mw8266d_get_ip(ip_buf);                         /* 获取IP地址 */
        if (ret != 0)
        {
            printf("Error to join ap!\r\n");
            while (1)
            {
                atk_md0280_show_string(30, 130, ATK_MD0280_LCD_WIDTH, 24, "Fail to join ap", ATK_MD0280_LCD_FONT_24, ATK_MD0280_WHITE);
                LED0_TOGGLE();
                delay_ms(200);
            }
        }
        //ap连接成功
        // atk_md0280_show_string(30, 130, ATK_MD0280_LCD_WIDTH, 32, "2", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE); // 在屏幕上显示WIFI

        /* 连接TCP服务器 */
        ret = atk_mw8266d_connect_tcp_server(DEMO_TCP_SERVER_IP, DEMO_TCP_SERVER_PORT);
        if (ret != 0)
        {
            printf("Error to connect TCP server!\r\n");
            while (1)
            {
                atk_md0280_show_string(30, 130, ATK_MD0280_LCD_WIDTH, 24, "Fail to connect TCP server", ATK_MD0280_LCD_FONT_24, ATK_MD0280_WHITE);
                LED0_TOGGLE();
                delay_ms(200);
            }
        }
        // atk_md0280_show_string(40, 130, ATK_MD0280_LCD_WIDTH, 32, "3", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE); // 在屏幕上显示WIFI

        /* 重新开始接收新的一帧数据 */
        atk_mw8266d_uart_rx_restart();

        t++;
    }
    atk_md0280_show_pic(5, 5, 50, 40, (uint8_t *)back_logo_re);                                                           // 左上角返回按钮

    key = key_scan(0);

    switch (key)
    {
        case KEY0_PRES:
        {
            /* 功能测试 */
            demo_key0_fun(is_unvarnished);
            break;
        }
        case KEY1_PRES:
        {
            /* 透传模式切换 */
            demo_key1_fun(&is_unvarnished);
            break;
        }
        default:
        {
            break;
        }
    }

    /* 发送透传接收自TCP Server的数据到串口调试助手 */
    demo_upload_data(is_unvarnished);

    delay_ms(10);
}

// 下面定义display_WIFI函数中需要的函数
/**
 * @brief       按键0功能，功能测试
 * @param       is_unvarnished: 0，未进入透传
 *                              1，已进入透传
 * @retval      无
 */
static void demo_key0_fun(uint8_t is_unvarnished)
{
    uint8_t ret;

    if (is_unvarnished == 0)
    {
        /* 进行AT指令测试 */
        ret = atk_mw8266d_at_test();
        if (ret == 0)
        {
            printf("AT test success!\r\n");
        }
        else
        {
            printf("AT test failed!\r\n");
        }
    }
    else
    {
        /* 通过透传，发送信息至TCP Server */
        atk_mw8266d_uart_printf("This ATK-MW8266D TCP Connect Test.\r\n");
    }
}

/**
 * @brief       按键1功能，功能测试
 * @param       is_unvarnished: 0，未进入透传
 *                              1，已进入透传
 * @retval      无
 */
static void demo_key1_fun(uint8_t *is_unvarnished)
{
    uint8_t ret;

    if (*is_unvarnished == 0)
    {
        /* 进入透传 */
        ret = atk_mw8266d_enter_unvarnished();
        if (ret == 0)
        {
            *is_unvarnished = 1;
            printf("Enter unvarnished!\r\n");
        }
        else
        {
            printf("Enter unvarnished failed!\r\n");
        }
    }
    else
    {
        /* 退出透传 */
        atk_mw8266d_exit_unvarnished();
        *is_unvarnished = 0;
        printf("Exit unvarnished!\r\n");
    }
}

/**
 * @brief       进入透传时，将接收自TCP Server的数据发送到串口调试助手
 * @param       is_unvarnished: 0，未进入透传
 *                              1，已进入透传
 * @retval      无
 */
static void demo_upload_data(uint8_t is_unvarnished)
{
    uint8_t *buf;

    if (is_unvarnished == 1)
    {
        /* 接收来自ATK-MW8266D UART的一帧数据 */
        buf = atk_mw8266d_uart_rx_get_frame();
        if (buf != NULL)
        {
            printf("%s", buf);
            //如果buf收到的是"1"，则在屏幕上显示“rainy”
            if (buf[0] == '1')
            {
                if(weather != 1)
                {
                    atk_md0280_fill(20, 180, 220, 220, ATK_MD0280_BLACK);
                    weather = 1;
                    atk_md0280_show_string(20, 180, ATK_MD0280_LCD_WIDTH, 16, "rainy", ATK_MD0280_LCD_FONT_16, ATK_MD0280_MAGENTA);
                }
            }
            //如果buf收到的是"2"，则在屏幕上显示“sunny”
            else if (buf[0] == '2')
            {
                if(weather != 2)
                {
                    atk_md0280_fill(20, 180, 220, 220, ATK_MD0280_BLACK);
                    weather = 2;
                    atk_md0280_show_string(20, 180, ATK_MD0280_LCD_WIDTH, 16, "sunny", ATK_MD0280_LCD_FONT_16, ATK_MD0280_MAGENTA);
                }
            }
            //如果buf收到的是"3"，则在屏幕上显示“cloudy”
            else if (buf[0] == '3')
            {
                if(weather != 3)
                {
                    atk_md0280_fill(20, 180, 220, 220, ATK_MD0280_BLACK);
                    weather = 3;
                    atk_md0280_show_string(20, 180, ATK_MD0280_LCD_WIDTH, 16, "cloudy", ATK_MD0280_LCD_FONT_16, ATK_MD0280_MAGENTA);
                }
            }
            //如果buf收到的是"4"，则在屏幕上显示“snowy”
            else if (buf[0] == '4')
            {
                if(weather != 4)
                {
                    atk_md0280_fill(20, 180, 220, 220, ATK_MD0280_BLACK);
                    weather = 4;
                    atk_md0280_show_string(20, 180, ATK_MD0280_LCD_WIDTH, 16, "snowy", ATK_MD0280_LCD_FONT_16, ATK_MD0280_MAGENTA);
                }
            }
            //如果buf收到的是"5"，则在屏幕上显示“windy”
            else if (buf[0] == '5')
            {
                if(weather != 5)
                {
                    atk_md0280_fill(20, 180, 220, 220, ATK_MD0280_BLACK);
                    weather = 5;
                    atk_md0280_show_string(20, 180, ATK_MD0280_LCD_WIDTH, 16, "windy", ATK_MD0280_LCD_FONT_16, ATK_MD0280_MAGENTA);
                }
            }
            //如果buf收到的是"6"，则在屏幕上显示“foggy”
            else if (buf[0] == '6')
            {
                if(weather != 6)
                {
                    atk_md0280_fill(20, 180, 220, 220, ATK_MD0280_BLACK);
                    weather = 6;
                    atk_md0280_show_string(20, 180, ATK_MD0280_LCD_WIDTH, 16, "foggy", ATK_MD0280_LCD_FONT_16, ATK_MD0280_CYAN);
                }
            }
            //如果buf收到的是"7"，则在屏幕上显示“thunder”
            else if (buf[0] == '7')
            {
                if(weather != 7)
                {
                    atk_md0280_fill(20, 180, 220, 220, ATK_MD0280_BLACK);
                    weather = 7;
                    atk_md0280_show_string(20, 180, ATK_MD0280_LCD_WIDTH, 16, "thunder", ATK_MD0280_LCD_FONT_16, ATK_MD0280_CYAN);
                }
            }
            //如果buf收到的是"8"，则在屏幕上显示“hail”
            else if (buf[0] == '8')
            {
                if(weather != 8)
                {
                    atk_md0280_fill(20, 180, 220, 220, ATK_MD0280_BLACK);
                    weather = 8;
                    atk_md0280_show_string(20, 180, ATK_MD0280_LCD_WIDTH, 16, "hail", ATK_MD0280_LCD_FONT_16, ATK_MD0280_CYAN);
                }
            }
            //如果buf收到的是"9"，则在屏幕上显示“tornado”
            else if (buf[0] == '9')
            {
                if(weather != 9)
                {
                    atk_md0280_fill(20, 180, 220, 220, ATK_MD0280_BLACK);
                    weather = 9;
                    atk_md0280_show_string(20, 180, ATK_MD0280_LCD_WIDTH, 16, "tornado", ATK_MD0280_LCD_FONT_16, ATK_MD0280_CYAN);
                }
            }

            /* 重开开始接收来自ATK-MW8266D UART的数据 */
            atk_mw8266d_uart_rx_restart();
        }
    }
}
