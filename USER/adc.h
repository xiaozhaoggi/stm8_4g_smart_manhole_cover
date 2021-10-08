#ifndef _ADC_H
#define _ADC_H

#include "stm8l15x_conf.h"

#define CH4_CH  ADC_Channel_9
#define H2S_CH  ADC_Channel_7
#define SW_CH   ADC_Channel_17
#define DY_CH   ADC_Channel_23
#define CO2_CH  ADC_Channel_3

#define CH4_ERR 60
#define H2S_ERR 40
#define DY_ERR  1500
#define CO2_ERR 200

struct AD_STA
{
  u8 ch4_sta;
  u8 h2s_sta;
  u8 dy_sta;
  u8 co2_sta;
};

void check_sw(void);
void check_ad(struct AD_STA *ad_sta);
void adc_init(void);
void scan_adc(void);
void get_adc(u16 *ad_value, u16 ch);


#endif
