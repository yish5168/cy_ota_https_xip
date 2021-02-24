#include "irq_cfg.h"

// #define MY_CHAN_CRYPTO          (uint32_t)(9u)       /* 加密的IPC数据通道 */
// #define MY_INTR_CRYPTO_SRV      (uint32_t)(1u)       /* 加密服务器的IPC中断结构 */
// #define MY_INTR_CRYPTO_CLI      (uint32_t)(2u)       /* 加密客户端的IPC中断结构 */

// const cy_stc_crypto_config_t cryptoConfig =
// {
//         /* .ipcChannel             */ MY_CHAN_CRYPTO,
//         /* .acquireNotifierChannel */ MY_INTR_CRYPTO_SRV,
//         /* .releaseNotifierChannel */ MY_INTR_CRYPTO_CLI,
//         /* .releaseNotifierConfig */ {
// #if (CY_CPU_CORTEX_M0P)
//             /* .intrSrc            */ NvicMux0_IRQn,/*CM0 + IPC中断多路复用编号Crypto客户端*/
//             /* .cm0pSrc            */ cpuss_interrupts_ipc_2_IRQn,
// #else
//             /* .intrSrc            */ cpuss_interrupts_ipc_2_IRQn,
// #endif
//             /* .intrPriority       */ 2u,
//         },
//         /* .userCompleteCallback   */ NULL,
//         /* .userGetDataHandler     */ NULL,
//         /* .userErrorHandler       */ NULL,
//         /* .acquireNotifierConfig */ {
// #if (CY_CPU_CORTEX_M0P)
//             /* .intrSrc            */ NvicMux1_IRQn,               /*CM0 + IPC中断多路复用号Crypto服务器 to use with DeepSleep mode should be in DeepSleep capable muxer's range */
//             /* .cm0pSrc            */ cpuss_interrupts_ipc_1_IRQn, /* depends on selected acquireNotifierChannel value */
// #else
//             /* .intrSrc            */ cpuss_interrupts_ipc_1_IRQn, /* depends on selected acquireNotifierChannel value */
// #endif
//             /* .intrPriority       */ 2u,
//         },
//         /* .cryptoErrorIntrConfig */ {
// #if (CY_CPU_CORTEX_M0P)
//             /* .intrSrc            */ NvicMux2_IRQn,    /*CM0 +错误中断多路复用器编号加密服务器*/
//             /* .cm0pSrc            */ cpuss_interrupt_crypto_IRQn,
// #else
//             /* .intrSrc            */ cpuss_interrupt_crypto_IRQn,
// #endif
//             /* .intrPriority       */ 2u,
//         }
// };

/*M0核中断*/
const cy_stc_sysint_t WDT_IRQ_cfg =
{
    .intrSrc = (IRQn_Type)NvicMux2_IRQn,
    .cm0pSrc = (cy_en_intr_t)srss_interrupt_IRQn,
    .intrPriority = 2u,
};

/* AllGpioInt */
const cy_stc_sysint_t AllGpioInt_cfg = {
    .intrSrc = (IRQn_Type)NvicMux3_IRQn,
    .cm0pSrc = (cy_en_intr_t)ioss_interrupts_gpio_5_IRQn,
    .intrPriority = 3u,
};
#if defined(CapSense_HW)
const cy_stc_sysint_t CapSense_ISR_cfg ={
    .intrSrc = (IRQn_Type)NvicMux4_IRQn,
    .cm0pSrc = (cy_en_intr_t)CapSense_IRQ,
    .intrPriority = 2u,
};
#endif
#if defined(UART_REAR_HW)
/* USART_EXT_SCB_IRQ */
const cy_stc_sysint_t UART_REAR_SCB_IRQ_cfg = {
    .intrSrc = (IRQn_Type)NvicMux5_IRQn,
    .cm0pSrc = (cy_en_intr_t)UART_REAR_IRQ,
    .intrPriority = 3u,
};
#endif
#if defined(FACE_SUPPORT)
/* USART_FACE_SCB_IRQ */
const cy_stc_sysint_t UART_FACE_SCB_IRQ_cfg = {
    .intrSrc = (IRQn_Type) NvicMux5_IRQn,
    .cm0pSrc = (cy_en_intr_t)UART_FACE_IRQ,
    .intrPriority = 4u,
};

/* USART_FACE_DMA_IRQ */
// const cy_stc_sysint_t UART_FACE_DMA_IRQ_cfg = {
//     .intrSrc = (IRQn_Type) NvicMux7_IRQn,
//     .cm0pSrc = (cy_en_intr_t)DMA_UART_TX_IRQ,
//     .intrPriority = 4u,
// };
const  cy_stc_sysint_t WifiToFace_IRQ_cfg =
{
        .intrSrc = (IRQn_Type) NvicMux7_IRQn,
        .cm0pSrc = (cy_en_intr_t)WifiToFace_IRQ,
        .intrPriority = 3u,
};

const  cy_stc_sysint_t FaceToWifi_IRQ_cfg =
{
        .intrSrc = (IRQn_Type) NvicMux7_IRQn,
        .cm0pSrc = (cy_en_intr_t)FaceToWifi_IRQ,
        .intrPriority = 3u,
};
#endif
// const cy_stc_sysint_t PWM_MOTO_1_cfg =
// {
//     .intrSrc = (IRQn_Type)NvicMux6_IRQn,
//     .cm0pSrc = (cy_en_intr_t)PWM_MOTO_1_IRQ,
//     .intrPriority = 2u,
// };

// const cy_stc_sysint_t PWM_MOTO_2_cfg =
// {
//     .intrSrc = (IRQn_Type)NvicMux6_IRQn,
//     .cm0pSrc = (cy_en_intr_t)PWM_MOTO_2_IRQ,
//     .intrPriority = 2u,
// };

#ifdef RGB_R_PORT
const cy_stc_sysint_t PWM_RGB_R_cfg =
{
    .intrSrc = (IRQn_Type)NvicMux6_IRQn,
    .cm0pSrc = (cy_en_intr_t)PWM_RGB_R_IRQ,
    .intrPriority = 3u,
};

const cy_stc_sysint_t PWM_RGB_G_cfg =
{
    .intrSrc = (IRQn_Type)NvicMux6_IRQn,
    .cm0pSrc = (cy_en_intr_t)PWM_RGB_G_IRQ,
    .intrPriority = 4u,
};

const cy_stc_sysint_t PWM_RGB_B_cfg =
{
    .intrSrc = (IRQn_Type)NvicMux6_IRQn,
    .cm0pSrc = (cy_en_intr_t)PWM_RGB_B_IRQ,
    .intrPriority = 5u,
};
#endif
