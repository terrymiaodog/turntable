#ifndef _MOTOR_H
#define _MOTOR_H

#include "./SYSTEM/sys/sys.h"
void motorRead(uint32_t Addr);                           /* �����ȡָ�� */
void motorStart(uint32_t Addr);                          /* �������ָ�� */
void motorPositionMode(uint32_t Addr);                   /* ����л�λ��ģʽָ�� */
void motorEnable(uint32_t Addr);                         /* ���ʹ��ָ�� */
void motorSetPosition(uint32_t Addr, int32_t nPosition); /* ����趨λ��ָ�� */
void motorAbsMove(uint32_t Addr);                        /* ��������˶�ָ�� */
void motorRelMove(uint32_t Addr);                        /* �������˶�ָ�� */
int32_t RadtoPulse_pitch_1(float nRad);                  /* �Ƕ�ת���������� */
int32_t RadtoPulse_roll_2(float nRad);
int32_t RadtoPulse_yaw_3(float nRad);
#endif
