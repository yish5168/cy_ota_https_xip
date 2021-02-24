/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the WiFi Onboarding Using BLE
*              project for ModusToolbox.
*
* Related Document: See Readme.md
*
*******************************************************************************/
/*******************************************************************************
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

#include "cybsp.h"
#include "cyhal_gpio.h"
#include "cy_retarget_io.h"
#include <FreeRTOS.h>
#include <task.h>


#include "cy_log.h"
#include "cybt_platform_trace.h"

#include "ipc_def.h"
 

#include "cy_log.h"
#include "ota_task.h"
#include "led_task.h"
#include "cyabs_rtos.h"
/* App specific configuration */
#include "ota_app_config.h"

#include "cy_serial_flash_qspi.h"
#include "cycfg_qspi_memslot.h"

#define QSPI_BUS_FREQUENCY_HZ   (50000000lu)
#define MEM_SLOT_NUM            (0u)

/* This enables RTOS aware debugging. */
volatile int uxTopUsedPriority;

// #define Lowpower 
/*******************************************************************************
* Macros
********************************************************************************/
/* OTA task configurations */
#define OTA_TASK_STACK_SIZE                 (1024 * 6)
#define OTA_TASK_PRIORITY                   (4)

/* OTA task configurations */
#define LED_TASK_STACK_SIZE                 (configMINIMAL_STACK_SIZE)
#define LED_TASK_PRIORITY                   (1)

/* Button task configurations */
#define BUTTON_TASK_STACK_SIZE              (configMINIMAL_STACK_SIZE*4)
#define BUTTON_TASK_PRIORITY                (1)

/* Power button state enumeration.*/
typedef enum {
    STATE_NO_PRESSED = 0,
    STATE_PRESSED = 1,
    STATE_HOLD_PRESSED = 2,
    STATE_RELEASED = 3,
} button_state_t;

/* button control structure definition.*/
typedef struct {
    button_state_t state;
    uint8_t pressed_cnt;
    uint8_t released_cnt;
} button_ctl_t;

/*******************************************************************************
* Global Variables
********************************************************************************/
/* OTA task handle */
TaskHandle_t ota_task_handle;

/* OTA task parameters */
ota_task_params_t ota_task_params;

/* LED task handle */
TaskHandle_t led_task_handle;

/* Button task handle */
TaskHandle_t button_task_handle;

/* This enables RTOS aware debugging. */
volatile int uxTopUsedPriority;

uint32_t TotalRunTime;
UBaseType_t ArraySize,x;
TaskStatus_t *StatusArray;





/*******************************************************************************
* Function Name: button_task
********************************************************************************
*
* Summary:
*   Check the button status. Use the button to trigger OTA task.
*
* Parameters:
*  arg: Required argument for task function.
*
* Side Effects :
*  None
*
*******************************************************************************/
void button_task(void *arg)
{
    /* Last wake tick time */
    TickType_t lastWakeTime;
    /* Buffer to hold the button control parameter. */
    volatile button_ctl_t button_ctl;
    volatile bool pinReadValue;

    /* OTA task input parameters */
    ota_task_params.isRunning = false;
    ota_task_params.pHostName = HTTP_SERVER;
    ota_task_params.port = HTTP_SERVER_PORT;
#if (MCUBOOT_IMAGE_NUMBER == 2)
    ota_task_params.file0 = "/app-fw-0.bin";
    ota_task_params.file1 = "/app-fw-1.bin";
#else
    ota_task_params.file0 = "/app-fw.bin";
#endif

    (void) arg;
    /* Initialize button control structure */
    button_ctl.state = STATE_NO_PRESSED;
    button_ctl.pressed_cnt = 0;
    button_ctl.released_cnt = 0;
    /* Initialze button pin */
    cyhal_gpio_init(P1_4, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, 1); //david P5_6  P1_4

    printf("creating button task\n");
    while (1) {
        lastWakeTime = xTaskGetTickCount();
        /* Check power button state */
         pinReadValue = cyhal_gpio_read(P1_4);  //david P5_6 P1_4

        if(!pinReadValue) {

            if((STATE_NO_PRESSED == button_ctl.state) && (button_ctl.pressed_cnt < 8)) {
                (button_ctl.pressed_cnt)++;
                if(2 == button_ctl.pressed_cnt)
                    button_ctl.state = STATE_PRESSED;
            }
            button_ctl.released_cnt = 0;
        } else {
            if((STATE_HOLD_PRESSED == button_ctl.state) && (button_ctl.released_cnt < 8)) {
                (button_ctl.released_cnt)++;
                if(2 == button_ctl.released_cnt)
                    button_ctl.state = STATE_RELEASED;
            }
            button_ctl.pressed_cnt = 0;
        }

        /* Power button state changed */
        switch(button_ctl.state) {
            /* Button didn't press */
            case STATE_NO_PRESSED:
                break;
            /* Button has been pressed, need to send this message to host */
            case STATE_PRESSED:
                /* Change the button to hold pressed state */
                button_ctl.state = STATE_HOLD_PRESSED;
                break;
            /* Button has been pressed and held */
            case STATE_HOLD_PRESSED:
                break;
            /* Button has been released, need to send the released message to host */
            case STATE_RELEASED:
                button_ctl.state = STATE_NO_PRESSED;
                /* Create OTA task */
                if(!(ota_task_params.isRunning)) {
                    cy_log_printf("Create OTA task\n");

                    xTaskCreate(ota_task, "OTA TASK", OTA_TASK_STACK_SIZE, &ota_task_params, OTA_TASK_PRIORITY, &ota_task_handle);
                
                } else {
                    /* Print the task list */
                    ArraySize = uxTaskGetNumberOfTasks();
                    StatusArray = pvPortMalloc(ArraySize*sizeof(TaskStatus_t));
                    if(StatusArray!=NULL) {
                        ArraySize = uxTaskGetSystemState((TaskStatus_t* )StatusArray, (UBaseType_t)ArraySize, (uint32_t*)&TotalRunTime);
                        for(x=0; x<ArraySize; x++) {
                            cy_log_printf("Name:%s\tPri:%d\tNum:%d\tState:%d\tStackHighWaterMark:%d\n",
                                StatusArray[x].pcTaskName, (int)StatusArray[x].uxCurrentPriority, (int)StatusArray[x].xTaskNumber,
                                (int)StatusArray[x].eCurrentState, (int)StatusArray[x].usStackHighWaterMark);
                        }
                        vPortFree(StatusArray);
                    }
                }
                break;
            /* Change the button to no pressed state */
            default:
                button_ctl.state = STATE_NO_PRESSED;
            break;
        }

        /* Delay 10 ms till next scan */
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(10));
    }
}


/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM4 CPU
*    1. Initializes the BSP
*    2. Configures the button for interrupt
*    3. Initializes retarget IO for UART debug printing
*    4. Initializes platform configuration
*    5. Initializes BT stack and heap
*    6. Creates WiFi connect and disconnect tasks
*    7. Starts the RTOS scheduler
*
* Return:
*  int
*
*******************************************************************************/
int main()
{
    /* Return status for EEPROM. */
    // cy_en_em_eeprom_status_t eepromReturnValue;
    /* This enables RTOS aware debugging in OpenOCD. */    
    uxTopUsedPriority = configMAX_PRIORITIES - 1;
 
    /* Initialize the board support package */
    if(CY_RSLT_SUCCESS != cybsp_init())
    {
        CY_ASSERT(0);
    }
 
    /* Enable global interrupts */
    __enable_irq();

    /* Unlock the semaphore and wake-up the CM0+ */
    Cy_IPC_Sema_Clear(SEMA_NUM, false);
    __SEV();

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                        CY_RETARGET_IO_BAUDRATE);

    /****Debug Init****/
    cy_log_init(CY_LOG_PRINTF,NULL,NULL);
    //cybt_platform_set_trace_level(CYBT_TRACE_ID_ALL,CYBT_TRACE_LEVEL_MAX);
    
    /* Enable XIP mode to execute Wi-Fi firmware from external memory. */
    cy_serial_flash_qspi_init(smifMemConfigs[MEM_SLOT_NUM], CYBSP_QSPI_D0,
                  CYBSP_QSPI_D1, CYBSP_QSPI_D2, CYBSP_QSPI_D3, NC, NC, NC, NC,
                  CYBSP_QSPI_SCK, CYBSP_QSPI_SS, QSPI_BUS_FREQUENCY_HZ);
    cy_serial_flash_qspi_enable_xip(true);

    cy_serial_flash_qspi_Sleep(1);



    printf("\n*****************************************************\n");
    printf("Wi-Fi OTA example based on XIP Demo\n");
    printf("Hardware        : CY8CKIT-062S2-43012 \n");
    printf("CM4 Build Date  : %s - Time:%s\n", __DATE__, __TIME__);
    printf("TEST Application: OTA Update version: %d.%d.%d\n",
            APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_BUILD);
    printf("*****************************************************\n\n\n");

  
#if 1
    /* Create the tasks */
    xTaskCreate(button_task, "BUTTON TASK", BUTTON_TASK_STACK_SIZE, NULL,
                BUTTON_TASK_PRIORITY, &button_task_handle);

    // xTaskCreate(led_task, "LED TASK", LED_TASK_STACK_SIZE, NULL,
    //             LED_TASK_PRIORITY, &led_task_handle);
#endif
    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler() ;

    /* Should never get here */
    CY_ASSERT(0) ;
}

/* [] END OF FILE */
