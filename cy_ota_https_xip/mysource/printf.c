/**
  ******************************************************************************
  * @file         printf.c
  * @brief        实现串口打印功能
  * @date         2019-1-29
  * @version      v001
  * @copyright    kaadas
  *
  * @note         凯迪仕·智能锁
  *
  ******************************************************************************
  */

#include <stdio.h>
#include <cy_pdl.h>
// #include "cycfg.h"
#include "cycfg_peripherals.h"

#define DEBUG_EN

void UartLog_PutChar(uint32_t ch)
{
    /* Place the call to your function here. */
#ifdef DEBUG_EN
    while(0U == Cy_SCB_UART_Put(UART_LOG_HW, ch))
    {
        /* Wait until FIFO is full */
    }
#else
    (void)(ch);
#endif
}


#if defined (__GNUC__)
/* Add an explicit reference to the floating point printf library to allow
the usage of floating point conversion specifier. */
asm (".global _printf_float");
/* For GCC compiler revise _write() function for printf functionality */
int _write(int file, char *ptr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        UartLog_PutChar((uint32_t)(*(ptr++)));
    }
    return(len);
}

#elif defined(__ARMCC_VERSION)
/* For MDK/RVDS compiler revise fputc() function for printf functionality */
struct __FILE
{
    int handle;
};
enum
{
    STDIN_HANDLE,
    STDOUT_HANDLE,
    STDERR_HANDLE
};

FILE __stdin = {STDIN_HANDLE};
FILE __stdout = {STDOUT_HANDLE};
FILE __stderr = {STDERR_HANDLE};

int fputc(int ch, FILE *file)
{
    int ret = EOF;
    switch(file->handle)
    {
    case STDOUT_HANDLE:
        UartLog_PutChar(ch);
        ret = ch;
        break;
    case STDERR_HANDLE:
        ret = ch;
        break;
    default:
        file = file;
        break;
    }
    return(ret);
}
#elif defined (__ICCARM__)
/* For IAR compiler revise __write() function for printf functionality */
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    size_t nChars = 0;
    for (/* Empty */; size != 0; --size)
    {
        SCB_UartPutChar(*buffer++);
        ++nChars;
    }
    return (nChars);
}
#endif /* (__GNUC__) */


