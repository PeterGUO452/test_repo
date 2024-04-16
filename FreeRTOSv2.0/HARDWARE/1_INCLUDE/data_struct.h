#ifndef _DATA_STRUCT_H
#define _DATA_STRUCT_H
#include "stm32f4xx.h"
#include "stdlib.h"
#include "usart.h"
#include "string.h"


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