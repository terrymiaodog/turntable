#ifndef __BTIM_H
#define __BTIM_H
#include "./SYSTEM/sys/sys.h"

#define BTIM_TIMX_INT                       TIM6
#define BTIM_TIMX_INT_IRQn                  TIM6_DAC_IRQn
#define BTIM_TIMX_INT_IRQHandler            TIM6_DAC_IRQHandler
#define BTIM_TIMX_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM6_CLK_ENABLE(); }while(0)

extern unsigned int modbus_time;                        /* MODBUS��ȡ����֡���ʱ�� */
void btim_timx_int_init(uint16_t arr, uint16_t psc);    /* ������ʱ�� ��ʱ�жϳ�ʼ������ */

#endif
