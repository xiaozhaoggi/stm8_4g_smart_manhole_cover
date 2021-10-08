#include "adc.h"
#include "stm8l15x_adc.h"
#include "delay.h"

u16 ch4_value,h2s_value,sw_value,dy_value,co2_value;//����ɼ�����
struct AD_STA ad_sta;

void check_sw(void)
{
  
}

void check_ad(struct AD_STA *ad_sta)
{
  struct AD_STA *psta = ad_sta;
  
  if(ch4_value > CH4_ERR)
    psta->ch4_sta = 1;
  
  if(h2s_value > H2S_ERR)
    psta->h2s_sta = 1;
  
  if(dy_value < DY_ERR)
    psta->dy_sta = 1;
  
  if(co2_value > CO2_ERR)
    psta->co2_sta = 1;
  
}

void adc_init(void)
{
  ADC_Cmd(ADC1 , DISABLE);               //�ر�ADC
  
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1 , ENABLE);              //ʹ��ADC1ʱ��
  
  GPIO_Init(GPIOD , GPIO_Pin_5 | GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);  //����AD���� Channel 9(CH4),7(H2S)
  GPIO_Init(GPIOB , GPIO_Pin_1, GPIO_Mode_In_FL_No_IT);  //����AD���� Channel 17(SW)
  GPIO_Init(GPIOE , GPIO_Pin_5, GPIO_Mode_In_FL_No_IT);  //����AD���� Channel 23(DY)
  
  
  ADC_Init(ADC1,
           ADC_ConversionMode_Single,   //����ת��ģʽ
           ADC_Resolution_12Bit,        //12λ����ת������
           ADC_Prescaler_1              //ʱ������Ϊ1��Ƶ
             );  
   
  ADC_Cmd(ADC1 , ENABLE);               //ʹ��ADC

}
void scan_adc(void)
{
  get_adc(&ch4_value, CH4_CH);
  get_adc(&h2s_value, H2S_CH);
  get_adc(&sw_value, SW_CH);
  get_adc(&dy_value, DY_CH);
  get_adc(&co2_value, CO2_CH);
}

void get_adc(u16 *ad_value, u16 ch)
{
  ADC_ChannelCmd(ADC1,
                 ch,         //����Ϊͨ��ch���в���
                 ENABLE); 
  
  ADC_SoftwareStartConv(ADC1);      //����ADC
  
  delay_ms(200); 
  
  while(ADC_GetFlagStatus(ADC1 , ADC_FLAG_EOC) == 0);  //�ȴ�ת������
  
  ADC_ClearFlag(ADC1 , ADC_FLAG_EOC);                 //����жϱ�־
  
  *ad_value = ADC_GetConversionValue(ADC1);           //��ȡADC1��ͨ����ת�����
  
  ADC_ChannelCmd(ADC1,
                 ch,         //����Ϊͨ��chֹͣ����
                 DISABLE); 

}

