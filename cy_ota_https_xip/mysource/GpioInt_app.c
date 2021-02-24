/*
 * GpioInt.c
 *
 *  Created on: Oct 17, 2019
 *      Author: Lwax
 */

#include "GpioInt_app.h"
#include "stdio.h"
#include "irq_cfg.h"

uint8_t gsensor_symbol = 0;
void GPIO_IRQHandler(void)
{
	printf("io wake= %x \r\n",AllGpioInt_cfg.intrSrc);

	/* Clears the triggered pin interrupt */
	if(1UL == Cy_GPIO_GetInterruptStatus(KEY_RESET_PORT,KEY_RESET_PIN))
	{
		Cy_GPIO_ClearInterrupt(KEY_RESET_PORT, KEY_RESET_PIN);
		printf("EXT_INT interrupt = %x \r\n",AllGpioInt_cfg.intrSrc);
	}

	// if(1UL == Cy_GPIO_GetInterruptStatus(KEY_BUTTON_PORT,KEY_BUTTON_PIN))
	// {
	// 	Cy_GPIO_ClearInterrupt(KEY_BUTTON_PORT,KEY_BUTTON_PIN);
	// 	printf("DESTROY_CHK interrupt= %x \r\n",AllGpioInt_cfg.intrSrc);
	// }

	if(1UL == Cy_GPIO_GetInterruptStatus(G_SENSOR_INT_PORT,G_SENSOR_INT_PIN))
	{
		Cy_GPIO_ClearInterrupt(G_SENSOR_INT_PORT,G_SENSOR_INT_PIN);
		printf("G_SENSOR_INT interrupt= %x \r\n",AllGpioInt_cfg.intrSrc);
		gsensor_symbol = 1;
	}
	NVIC_ClearPendingIRQ(AllGpioInt_cfg.intrSrc);
}

void CPU_InitGpioInterrupt(void)
{
    /* Initialize and enable GPIO interrupt assigned to CM0+ */
	Cy_SysInt_Init(&AllGpioInt_cfg, GPIO_IRQHandler);
	NVIC_ClearPendingIRQ(AllGpioInt_cfg.intrSrc);
	NVIC_EnableIRQ(AllGpioInt_cfg.intrSrc);
	printf("GPIO interrupt init \r\n");
}

