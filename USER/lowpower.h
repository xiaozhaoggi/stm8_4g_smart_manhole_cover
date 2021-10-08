#ifndef __LOWPOWER_H
#define __LOWPOWER_H

#define JDQ_GPIO GPIO_Pin_1


void lowpower_mode(void);
void sleep(void);
void wakeup(void);
void jdq_gpio_init(void);
void close_clock(void);
void close_gpio(void);
void RTC_Config(void);
void high_to_low_clk(void);
void uart2rx_to_exti(void);
void exti_to_uart2rx(void);

#endif