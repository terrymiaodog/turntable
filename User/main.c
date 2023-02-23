/**
 ****************************************************************************************************
 * @brief       通过接收到的通信帧，改变转台对应电机的位置。
                串口1（板子上的Usb口）连接电脑端接收定长通信帧，取出其中的三个float数据，转化为对应的
                脉冲数，通过串口6发送控制电机移动到绝对位置的modbus请求指令，并接收返还的modbus应答。
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

#define Motor1 0x01     /*1号电机地址为0x01*/
#define Motor2 0x02     /*2号电机地址为0x02*/
#define Motor3 0x03     /*3号电机地址为0x03*/

#define offset1 4       /*读取的1号电机数据偏移量*/
#define offset2 8       /*读取的2号电机数据偏移量*/
#define offset3 12      /*读取的3号电机数据偏移量*/


int main(void)
{
    uint8_t len;
    
    float pitRad;           /*输入的俯仰角数据，对应1号电机*/
    uint8_t readBuff1[4];   
    int32_t pitchPulse;     /*对应的俯仰角脉冲数*/
    float rolRad;           /*输入的横滚角数据，对应2号电机*/
    uint8_t readBuff2[4];   
    int32_t rollPulse;      /*对应的横滚角脉冲数*/
    float yawRad;           /*输入的偏航角数据，对应3号电机*/
    uint8_t readBuff3[4];   
    int32_t yawPulse;       /*对应的偏航角脉冲数*/
    
    uint16_t times = 0;     /*LED闪烁间隔时间*/
    
    HAL_Init();                             /*初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);     /*设置时钟,168Mhz*/
    delay_init(168);                        /*延时初始化 */
    usart_init(115200);                     /*串口初始化为115200 */
    usart6_init(115200);                    /*串口6初始化为115200 */
    btim_timx_int_init( 84-1,50-1 );        /*设定定时器的溢出时间*/
    led_init();                             /*初始化LED*/
    
    motorStart(Motor1);                     /*电机1启动，位置模式，使能指令*/
    motorPositionMode(Motor1);
    motorEnable(Motor1);
    
    motorStart(Motor2);                     /*电机2启动，位置模式，使能指令*/
    motorPositionMode(Motor2);
    motorEnable(Motor2);
    
    motorStart(Motor3);                     /*电机3启动，位置模式，使能指令*/
    motorPositionMode(Motor3);
    motorEnable(Motor3);
    
    while(1)
    {
       if (g_usart_rx_sta & 0x8000)                 /*串口1接收到一帧数据*/
        {
            len = g_usart_rx_sta & 0x3fff;          /*得到此次接收到的数据长度*/
            
            readBuff1[0]=g_usart_rx_buf[0+offset1]; /*注意高低位关系*/
            readBuff1[1]=g_usart_rx_buf[1+offset1];
            readBuff1[2]=g_usart_rx_buf[2+offset1];
            readBuff1[3]=g_usart_rx_buf[3+offset1];
            memcpy(&pitRad,readBuff1,4);            /*读取俯仰角*/
            
            pitchPulse=RadtoPulse_pitch_1(pitRad);  /*角度转化为脉冲数*/
            motorSetPosition(Motor1, pitchPulse);   /*设定电机1具体角度，控制其绝对位置模式运动*/
            motorAbsMove(Motor1);
            
            readBuff2[0]=g_usart_rx_buf[0+offset2]; /*注意高低位关系*/
            readBuff2[1]=g_usart_rx_buf[1+offset2];
            readBuff2[2]=g_usart_rx_buf[2+offset2];
            readBuff2[3]=g_usart_rx_buf[3+offset2];
            memcpy(&rolRad,readBuff2,4);            /*读取横滚角*/
            
            rollPulse=RadtoPulse_roll_2(rolRad);   /*角度转化为脉冲数*/
            motorSetPosition(Motor2, rollPulse);    /*设定电机2具体角度，控制其绝对位置模式运动*/
            motorAbsMove(Motor2);
            
            readBuff3[0]=g_usart_rx_buf[0+offset3]; /*注意高低位关系*/
            readBuff3[1]=g_usart_rx_buf[1+offset3];
            readBuff3[2]=g_usart_rx_buf[2+offset3];
            readBuff3[3]=g_usart_rx_buf[3+offset3];
            memcpy(&yawRad,readBuff3,4);            /*读取偏航角*/
            
            yawPulse=RadtoPulse_yaw_3(yawRad);    /*角度转化为脉冲数*/
            motorSetPosition(Motor3, yawPulse);     /*设定电机3具体角度，控制其绝对位置模式运动*/
            motorAbsMove(Motor3);
            
            g_usart_rx_sta = 0;
        }
        else
        {
            times++;
            
            if (times % 200  == 0) LED0_TOGGLE();   /*闪烁LED,提示系统正在运行*/

            delay_ms(10);
        }
    }
}
