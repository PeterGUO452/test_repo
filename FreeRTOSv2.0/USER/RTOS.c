#include "RTOS.h"
#include "RTOSconfig.h"

void freertos_demo(void)
{	
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          										//�����������

}
//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //����JX90614���񡪡��޽Ӵ�������
    xTaskCreate((TaskFunction_t )JX90614_task,            //������
                (const char*    )"JX90614_task",          //��������
                (uint16_t       )JX90614_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )JX90614_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&JX90614Task_Handler);   //������    
    //����R60ABD1���񡪡��״�����ʡ���������
    xTaskCreate((TaskFunction_t )R60ABD1_task,            //������
                (const char*    )"R60ABD1_task",          //��������
                (uint16_t       )R60ABD1_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )R60ABD1_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&R60ABD1Task_Handler);   //������  
    //����R60AFD1���񡪡��������ģ��
    xTaskCreate((TaskFunction_t )R60AFD1_task,            //������
                (const char*    )"R60AFD1_task",          //��������
                (uint16_t       )R60AFD1_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )R60AFD1_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&R60AFD1Task_Handler);   //������  
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

void JX90614_task(void *pvParameters)
{
	
	JX90614_init();
	while(1)
	{
		JX90614_Read_Data(&people_temp);
		printf("people_temp = %.2f",people_temp);
		vTaskDelay(1000);
	}

}

void R60ABD1_task(void *pvParameters)
{
	
	USART2_init(115200);
	while(1)
	{
			Serial_SendArray(stay,sizeof(stay));
			vTaskDelay(20);
			data_analysis();
			Serial_SendArray(heart,sizeof(heart));
			vTaskDelay(20);
			data_analysis();
			Serial_SendArray(breathe,sizeof(breathe));
			vTaskDelay(20);
			data_analysis();
			printf("%d %d %d\n",people_state[0],people_state[1],people_state[2]);
			vTaskDelay(1000);
	}

}

void R60AFD1_task(void *pvParameters)
{
	
	while(1)
	{
		
		
		
	}
}

