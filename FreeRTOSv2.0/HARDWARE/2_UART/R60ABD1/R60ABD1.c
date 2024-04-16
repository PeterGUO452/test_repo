#include "sys.h"
#include "delay.h"
#include "stdarg.h"
#include "string.h"
#include "R60ABD1.h"


//���ڷ��ͻ�����
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//���ͻ���,��USART2_REC_LEN�ֽ�
#ifdef USART2_RX_EN   							          	    //���ʹ���˽���   	  
//���ڽ��ջ�����
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,��USART2_REC_LEN���ֽ�.

u8 r=0,w=0;
u8 usart2_rx_circle[5][USART2_MAX_RECV_LEN];

   u8 stay[11] = {0x53,0x59,0x80,0x81,0x00,0x01,0x0F,0xBD,0x54,0x43};
  u8 heart[11] = {0x53,0x59,0x85,0x82,0x00,0x01,0x0F,0xC3,0x54,0x43};
u8 breathe[11] = {0x53,0x59,0x81,0x82,0x00,0x01,0x0F,0xBF,0x54,0x43};

u8 people_state[3];
static void circle_put(void)    //�뻷����
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

int circle_get(u8 buf[])        //��������
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

int data_analysis(void)      //���ݽ�������
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

void Serial_SendByte(uint8_t Byte)      //����һ���ֽ�
{
	USART_SendData(USART2, Byte);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)  //����Length���ֽ�
{ 
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}


//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 USART2_RX_STA = 0;
u8 res_before;
void USART2_IRQHandler(void)
{
    u8 res;

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
    {
        res = USART_ReceiveData(USART2);
        if ((USART2_RX_STA & (1 << 15)) == 0)        //�������һ������,��û�б�����,���ٽ�����������
        {
            if (USART2_RX_STA < USART2_MAX_RECV_LEN) //�����Խ�������
            {
								USART2_RX_BUF[USART2_RX_STA++] = res; //��¼���յ���ֵ
                if(res != 0x43)
								{
									res_before = res;
								}
								else
								{
									 if(res_before == 0x54)
									 {
										 USART2_RX_STA |= 1 << 15;							//ǿ�Ʊ�ǽ������
										 circle_put();                          //���뻷�λ�����
									 }
									 else
									 {
									   res_before = res;
									 }
								}
            }
            else
            {
                USART2_RX_STA |= 1 << 15; //ǿ�Ʊ�ǽ������
								circle_put();             //���뻷�λ�����
            }
        }
    }
}
#endif

USART_InitTypeDef USART_InitStructure;
//��ʼ��IO ����2
//bound:������
void USART2_init(u32 bound)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);     //ʹ��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);    //ʹ��USART2ʱ��

    USART_DeInit(USART2);                                     //��λ����2

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //GPIOA2����ΪUSART2_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA3����ΪUSART2_RX

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2| GPIO_Pin_3;  //GPIOA2��GPIOA3��ʼ��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;              //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      //�ٶ�50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;              //����
    GPIO_Init(GPIOA, &GPIO_InitStructure);                    //��ʼ��GPIOA2����GPIOA3

    USART_InitStructure.USART_BaudRate = bound;                                    //������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;       	       //�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure);                                      //��ʼ������2

    USART_Cmd(USART2, ENABLE);                                                     //ʹ�ܴ���

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);            //�����ж�

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ; //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	                        //����ָ���Ĳ�����ʼ��VIC�Ĵ���
    USART2_RX_STA = 0;		                              //����
}

//����3,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u2_printf(char *fmt, ...)
{
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)USART2_TX_BUF, fmt, ap);
    va_end(ap);
    i = strlen((const char *)USART2_TX_BUF);                  //�˴η������ݵĳ���

    for (j = 0; j < i; j++)                                   //ѭ����������
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); //�ȴ��ϴδ������

        USART_SendData(USART2, (uint8_t)USART2_TX_BUF[j]); 	 //�������ݵ�����2
    }

}
