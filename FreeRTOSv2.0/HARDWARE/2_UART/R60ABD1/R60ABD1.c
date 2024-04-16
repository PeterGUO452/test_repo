#include "sys.h"
#include "delay.h"
#include "stdarg.h"
#include "string.h"
#include "R60ABD1.h"


//串口发送缓存区
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//发送缓冲,最USART2_REC_LEN字节
#ifdef USART2_RX_EN   							          	    //如果使能了接收   	  
//串口接收缓存区
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最USART2_REC_LEN个字节.

u8 r=0,w=0;
u8 usart2_rx_circle[5][USART2_MAX_RECV_LEN];

   u8 stay[11] = {0x53,0x59,0x80,0x81,0x00,0x01,0x0F,0xBD,0x54,0x43};
  u8 heart[11] = {0x53,0x59,0x85,0x82,0x00,0x01,0x0F,0xC3,0x54,0x43};
u8 breathe[11] = {0x53,0x59,0x81,0x82,0x00,0x01,0x0F,0xBF,0x54,0x43};

u8 people_state[3];
static void circle_put(void)    //入环函数
{
	if(((w+1)%5) != r)
	{
		memcpy(usart2_rx_circle[w],USART2_RX_BUF,11);
		w=(w+1)%5;
	}
	else
	{
		memcpy(usart2_rx_circle[w],USART2_RX_BUF,11);
		r=(r+1)%5;
		w=(w+1)%5;
	}
	USART2_RX_STA = 0;
}

int circle_get(u8 buf[])        //出环函数
{
	if(r != w)
	{
		memcpy(buf,usart2_rx_circle[r],11);
		r=(r+1)%5;
		return 1;
	}
	else
		return 0;
}

int data_analysis(void)      //数据解析函数
{
	u8 buf[200];
	u8 sum = 0x00;
	if(circle_get(buf) == 1)
	{
		for(int i = 0; i < 7 ; i++)
		{
				sum = sum + buf[i];
		}
		if(sum == buf[7])
		{
			if(buf[2] == 0x80)
			{
				people_state[0] = buf[6];
				return 1;
			}
			else if(buf[2] == 0x81)
			{
				people_state[1] = buf[6];
				return 1;
			}
			else if(buf[2] == 0x85)
			{
				people_state[2] = buf[6];
				return 1;
			}
		}
	}
	return 0;
}

void Serial_SendByte(uint8_t Byte)      //发送一个字节
{
	USART_SendData(USART2, Byte);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)  //发送Length个字节
{ 
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}


//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 USART2_RX_STA = 0;
u8 res_before;
void USART2_IRQHandler(void)
{
    u8 res;

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
    {
        res = USART_ReceiveData(USART2);
        if ((USART2_RX_STA & (1 << 15)) == 0)        //接收完的一批数据,还没有被处理,则不再接收其他数据
        {
            if (USART2_RX_STA < USART2_MAX_RECV_LEN) //还可以接收数据
            {
								USART2_RX_BUF[USART2_RX_STA++] = res; //记录接收到的值
                if(res != 0x43)
								{
									res_before = res;
								}
								else
								{
									 if(res_before == 0x54)
									 {
										 USART2_RX_STA |= 1 << 15;							//强制标记接收完成
										 circle_put();                          //加入环形缓冲区
									 }
									 else
									 {
									   res_before = res;
									 }
								}
            }
            else
            {
                USART2_RX_STA |= 1 << 15; //强制标记接收完成
								circle_put();             //加入环形缓冲区
            }
        }
    }
}
#endif

USART_InitTypeDef USART_InitStructure;
//初始化IO 串口2
//bound:波特率
void USART2_init(u32 bound)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);     //使能GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);    //使能USART2时钟

    USART_DeInit(USART2);                                     //复位串口2

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //GPIOA2复用为USART2_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA3复用为USART2_RX

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2| GPIO_Pin_3;  //GPIOA2和GPIOA3初始化
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;              //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;              //上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);                    //初始化GPIOA2，和GPIOA3

    USART_InitStructure.USART_BaudRate = bound;                                    //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;       	       //收发模式

    USART_Init(USART2, &USART_InitStructure);                                      //初始化串口2

    USART_Cmd(USART2, ENABLE);                                                     //使能串口

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);            //开启中断

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ; //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	                        //根据指定的参数初始化VIC寄存器
    USART2_RX_STA = 0;		                              //清零
}

//串口3,printf 函数
//确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void u2_printf(char *fmt, ...)
{
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)USART2_TX_BUF, fmt, ap);
    va_end(ap);
    i = strlen((const char *)USART2_TX_BUF);                  //此次发送数据的长度

    for (j = 0; j < i; j++)                                   //循环发送数据
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); //等待上次传输完成

        USART_SendData(USART2, (uint8_t)USART2_TX_BUF[j]); 	 //发送数据到串口2
    }

}
