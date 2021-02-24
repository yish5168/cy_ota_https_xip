/*
 * uart_app.c
 *
 *  Created on: Oct 17, 2019
 *      Author: Lwax
 */

#ifndef SOURCE_UART_APP_H_
#define SOURCE_UART_APP_H_

#include "cy_pdl.h"
// #include "cycfg.h"
#include "cyip_scb.h"
#include "cycfg_peripherals.h"
#include "cycfg_clocks.h"

void UART_LOG_Start(void);
void Uart_Init(void);

#endif /* SOURCE_RTC_APP_H_ */
