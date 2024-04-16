#include "RTOS.h"

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

	
	Cache_queue* q1=NULL;
	Cache_QueueReset(&q1,sizeof(int));

	while(1)
	{
		printf("123456\r\n");
		
		while(1)
		{
			printf("67890\r\n");
		}
	}
	esp8266_start_mqtt_trans();
	freertos_demo();
	//���Բ��Բ���
}


