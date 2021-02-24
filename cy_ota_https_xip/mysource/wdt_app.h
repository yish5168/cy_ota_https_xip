/*
 * uart_app.c
 *
 *  Created on: Oct 17, 2019
 *      Author: Lwax
 */

#ifndef SOURCE_WDT_APP_H_
#define SOURCE_WDT_APP_H_
#include "cy_pdl.h"
// #include "cycfg.h"
#include "cy_mcwdt.h"


void Watchdog_Feed(void);
void CPU_InitWatchdog(void);

#endif /* SOURCE_MCWDT_APP_H_ */
