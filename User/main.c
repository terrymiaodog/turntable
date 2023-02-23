/**
 ****************************************************************************************************
 * @brief       ͨ�����յ���ͨ��֡���ı�ת̨��Ӧ�����λ�á�
                ����1�������ϵ�Usb�ڣ����ӵ��Զ˽��ն���ͨ��֡��ȡ�����е�����float���ݣ�ת��Ϊ��Ӧ��
                ��������ͨ������6���Ϳ��Ƶ���ƶ�������λ�õ�modbus����ָ������շ�����modbusӦ��
 ****************************************************************************************************
 */
 
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/crc16/crc16.h"
#include "./BSP/TIMER/btim.h"
#include "./SYSTEM/motor/motor.h"

#include<string.h>

#define Motor1 0x01     /*1�ŵ����ַΪ0x01*/
#define Motor2 0x02     /*2�ŵ����ַΪ0x02*/
#define Motor3 0x03     /*3�ŵ����ַΪ0x03*/

#define offset1 4       /*��ȡ��1�ŵ������ƫ����*/
#define offset2 8       /*��ȡ��2�ŵ������ƫ����*/
#define offset3 12      /*��ȡ��3�ŵ������ƫ����*/


int main(void)
{
    uint8_t len;
    
    float pitRad;           /*����ĸ��������ݣ���Ӧ1�ŵ��*/
    uint8_t readBuff1[4];   
    int32_t pitchPulse;     /*��Ӧ�ĸ�����������*/
    float rolRad;           /*����ĺ�������ݣ���Ӧ2�ŵ��*/
    uint8_t readBuff2[4];   
    int32_t rollPulse;      /*��Ӧ�ĺ����������*/
    float yawRad;           /*�����ƫ�������ݣ���Ӧ3�ŵ��*/
    uint8_t readBuff3[4];   
    int32_t yawPulse;       /*��Ӧ��ƫ����������*/
    
    uint16_t times = 0;     /*LED��˸���ʱ��*/
    
    HAL_Init();                             /*��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /*����ʱ��,168Mhz*/
    delay_init(168);                        /*��ʱ��ʼ�� */
    usart_init(115200);                     /*���ڳ�ʼ��Ϊ115200 */
    usart6_init(115200);                    /*����6��ʼ��Ϊ115200 */
    btim_timx_int_init( 84-1,50-1 );        /*�趨��ʱ�������ʱ��*/
    led_init();                             /*��ʼ��LED*/
    
    motorStart(Motor1);                     /*���1������λ��ģʽ��ʹ��ָ��*/
    motorPositionMode(Motor1);
    motorEnable(Motor1);
    
    motorStart(Motor2);                     /*���2������λ��ģʽ��ʹ��ָ��*/
    motorPositionMode(Motor2);
    motorEnable(Motor2);
    
    motorStart(Motor3);                     /*���3������λ��ģʽ��ʹ��ָ��*/
    motorPositionMode(Motor3);
    motorEnable(Motor3);
    
    while(1)
    {
       if (g_usart_rx_sta & 0x8000)                 /*����1���յ�һ֡����*/
        {
            len = g_usart_rx_sta & 0x3fff;          /*�õ��˴ν��յ������ݳ���*/
            
            readBuff1[0]=g_usart_rx_buf[0+offset1]; /*ע��ߵ�λ��ϵ*/
            readBuff1[1]=g_usart_rx_buf[1+offset1];
            readBuff1[2]=g_usart_rx_buf[2+offset1];
            readBuff1[3]=g_usart_rx_buf[3+offset1];
            memcpy(&pitRad,readBuff1,4);            /*��ȡ������*/
            
            pitchPulse=RadtoPulse_pitch_1(pitRad);  /*�Ƕ�ת��Ϊ������*/
            motorSetPosition(Motor1, pitchPulse);   /*�趨���1����Ƕȣ����������λ��ģʽ�˶�*/
            motorAbsMove(Motor1);
            
            readBuff2[0]=g_usart_rx_buf[0+offset2]; /*ע��ߵ�λ��ϵ*/
            readBuff2[1]=g_usart_rx_buf[1+offset2];
            readBuff2[2]=g_usart_rx_buf[2+offset2];
            readBuff2[3]=g_usart_rx_buf[3+offset2];
            memcpy(&rolRad,readBuff2,4);            /*��ȡ�����*/
            
            rollPulse=RadtoPulse_roll_2(rolRad);   /*�Ƕ�ת��Ϊ������*/
            motorSetPosition(Motor2, rollPulse);    /*�趨���2����Ƕȣ����������λ��ģʽ�˶�*/
            motorAbsMove(Motor2);
            
            readBuff3[0]=g_usart_rx_buf[0+offset3]; /*ע��ߵ�λ��ϵ*/
            readBuff3[1]=g_usart_rx_buf[1+offset3];
            readBuff3[2]=g_usart_rx_buf[2+offset3];
            readBuff3[3]=g_usart_rx_buf[3+offset3];
            memcpy(&yawRad,readBuff3,4);            /*��ȡƫ����*/
            
            yawPulse=RadtoPulse_yaw_3(yawRad);    /*�Ƕ�ת��Ϊ������*/
            motorSetPosition(Motor3, yawPulse);     /*�趨���3����Ƕȣ����������λ��ģʽ�˶�*/
            motorAbsMove(Motor3);
            
            g_usart_rx_sta = 0;
        }
        else
        {
            times++;
            
            if (times % 200  == 0) LED0_TOGGLE();   /*��˸LED,��ʾϵͳ��������*/

            delay_ms(10);
        }
    }
}
