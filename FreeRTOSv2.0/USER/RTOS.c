#include "RTOS.h"
#include "RTOSconfig.h"

void freertos_demo(void)
{	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          										//开启任务调度

}
//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    //创建JX90614任务——无接触测体温
    xTaskCreate((TaskFunction_t )JX90614_task,            //任务函数
                (const char*    )"JX90614_task",          //任务名称
                (uint16_t       )JX90614_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )JX90614_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&JX90614Task_Handler);   //任务句柄    
    //创建R60ABD1任务——雷达测心率、呼吸速率
    xTaskCreate((TaskFunction_t )R60ABD1_task,            //任务函数
                (const char*    )"R60ABD1_task",          //任务名称
                (uint16_t       )R60ABD1_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )R60ABD1_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&R60ABD1Task_Handler);   //任务句柄  
    //创建R60AFD1任务——跌倒检测模块
    xTaskCreate((TaskFunction_t )R60AFD1_task,            //任务函数
                (const char*    )"R60AFD1_task",          //任务名称
                (uint16_t       )R60AFD1_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )R60AFD1_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&R60AFD1Task_Handler);   //任务句柄  
    taskEXIT_CRITICAL();            //退出临界区
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

