#include "stm8l15x_conf.h"
#include "delay.h"
#include "stm8l15x_usart.h"
#include "mpu6050.h"
#include "uart.h"
#include "adc.h"
#include "4g.h"
#include "lowpower.h"
#include "stdio.h"
#include "stm8l15x_pwr.h"
/*
       */
extern MPU6050_BUFF mpu6050_buff;

extern struct AT at;
extern u8 yf4g_init_done;
extern char *dev_id;
extern u16 ch4_value,h2s_value,sw_value,dy_value,co2_value;
extern u8 getdata_flag;    //上位机查询标志位
char data_pub[300];
int main(void)
{ 
  
  
  Sysclk_Init();        //时钟初始化
  delay_init(0);        //延时函数初始化
  uart_init();
  adc_init();
  yf4g_init();
  jdq_gpio_init();
  RTC_Config(); 
  GPIO_SetBits(GPIOE, GPIO_Pin_1);
  while(1)
 {
   
      send_yf4g();
      
      if(yf4g_init_done)
      {   
        get_6050data(&mpu6050_buff);
        scan_adc();
        sprintf(data_pub,"AT+MQTTPUB=0,0,\"data\",\"{\'client_id\':\'%s\',\'data\':{\'ch4\':\'%d\',\'h2s\':\'%d\',\'co2\':\'%d\',\'sw\':\'%d\',\'dy\':\'%d\',\'ola\':{\'roll\':\'%d\',\'pitch\':\'%d\',\'yaw\':\'%d\'} } }\""
              ,dev_id, ch4_value, h2s_value, co2_value, sw_value, dy_value, mpu6050_buff.roll, mpu6050_buff.pitch, mpu6050_buff.yaw);
        //pub_data(data_pub, getdata_flag);
        send_4gdata(data_pub);     
        delay_ms(200);
        lowpower_mode();  
        
      }
      
     
     
  }

}


