#include "./BSP/TIMER/btim.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"

TIM_HandleTypeDef g_timx_handler;         /* 定时器参数句柄 */
unsigned int modbus_time;                 /* MODBUS读取报文帧间隔时间 */

void btim_timx_int_init(uint16_t arr, uint16_t psc)
{
    g_timx_handler.Instance = BTIM_TIMX_INT;                      /* 定时器x */
    g_timx_handler.Init.Prescaler = psc;                          /* 分频 */
    g_timx_handler.Init.CounterMode = TIM_COUNTERMODE_UP;         /* 递增计数模式 */
    g_timx_handler.Init.Period = arr;                             /* 自动装载值 */
    HAL_TIM_Base_Init(&g_timx_handler);
    HAL_TIM_Base_Start_IT(&g_timx_handler);                       /* 使能定时器x和定时器更新中断 */
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIMX_INT)
    {
        BTIM_TIMX_INT_CLK_ENABLE();                               /* 使能TIMx时钟 */
        HAL_NVIC_SetPriority(BTIM_TIMX_INT_IRQn, 1, 3);           /* 抢占1，子优先级3 */
        HAL_NVIC_EnableIRQ(BTIM_TIMX_INT_IRQn);                   /* 开启ITMx中断 */
    }
}

void BTIM_TIMX_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_handler);                          /* 定时器回调函数 */
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIMX_INT)
    {
        modbus_time++;
        if(modbus_time>4 && ((g_usart6_rx_sta&0X3FFF)!=0)){       /* 溢出5次时串口6接收标志位置1 */
            g_usart6_rx_sta|=0x8000;
        }
    }
}
