#ifndef __WIFI_H
#define __WIFI_H	 
#include "sys.h"  


#define USART3_MAX_RECV_LEN		1000					//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		1000					//最大发送缓存字节数


extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 USART3_RX_STA;   						//接收数据状态
extern u16 USART3_RX_LEN;							//接收到的长度


void usart3_init(u32 bound);			                    //串口3初始化 
void DMA_config(void);
void u3_printf(char* fmt, ...);
void esp8266_start_mqtt_trans(void);
void Usart3_SendByte(uint8_t Byte);                      //发送一个字节;
void Usart3_SendArray(uint8_t *Array, uint16_t Length);  //发送Length个字节

#endif


