/**
 ****************************************************************************************************
 * @brief       电机控制指令（含接收应答部分）
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/crc16/crc16.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/motor/motor.h"
#include "./SYSTEM/delay/delay.h"

uint8_t buffer[100]={0};
uint8_t Ebuffer[100]="ERROR";
uint8_t Mlen;

void motorRead(uint32_t Addr){                                               /* 电机读取指令 */
    uint16_t crc16 = 0;                                                      /* 16位CRC校验码 */
    buffer[0] = Addr;
    buffer[1] = 0x04;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x02;
    crc16 = CRC16(buffer, 6);
    buffer[6] = (uint8_t)(crc16 & 0xff);
    buffer[7] = (uint8_t)(crc16 >> 8);
    HAL_UART_Transmit(&g_uart6_handle,(uint8_t*)buffer,8,1000);              /* 串口6发送读取指令 */
    while(__HAL_UART_GET_FLAG(&g_uart6_handle,UART_FLAG_TC)!=SET);           /* 等待发送结束 */
    
    delay_ms(5);                                                            /* 等待应答时间 */
    
    if(g_usart6_rx_sta & 0x8000){                                            /* 若串口6收到应答帧 */
            Mlen = g_usart6_rx_sta & 0x3fff;
            if(g_usart6_rx_buf[1] > 0x80){                                   /* 若功能码异常，发送error */
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)Ebuffer,20,1000); 
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            else{                                                            /* 若功能码正常，串口1发送接收到的数据 */
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart6_rx_buf,Mlen,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            
        }
    
}

void motorStart(uint32_t Addr){                                              /* 电机启动指令 */
    uint16_t crc16 = 0;
    buffer[0] = Addr;
    buffer[1] = 0x10;
    buffer[2] = 0x00;
    buffer[3] = 0x51;
    buffer[4] = 0x00;
    buffer[5] = 0x01;
    buffer[6] = 0x02;
    buffer[7] = 0x00;
    buffer[8] = 0x06;
    crc16 = CRC16(buffer, 9);
    buffer[9] = (uint8_t)(crc16 & 0xff);
    buffer[10] = (uint8_t)(crc16 >> 8);
    HAL_UART_Transmit(&g_uart6_handle,(uint8_t*)buffer,11,1000);
    while(__HAL_UART_GET_FLAG(&g_uart6_handle,UART_FLAG_TC)!=SET);
    
    delay_ms(5);
    
    if(g_usart6_rx_sta & 0x8000){
            Mlen = g_usart6_rx_sta & 0x3fff;
            if(g_usart6_rx_buf[1] > 0x80){
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)Ebuffer,20,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            else{
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart6_rx_buf,Mlen,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            
        }
}

void motorEnable(uint32_t Addr){                                             /* 电机使能指令 */
    uint16_t crc16 = 0;
    buffer[0] = Addr;
    buffer[1] = 0x10;
    buffer[2] = 0x00;
    buffer[3] = 0x51;
    buffer[4] = 0x00;
    buffer[5] = 0x01;
    buffer[6] = 0x02;
    buffer[7] = 0x00;
    buffer[8] = 0x07;
    crc16 = CRC16(buffer, 9);
    buffer[9] = (uint8_t)(crc16 & 0xff);
    buffer[10] = (uint8_t)(crc16 >> 8);
    HAL_UART_Transmit(&g_uart6_handle,(uint8_t*)buffer,11,1000);
    while(__HAL_UART_GET_FLAG(&g_uart6_handle,UART_FLAG_TC)!=SET);
    
    delay_ms(5);
    if(g_usart6_rx_sta & 0x8000){
            Mlen = g_usart6_rx_sta & 0x3fff;
            if(g_usart6_rx_buf[1] > 0x80){
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)Ebuffer,20,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            else{
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart6_rx_buf,Mlen,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            
        }
}

void motorPositionMode(uint32_t Addr){                                       /* 电机切换位置模式指令 */
    uint16_t crc16 = 0;
    buffer[0] = Addr;
    buffer[1] = 0x10;
    buffer[2] = 0x00;
    buffer[3] = 0x39;
    buffer[4] = 0x00;
    buffer[5] = 0x01;
    buffer[6] = 0x02;
    buffer[7] = 0x00;
    buffer[8] = 0x01;
    crc16 = CRC16(buffer, 9);
    buffer[9] = (uint8_t)(crc16 & 0xff);
    buffer[10] = (uint8_t)(crc16 >> 8);
    HAL_UART_Transmit(&g_uart6_handle,(uint8_t*)buffer,11,1000);
    while(__HAL_UART_GET_FLAG(&g_uart6_handle,UART_FLAG_TC)!=SET);
    
    delay_ms(5);
    if(g_usart6_rx_sta & 0x8000){
            Mlen = g_usart6_rx_sta & 0x3fff;
            if(g_usart6_rx_buf[1] > 0x80){
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)Ebuffer,20,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET); 
                g_usart6_rx_sta = 0;
            }
            else{
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart6_rx_buf,Mlen,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            
        }
}

void motorAbsMove(uint32_t Addr){                                            /* 电机绝对运动指令 */
    uint16_t crc16 = 0;
    buffer[0] = Addr;
    buffer[1] = 0x10;
    buffer[2] = 0x00;
    buffer[3] = 0x51;
    buffer[4] = 0x00;
    buffer[5] = 0x01;
    buffer[6] = 0x02;
    buffer[7] = 0x00;
    buffer[8] = 0x0F;
    crc16 = CRC16(buffer, 9);
    buffer[9] = (uint8_t)(crc16 & 0xff);
    buffer[10] = (uint8_t)(crc16 >> 8);
    HAL_UART_Transmit(&g_uart6_handle,(uint8_t*)buffer,11,1000);
    while(__HAL_UART_GET_FLAG(&g_uart6_handle,UART_FLAG_TC)!=SET);
    
    delay_ms(5);
    if(g_usart6_rx_sta & 0x8000){
            Mlen = g_usart6_rx_sta & 0x3fff;
            if(g_usart6_rx_buf[1] > 0x80){
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)Ebuffer,20,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            else{
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart6_rx_buf,Mlen,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            
        }
    
    buffer[8] = 0x1F;
    crc16 = CRC16(buffer, 9);
    buffer[9] = (uint8_t)(crc16 & 0xff);
    buffer[10] = (uint8_t)(crc16 >> 8);
    HAL_UART_Transmit(&g_uart6_handle,(uint8_t*)buffer,11,1000);
    while(__HAL_UART_GET_FLAG(&g_uart6_handle,UART_FLAG_TC)!=SET);
    
    delay_ms(5);
    if(g_usart6_rx_sta & 0x8000){
            Mlen = g_usart6_rx_sta & 0x3fff;
            if(g_usart6_rx_buf[1] > 0x80){
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)Ebuffer,20,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            else{
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart6_rx_buf,Mlen,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            
        }
}

void motorRelMove(uint32_t Addr){                                            /* 电机相对运动指令 */
    uint16_t crc16 = 0;
    buffer[0] = Addr;
    buffer[1] = 0x10;
    buffer[2] = 0x00;
    buffer[3] = 0x51;
    buffer[4] = 0x00;
    buffer[5] = 0x01;
    buffer[6] = 0x02;
    buffer[7] = 0x00;
    buffer[8] = 0x4F;
    crc16 = CRC16(buffer, 9);
    buffer[9] = (uint8_t)(crc16 & 0xff);
    buffer[10] = (uint8_t)(crc16 >> 8);
    HAL_UART_Transmit(&g_uart6_handle,(uint8_t*)buffer,11,1000);
    while(__HAL_UART_GET_FLAG(&g_uart6_handle,UART_FLAG_TC)!=SET);
    
    delay_ms(5);
    if(g_usart6_rx_sta & 0x8000){
            Mlen = g_usart6_rx_sta & 0x3fff;
            if(g_usart6_rx_buf[1] > 0x80){
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)Ebuffer,20,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            else{
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart6_rx_buf,Mlen,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            
        }
    
    buffer[8] = 0x5F;
    crc16 = CRC16(buffer, 9);
    buffer[9] = (uint8_t)(crc16 & 0xff);
    buffer[10] = (uint8_t)(crc16 >> 8);
    HAL_UART_Transmit(&g_uart6_handle,(uint8_t*)buffer,11,1000);
    while(__HAL_UART_GET_FLAG(&g_uart6_handle,UART_FLAG_TC)!=SET);           /* 等待发送结束 */
    
    delay_ms(5);
    if(g_usart6_rx_sta & 0x8000){
            Mlen = g_usart6_rx_sta & 0x3fff;
            if(g_usart6_rx_buf[1] > 0x80){
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)Ebuffer,20,1000);    /* 发送接收到的数据 */
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);           /* 等待发送结束 */
                g_usart6_rx_sta = 0;
            }
            else{
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart6_rx_buf,Mlen,1000);    /* 发送接收到的数据 */
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);           /* 等待发送结束 */
                g_usart6_rx_sta = 0;
            }
            
        }
}


void motorSetPosition(uint32_t Addr, int32_t nPosition){                     /* 电机设定位置指令 */
    uint16_t crc16 = 0;
    buffer[0] = Addr;
    buffer[1] = 0x10;
    buffer[2] = 0x00;
    buffer[3] = 0x53;
    buffer[4] = 0x00;
    buffer[5] = 0x02;
    buffer[6] = 0x04;
    buffer[7] = (nPosition >> 24) & 0xff;
    buffer[8] = (nPosition >> 16) & 0xff;
    buffer[9] = (nPosition >> 8) & 0xff;
    buffer[10] = (nPosition >> 0) & 0xff;
    crc16 = CRC16(buffer, 11);
    buffer[11] = (uint8_t)(crc16 & 0xff);
    buffer[12] = (uint8_t)(crc16 >> 8);
    HAL_UART_Transmit(&g_uart6_handle,(uint8_t*)buffer,13,1000);
    while(__HAL_UART_GET_FLAG(&g_uart6_handle,UART_FLAG_TC)!=SET);
    
    delay_ms(5);
    
    if(g_usart6_rx_sta & 0x8000){
            Mlen = g_usart6_rx_sta & 0x3fff;
            if(g_usart6_rx_buf[1] > 0x80){
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)Ebuffer,20,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            else{
                HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart6_rx_buf,Mlen,1000);
                while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);
                g_usart6_rx_sta = 0;
            }
            
        }
}

int32_t RadtoPulse_pitch_1(float nRad){                                      /* 角度转脉冲数函数 */
    int32_t puls_num1;
    puls_num1=nRad*400;
    return puls_num1;
}

int32_t RadtoPulse_roll_2(float nRad){                                      /* 角度转脉冲数函数 */
    int32_t puls_num2;
    puls_num2=nRad*400;
    return puls_num2;
}

int32_t RadtoPulse_yaw_3(float nRad){                                      /* 角度转脉冲数函数 */
    int32_t puls_num3;
    puls_num3=nRad*320;
    return puls_num3;
}
