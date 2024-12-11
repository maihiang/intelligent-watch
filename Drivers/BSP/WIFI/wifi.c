#include "wifi.h"
#include "img.h"
#include "./BSP/ATK_MD0280/atk_md0280.h"
#include "./BSP/ATK_MW8266D/atk_mw8266d.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

//定义WIFI的SSID，密码，TCP服务器的IP和端口
#define DEMO_WIFI_SSID          "LCH"
#define DEMO_WIFI_PWD           "LI,cai,hou,1105"
#define DEMO_TCP_SERVER_IP      "192.168.43.159"
#define DEMO_TCP_SERVER_PORT    "8080"

//声明display_WIFI函数中需要的函数
static void demo_show_ip(char *buf);
static void demo_key0_fun(uint8_t is_unvarnished);
static void demo_key1_fun(uint8_t *is_unvarnished);
static void demo_upload_data(uint8_t is_unvarnished);

//定义静态全局变量，标志是否已经连接WIFI
static int t=0;
/*
以下函数的功能是在屏幕上显示WIFI界面
*/
void display_WIFI(void)
{
    uint8_t ret;
    char ip_buf[16];
    uint8_t key;
    uint8_t is_unvarnished = 0;
    if(t==0)
    {
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
        ret  = atk_mw8266d_restore();                               /* 恢复出厂设置 */
        ret += atk_mw8266d_at_test();                               /* AT测试 */
        ret += atk_mw8266d_set_mode(1);                             /* station模式 */
        ret += atk_mw8266d_sw_reset();                              /* 软件复位 */
        ret += atk_mw8266d_ate_config(0);                           /* 关闭回显功能 */
        ret += atk_mw8266d_join_ap(DEMO_WIFI_SSID, DEMO_WIFI_PWD);  /* 连接WIFI */
        ret += atk_mw8266d_get_ip(ip_buf);                          /* 获取IP地址 */
        if (ret != 0)
        {
            printf("Error to join ap!\r\n");
            while (1)
            {
                LED0_TOGGLE();
                delay_ms(200);
            }
        }
        demo_show_ip(ip_buf);

        /* 连接TCP服务器 */
        ret = atk_mw8266d_connect_tcp_server(DEMO_TCP_SERVER_IP, DEMO_TCP_SERVER_PORT);
        if (ret != 0)
        {
            printf("Error to connect TCP server!\r\n");
            while (1)
            {
                LED0_TOGGLE();
                delay_ms(200);
            }
        }
        
        /* 重新开始接收新的一帧数据 */
        atk_mw8266d_uart_rx_restart();

        t++;
    }
    atk_md0280_show_pic(5, 5, 50, 40, back_logo);//左上角返回按钮
    atk_md0280_show_string(20, 130, ATK_MD0280_LCD_WIDTH, 32, "WIFI", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);//在屏幕上显示WIFI
    atk_md0280_show_string(20, 180, ATK_MD0280_LCD_WIDTH, 32, "connecting", ATK_MD0280_LCD_FONT_32, ATK_MD0280_WHITE);//在屏幕上显示连接中

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

//下面定义display_WIFI函数中需要的函数
/**
 * @brief       显示IP地址
 * @param       无
 * @retval      无
 */
static void demo_show_ip(char *buf)
{
    printf("IP: %s\r\n", buf);
    atk_md0280_show_string(60, 151,ATK_MD0280_LCD_WIDTH, 16, buf, ATK_MD0280_LCD_FONT_16, ATK_MD0280_BLUE);
}

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
            /* 重开开始接收来自ATK-MW8266D UART的数据 */
            atk_mw8266d_uart_rx_restart();
        }
    }
}
