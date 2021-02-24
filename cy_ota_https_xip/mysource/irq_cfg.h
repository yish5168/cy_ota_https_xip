#ifndef _IRQ_CFG_H_
#define _IRQ_CFG_H_
#include <cy_pdl.h>
// #include "cycfg.h"
#include "cycfg_peripherals.h"

extern const cy_stc_crypto_config_t cryptoConfig;

extern const cy_stc_sysint_t WDT_IRQ_cfg;

/* AllGpioInt */
extern const cy_stc_sysint_t AllGpioInt_cfg;

extern const cy_stc_sysint_t CapSense_ISR_cfg;

/* USART_EXT_SCB_IRQ */
extern const cy_stc_sysint_t UART_REAR_SCB_IRQ_cfg;


/* USART_FACE_SCB_IRQ */
extern const cy_stc_sysint_t UART_FACE_SCB_IRQ_cfg;

/* USART_FACE_DMA_IRQ */
extern const  cy_stc_sysint_t FaceToWifi_IRQ_cfg;
extern const  cy_stc_sysint_t WifiToFace_IRQ_cfg;

extern const cy_stc_sysint_t PWM_MOTO_1_cfg;

extern const cy_stc_sysint_t PWM_MOTO_2_cfg;

extern const cy_stc_sysint_t PWM_RGB_R_cfg;
extern const cy_stc_sysint_t PWM_RGB_G_cfg;
extern const cy_stc_sysint_t PWM_RGB_B_cfg;

#endif

