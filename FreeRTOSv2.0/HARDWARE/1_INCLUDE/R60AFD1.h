#ifndef _R60AFDA_H
#define _R60AFDA_H
#include "sys.h"

#define USART6_MAX_RECV_LEN				200			  	//定义最大接收字节数 200


extern uint8_t Frame_flag;
extern uint8_t USART6_RX_BUF[USART6_MAX_RECV_LEN];
void u6_printf(char *fmt, ...);
void R60AFD1_init(void);
void R60AFD1_Reset(void);

#endif