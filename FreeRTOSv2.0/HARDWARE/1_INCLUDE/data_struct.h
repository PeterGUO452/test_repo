#ifndef _DATA_STRUCT_H
#define _DATA_STRUCT_H
#include "stm32f4xx.h"
#include "stdlib.h"
#include "usart.h"
#include "string.h"

//#define QUEUE_LENGHT	180
////typedef    your_type		Data_Type
//typedef struct
//{
////	float 			t1;
////	int 			t2;
////	struct 			v1;
////	float 			v2;
////	float 			motor_speed;
////	float 			tds1;
////	float			tds2;
////	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
////	uint16_t 		lxin;
////	uint8_t 		b_id;
//////	uint8_t 		tds1;
//////	uint8_t			tds2;
////	uint8_t			event_flag;			//记录断网期间是否发生饮水事件
////	uint8_t			td2_warningFlag;	//断网期间水质警告标志
////	uint8_t			switch_errorFlag;	//断网期间设备故障标志
////	uint8_t			total_flow;			//断网期间饮水事件的饮水总量
//}Info;
//typedef      Info	        Data_Type;//将需要的数据类型替换到 your_type 位置即可得到该类型的队列


//typedef struct
//{
//	uint16_t 	head;
//	uint16_t 	tail;
//	uint32_t 	length;
//	Data_Type 	data[QUEUE_LENGHT];
//	//uint8_t data[1024];

//}Data_Queue;



//extern Data_Queue* Cache_Queue;
//extern Data_Type   Cur_Info;
//extern uint8_t conc_flag;

//
//void Cache_QueuePush(Data_Queue* p_queue,Data_Type data);
//void Cache_QueueRead_OneItem(Data_Queue* p_queue,Data_Type* p_data);
//uint8_t Cache_QueueEmpty(Data_Queue* p_queue);
//int Cache_QueueLength(Data_Queue* p_queue);

	

#define QUEUE_LENGHT	1024
typedef struct
{
	uint16_t 	head;
	uint16_t 	tail;
	
	
	uint32_t 	iterm_length;	//项长度	
	uint8_t 	item_size;		//项大小
	uint32_t	Max_length;		//根据项大小和缓存区大小计算出来的最大存储度
	
	uint8_t data[QUEUE_LENGHT];
}Cache_queue;

void Cache_QueueReset(Cache_queue** p_queue,uint8_t size);

void Cache_QueuePush(Cache_queue* p_queue,uint8_t* p_data);

void Cache_QueueRead_OneItem(Cache_queue* p_queue,uint8_t* p_data);
uint8_t Cache_QueueEmpty(Cache_queue* p_queue);
int Cache_QueueLength(Cache_queue* p_queue);


#endif