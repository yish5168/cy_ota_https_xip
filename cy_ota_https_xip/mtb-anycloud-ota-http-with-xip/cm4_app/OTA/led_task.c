/******************************************************************************
* File Name: led_task.c
*
* Description: This file contains task and functions related to the LED task.
*
*******************************************************************************
* (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
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

/* Header file includes */
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

/* FreeRTOS header file */
#include <FreeRTOS.h>
#include <task.h>

/**********************************************
 * Other configuration
 *********************************************/
/* User LED blink delay */
#ifdef BOOT_IMG
    #define BLINKY_DELAY_MS     (1000u)
#elif defined(UPGRADE_IMG)
    #define BLINKY_DELAY_MS     (250u)
#else
    #error "Please define the image type: BOOT_IMG or UPGRADE_IMG\n"
#endif

/*******************************************************************************
 * Function Name: led_task
 *******************************************************************************
 * Summary:
 *  Task to initialize and toggle the user LED
 *
 * Parameters:
 *  void *args : Task parameter defined during task creation (unused)
 *
 * Return:
 *  void
 *
 *******************************************************************************/
void led_task(void *args)
{
    cy_rslt_t result ;
    /* Initialize the User LED */
#ifdef CYBSP_USER_LED
    result = cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT,
                            CYHAL_GPIO_DRIVE_PULLUP, CYBSP_LED_STATE_OFF);

    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* To avoid compiler warning */
    (void)result;
    
    while( true )
    {
        /* Toggle the state of user LED */
        cyhal_gpio_toggle(CYBSP_USER_LED);

        /* Provide a delay between toggles */
        vTaskDelay(pdMS_TO_TICKS(BLINKY_DELAY_MS));
    }
#endif
}
