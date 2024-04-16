#include "iic_core.h"

#include "jx90614.h"
#include "delay.h"
#include <math.h>

/* 非接触式人体体温测量模块 */
/* 全局的数据存储变量 */
IIC_Slave* Jx90614_Slave;
double people_temp;

void MyI2C_Init(void)
{					     
	Jx90614_Slave = (IIC_Slave*)malloc(sizeof(Jx90614_Slave));
	Jx90614_Slave->SCL_PIN  = JX90614_I2C_SCL_PIN;
	Jx90614_Slave->SCL_PORT =JX90614_GPIO_PORT_I2C;
	Jx90614_Slave->SDA_PIN  =JX90614_I2C_SDA_PIN;
	Jx90614_Slave->SDA_PORT =JX90614_GPIO_PORT_I2C;
	IIC_DeviceInit(Jx90614_Slave);
}

void MyI2C_Start(void)
{
	IIC_Start(Jx90614_Slave);
}

void MyI2C_Stop(void)
{

	IIC_Stop(Jx90614_Slave);
}

void MyI2C_SendByte(uint8_t Byte)
{
	IIC_Send_Byte(Jx90614_Slave,Byte);
}

void MyI2C_SendAck(uint8_t AckBit)
{
	if(AckBit)
	{
		IIC_Ack(Jx90614_Slave);
	}else
	{
		IIC_NAck(Jx90614_Slave);
	}
}


uint8_t MyI2C_ReceiveByte(void)
{
	return IIC_Read_Byte(Jx90614_Slave);
}


uint8_t MyI2C_ReceiveAck(void)
{
	//0 ack 
	//1 nack
	return IIC_Wait_Ack(Jx90614_Slave);
}
void JX90614_init(void)
{
	MyI2C_Init();
	delay_ms(3);
}

void JX90614_Read_Data(double *DATA)
{
	int JX90614_DATA = 0;
	uint8_t dataL = 0,dataH = 0;
	MyI2C_Start();
	MyI2C_SendByte((JX90614_ADDR << 1) | 0);         //写数据
	while(MyI2C_ReceiveAck());                       //等待应答
	MyI2C_SendByte(0x07);                            //指定写入地址 0x07
	while(MyI2C_ReceiveAck());                       //等待应答
	
	MyI2C_Start();
	MyI2C_SendByte((JX90614_ADDR << 1) | 1);         //读数据
	while(MyI2C_ReceiveAck());                       //等待应答
	dataL = MyI2C_ReceiveByte();                
	dataH = MyI2C_ReceiveByte();                
	MyI2C_Stop();

	JX90614_DATA = (dataH << 8) | dataL;
	*DATA = (JX90614_DATA*0.02 - 273.15)*1.12;
}



