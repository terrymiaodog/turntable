/**
 ****************************************************************************************************
 * @brief       串口初始化代码(一般是串口1)，支持printf
 ****************************************************************************************************
 */

#ifndef _USART6_H
#define _USART6_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"


/*******************************************************************************************************/
/* 引脚 和 串口 定义 
 * 默认是针对USART6的.
 * 注意: 通过修改这12个宏定义,可以支持USART1~UART7任意一个串口.
 */

#define USART6_TX_GPIO_PORT              GPIOC
#define USART6_TX_GPIO_PIN               GPIO_PIN_6
#define USART6_TX_GPIO_AF                GPIO_AF8_USART6
#define USART6_TX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* 发送引脚时钟使能 */

#define USART6_RX_GPIO_PORT              GPIOC
#define USART6_RX_GPIO_PIN               GPIO_PIN_7
#define USART6_RX_GPIO_AF                GPIO_AF8_USART6
#define USART6_RX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* 接收引脚时钟使能 */

#define USART6_UX                        USART6
#define USART6_UX_IRQn                   USART6_IRQn
#define USART6_UX_IRQHandler             USART6_IRQHandler
#define USART6_UX_CLK_ENABLE()           do{ __HAL_RCC_USART6_CLK_ENABLE(); }while(0)  /* USART6 时钟使能 */

/*******************************************************************************************************/

#define USART6_REC_LEN   200                     /* 定义最大接收字节数 200 */
#define USART6_EN_RX     1                       /* 使能（1）/禁止（0）串口1接收 */
#define RXBUFFERSIZE6    1                       /* 缓存大小 */

extern UART_HandleTypeDef g_uart6_handle;       /* UART句柄 */

extern uint8_t  g_usart6_rx_buf[USART6_REC_LEN];  /* 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 */
extern uint16_t g_usart6_rx_sta;                 /* 接收状态标记 */
extern uint8_t g_rx6_buffer[RXBUFFERSIZE6];       /* HAL库USART接收Buffer */


void usart_init(uint32_t baudrate);             /* 串口初始化函数 */

#endif







