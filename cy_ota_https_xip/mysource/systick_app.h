/*
 * uart_app.c
 *
 *  Created on: Oct 17, 2019
 *      Author: Lwax
 */

#ifndef SOURCE_SYSTICK_APP_H_
#define SOURCE_SYSTICK_APP_H_

#include "cy_pdl.h"
// #include "cycfg.h"
#include "cyip_scb.h"
#include "cycfg_peripherals.h"

/* 定义系统滴答频率1000Hz */
#define  OS_CFG_TICK_RATE_HZ   (1000U)

void OSAL_SysTickHandler(void);
#define OSAL_SysTickInit(ticks)      Cy_SysTick_Init( CY_SYSTICK_CLOCK_SOURCE_CLK_IMO,  8000000 / ticks); /* 滴答定时器时钟源: 8000000HZ */ \
                                     Cy_SysTick_SetCallback( 0,  OSAL_SysTickHandler ); //NVIC_SetPriority (SysTick_IRQn, 2);
void SYSTICK_Init(void);

#endif /* SOURCE_SYSTICK_APP_H_ */
