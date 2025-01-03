#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/btim.h"
#include "demo.h"
#include "math.h"
#include "arm_math.h"

/**
 * @brief       显示实验信息
 * @param       无
 * @retval      无
 */
void show_mesg(void)
{
  /* 串口输出实验信息 */
  printf("\n");
  printf("********************************\r\n");
  printf("STM32\r\n");
  printf("ATK-MD0280\r\n");
  printf("ATOM@ALIENTEK\r\n");
  printf("********************************\r\n");
  printf("\r\n");
}

int main(void)
{
  HAL_Init();                              /* 初始化HAL库 */
  sys_stm32_clock_init(RCC_PLL_MUL9);      /* 设置时钟, 72Mhz */
  delay_init(72);                          /* 延时初始化 */
  usart_init(115200);                      /* 串口初始化为115200 */
  led_init();                              /* 初始化LED */
  key_init();                              /* 初始化按键 */
  btim_timx_int_init(10000 - 1, 7200 - 1); /* 初始化定时器，10kHz的技术频率，计数10000次为1s */
  show_mesg();                             /* 显示实验信息 */
  delay_ms(1000);
  demo_run(); /* 运行示例程序 */
}
