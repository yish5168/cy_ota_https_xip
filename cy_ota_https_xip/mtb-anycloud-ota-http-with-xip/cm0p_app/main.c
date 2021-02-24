/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for CM0+ in the the Dual CPU Empty 
*              Application for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* (c) (2020), Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cycfg.h"
#include "ipc_def.h"
#include "cycfg_pins.h"
#include <stdio.h>
//#include "uart_app.h"
#include "GpioInt_app.h"
 

/*******************************************************************************
* Macros
********************************************************************************/
/* The application ID */
CY_SECTION(".cy_boot_noinit.appId") __USED static uint8_t cy_boot_appId;

/* The application address */
CY_SECTION(".cy_boot_noinit.appAddr") __USED static uint32_t cy_boot_appAddr;

int main(void)
{
    uint8_t num = 0;
    float table[3];
    init_cycfg_all();
    /* Enable global interrupts */
    __enable_irq();

    ////////////////////////////
    /* Lock the sempahore to wait for CM4 to be init */
   Cy_IPC_Sema_Set(SEMA_NUM, false);

   /* Enable CM4. CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
   //Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR);
    cy_boot_appId = 0;
    Cy_SysEnableCM4(CY_FLASH_BASE + CY_BOOT_BOOTLOADER_SIZE + CY_BOOT_APP_CM0P_SIZE);

   /* Wait till CM4 unlocks the semaphore */
   do
   {
       __WFE();
   }
   while (Cy_IPC_Sema_Status(SEMA_NUM) == CY_IPC_SEMA_STATUS_LOCKED);


    

/*****************************************/
//    UART_LOG_Start();
    CPU_InitGpioInterrupt();
  
    for (;;)
    {
   
    }
}

/* [] END OF FILE */
