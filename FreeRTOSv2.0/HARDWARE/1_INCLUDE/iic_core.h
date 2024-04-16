#ifndef _IIC_CORE_H
#define _IIC_CORE_H
#include "stm32f4xx.h"
#include "stdlib.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"							  

/* 记录使用了哪个iic接口 */
typedef struct IIC_slave
{
	GPIO_TypeDef 	*SCL_PORT;
	GPIO_TypeDef 	*SDA_PORT;
	uint32_t		SCL_PIN;
	uint32_t		SDA_PIN;
}IIC_Slave;


               
uint8_t IIC_SDA_READ(IIC_Slave* pslave);
void SDA_IN(IIC_Slave* pslave);



void IIC_Start(IIC_Slave* pslave);
void IIC_Stop(IIC_Slave* pslave);
uint8_t IIC_Wait_Ack(IIC_Slave* pslave);
void IIC_Ack(IIC_Slave* pslave);
void IIC_NAck(IIC_Slave* pslave);
void IIC_Send_Byte(IIC_Slave* pslave,uint8_t txd);
uint8_t IIC_Read_Byte(IIC_Slave* pslave);


void IIC_DeviceInit(IIC_Slave* I2c_Device);



#endif
