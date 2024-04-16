#ifndef _RTOSCONFIG_H
#define _RTOSCONFIG_H
#include "FreeRTOS.h"
#include "task.h"

/********************************************************************************/
#define 		START_TASK_PRIO		1	    
#define 		START_STK_SIZE 		128	  
TaskHandle_t 	StartTask_Handler;	      
void 			start_task(void *pvParameters);	
/********************************************************************************/
#define 		JX90614_TASK_PRIO		1	    
#define 		JX90614_STK_SIZE 		128	  
TaskHandle_t 	JX90614Task_Handler;	      
void 			JX90614_task(void *pvParameters);	
/********************************************************************************/
#define 		R60ABD1_TASK_PRIO		1	    
#define 		R60ABD1_STK_SIZE 		128	  
TaskHandle_t 	R60ABD1Task_Handler;	      
void 			R60ABD1_task(void *pvParameters);	
/********************************************************************************/
#define 		R60AFD1_TASK_PRIO		1	    
#define 		R60AFD1_STK_SIZE 		128	  
TaskHandle_t 	R60AFD1Task_Handler;	      
void 			R60AFD1_task(void *pvParameters);	
#endif

