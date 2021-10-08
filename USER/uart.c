#include "uart.h"
#include "stm8l15x_conf.h"
#include "mpu6050.h"
#include "4g.h"

void uart_init(void)
{
  uart1_init();
  uart2_init();
  //enableInterrupts(); /* �������ж� */
}

void uart1_init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
    // USART init
    USART_DeInit(USART1);  
  
    GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast);//TXD
    GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);//RXD

    // ��ʼ������
    USART_Init(USART1,
                (uint32_t)9600,
                USART_WordLength_8b,
                USART_StopBits_1,
                USART_Parity_No,
                (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));  // �������д

    // ���ж�
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  
    //ITC_SetSoftwarePriority(USART1_RX_TIM5_CC_IRQn,ITC_PriorityLevel_2);
    //ITC_SetSoftwarePriority(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQn,ITC_PriorityLevel_3);
    //USART_ITConfig(USART1, USART_IT_TC, ENABLE);

    /* Enable USART ʹ�� */
    USART_Cmd(USART1, ENABLE);
}

void uart2_init(void)//���ڳ�ʼ������
{  
    CLK_PeripheralClockConfig(CLK_Peripheral_USART2, ENABLE);
    // USART init
    USART_DeInit(USART2);  

    // PE3-RX PE4-TX   
    GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);//TXD
    GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);//RXD
    // ��ʼ������
    USART_Init(USART2,
                (uint32_t)9600,
                USART_WordLength_8b,
                USART_StopBits_1,
                USART_Parity_No,
                (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));  // �������д

    // ���ж�
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  
    //ITC_SetSoftwarePriority(TIM2_CC_USART2_RX_IRQn,ITC_PriorityLevel_3);
    //USART_ITConfig(USART2, USART_IT_TC, ENABLE);

    /* Enable USART ʹ�� */
    USART_Cmd(USART2, ENABLE);

 }

INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
{
  if(SET == USART_GetITStatus(USART1,USART_IT_RXNE))
  {
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//    USART_ClearFlag(USART1, USART_FLAG_OR);
    uart1_irq();
    
  }
}


INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler,20)
{
  if(SET == USART_GetITStatus(USART2,USART_IT_RXNE))
  {
//    usart_rec_buf[usart_rec_index++] = USART1->DR;
    USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    uart2_irq();
    
  }
}
