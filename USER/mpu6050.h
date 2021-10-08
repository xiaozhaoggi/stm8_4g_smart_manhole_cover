#ifndef _MPU6050_H
#define _MPU6050_H
#include "stm8l15x.h"

//串口接收起始位
#define RIGHT_GET1 0X5A
#define RIGHT_GET2 0x5A

#define ROLL_ERR        0
#define PITCH_ERR       0
#define YAW_ERR         0

typedef struct
{
  u8 tmpdata[2];
  u8 oladata[6];
  s16 tmp;
  s16 roll;
  s16 pitch;
  s16 yaw;
}MPU6050_BUFF;

u8 check_6050state(MPU6050_BUFF *buff);
void get_6050data(MPU6050_BUFF *buff);
void send_6050data(u8 *data, u8 len);
void uart1_irq();

#endif 

