#include "mpu6050.h"
#include "stm8l15x_usart.h"
#include "delay.h"
#include "string.h"
#include "stdlib.h"

MPU6050_BUFF mpu6050_buff;

u8 send_data6050[4] = {0xA5,0x56,0x01,0xFC};//查询帧 只查询欧拉角和温度

u8 check_6050state(MPU6050_BUFF *buff)
{
  u8 sta = 0; 
    
  MPU6050_BUFF *pbuff = buff;
  
  if(abs(pbuff->roll) > ROLL_ERR)
    sta = 1;
  
  if(abs(pbuff->pitch) > PITCH_ERR)
    sta = 1;
    
  if(abs(pbuff->yaw) > YAW_ERR)
    sta = 1;
    
  return sta;
     
}

void get_6050data(MPU6050_BUFF *buff) //发送查询帧并获取数据
{
  
  MPU6050_BUFF *pbuff = buff;
  
  send_6050data(send_data6050, 4);
  
  delay_ms(500);
  delay_ms(500);
  
  /*memset(pbuff->tmpdata,2*sizeof(s8), 0);
  memset(pbuff->oladata,6*sizeof(s8), 0);
  memset(&pbuff->tmp,sizeof(s16), 0);
  memset(&pbuff->roll,sizeof(s16), 0);
  memset(&pbuff->pitch,sizeof(s16), 0);
  memset(&pbuff->yaw,sizeof(s16), 0);*/
  
  
  pbuff->tmp = ((pbuff->tmpdata[0] & 0xff)<<8)|(pbuff->tmpdata[1]);
  pbuff->roll = ((pbuff->oladata[0] & 0xff)<<8)|(pbuff->oladata[1]);
  pbuff->pitch = ((pbuff->oladata[2] & 0xff)<<8)|(pbuff->oladata[3]);
  pbuff->yaw = ((pbuff->oladata[4] & 0xff)<<8)|(pbuff->oladata[5]);
  
}


void send_6050data(u8 *data, u8 len)
{
  u8 i;
  u8 *pdata = data;
  
  for(i=0; i<len; i++)
  {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData8(USART1,*(pdata++));
  }
  
}

void uart1_irq(void)
{
  
  u8 res;
  static u8 index;
  MPU6050_BUFF *pbuff = &mpu6050_buff;
  res = USART1->DR;
  
  switch(index)
  {
  case 0:
    if(res == RIGHT_GET1)
      index++;
    else
      index = 0;
    break;
  case 1:
    if(res == RIGHT_GET2)
      index++;
    else
      index = 0;
    break;
  case 2:
    index++;
    break;
  case 3:
    index++;
    break;
  case 4:
    pbuff->oladata[0] = res;
    index++;
    break;
  case 5:
    pbuff->oladata[1] = res;
    index++;
    break;
  case 6:
    pbuff->oladata[2] = res;
    index++;
    break;
  case 7:
    pbuff->oladata[3] = res;
    index++;
    break;
  case 8:
    pbuff->oladata[4] = res;
    index++;
    break;
  case 9:
    pbuff->oladata[5] = res;
    index++;
    break;
  case 10:
    pbuff->tmpdata[0] = res;
    index++;
    break;
  case 11:
    pbuff->tmpdata[1] = res;
    index = 0;
    break;
  
  }

  
  
}
