#include "./BSP/TIMER/btim.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"

TIM_HandleTypeDef g_timx_handler;         /* ��ʱ��������� */
unsigned int modbus_time;                 /* MODBUS��ȡ����֡���ʱ�� */

void btim_timx_int_init(uint16_t arr, uint16_t psc)
{
    g_timx_handler.Instance = BTIM_TIMX_INT;                      /* ��ʱ��x */
    g_timx_handler.Init.Prescaler = psc;                          /* ��Ƶ */
    g_timx_handler.Init.CounterMode = TIM_COUNTERMODE_UP;         /* ��������ģʽ */
    g_timx_handler.Init.Period = arr;                             /* �Զ�װ��ֵ */
    HAL_TIM_Base_Init(&g_timx_handler);
    HAL_TIM_Base_Start_IT(&g_timx_handler);                       /* ʹ�ܶ�ʱ��x�Ͷ�ʱ�������ж� */
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIMX_INT)
    {
        BTIM_TIMX_INT_CLK_ENABLE();                               /* ʹ��TIMxʱ�� */
        HAL_NVIC_SetPriority(BTIM_TIMX_INT_IRQn, 1, 3);           /* ��ռ1�������ȼ�3 */
        HAL_NVIC_EnableIRQ(BTIM_TIMX_INT_IRQn);                   /* ����ITMx�ж� */
    }
}

void BTIM_TIMX_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_handler);                          /* ��ʱ���ص����� */
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIMX_INT)
    {
        modbus_time++;
        if(modbus_time>4 && ((g_usart6_rx_sta&0X3FFF)!=0)){       /* ���5��ʱ����6���ձ�־λ��1 */
            g_usart6_rx_sta|=0x8000;
        }
    }
}
