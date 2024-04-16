#ifndef _R60ABD1_USART2_H
#define _R60ABD1_USART2_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART2_MAX_SEND_LEN  			200  	    //����������ֽ��� 200
#define USART2_MAX_RECV_LEN				200			  //�����������ֽ��� 200
#define USART2_RX_EN 			        1		      //ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART2_RX_BUF[USART2_MAX_SEND_LEN];    //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		            //����״̬���	
extern u8 usart2_rx_circle[5][USART2_MAX_RECV_LEN];
extern u8 stay[11] ;
extern u8 heart[11];
extern u8 breathe[11];
extern u8 people_state[3];

void USART2_init(u32 bound);
void u2_printf(char *fmt, ...);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
int circle_get(u8 buf[]);
int data_analysis(void);
#endif

