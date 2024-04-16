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
////	uint8_t			event_flag;			//��¼�����ڼ��Ƿ�����ˮ�¼�
////	uint8_t			td2_warningFlag;	//�����ڼ�ˮ�ʾ����־
////	uint8_t			switch_errorFlag;	//�����ڼ��豸���ϱ�־
////	uint8_t			total_flow;			//�����ڼ���ˮ�¼�����ˮ����
//}Info;
//typedef      Info	        Data_Type;//����Ҫ�����������滻�� your_type λ�ü��ɵõ������͵Ķ���


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
	
	
	uint32_t 	iterm_length;	//���	
	uint8_t 	item_size;		//���С
	uint32_t	Max_length;		//�������С�ͻ�������С������������洢��
	
	uint8_t data[QUEUE_LENGHT];
}Cache_queue;

void Cache_QueueReset(Cache_queue** p_queue,uint8_t size);

void Cache_QueuePush(Cache_queue* p_queue,uint8_t* p_data);

void Cache_QueueRead_OneItem(Cache_queue* p_queue,uint8_t* p_data);
uint8_t Cache_QueueEmpty(Cache_queue* p_queue);
int Cache_QueueLength(Cache_queue* p_queue);


#endif