/*
 * uart_app.c
 *
 *  Created on: Oct 17, 2019
 *      Author: Lwax
 */

#include "uart_app.h"
#include <stdio.h>
#include "irq_cfg.h"

__IO static uint8_t uart_RearRxData;

cy_stc_scb_uart_context_t UART_LOG_context;
static cy_stc_scb_uart_context_t UART_REAR_context;
static uint8_t UART_LOG_initVar = 0;
static uint8_t UART_REAR_initVar = 0;
#if 0
/**
  * @brief  
  *
  * @param  
  */
static void UART_REAR_Callback(uint32_t event)
{
	if (event == CY_SCB_UART_TRANSMIT_IN_FIFO_EVENT)
	{
	}
	else if (event == CY_SCB_UART_TRANSMIT_IN_FIFO_EVENT)
	{
	}
	else if (event == CY_SCB_UART_TRANSMIT_DONE_EVENT)
	{
	}
	else if (event == CY_SCB_UART_RECEIVE_DONE_EVENT)
	{
		// OSAL_RingBufferWriteFromISR(uartExt_rbHandle, (void *)&uart_RearRxData);
		printf("%c",uart_RearRxData);
		Cy_SCB_UART_Receive(UART_REAR_HW, (void *)&uart_RearRxData, sizeof(uart_RearRxData), &UART_REAR_context);
	}
	else if (event == CY_SCB_UART_RB_FULL_EVENT)
	{
	}
	else if (event == CY_SCB_UART_RECEIVE_ERR_EVENT)
	{
	}
	else if (event == CY_SCB_UART_TRANSMIT_ERR_EVENT)
	{
	}
}

static void UART_REAR_Interrupt(void)
{
    Cy_SCB_UART_Interrupt(UART_REAR_HW, &UART_REAR_context);
}

/**
  * @brief  人脸串口和扩展模块串口共用中断入口
  *
  * @param  
  */
void ShareIntIsr(void) 
{
	if (UART_REAR_SCB_IRQ_cfg.cm0pSrc == Cy_SysInt_GetInterruptActive(UART_REAR_SCB_IRQ_cfg.intrSrc) )
	{
		UART_REAR_Interrupt();
	}
#if defined(FACE_SUPPORT)
	else
	{
		UART_FACE_Interrupt();
	}
#endif
}

static void UART_REAR_Start(void)
{
	if (0U == UART_REAR_initVar)
    {
        /* Configure component */
        (void) Cy_SCB_UART_Init(UART_REAR_HW, &UART_REAR_config, &UART_REAR_context);

        (void) Cy_SysInt_Init(&UART_REAR_SCB_IRQ_cfg, &ShareIntIsr);
    
        /* Component is configured */
        UART_REAR_initVar = 1U;
    }

    /* Enable interrupt in NVIC */
    NVIC_EnableIRQ((IRQn_Type) UART_REAR_SCB_IRQ_cfg.intrSrc);

    Cy_SCB_UART_Enable(UART_REAR_HW);
}

/**
  * @brief 
  * @note
  *
  */
void Uart_Init(void)
{
	UART_REAR_Start();

	Cy_SCB_UART_RegisterCallback(UART_REAR_HW, UART_REAR_Callback, &UART_REAR_context);

	Cy_SCB_UART_Receive(UART_REAR_HW, (void *)&uart_RearRxData, sizeof(uart_RearRxData), &UART_REAR_context);

	// if (NULL == uartExt_rbHandle)
	// {
	// 	uartExt_rbHandle = OSAL_RingBufferCreate(UART_REAR_RB_RXBUF_LENGTH, 1);
	// 	if (uartExt_rbHandle == NULL)
	// 	{
	// 		printf("uartExt_rbHandle creator is error\r\n");
	// 	}
	// }
}

/**
  * @brief  
  * @note
  *
  * @param  baudRate
  */
void Uart_SetBaudrate(uint32_t baudrate)
{
	/* ���÷�Ƶֵ��CLK_PeriΪ48M, ��������Ϊ8�� */
	(void)Cy_SysClk_PeriphSetDivider(UART_REAR_CLOCK_HW, UART_REAR_CLOCK_NUM, 48000000 / 8 / baudrate - 1);
	CyDelay(10);
	// OSAL_RingBufferReset(uartExt_rbHandle);
}
#endif
void UART_LOG_Start(void)
{
	if (UART_LOG_initVar == 0)
	{
		Cy_SCB_UART_Init(UART_LOG_HW, &UART_LOG_config, &UART_LOG_context);
		UART_LOG_initVar = 1;
	}
    Cy_SCB_UART_Enable(UART_LOG_HW);
	
    Cy_SCB_UART_PutString(UART_LOG_HW, "This is UART example, which uses User ISR to demonstrate UART operation\r\n");
    Cy_SCB_UART_PutString(UART_LOG_HW, "If you are able to read this text the terminal connection is configured correctly.\r\n");
    Cy_SCB_UART_PutString(UART_LOG_HW, "Start transmitting the characters to see an echo in the terminal.\r\n");
    Cy_SCB_UART_PutString(UART_LOG_HW, "\r\n");

}



