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
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  	  // 配置数据位字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   		  //1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;   			  // 无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 无硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	USART_Init(USART3, &USART_InitStructure);  

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);	
	USART3_RX_STA=0;	

	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);				//开启USART3空闲中断
	USART_DMACmd(USART3, USART_DMAReq_Rx,ENABLE);  				//使能串口的DMA接收
	
	DMA_config();                      							//DMA初始化
	
	USART_Cmd(USART3, ENABLE); 
}

void DMA_config(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	//DMA1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	//DMA1复位 将CREN=0
	DMA_DeInit(DMA1_Stream1);
	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4; 							//通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;		//DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART3_RX_BUF;		//DMA 存储器地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;   				//外设到存储器模式
    DMA_InitStructure.DMA_BufferSize = USART3_MAX_RECV_LEN;					//数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;				    //高等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //不开启FIFO模式
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //FIFO阈值
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);								//DMA Stream初始化
	DMA_Cmd(DMA1_Stream1, ENABLE);											//DMA使能 
}

//串口3断函数
void USART3_IRQHandler(void)                	
{
	u16 num=0;
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)			//串口3空闲中断
	{
		//注意：如果要清除空闲中断就必须读一次串口，于是使用USART_ReceiveData库函数读一次串口即可。
		USART_ClearITPendingBit(USART3 , USART_IT_IDLE);			//必须先清除总线空闲中断标识，然后读一下数据寄存器，DMA接收才会正确（先读SR，然后读DR才能清除空闲中断标识）注意：这句必须要，否则不能够清除中断标志位。
		u8 ch =  USART_ReceiveData(USART3);							//必须先清除总线空闲中断标识，然后读一下数据寄存器，DMA接收才会正确（先读SR，然后读DR才能清除空闲中断标识）注意：这句必须要，否则不能够清除中断标志位。
		DMA_Cmd(DMA1_Stream1, DISABLE);    							//关闭USART3 RX DMA1 所指示的通道
		DMA_ClearFlag(DMA1_Stream1 , DMA_FLAG_TCIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1);//必须清除DMA标志位，否则下次DMA不会开始转运
		num = DMA_GetCurrDataCounter(DMA1_Stream1);
		USART3_RX_LEN = USART3_MAX_RECV_LEN - num;	                //获得接收到的字节数
		while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);
		DMA_SetCurrDataCounter(DMA1_Stream1,USART3_MAX_RECV_LEN);	//重新设置DMA通道的DMA缓存的大小
		DMA_Cmd(DMA1_Stream1, ENABLE);     							//使能USART3 RX DMA1 所指示的通道
		USART3_RX_STA|=1<<15;   									//标记已经接收完
  }
}




void Usart3_SendByte(uint8_t Byte)      //发送一个字节
{
	USART_SendData(USART3, Byte);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

void Usart3_SendArray(uint8_t *Array, uint16_t Length)  //发送Length个字节
{ 
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Usart3_SendByte(Array[i]);
	}
}

//对u3_printf进行重定向，使用其函数，便可以通过串口三进行通信
void u3_printf(char* fmt,...)  
{  
	u16 i,j=0; 
	va_list ap; 																				// 定义指向栈区某个元素的指针
	va_start(ap,fmt);																			// ap指向可变参数列表的首地址(栈区的末端元素地址)
	vsprintf((char*)USART3_TX_BUF,fmt,ap);														// 将函数首地址~ap之间的可变形参列表赋值给vsprintf函数形参列表
	va_end(ap);																					// ap指针置空(NULL)
	i=strlen((const char*)USART3_TX_BUF);		
	for(j=0;j<i;j++)						
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);   
		USART_SendData(USART3,USART3_TX_BUF[j]); 
	} 
}




//向ESP8266发送命令
//cmd:发送的命令字符串;ack:期待的应答结果,如果为空,则表示不需要等待应答;waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果);1,发送失败
u8 AT_CMD_Send(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	u3_printf("%s\r\n",cmd);	//发送命令

	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_xms(100);
			if(USART3_RX_STA & 0x8000)  //接收到期待的应答结果
			{
				if(strstr((const char*)USART3_RX_BUF,(const char*)ack))
					break;//得到有效数据 
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
	
	//连接WIFI
	sprintf(Con_Buf,"AT+CWJAP=\"%s\",\"%s\"",wifi_name,wifi_passwd);
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"WIFI GOT IP",20));		
	printf("WIFI OK!\r\n");
	
	//关闭回显
	sprintf(Con_Buf,"ATE0");
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"OK",20));
	printf("ATE0!\r\n");
	
	sprintf(Con_Buf,"AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"");
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"OK",20));
	printf("AT+CIPSNTPCFG!\r\n");

	//设置MQTT登陆阿里云的用户名与密码
	sprintf(Con_Buf,"AT+MQTTUSERCFG=0,1,\"NULL\",\"%s\",\"%s\",0,0,\"\"",aliyun_username,aliyun_passwd);
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"OK",20));
	printf("AT+MQTTUSERCFG!\r\n");
	
	//设置MQTT登陆阿里云的ClientID
	sprintf(Con_Buf,"AT+MQTTCLIENTID=0,\"%s\"",aliyu_id);
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"OK",20));
	printf("AT+MQTTCLIENTID!\r\n");
	
	//连接阿里云
	sprintf(Con_Buf,"AT+MQTTCONN=0,\"%s\",1883,1",aliyu_ip);
	while(AT_CMD_Send((u8*)Con_Buf,(u8*)"OK",20));
	printf("AT+MQTTCONN!\r\n");
	
	memset(USART3_RX_BUF,0,sizeof(USART3_RX_BUF));
	USART3_RX_STA=0;
}

