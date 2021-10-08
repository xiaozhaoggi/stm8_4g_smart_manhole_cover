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
  Sysclk_Init();        //ʱ�ӳ�ʼ��
  delay_init(0);        //��ʱ������ʼ��
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
 /*  //IO����Ϊ���
  GPIO_Init(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); 
  GPIO_Init(GPIOB, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); 
  GPIO_Init(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast); 
  GPIO_Init(GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOF, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_6 |GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOG, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast);
 
  //IO����Ϊ����ͣ�ע��͵�ƽʹ�ܵĵ�����Ϊ����ߣ�
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

  //�Ĵ������� 
  CLK_DeInit();              
  
  //ʱ��8��Ƶ��2MHz
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);
  
}

void RTC_Config(void)

{     
  
    disableInterrupts();

    //CLK_LSEConfig(CLK_LSE_ON);

    CLK_LSICmd(ENABLE);//ʹ��LSI

    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_64);//RTCʱ��ԴLSI��1��Ƶ=38K    change��64��Ƶ

    while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);//�ȴ�LSI����

    

    RTC_WakeUpCmd(DISABLE);

    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//RTCʱ��ʹ��

    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//38K/16=2.375k=0.421ms   

    RTC_ITConfig(RTC_IT_WUT, ENABLE);//�����ж�

    RTC_SetWakeUpCounter(2375*10);//2375*0.421=1S����       2375  5����     65535 13.5����
    
 
    //ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_3);//���ȼ�
    
    enableInterrupts();
    
    RTC_WakeUpCmd(ENABLE);//RTC����ʹ��
    PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��
    PWR_UltraLowPowerCmd(ENABLE);//���͹���
    
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


