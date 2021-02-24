/******************************************************************************
* File Name: ota_task.c
*
* Description: This file contains task and functions related to OTA operation.
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

/* Wi-Fi connection manager header files. */
#include "cy_wcm.h"

/* IoT SDK, Secure Sockets initialization */
#include "iot_init.h"
#include "cy_iot_network_secured_socket.h"

/* FreeRTOS header file */
#include <FreeRTOS.h>
#include <task.h>

/* OTA API */
#include "cy_ota_api.h"

/* App specific configuration */
#include "ota_app_config.h"

#ifdef CY_BOOT_USE_EXTERNAL_FLASH
#include "cy_smif_psoc6.h"
#endif

#include "cyabs_rtos.h"
#include "ota_task.h"
#include "cy_lwip.h"


#define EN_WIFI_AP            0 //1:enable wifi AP 0:disable wifi AP
/*******************************************************************************
* Macros
*******************************************************************************/
/* MAX connection retries to join WI-FI AP */
#define MAX_CONNECTION_RETRIES              (10u)

/* Wait between connection retries */
#define WIFI_CONN_RETRY_DELAY_MS            (100)  //david

/* OTA task events */
typedef enum
{
    
    OTA_TASK_EVENT_COMPLETE             = (1 <<  0),    /**< Notify OTA complete    */
    OTA_TASK_EVENT_FAILURE              = (1 <<  1),    /**< Notify OTA failure     */
} ota_task_events_t;

/*******************************************************************************
* Forward declaration
*******************************************************************************/
cy_rslt_t connect_to_wifi_ap(void);
cy_ota_callback_results_t ota_callback(cy_ota_cb_struct_t *cb_data);

//extern cy_rslt_t wlan_powersave_handler(struct netif *wifi, enum wlan_powersave_mode_t mode);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* OTA context */
cy_ota_context_ptr ota_context;

/* HTTP Credentials for OTA */
struct IotNetworkCredentials credentials =
{
    .pRootCa = ROOT_CA_CERTIFICATE,
    .rootCaSize = sizeof(ROOT_CA_CERTIFICATE),
    .pClientCert = CLIENT_CERTIFICATE,
    .clientCertSize = sizeof(CLIENT_CERTIFICATE),
    .pPrivateKey = CLIENT_KEY,
    .privateKeySize = sizeof(CLIENT_KEY),
};

/* Server parameters for OTA */
cy_ota_server_info_t ota_server_info = {
    .pHostName = HTTP_SERVER,
    .port = HTTP_SERVER_PORT
};

/* Network parameters for OTA */
cy_ota_network_params_t ota_network_params =
{
    .http =
    {
        .server =
        {
            .pHostName = HTTP_SERVER,
            .port = HTTP_SERVER_PORT
        },
        .file = OTA_HTTP_JOB_FILE,
#if (ENABLE_TLS == true)
        .credentials = &credentials
#else
        .credentials = NULL
#endif
    },
    .use_get_job_flow = CY_OTA_JOB_FLOW,
#if (ENABLE_TLS == true)
    .initial_connection = CY_OTA_CONNECTION_HTTPS,
#else
    .initial_connection = CY_OTA_CONNECTION_HTTP
#endif
};

/* Parameters for OTA agent */
cy_ota_agent_params_t ota_agent_params =
{
    .cb_func = ota_callback,
    .cb_arg = &ota_context,
    .reboot_upon_completion = 1,
};

/* Event signaling @ref ota_task_events_t */
cy_event_t ota_task_event;
volatile uint8_t ota_agent_stop_flag = 0;


  


/*******************************************************************************
 * Function Name: ota_task
 *******************************************************************************
 * Summary:
 *  Task to initialize required libraries and start OTA agent.
 *
 * Parameters:
 *  void *args : Task parameter defined during task creation (unused)
 *
 * Return:
 *  void
 *
 ******************************************************************************/
void ota_task(void *args)
{
    uint32_t waitfor;
    volatile uint8_t ota_task_status = 0;

    if(args == NULL) {
        printf("\n OTA task parameter invaild.\n");
        CY_ASSERT(0);
    }
    ((ota_task_params_t *)args)->isRunning = true;

 
 

//#if (EN_WIFI_AP == 1)

    printf("Prepare to conect AP\n");
    /* Connect to Wi-Fi AP */
    if( connect_to_wifi_ap() != CY_RSLT_SUCCESS )
    {
        printf("\n Failed to connect to Wi-FI AP.\n");
        CY_ASSERT(0);
    }


#if 1   //david security MQTT using
    /* Initialize the underlying support code that is needed for OTA */
    if ( !IotSdk_Init() )
    {
        printf("\n IotSdk_Init Failed.\n");
        CY_ASSERT(0);
    }


    /* Call the Network Secured Sockets initialization function. */
    if( IotNetworkSecureSockets_Init() != IOT_NETWORK_SUCCESS )
    {
        printf("\n IotNetworkSecureSockets_Init Failed.\n");
        CY_ASSERT(0);
    }
#endif

    /* create event flags */
    if (cy_rtos_init_event(&ota_task_event) != CY_RSLT_SUCCESS)
    {
        /* Event create failed */
        printf("%s() OTA Task Event Create Failed!\n", __func__);
        CY_ASSERT(0);
    }

    /* Add the network interface to the OTA network parameters */
    ota_network_params.network_interface = (void *)IOT_NETWORK_INTERFACE_CY_SECURE_SOCKETS;

    /* Update the sencondary 0 image */
    printf("\r\n\r\n************Update the sencondary 0 image**************\r\n");
    ota_task_status = 0;
    ota_agent_stop_flag = 0;
    /* Set the network params: http server, http port, update file, image number */
    ota_network_params.http.server.pHostName = ((ota_task_params_t *)args)->pHostName;
    ota_network_params.http.server.port = ((ota_task_params_t *)args)->port;
    ota_network_params.http.file = ((ota_task_params_t *)args)->file0;
    ota_network_params.image_num = 0;
    ota_network_params.use_get_job_flow = CY_OTA_DIRECT_FLOW;
    /* Set the OTA agent params */
    ota_agent_params.reboot_upon_completion = 0;
    /* Initialize and start the OTA agent */
    if( cy_ota_agent_start(&ota_network_params, &ota_agent_params, &ota_context) != CY_RSLT_SUCCESS ) {
        printf("\n Initializing and starting the OTA agent failed.\n");
        CY_ASSERT(0);
    }
    /* wait for the OTA sencondary 0 complete or failure */
    while (1) {
        waitfor = OTA_TASK_EVENT_COMPLETE | OTA_TASK_EVENT_FAILURE;
        if (cy_rtos_waitbits_event(&ota_task_event, &waitfor, 1, 0, CY_RTOS_NEVER_TIMEOUT) != CY_RSLT_SUCCESS) {
            CY_ASSERT(0);
        }
        if(waitfor & OTA_TASK_EVENT_COMPLETE) {
            printf("OTA sencondary 0 complete\r\n");
            break;
        }
        if(waitfor & OTA_TASK_EVENT_FAILURE) {
            ota_task_status = 1;
            printf("OTA sencondary 0 fail\r\n");
        }
    }
    /* Stop the TA sencondary 0 task */
    if (cy_ota_agent_stop(&ota_context) != CY_RSLT_SUCCESS) {
        printf("Stop OTA sencondary 0 error!\r\n");
        CY_ASSERT(0);
    }
    if(0 != ota_task_status) {
        printf("************Update the sencondary 0 FAIL**************\r\n");
        goto _ota_task_exit;
    }

#if (MCUBOOT_IMAGE_NUMBER == 2)
    /* Update the sencondary 1 image */
    printf("\r\n\r\n************Update the sencondary 1 image**************\r\n");
    ota_task_status = 0;
    ota_agent_stop_flag = 0;
    /* Set the network params: http server, http port, update file, image number */
    ota_network_params.http.server.pHostName = ((ota_task_params_t *)args)->pHostName;
    ota_network_params.http.server.port = ((ota_task_params_t *)args)->port;
    ota_network_params.http.file = ((ota_task_params_t *)args)->file1;
    ota_network_params.image_num = 1;
    ota_network_params.use_get_job_flow = CY_OTA_DIRECT_FLOW;
    /* Set the OTA agent params */
    ota_agent_params.reboot_upon_completion = 0;
    /* Initialize and start the OTA agent */
    if( cy_ota_agent_start(&ota_network_params, &ota_agent_params, &ota_context) != CY_RSLT_SUCCESS ) {
        printf("\n Initializing and starting the OTA agent failed.\n");
        CY_ASSERT(0);
    }
    /* wait for the OTA sencondary 1 complete or failure */
    while (1) {
        waitfor = OTA_TASK_EVENT_COMPLETE | OTA_TASK_EVENT_FAILURE;
        if (cy_rtos_waitbits_event(&ota_task_event, &waitfor, 1, 0, CY_RTOS_NEVER_TIMEOUT) != CY_RSLT_SUCCESS) {
            CY_ASSERT(0);
        }
        if(waitfor & OTA_TASK_EVENT_COMPLETE) {
            printf("OTA sencondary 1 complete\r\n");
            break;
        }
        if(waitfor & OTA_TASK_EVENT_FAILURE) {
            ota_task_status = 1;
            printf("OTA sencondary 1 fail\r\n");
        }
    }
    /* Stop the TA sencondary 0 task */
    if (cy_ota_agent_stop(&ota_context) != CY_RSLT_SUCCESS) {
        printf("Stop OTA sencondary 1 error!\r\n");
        CY_ASSERT(0);
    }
    if(0 != ota_task_status) {
        printf("************Update the sencondary 1 FAIL**************\r\n");
        goto _ota_task_exit;
    }
    printf("************Update the sencondary 0 and 1 complete**************\r\n");
#endif

_ota_task_exit:
    cy_rtos_deinit_event(&ota_task_event);
#if (EN_WIFI_AP == 1)
    IotNetworkSecureSockets_Cleanup();
    IotSdk_Cleanup();
#endif
    printf("OTA task end\r\n");

    ((ota_task_params_t *)args)->isRunning = false;
    
    vTaskDelete(NULL);
 }

#if 1
/*******************************************************************************
 * Function Name: connect_to_wifi_ap()
 *******************************************************************************
 * Summary:
 *  Connects to Wi-Fi AP using the user-configured credentials, retries up to a
 *  configured number of times until the connection succeeds.
 *
 ******************************************************************************/



cy_rslt_t connect_to_wifi_ap(void)
{
    cy_wcm_config_t wifi_config = { .interface = CY_WCM_INTERFACE_TYPE_STA};
    cy_wcm_connect_params_t wifi_conn_param;
    cy_wcm_ip_address_t ip_address;
    cy_rslt_t result;

    /* Variable to track the number of connection retries to the Wi-Fi AP specified
     * by WIFI_SSID macro. */
    uint32_t conn_retries = 0;

    /* Initialize Wi-Fi connection manager. */
    cy_wcm_init(&wifi_config);
    printf("wcm intitaled\n");
     /* Set the Wi-Fi SSID, password and security type. */
    memset(&wifi_conn_param, 0, sizeof(cy_wcm_connect_params_t));
    memcpy(wifi_conn_param.ap_credentials.SSID, WIFI_SSID, sizeof(WIFI_SSID));
    memcpy(wifi_conn_param.ap_credentials.password, WIFI_PASSWORD, sizeof(WIFI_PASSWORD));
    wifi_conn_param.ap_credentials.security = WIFI_SECURITY;

  
    /* Connect to the Wi-Fi AP */
    for(conn_retries = 0; conn_retries < MAX_CONNECTION_RETRIES; conn_retries++)
    {
        printf("Connecting to Wi-Fi AP '%s','%s'\n", wifi_conn_param.ap_credentials.SSID,wifi_conn_param.ap_credentials.password);
        result = cy_wcm_connect_ap( &wifi_conn_param, &ip_address );

        if (result == CY_RSLT_SUCCESS)
        {
            printf( "Successfully connected to Wi-Fi network '%s'.\n",
                    wifi_conn_param.ap_credentials.SSID);
            return result;
        }

        printf( "Connection to Wi-Fi network failed with error code %d."
                "Retrying in %d ms...\n", (int) result, WIFI_CONN_RETRY_DELAY_MS );
        vTaskDelay(pdMS_TO_TICKS(WIFI_CONN_RETRY_DELAY_MS));
    }

    printf( "Exceeded maximum Wi-Fi connection attempts\n" );

    return result;
}
#endif

/*******************************************************************************
 * Function Name: ota_callback()
 *******************************************************************************
 * Summary:
 *  Prints the status of the OTA agent on every event. This callback is optional,
 *  but be aware that the OTA middleware will not print the status of OTA agent
 *  on its own.
 *
 * Return:
 *  CY_OTA_CB_RSLT_OTA_CONTINUE - OTA Agent to continue with function.
 *  CY_OTA_CB_RSLT_OTA_STOP     - OTA Agent to End current update session.
 *  CY_OTA_CB_RSLT_APP_SUCCESS  - Application completed task, success.
 *  CY_OTA_CB_RSLT_APP_FAILED   - Application completed task, failure.
 *
 *******************************************************************************/
cy_ota_callback_results_t ota_callback(cy_ota_cb_struct_t *cb_data)
{
    cy_ota_callback_results_t   cb_result = CY_OTA_CB_RSLT_OTA_CONTINUE;
    const char                  *state_string;
    const char                  *error_string;

    if (cb_data == NULL)
    {
        return CY_OTA_CB_RSLT_OTA_STOP;
    }

    state_string  = cy_ota_get_state_string(cb_data->state);
    error_string  = cy_ota_get_error_string(cy_ota_get_last_error());

    switch (cb_data->reason)
    {

        case CY_OTA_LAST_REASON:
            break;

        case CY_OTA_REASON_SUCCESS:
            printf(">> APP CB OTA SUCCESS state:%d %s last_error:%s\n\n", cb_data->state, state_string, error_string);
            break;

        case CY_OTA_REASON_FAILURE:
            printf(">> APP CB OTA FAILURE state:%d %s last_error:%s\n\n", cb_data->state, state_string, error_string);
            cb_result = CY_OTA_CB_RSLT_OTA_STOP;
            cy_rtos_setbits_event(&ota_task_event, OTA_TASK_EVENT_FAILURE, 0);
            ota_agent_stop_flag = 1;
            break;

        case CY_OTA_REASON_STATE_CHANGE:
            switch (cb_data->state)
            {
                case CY_OTA_STATE_NOT_INITIALIZED:
                case CY_OTA_STATE_EXITING:
                case CY_OTA_STATE_INITIALIZING:
                case CY_OTA_STATE_AGENT_STARTED:
                    break;

                case CY_OTA_STATE_AGENT_WAITING:
                    if(1 == ota_agent_stop_flag) {
                        cy_rtos_setbits_event(&ota_task_event, OTA_TASK_EVENT_COMPLETE, 0);
                    }
                    break;

                case CY_OTA_STATE_START_UPDATE:
                    printf("APP CB OTA STATE CHANGE CY_OTA_STATE_START_UPDATE\n");
                    break;

                case CY_OTA_STATE_JOB_CONNECT:
                    printf("APP CB OTA CONNECT FOR JOB using ");
                    /* NOTE:
                     *  HTTP - json_doc holds the HTTP "GET" request
                     */
                    if ((cb_data->broker_server.pHostName == NULL) ||
                        ( cb_data->broker_server.port == 0) ||
                        ( strlen(cb_data->file) == 0) )
                    {
                        printf("ERROR in callback data: HTTP: server: %p port: %d topic: '%p'\n",
                                cb_data->broker_server.pHostName, cb_data->broker_server.port, cb_data->file);
                        return CY_OTA_CB_RSLT_OTA_STOP;
                    }
                    printf("HTTP: server:%s port: %d file: '%s'\n",
                            cb_data->broker_server.pHostName, cb_data->broker_server.port, cb_data->file);

                    break;

                case CY_OTA_STATE_JOB_DOWNLOAD:
                    printf("APP CB OTA JOB DOWNLOAD using ");
                    /* NOTE:
                     *  HTTP - json_doc holds the HTTP "GET" request
                     */
                    printf("HTTP: '%s'\n", cb_data->file);
                    break;

                case CY_OTA_STATE_JOB_DISCONNECT:
                    printf("APP CB OTA JOB DISCONNECT\n");
                    break;

                case CY_OTA_STATE_JOB_PARSE:
                    printf("APP CB OTA PARSE JOB: '%.*s' \n", strlen(cb_data->json_doc), cb_data->json_doc);
                    break;

                case CY_OTA_STATE_JOB_REDIRECT:
                    printf("APP CB OTA JOB REDIRECT\n");
                    break;

                case CY_OTA_STATE_DATA_CONNECT:
                    printf("APP CB OTA CONNECT FOR DATA using ");
                    printf("HTTP: %s:%d \n", cb_data->broker_server.pHostName, cb_data->broker_server.port);
                    break;

                case CY_OTA_STATE_DATA_DOWNLOAD:
                    printf("APP CB OTA DATA DOWNLOAD using ");
                    /* NOTE:
                     *  HTTP - json_doc holds the HTTP "GET" request
                     */
                    printf("HTTP: '%.*s' ", strlen(cb_data->json_doc), cb_data->json_doc);
                    printf("File: '%s'\n\n", cb_data->file);
                    break;

                case CY_OTA_STATE_DATA_DISCONNECT:
                    printf("APP CB OTA DATA DISCONNECT\n");
                    break;

                case CY_OTA_STATE_RESULT_CONNECT:
                    printf("APP CB OTA SEND RESULT CONNECT using ");
                    /* NOTE:
                     *  HTTP - json_doc holds the HTTP "GET" request
                     */
                    printf("HTTP: Server:%s port: %d\n", cb_data->broker_server.pHostName, cb_data->broker_server.port);
                    break;

                case CY_OTA_STATE_RESULT_SEND:
                    printf("APP CB OTA SENDING RESULT using ");
                    /* NOTE:
                     *  HTTP - json_doc holds the HTTP "PUT"
                     */
                    printf("HTTP: '%s' \n", cb_data->json_doc);
                    break;

                case CY_OTA_STATE_RESULT_RESPONSE:
                    printf("APP CB OTA Got Result response\n");
                    break;

                case CY_OTA_STATE_RESULT_DISCONNECT:
                    printf("APP CB OTA Result Disconnect\n");
                    break;

                case CY_OTA_STATE_OTA_COMPLETE:
                    printf("APP CB OTA Session Complete\n");
                    ota_agent_stop_flag = 1;
                    break;

                case CY_OTA_STATE_STORAGE_OPEN:
                    printf("APP CB OTA STORAGE OPEN\n");
                    break;

                case CY_OTA_STATE_STORAGE_WRITE:
                    printf("APP CB OTA STORAGE WRITE %ld%% (%ld of %ld)\n", cb_data->percentage, cb_data->bytes_written, cb_data->total_size);
                    break;

                case CY_OTA_STATE_STORAGE_CLOSE:
                    printf("APP CB OTA STORAGE CLOSE\n");
                    break;

                case CY_OTA_STATE_VERIFY:
                    printf("APP CB OTA VERIFY\n");
                    break;

                case CY_OTA_STATE_RESULT_REDIRECT:
                    printf("APP CB OTA RESULT REDIRECT\n");
                    break;

                case CY_OTA_NUM_STATES:
                    break;
            }   /* switch state */
            break;
    }

    return cb_result;
}
