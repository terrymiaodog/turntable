/**
 ****************************************************************************************************
 * @brief       ���ڳ�ʼ������(һ���Ǵ���1)��֧��printf
 ****************************************************************************************************
 */

#ifndef _USART6_H
#define _USART6_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"


/*******************************************************************************************************/
/* ���� �� ���� ���� 
 * Ĭ�������USART6��.
 * ע��: ͨ���޸���12���궨��,����֧��USART1~UART7����һ������.
 */

#define USART6_TX_GPIO_PORT              GPIOC
#define USART6_TX_GPIO_PIN               GPIO_PIN_6
#define USART6_TX_GPIO_AF                GPIO_AF8_USART6
#define USART6_TX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

#define USART6_RX_GPIO_PORT              GPIOC
#define USART6_RX_GPIO_PIN               GPIO_PIN_7
#define USART6_RX_GPIO_AF                GPIO_AF8_USART6
#define USART6_RX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

#define USART6_UX                        USART6
#define USART6_UX_IRQn                   USART6_IRQn
#define USART6_UX_IRQHandler             USART6_IRQHandler
#define USART6_UX_CLK_ENABLE()           do{ __HAL_RCC_USART6_CLK_ENABLE(); }while(0)  /* USART6 ʱ��ʹ�� */

/*******************************************************************************************************/

#define USART6_REC_LEN   200                     /* �����������ֽ��� 200 */
#define USART6_EN_RX     1                       /* ʹ�ܣ�1��/��ֹ��0������1���� */
#define RXBUFFERSIZE6    1                       /* �����С */

extern UART_HandleTypeDef g_uart6_handle;       /* UART��� */

extern uint8_t  g_usart6_rx_buf[USART6_REC_LEN];  /* ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� */
extern uint16_t g_usart6_rx_sta;                 /* ����״̬��� */
extern uint8_t g_rx6_buffer[RXBUFFERSIZE6];       /* HAL��USART����Buffer */


void usart_init(uint32_t baudrate);             /* ���ڳ�ʼ������ */

#endif







