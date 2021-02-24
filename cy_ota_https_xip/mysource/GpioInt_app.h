/*
 * rtc_app.h
 *
 *  Created on: Oct 17, 2019
 *      Author: Lwax
 */

#ifndef SOURCE_GPIOINT_H_
#define SOURCE_GPIOINT_H_

#include "cy_pdl.h"
// #include "cycfg.h"
#include "cycfg_pins.h"
extern uint8_t gsensor_symbol;
void GPIO_IRQHandler(void);
void CPU_InitGpioInterrupt(void);

#endif /* SOURCE_RTC_APP_H_ */
