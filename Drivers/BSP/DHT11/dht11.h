#ifndef _DHT_H_
#define _DHT_H_
//#include "stm32f10x.h"
#include "./SYSTEM/delay/delay.h"
extern void DHT11_IO_OUT();
extern void DHT11_IO_IN();
extern uint8_t DHT11_Init(void);//
extern uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);//
extern uint8_t DHT11_Read_Byte(void);//
extern uint8_t DHT11_Read_Bit(void);
extern uint8_t DHT11_Check(void);
extern void DHT11_Rst(void);
//extern uint8_t buffer[5];
void updateDHT(uint8_t *temp,uint8_t *humi);
#endif
