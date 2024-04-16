#include "RTOS.h"
uint8_t add(uint8_t a,uint8_t b);

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

	uint8_t a,b;
	while(1)
	{
		printf("123456\r\n");
		

		a=b=0;
		printf("%d\r\n",add(a,b));
		

		while(1)
		{
			printf("67890\r\n");
		}

	}
	esp8266_start_mqtt_trans();
	freertos_demo();
	//���Բ��Բ���
}

uint8_t add(uint8_t a,uint8_t b)
{
	return a+b;
}


