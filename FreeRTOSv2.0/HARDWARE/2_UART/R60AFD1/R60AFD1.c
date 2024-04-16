#include "R60AFD1.h"

#include "stdarg.h"
#include "string.h"
#include "stdio.h"


//��ʼ��IO ����2
//bound:������

// PC6:I2S2_MCK /TIM8_CH1/SDIO_D6 /USART6_TX /DCMI_D0/TIM3_CH1/EVENTOUT
// PC7:I2S3_MCK /TIM8_CH2/SDIO_D7 /USART6_RX /DCMI_D1/TIM3_CH2/EVENTOUT

#define USART6_MAX_RECV_LEN				200			  	//�����������ֽ��� 200
#define USART6_MAX_SEND_LEN				200

uint8_t USART6_RX_BUF[USART6_MAX_RECV_LEN];
uint8_t USART6_TX_BUF[USART6_MAX_SEND_LEN]; 				//���ջ���,��USART2_REC_LEN���ֽ�.
uint8_t len=0;
uint8_t Frame_flag=0;

static USART_InitTypeDef USART_InitStructure;
void R60AFD1_uart_init(void)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);     //ʹ��GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);    //ʹ��USART2ʱ��

    USART_DeInit(USART6);                                     //��λ����2

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6); //GPIOA2����ΪUSART2_TX
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6); //GPIOA3����ΪUSART2_RX

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7;  //GPIOA2��GPIOA3��ʼ��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;              //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      //�ٶ�50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;              //����
    GPIO_Init(GPIOC, &GPIO_InitStructure);                    //��ʼ��GPIOA2����GPIOA3

    USART_InitStructure.USART_BaudRate = 115200;                                    //������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;       	       //�շ�ģʽ

    USART_Init(USART6, &USART_InitStructure);                                      //��ʼ������2

    USART_Cmd(USART6, ENABLE);                                                     //ʹ�ܴ���

    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);            //�����ж�

    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ; //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	                        //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
                                  //����
}

uint8_t head_flag=0,tail_falg=0;
void USART6_IRQHandler(void)
{
	 uint8_t res;
	
	 if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) //���յ�����
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
				 Frame_flag = 1;//ֻҪ֡��־�������ܴ������ݣ����ﻹ���ڴ��ڵĻ�������Ӧ�ÿ������������Ļ�����
				 
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
    i = strlen((const char *)USART6_TX_BUF);                  //�˴η������ݵĳ���

    for (j = 0; j < i; j++)                                   //ѭ����������
    {
        while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET); //�ȴ��ϴδ������

        USART_SendData(USART6, (uint8_t)USART6_TX_BUF[j]); 	 //�������ݵ�����2
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



//����ʹ��strlen������յ�����֡�ĳ��ȣ�ÿһ֡�������е��ֶξ��� 0x00
void data_proc(void)
{
	
	
}