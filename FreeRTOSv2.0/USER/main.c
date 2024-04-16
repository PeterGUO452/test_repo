#include "RTOS.h"

typedef struct
{
	char a;
	int b;
	float c;
}data_type;

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		//����ϵͳ�ж����ȼ�����4
	delay_init(168);									//��ʼ����ʱ����
	uart_init(115200);     								//��ʼ������
	LED_Init();		        							//��ʼ��LED�˿�
	POINT_COLOR = RED;
	LCD_Init();											//��ʼ��LCD
	usart3_init(115200);
	R60AFD1_init();
//	while(1)
//	{
//		//�����Ƿ����� �������ģ��ͨ��
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
		printf("12345\r\n");
			
	}
	

	esp8266_start_mqtt_trans();
	freertos_demo();
	//���Բ��Բ���
}


