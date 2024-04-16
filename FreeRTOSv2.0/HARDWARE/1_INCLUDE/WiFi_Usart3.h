#ifndef __WIFI_H
#define __WIFI_H	 
#include "sys.h"  


#define USART3_MAX_RECV_LEN		1000					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		1000					//����ͻ����ֽ���


extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART3_RX_STA;   						//��������״̬
extern u16 USART3_RX_LEN;							//���յ��ĳ���


void usart3_init(u32 bound);			                    //����3��ʼ�� 
void DMA_config(void);
void u3_printf(char* fmt, ...);
void esp8266_start_mqtt_trans(void);
void Usart3_SendByte(uint8_t Byte);                      //����һ���ֽ�;
void Usart3_SendArray(uint8_t *Array, uint16_t Length);  //����Length���ֽ�

#endif


