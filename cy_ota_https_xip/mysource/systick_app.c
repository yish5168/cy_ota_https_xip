/*
 * uart_app.c
 *
 *  Created on: Oct 17, 2019
 *      Author: Lwax
 */

#include "systick_app.h"
#include <stdio.h>

/* 系统滴答中断处理回调 */
static void(*pSystickEventFunction)(void);

/* 系统滴答计数器 */
static volatile uint32_t xTickCount = 0U;

void OSAL_SysTickHandler(void)
{
	xTickCount = xTickCount + 1;
    // printf("hi\r\n");
    if (pSystickEventFunction != NULL)
    {
        pSystickEventFunction();
    }
}

void SYSTICK_Init(void)
{
	OSAL_SysTickInit(OS_CFG_TICK_RATE_HZ);
}



