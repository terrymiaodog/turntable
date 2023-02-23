/**
 ****************************************************************************************************
 * @brief       ���ڳ�ʼ������(һ���Ǵ���1)��֧��printf
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart6/usart6.h"


/* ���ʹ��os,����������ͷ�ļ����� */
#if SYS_SUPPORT_OS
#include "includes.h"                               /* os ʹ�� */
#endif

/******************************************************************************************/
/* �������´���, ֧��printf����, ������Ҫѡ��use MicroLIB */

#if 1
#if (__ARMCC_VERSION >= 6010050)                    /* ʹ��AC6������ʱ */
__asm(".global __use_no_semihosting\n\t");          /* ������ʹ�ð�����ģʽ */
__asm(".global __ARM_use_no_argv \n\t");            /* AC6����Ҫ����main����Ϊ�޲�����ʽ�����򲿷����̿��ܳ��ְ�����ģʽ */

#else
/* ʹ��AC5������ʱ, Ҫ�����ﶨ��__FILE �� ��ʹ�ð�����ģʽ */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* ��ʹ�ð�����ģʽ��������Ҫ�ض���_ttywrch\_sys_exit\_sys_command_string����,��ͬʱ����AC6��AC5ģʽ */
//int _ttywrch(int ch)
//{
//    ch = ch;
//    return ch;
//}

/* ����_sys_exit()�Ա���ʹ�ð�����ģʽ */
//void _sys_exit(int x)
//{
//    x = x;
//}

//char *_sys_command_string(char *cmd, int len)
//{
//    return NULL;
//}

/* FILE �� stdio.h���涨��. */
//FILE __stdout;

/* �ض���fputc����, printf�������ջ�ͨ������fputc����ַ��������� */
//int fputc(int ch, FILE *f)
//{
//    while ((USART6->SR & 0X40) == 0);               /* �ȴ���һ���ַ�������� */

//    USART6->DR = (uint8_t)ch;                       /* ��Ҫ���͵��ַ� ch д�뵽DR�Ĵ��� */
//    return ch;
//}
#endif
/***********************************************END*******************************************/
    
#if USART6_EN_RX                                     /* ���ʹ���˽��� */

/* ���ջ���, ���USART_REC_LEN���ֽ�. */
uint8_t g_usart6_rx_buf[USART6_REC_LEN];

/*  ����״̬
 *  bit15��      ������ɱ�־
 *  bit14��      ���յ�0x0d
 *  bit13~0��    ���յ�����Ч�ֽ���Ŀ
*/
uint16_t g_usart6_rx_sta = 0;

uint8_t g_rx6_buffer[RXBUFFERSIZE6];                  /* HAL��ʹ�õĴ��ڽ��ջ��� */

UART_HandleTypeDef g_uart6_handle;                  /* UART��� */

/**
 * @brief       ����X��ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @note        ע��: ����������ȷ��ʱ��Դ, ���򴮿ڲ����ʾͻ������쳣.
 *              �����USART��ʱ��Դ��sys_stm32_clock_init()�������Ѿ����ù���.
 * @retval      ��
 */
void usart6_init(uint32_t baudrate)
{
    g_uart6_handle.Instance = USART6_UX;                         /* USART6 */
    g_uart6_handle.Init.BaudRate = baudrate;                    /* ������ */
    g_uart6_handle.Init.WordLength = UART_WORDLENGTH_8B;        /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_uart6_handle.Init.StopBits = UART_STOPBITS_1;             /* һ��ֹͣλ */
    g_uart6_handle.Init.Parity = UART_PARITY_NONE;              /* ����żУ��λ */
    g_uart6_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;        /* ��Ӳ������ */
    g_uart6_handle.Init.Mode = UART_MODE_TX_RX;                 /* �շ�ģʽ */
    HAL_UART_Init(&g_uart6_handle);                             /* HAL_UART_Init()��ʹ��UART1 */
    
    /* �ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ��������������� */
    HAL_UART_Receive_IT(&g_uart6_handle, (uint8_t *)g_rx6_buffer, RXBUFFERSIZE6);
}

/**
 * @brief       UART�ײ��ʼ������
 * @param       huart: UART�������ָ��
 * @note        �˺����ᱻHAL_UART_Init()����
 *              ���ʱ��ʹ�ܣ��������ã��ж�����
 * @retval      ��
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct;
    if(huart->Instance == USART6_UX)                             /* ����Ǵ���1�����д���1 MSP��ʼ�� */
    {
        USART6_UX_CLK_ENABLE();                                  /* USART6 ʱ��ʹ�� */
        USART6_TX_GPIO_CLK_ENABLE();                             /* ��������ʱ��ʹ�� */
        USART6_RX_GPIO_CLK_ENABLE();                             /* ��������ʱ��ʹ�� */

        gpio_init_struct.Pin = USART6_TX_GPIO_PIN;               /* TX���� */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* ����������� */
        gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
        gpio_init_struct.Alternate = USART6_TX_GPIO_AF;          /* ����ΪUSART1 */
        HAL_GPIO_Init(USART6_TX_GPIO_PORT, &gpio_init_struct);   /* ��ʼ���������� */

        gpio_init_struct.Pin = USART6_RX_GPIO_PIN;               /* RX���� */
        gpio_init_struct.Alternate = USART6_RX_GPIO_AF;          /* ����ΪUSART1 */
        HAL_GPIO_Init(USART6_RX_GPIO_PORT, &gpio_init_struct);   /* ��ʼ���������� */

#if USART6_EN_RX
        HAL_NVIC_EnableIRQ(USART6_UX_IRQn);                      /* ʹ��USART6�ж�ͨ�� */
        HAL_NVIC_SetPriority(USART6_UX_IRQn, 3, 3);              /* ��ռ���ȼ�3�������ȼ�3 */
#endif
    }
}

/**
 * @brief       Rx����ص�����
 * @param       huart: UART�������ָ��
 * @retval      ��
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART6_UX)             /* ����Ǵ���6 */
    {
        if((g_usart6_rx_sta & 0x8000) == 0)      /* ����δ��� */
        {
            if(g_usart6_rx_sta & 0x4000)         /* ���յ���0x0d */
            {
                if(g_rx6_buffer[0] != 0x0a) 
                {
                    g_usart6_rx_sta = 0;         /* ���մ���,���¿�ʼ */
                }
                else 
                {
                    g_usart6_rx_sta |= 0x8000;   /* ��������� */
                }
            }
            else                                /* ��û�յ�0X0D */
            {
                if(g_rx6_buffer[0] == 0x0d)
                {
                    g_usart6_rx_sta |= 0x4000;
                }
                else
                {
                    g_usart6_rx_buf[g_usart6_rx_sta & 0X3FFF] = g_rx6_buffer[0] ;
                    g_usart6_rx_sta++;
                    if(g_usart6_rx_sta > (USART6_REC_LEN - 1))
                    {
                        g_usart6_rx_sta = 0;     /* �������ݴ���,���¿�ʼ���� */
                    }
                }
            }
        }
    }
}

/**
 * @brief       ����6�жϷ�����
 * @param       ��
 * @retval      ��
 */
void USART6_UX_IRQHandler(void)
{ 
    uint32_t timeout = 0;
    uint32_t maxDelay = 0x1FFFF;
    
#if SYS_SUPPORT_OS                              /* ʹ��OS */
    OSIntEnter();    
#endif

    HAL_UART_IRQHandler(&g_uart6_handle);       /* ����HAL���жϴ����ú��� */

    timeout = 0;
    while (HAL_UART_GetState(&g_uart6_handle) != HAL_UART_STATE_READY) /* �ȴ����� */
    {
        timeout++;                              /* ��ʱ���� */
        if(timeout > maxDelay)
        {
            break;
        }
    }
     
    timeout=0;
    
    /* һ�δ������֮�����¿����жϲ�����RxXferCountΪ1 */
    while (HAL_UART_Receive_IT(&g_uart6_handle, (uint8_t *)g_rx6_buffer, RXBUFFERSIZE6) != HAL_OK)
    {
        timeout++;                              /* ��ʱ���� */
        if (timeout > maxDelay)
        {
            break;
        }
    }

#if SYS_SUPPORT_OS                              /* ʹ��OS */
    OSIntExit();
#endif

}

#endif


 

 




