#include "RTOS.h"

typedef struct
{
	char a;
	int b;
	float c;
}data_type;

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		//设置系统中断优先级分组4
	delay_init(168);									//初始化延时函数
	uart_init(115200);     								//初始化串口
	LED_Init();		        							//初始化LED端口
	POINT_COLOR = RED;
	LCD_Init();											//初始化LCD
	usart3_init(115200);
	R60AFD1_init();
//	while(1)
//	{
//		//测试是否能与 跌倒检测模块通信
//		if(Frame_flag)
//		{
//			R60AFD1_Reset();
//			printf("%s\r\n",USART6_RX_BUF);
//			
//		}else
//		{
//			//printf("not recv\r\n");
//		}
//		delay_ms(100);
//	}
	
	Cache_queue* q1=NULL;
	Cache_queue* q2=NULL;
	
	Cache_QueueReset(&q1,sizeof(int));
	Cache_QueueReset(&q2,sizeof(data_type));
	

	int x=0;
	data_type a;	
	
	int i=0;
	while(1)
	{
		while(1)
		{
			x++;
			Cache_QueuePush(q1,(uint8_t*)&x);
			if(q1->Max_length == (q1->iterm_length + 1))
				break;
		}
		x=0;
		
		while(1)
		{
			a.a+=1;
			a.b+=1;
			a.c+=0.1;
			Cache_QueuePush(q2,(uint8_t*)&a);
			if(q2->Max_length == (q2->iterm_length + 1))
				break;
		}
		
		while(!Cache_QueueEmpty(q1))
		{
			
			Cache_QueueRead_OneItem(q1,(uint8_t*)&x);
			printf("x_%d: %d\r\n",i++,x);
		}
		
		i=0;
		
		do
		{
			
			Cache_QueueRead_OneItem(q2,(uint8_t*)&a);
			printf("a_%d: a=%c, b=%d, c=%.2f\r\n",i++,a.a,a.b,a.c);
		}while(!Cache_QueueEmpty(q2));
			
	}
	

	esp8266_start_mqtt_trans();
	freertos_demo();
	//测试测试测试
}


