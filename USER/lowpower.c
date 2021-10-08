#include "lowpower.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_exti.h"
#include "stm8l15x_conf.h"
#include "delay.h"
#include "stm8l15x_usart.h"
#include "mpu6050.h"
#include "uart.h"
#include "adc.h"
#include "4g.h"
#include "stdio.h"
#include "stdio.h"
#include "string.h"

extern u8 yf4g_init_done;
extern struct AT_REC_FLAG at_rec_flag;

void lowpower_mode(void)
{
  sleep();
  wakeup();
}

void sleep(void)
{
   
   GPIO_ResetBits(GPIOE, GPIO_Pin_1);
   close_clock();
   close_gpio();
   //uart2rx_to_exti();
   halt();
   
}

void wakeup(void)
{
  Sysclk_Init();        //时钟初始化
  delay_init(0);        //延时函数初始化
  uart_init();
  adc_init();
  yf4g_init_done = 0;
  yf4g_init();
  GPIO_SetBits(GPIOE, GPIO_Pin_1);
  delay_ms(200);

}


void jdq_gpio_init(void)
{
  GPIO_Init(GPIOE, JDQ_GPIO, GPIO_Mode_Out_PP_Low_Slow);
}


void close_clock(void)
{

  ADC_DeInit(ADC1);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  
  
  USART_DeInit(USART1);
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1, DISABLE);
  
  
  USART_DeInit(USART2);
  CLK_PeripheralClockConfig(CLK_Peripheral_USART2, DISABLE);
}

void close_gpio(void)
{
 /*  //IO配置为输出
  GPIO_Init(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); 
  GPIO_Init(GPIOB, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); 
  GPIO_Init(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast); 
  GPIO_Init(GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOF, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_6 |GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOG, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast);
 
  //IO配置为输出低（注意低电平使能的得设置为输出高）
  GPIO_WriteBit(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, RESET);
  GPIO_WriteBit(GPIOB, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, RESET);
  GPIO_WriteBit(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6, RESET);
  GPIO_WriteBit(GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3, RESET);
  GPIO_WriteBit(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7, RESET);  
  GPIO_WriteBit(GPIOF, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_6 |GPIO_Pin_7, RESET);  
  GPIO_WriteBit(GPIOG, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, RESET);  */
  
   GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
   GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
   GPIO_Init(GPIOC, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
   GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
   GPIO_Init(GPIOE, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
   GPIO_Init(GPIOF, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
   GPIO_Init(GPIOG, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
}

void high_to_low_clk(void)
{

  //寄存器重置 
  CLK_DeInit();              
  
  //时钟8分频，2MHz
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);
  
}

void RTC_Config(void)

{     
  
    disableInterrupts();

    //CLK_LSEConfig(CLK_LSE_ON);

    CLK_LSICmd(ENABLE);//使能LSI

    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_64);//RTC时钟源LSI，1分频=38K    change：64分频

    while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);//等待LSI就绪

    

    RTC_WakeUpCmd(DISABLE);

    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//RTC时钟使能

    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//38K/16=2.375k=0.421ms   

    RTC_ITConfig(RTC_IT_WUT, ENABLE);//开启中断

    RTC_SetWakeUpCounter(2375*10);//2375*0.421=1S左右       2375  5分钟     65535 13.5分钟
    
 
    //ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_3);//优先级
    
    enableInterrupts();
    
    RTC_WakeUpCmd(ENABLE);//RTC唤醒使能
    PWR_FastWakeUpCmd(ENABLE);  //快速唤醒使能
    PWR_UltraLowPowerCmd(ENABLE);//超低功耗
    
}

void uart2rx_to_exti(void)
{
  
  disableInterrupts();
  EXTI_DeInit();
  EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Falling);
  ITC_SetSoftwarePriority(EXTI3_IRQn, ITC_PriorityLevel_1);
  GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_In_FL_IT);
  enableInterrupts();
}

void exti_to_uart2rx(void)
{
  uart2_init();
}


