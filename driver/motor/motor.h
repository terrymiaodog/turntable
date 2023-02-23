#ifndef _MOTOR_H
#define _MOTOR_H

#include "./SYSTEM/sys/sys.h"
void motorRead(uint32_t Addr);                           /* 电机读取指令 */
void motorStart(uint32_t Addr);                          /* 电机启动指令 */
void motorPositionMode(uint32_t Addr);                   /* 电机切换位置模式指令 */
void motorEnable(uint32_t Addr);                         /* 电机使能指令 */
void motorSetPosition(uint32_t Addr, int32_t nPosition); /* 电机设定位置指令 */
void motorAbsMove(uint32_t Addr);                        /* 电机绝对运动指令 */
void motorRelMove(uint32_t Addr);                        /* 电机相对运动指令 */
int32_t RadtoPulse_pitch_1(float nRad);                  /* 角度转脉冲数函数 */
int32_t RadtoPulse_roll_2(float nRad);
int32_t RadtoPulse_yaw_3(float nRad);
#endif
