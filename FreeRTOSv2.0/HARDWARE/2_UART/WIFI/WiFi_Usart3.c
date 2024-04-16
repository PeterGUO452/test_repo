#include "WiFi_usart3.h"
#include "usart.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "delay.h"


#define aliyu_ip    	"iot-06z00cqfiucuvpf.mqtt.iothub.aliyuncs.com"
#define aliyu_id 		"ipeiDps5gZM.ESP8266|securemode=2\\,signmethod=hmacsha256\\,timestamp=1711433283910|"
#define aliyun_username "ESP8266&ipeiDps5gZM"
#define aliyun_passwd 	"4f823c9ff1b2f7a56d9b202236564ce4a74662809f63393cc41e1f8e812bc4e0"

#define wifi_name   	"8p"
#define wifi_passwd		"pppppppp"

u16 USART3_RX_STA=0;
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]={'\0'};
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]={'\0'};
u16 USART3_RX_LEN=0;
void usart3_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);   //USART3_TX
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);   //USART3_RX

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure); 

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  	  // ��������λ�ֳ�
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   		  //1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;   			  // ��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // ��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	USART_Init(USART3, &USART_InitStructure);  

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);	
	USART3_RX_STA=0;	

	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);				//����USART3�����ж�
	USART_DMACmd(USART3, USART_DMAReq_Rx,ENABLE);  				//ʹ�ܴ��ڵ�DMA����
	
	DMA_config();                      							//DMA��ʼ��
	
	USART_Cmd(USART3, ENABLE); 
}

void DMA_config(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	//DMA1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	//DMA1��λ ��CREN=0
	DMA_DeInit(DMA1_Stream1);
	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4; 							//ͨ��ѡ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;		//DMA�����ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART3_RX_BUF;		//DMA �洢����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;   				//���赽�洢��ģʽ
    DMA_InitStructure.DMA_BufferSize = USART3_MAX_RECV_LEN;					//���ݴ�����
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//���������ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//�洢������ģʽ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ���:8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//�洢�����ݳ���:8λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//ʹ����ͨģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;				    //�ߵ����ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //������FIFOģʽ
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //FIFO��ֵ
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//�洢��ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//����ͻ�����δ���
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);								//DMA Stream��ʼ��
	DMA_Cmd(DMA1_Stream1, ENABLE);											//DMAʹ�� 
}

//����3�Ϻ���
void USART3_IRQHandler(void)                	
{
	u16 num=0;
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)			//����3�����ж�
	{
		//ע�⣺���Ҫ��������жϾͱ����һ�δ��ڣ�����ʹ��USART_ReceiveData�⺯����һ�δ��ڼ��ɡ�
		USART_ClearITPendingBit(USART3 , USART_IT_IDLE);			//������������߿����жϱ�ʶ��Ȼ���һ�����ݼĴ�����DMA���ղŻ���ȷ���ȶ�SR��Ȼ���DR������������жϱ�ʶ��ע�⣺������Ҫ�������ܹ�����жϱ�־λ��
		u8 ch =  USART_ReceiveData(USART3);							//������������߿����жϱ�ʶ��Ȼ���һ�����ݼĴ�����DMA���ղŻ���ȷ���ȶ�SR��Ȼ���DR������������жϱ�ʶ��ע�⣺������Ҫ�������ܹ�����жϱ�־λ��
		DMA_Cmd(DMA1_Stream1, DISABLE);    							//�ر�USART3 RX DMA1 ��ָʾ��ͨ��
		DMA_ClearFlag(DMA1_Stream1 , DMA_FLAG_TCIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1);//�������DMA��־λ�������´�DMA���Ὺʼת��
		num = DMA_GetCurrDataCounter(DMA1_Stream1);
		USART3_RX_LEN = USART3_MAX_RECV_LEN - num;	                //��ý��յ����ֽ���
		while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);
		DMA_SetCurrDataCounter(DMA1_Stream1,USART3_MAX_RECV_LEN);	//��������DMAͨ����DMA����Ĵ�С
		DMA_Cmd(DMA1_Stream1, ENABLE);     							//ʹ��USART3 RX DMA1 ��ָʾ��ͨ��
		USART3_RX_STA|=1<<15;   									//����Ѿ�������
  }
}




void Usart3_SendByte(uint8_t Byte)      //����һ���ֽ�
{
	USART_SendData(USART3, Byte);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

void Usart3_SendArray(uint8_t *Array, uint16_t Length)  //����Length���ֽ�
{ 
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Usart3_SendByte(Array[i]);
	}
}

//��u3_printf�����ض���ʹ���亯���������ͨ������������ͨ��
void u3_printf(char* fmt,...)  
{  
	u16 i,j=0; 
	va_list ap; 																				// ����ָ��ջ��ĳ��Ԫ�ص�ָ��
	va_start(ap,fmt);																			// apָ��ɱ�����б���׵�ַ(ջ����ĩ��Ԫ�ص�ַ)
	vsprintf((char*)USART3_TX_BUF,fmt,ap);														// �������׵�ַ~ap֮��Ŀɱ��β��б�ֵ��vsprintf�����β��б�
	va_end(ap);																					// apָ���ÿ�(NULL)
	i=strlen((const char*)USART3_TX_BUF);		
	for(j=0;j<i;j++)						
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);   
		USART_SendData(USART3,USART3_TX_BUF[j]); 
	} 
}




//��ESP8266��������
//cmd:���͵������ַ���;ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��;waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����);1,����ʧ��
u8 AT_CMD_Send(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	u3_printf("%s\r\n",cmd);	//��������

	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_xms(100);
			if(USART3_RX_STA & 0x8000)  //���յ��ڴ���Ӧ����
			{
				if(strstr((const char*)USART3_RX_BUF,(const char*)ack))
					break;//�õ���Ч���� 
			} 
		}
		if(waittime==0)res=1; 
	}
	memset(USART3_RX_BUF,0,strlen((const char*)USART3_RX_BUF));
	USART3_RX_STA=0;
	return res;
} 


void esp8266_start_mqtt_trans(void)   
{
	char Con_Buf[100];
	
	sprintf(Con_Buf,"AT+RESTORE");
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"OK",20));		
	printf("AT+RESTORE OK!\r\n");
	
	sprintf(Con_Buf,"AT+RST");
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"OK",20));
	printf("WIFI_CONNECT_START!\r\n");
	
  
	sprintf(Con_Buf,"AT+CWMODE=1");
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"OK",20));		
	printf("AT+CWMODE OK!\r\n");
	
	//����WIFI
	sprintf(Con_Buf,"AT+CWJAP=\"%s\",\"%s\"",wifi_name,wifi_passwd);
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"WIFI GOT IP",20));		
	printf("WIFI OK!\r\n");
	
	//�رջ���
	sprintf(Con_Buf,"ATE0");
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"OK",20));
	printf("ATE0!\r\n");
	
	sprintf(Con_Buf,"AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"");
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"OK",20));
	printf("AT+CIPSNTPCFG!\r\n");

	//����MQTT��½�����Ƶ��û���������
	sprintf(Con_Buf,"AT+MQTTUSERCFG=0,1,\"NULL\",\"%s\",\"%s\",0,0,\"\"",aliyun_username,aliyun_passwd);
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"OK",20));
	printf("AT+MQTTUSERCFG!\r\n");
	
	//����MQTT��½�����Ƶ�ClientID
	sprintf(Con_Buf,"AT+MQTTCLIENTID=0,\"%s\"",aliyu_id);
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"OK",20));
	printf("AT+MQTTCLIENTID!\r\n");
	
	//���Ӱ�����
	sprintf(Con_Buf,"AT+MQTTCONN=0,\"%s\",1883,1",aliyu_ip);
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"OK",20));
	printf("AT+MQTTCONN!\r\n");
	
	memset(USART3_RX_BUF,0,sizeof(USART3_RX_BUF));
	USART3_RX_STA=0;
}

