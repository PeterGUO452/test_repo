#ifndef _RTOS_H
#define _RTOS_H

#include "RTOS.h"
#include "stm32f4xx.h"

/* 库文件 */
#include <stdio.h>
#include <string.h>
#include <math.h>

/* freertos 头文件 */
#include "FreeRTOS.h"
#include "task.h"

/* 数据结构 */
#include "data_struct.h"


/* 片上外设头文件 */
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"

/* 外接设备头文件 */
#include "led.h"
#include "lcd.h"
#include "WiFi_usart3.h"
#include "jx90614.h"
#include "R60ABD1.h"
#include "R60AFD1.h"

void freertos_demo(void);

#endif
