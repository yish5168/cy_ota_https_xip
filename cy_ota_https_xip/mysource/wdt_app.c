
#include "cy_pdl.h"
//#include "cybsp.h"
#include "WDT_app.h"
#include "stdio.h"
#include "irq_cfg.h"

/* WDT periodic interrupt timing */
#define WDT_INTERRUPT_INTERVAL              300  //ms  1000

/* Match count =  Desired interrupt interval in seconds x ILO Frequency in Hz */
#define WDT_MATCH_COUNT                     WDT_INTERRUPT_INTERVAL * CY_SYSCLK_ILO_FREQ / 1000

static __IO uint32_t interruptCount = 0;

static void WakeTimer_IRQHandler(void)
{
	// Cy_GPIO_Inv(TEST_PIN_PORT, TEST_PIN_NUM);

	if ((++interruptCount) <= 3)
	{
		if (SRSS_SRSS_INTR & SRSS_SRSS_INTR_WDT_MATCH_Msk)
		{
			/* Clear WDT Interrupt */
			Cy_WDT_ClearInterrupt();

			/* Update the match count */
			Cy_WDT_Unlock();

			/* Update the match count. Note that, if the ignore bits in the match is set
			 * to a non-zero value, the below code requires a modification. */
			Cy_WDT_SetMatch((uint16_t)(Cy_WDT_GetMatch() + WDT_MATCH_COUNT));
			Cy_WDT_Lock();
		}
        // printf("hi\r\n");
		// Device_IRQHandler(NULL, ISR_WAKE_TIMER);
	}
	else
	{
		interruptCount = 3;
	}
}

void Watchdog_Feed(void)
{
	/* 锟节诧拷喂锟斤拷 */
	interruptCount = 0;

#if defined(WDT_DONE_PORT)
	Cy_GPIO_Clr(WDT_DONE_PORT, WDT_DONE_NUM);
	CyDelayUs(1);
	Cy_GPIO_Set(WDT_DONE_PORT, WDT_DONE_NUM);
#endif
}

void CPU_InitWatchdog(void)
{
     /* Step 1- Unlock WDT */
    Cy_WDT_Unlock();

    /* Step 2- Write the ignore bits - operate with full 16 bits */
    Cy_WDT_SetIgnoreBits(0);

    /* Step 3- Write match value */
    Cy_WDT_SetMatch(WDT_MATCH_COUNT);

    /* Step 4- Clear match event interrupt, if any */
    Cy_WDT_ClearInterrupt();

    /* Step 5- Enable ILO */
    Cy_SysClk_IloEnable();

    /* Step 6 - Enable interrupt if periodic interrupt mode selected */

    Cy_SysInt_Init(&WDT_IRQ_cfg, WakeTimer_IRQHandler);
    NVIC_EnableIRQ(WDT_IRQ_cfg.intrSrc);
    Cy_WDT_UnmaskInterrupt();

    /* Step 7- Enable WDT */
    Cy_WDT_Enable();

    /* Step 8- Lock WDT configuration */
    Cy_WDT_Lock();
}

/* [] END OF FILE */
