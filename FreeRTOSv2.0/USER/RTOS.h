#ifndef _RTOS_H
#define _RTOS_H

#include "RTOS.h"
#include "stm32f4xx.h"

/* ���ļ� */
#include <stdio.h>
#include <string.h>
#include <math.h>

/* freertos ͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"

/* ���ݽṹ */
#include "data_struct.h"


/* Ƭ������ͷ�ļ� */
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"

/* ����豸ͷ�ļ� */
#include "led.h"
#include "lcd.h"
#include "WiFi_usart3.h"
#include "jx90614.h"
#include "R60ABD1.h"
#include "R60AFD1.h"

void freertos_demo(void);

#endif
