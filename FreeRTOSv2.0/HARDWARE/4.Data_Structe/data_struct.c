#include "data_struct.h"
uint8_t 	conc_flag 	= 0;
void Cache_QueueReset(Cache_queue** p_queue,uint8_t size)
{
	//将队列的空间开辟到堆上
	if(!(*p_queue))
	{
		(*p_queue )= (Cache_queue*)malloc(sizeof(Cache_queue));
		(*p_queue)->item_size = size;
		(*p_queue)->Max_length = QUEUE_LENGHT / size;//取整数的
		(*p_queue)->head = (*p_queue)->tail=0;
		printf(" Cache_queue size %d \r\n",sizeof(Cache_queue));
		if(!(*p_queue))
		{
			printf("Data_Queue malloc failt\r\n");
		}
	}
	if(!conc_flag)//上一次断网的数据完全恢复了 conc_flag = 0
		(*p_queue)->head = (*p_queue)->tail = 0;
}

void Cache_QueuePush(Cache_queue* p_queue,uint8_t* p_data)
{
	memcpy(&(p_queue->data[(p_queue->tail)]) ,p_data,p_queue->item_size);
	
	p_queue->tail = ((p_queue->tail + p_queue->item_size)) % (p_queue->Max_length * p_queue->item_size);
	
	if(p_queue->tail == p_queue->head)
	{
		p_queue->head = (p_queue->head + p_queue->item_size)%p_queue->Max_length;
	}else
	{
		p_queue->iterm_length +=1;
	}

}

void Cache_QueueRead_OneItem(Cache_queue* p_queue,uint8_t* p_data)
{
	memcpy(p_data ,&(p_queue->data[(p_queue->head)]) ,p_queue->item_size);
	p_queue->head = (p_queue->head + p_queue->item_size)%(p_queue->Max_length * p_queue->item_size);
	
	if(p_queue->iterm_length)
	{
		p_queue->iterm_length -=1;
	}
}
uint8_t Cache_QueueEmpty(Cache_queue* p_queue)
{
	if(p_queue->head == p_queue->tail)
		return 1;//空
	else
		return 0;
}
int Cache_QueueLength(Cache_queue* p_queue)
{
	return p_queue->iterm_length;
}