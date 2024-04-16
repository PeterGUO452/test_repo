#include "R60AFD1.h"

#include "stdarg.h"
#include "string.h"
#include "stdio.h"


//初始化IO 串口2
//bound:波特率

// PC6:I2S2_MCK /TIM8_CH1/SDIO_D6 /USART6_TX /DCMI_D0/TIM3_CH1/EVENTOUT
// PC7:I2S3_MCK /TIM8_CH2/SDIO_D7 /USART6_RX /DCMI_D1/TIM3_CH2/EVENTOUT

#define USART6_MAX_RECV_LEN				200			  	//定义最大接收字节数 200
#define USART6_MAX_SEND_LEN				200

uint8_t USART6_RX_BUF[USART6_MAX_RECV_LEN];
uint8_t USART6_TX_BUF[USART6_MAX_SEND_LEN]; 				//接收缓冲,最USART2_REC_LEN个字节.
uint8_t len=0;
uint8_t Frame_flag=0;

static USART_InitTypeDef USART_InitStructure;
void R60AFD1_uart_init(void)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);     //使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);    //使能USART2时钟

    USART_DeInit(USART6);                                     //复位串口2

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6); //GPIOA2复用为USART2_TX
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6); //GPIOA3复用为USART2_RX

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7;  //GPIOA2和GPIOA3初始化
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;              //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;              //上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);                    //初始化GPIOA2，和GPIOA3

    USART_InitStructure.USART_BaudRate = 115200;                                    //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;       	       //收发模式

    USART_Init(USART6, &USART_InitStructure);                                      //初始化串口2

    USART_Cmd(USART6, ENABLE);                                                     //使能串口

    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);            //开启中断

    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ; //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	                        //根据指定的参数初始化VIC寄存器
	
                                  //清零
}

uint8_t head_flag=0,tail_falg=0;
void USART6_IRQHandler(void)
{
	 uint8_t res;
	
	 if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) //接收到数据
	 {
		 res = USART_ReceiveData(USART6);
		 USART6_RX_BUF[len++] = res;
		 if(len >= 2 )
		 {
			 if(head_flag)
			 {
				 if(USART6_RX_BUF[len-1] == 0x43 && USART6_RX_BUF[len-2] == 0x54)
				 {
					 tail_falg = 1;
				 }
				 
			 }else if(USART6_RX_BUF[len-1] == 0x59 && USART6_RX_BUF[len-2] == 0x53)
			 {
				 head_flag = 1;
			 }
			 
			 if(tail_falg && head_flag)
			 {
				 Frame_flag = 1;//只要帧标志成立就能处理数据，这里还是在串口的缓冲区，应该拷贝到处理函数的缓冲区
				 
				 head_flag = tail_falg = 0;
				 USART6_RX_BUF[len] = 0;
			 }
			 
		 }
 
		
	 }
}

void u6_printf(char *fmt, ...)
{
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)USART6_TX_BUF, fmt, ap);
    va_end(ap);
    i = strlen((const char *)USART6_TX_BUF);                  //此次发送数据的长度

    for (j = 0; j < i; j++)                                   //循环发送数据
    {
        while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET); //等待上次传输完成

        USART_SendData(USART6, (uint8_t)USART6_TX_BUF[j]); 	 //发送数据到串口2
    }

}

void R60AFD1_init(void)
{
	R60AFD1_uart_init();
	
}
void R60AFD1_Reset(void)
{
	Frame_flag = 0;
	len = 0;
}



//不能使用strlen计算接收到数据帧的长度，每一帧数据中有的字段就是 0x00
void data_proc(void)
{
	
	
}