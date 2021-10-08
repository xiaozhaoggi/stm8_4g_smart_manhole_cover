#include "stm8l15x_conf.h"
#include "delay.h"

volatile u8 fac_us=0;

void delay_init(u8 clk)
{
  if(clk>16)fac_us=(16-4)/4;
  else if(clk>4)fac_us=(clk-4)/4;
  else fac_us=1;
}

void delay_us(u16 nus)
{
  __asm(
"PUSH A          \n"    //1T,ѹջ
"DELAY_XUS:      \n"
"LD A,fac_us     \n"    //1T,fac_us���ص��ۼ���A
"DELAY_US_1:     \n"
"NOP             \n"    //1T,nop��ʱ
"DEC A           \n"    //1T,A--
"JRNE DELAY_US_1 \n"    //������0,����ת(2T)��DELAY_US_1����ִ��,������0,����ת(1T).
"NOP             \n"    //1T,nop��ʱ
"DECW X          \n"    //1T,x--
"JRNE DELAY_XUS  \n"    //������0,����ת(2T)��DELAY_XUS����ִ��,������0,����ת(1T).
"POP A           \n"    //1T,��ջ
);
}

/*void delay_ms(u32 nms)
{
  u8 t;
  if(nms>65)
  {
    t=nms/65;
    while(t--)delay_us(65000);
    nms=nms%65;
  }
  delay_us(nms*1000);
}*/

void delay_ms(u16 nms)
{
   u16 i = 0;
   u16 j = 0;

   for (j=0;j<nms;j++)
   {
      for(i=0; i<2000; i++);
   }
}

void Sysclk_Init(void)

{

 //HSI�ڲ�ʱ�ӣ�0��Ƶ��16MHZ

  CLK_HSICmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);   
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  
  /*CLK_LSICmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);   
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);*/


}