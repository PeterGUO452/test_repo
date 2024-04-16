#ifndef __JX90614_H
#define __JX90614_H
#include "sys.h"

extern double people_temp;


#define JX90614_GPIO_PORT_I2C		GPIOB						
#define JX90614_RCC_I2C_PORT 		RCC_AHB1Periph_GPIOB		
#define JX90614_I2C_SCL_PIN		  GPIO_Pin_12				
#define JX90614_I2C_SDA_PIN		  GPIO_Pin_13 		
#define JX90614_ADDR  0X00
void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(void);

void JX90614_init(void);
//int JX90614_Wait_Data_Ready(void);
void JX90614_Read_Data(double *DATA);
#endif
















