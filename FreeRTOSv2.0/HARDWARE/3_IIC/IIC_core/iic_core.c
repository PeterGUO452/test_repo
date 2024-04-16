#include "iic_core.h"
//��ʼ��IIC

static void IIC_SCL_1(IIC_Slave* pslave)
{
	GPIO_SetBits(pslave->SCL_PORT, pslave->SCL_PIN);
}
			
static void IIC_SCL_0(IIC_Slave* pslave)
{
	GPIO_ResetBits(pslave->SCL_PORT, pslave->SCL_PIN);
}															

static void IIC_SDA_1(IIC_Slave* pslave)
{
	GPIO_SetBits(pslave->SDA_PORT, pslave->SDA_PIN);
}	
				
static void IIC_SDA_0(IIC_Slave* pslave)
{
	GPIO_ResetBits(pslave->SDA_PORT, pslave->SDA_PIN);
}                                

uint8_t IIC_SDA_READ(IIC_Slave* pslave)
{
	return GPIO_ReadInputDataBit(pslave->SDA_PORT, pslave->SDA_PIN);
}

void SDA_IN(IIC_Slave* pslave)
{
	IIC_SDA_1(pslave);	  	  
}

void IIC_Start(IIC_Slave* pslave)
{
	//SDA_OUT();     //sda�����
	IIC_SDA_1(pslave);	  	  
	IIC_SCL_1(pslave);
	delay_us(4);

 	IIC_SDA_0(pslave);//START:when CLK is high,DATA change form high to low //������->������ �ı�Ե�ź�
	delay_us(4);

	IIC_SCL_1(pslave);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	

void IIC_Stop(IIC_Slave* pslave)
{
	//SDA_OUT();//sda�����
	IIC_SCL_0(pslave);
	IIC_SDA_0(pslave);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);

	IIC_SCL_1(pslave); 
	IIC_SDA_1(pslave);//����I2C���߽����ź�

	delay_us(4);							   	
}

uint8_t IIC_Wait_Ack(IIC_Slave* pslave)
{
	uint32_t ucErrTime=0;
	SDA_IN(pslave);      //SDA����Ϊ����  
	IIC_SDA_1(pslave);delay_us(1);	   
	IIC_SCL_1(pslave);delay_us(1);	 
	//while(READ_SDA)
	while(IIC_SDA_READ(pslave) == SET)
	{
		ucErrTime++;
		if(ucErrTime>500)//һֱ���Ǹߵ�ƽ
		{
			IIC_Stop(pslave);//����ֹͣ�ź�
			return 1;
		}
	}
	IIC_SCL_0(pslave);//ʱ�����0 	   
	return 0;  
} 


void IIC_Ack(IIC_Slave* pslave)
{
	IIC_SCL_0(pslave);
	//SDA_OUT();
	IIC_SDA_0(pslave);
	delay_us(2);
	IIC_SCL_1(pslave);
	delay_us(2);
	IIC_SCL_0(pslave);
	
}

void IIC_NAck(IIC_Slave* pslave)//SCK �ߵ�ƽ�ڼ䣬SDA Ϊ�ߵ�ƽ��Ϊ��ЧӦ��
{
	IIC_SCL_0(pslave);
	//SDA_OUT(pslave);
	IIC_SDA_1(pslave);
	delay_us(2);
	IIC_SCL_1(pslave);
	delay_us(2);
	IIC_SCL_0(pslave);
}		

void IIC_Send_Byte(IIC_Slave* pslave,uint8_t txd)
{                        
    uint8_t t;   
		//SDA_OUT(); 	    
    IIC_SCL_0(pslave);//����ʱ�ӿ�ʼ���ݴ���
		delay_us(2);
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
			if((txd&0x80)>>7)
				IIC_SDA_1(pslave);
			else
				IIC_SDA_0(pslave);
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_1(pslave);
		delay_us(2); 
		IIC_SCL_0(pslave);	
		delay_us(1);
    }	 
} 	

uint8_t IIC_Read_Byte(IIC_Slave* pslave)
{
	unsigned char i,receive=0;
	SDA_IN(pslave);//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
      IIC_SCL_0(pslave); 
      delay_us(2);
			IIC_SCL_1(pslave);
      receive<<=1;
      if(IIC_SDA_READ(pslave))
				receive++;   
			delay_us(1); 
    }					  
    return receive;
}



//���iic�˿ڳ�ʼ������
void IIC_DeviceInit(IIC_Slave* I2c_Device)
{
	GPIO_InitTypeDef  GPIO_Structe;
	//�򿪶˿�ʱ��
	if(I2c_Device->SCL_PORT == GPIOA  || I2c_Device->SDA_PORT == GPIOA)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOBʱ��
	}
	if(I2c_Device->SCL_PORT == GPIOB  || I2c_Device->SDA_PORT == GPIOB)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	}
	if(I2c_Device->SCL_PORT == GPIOC  || I2c_Device->SDA_PORT == GPIOC)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOBʱ��
	}
	if(I2c_Device->SCL_PORT == GPIOD  || I2c_Device->SDA_PORT == GPIOD)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOBʱ��
	}
	if(I2c_Device->SCL_PORT == GPIOE  || I2c_Device->SDA_PORT == GPIOE)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOBʱ��
	}
	if(I2c_Device->SCL_PORT == GPIOF  || I2c_Device->SDA_PORT == GPIOF)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOBʱ��
	}
	GPIO_Structe.GPIO_Pin = I2c_Device->SCL_PIN;
	GPIO_Structe.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_Structe.GPIO_OType = GPIO_OType_OD;//�������
	GPIO_Structe.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_Structe.GPIO_PuPd = GPIO_PuPd_UP;//����	
	GPIO_Init(I2c_Device->SCL_PORT, &GPIO_Structe);//��ʼ��
	
	GPIO_Structe.GPIO_Pin = I2c_Device->SDA_PIN;
	GPIO_Init(I2c_Device->SDA_PORT,&GPIO_Structe);
	
	IIC_SCL_1(I2c_Device);
	IIC_SDA_1(I2c_Device);

}





