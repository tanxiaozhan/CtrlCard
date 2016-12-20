#include "stm32f10x_gpio.h"

#ifndef  __BOARD_LED_H
#define	 __BOARD_LED_H


//LED状态灯连接管脚定义
#define Board_LED_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   // 开启GPIO的外设时钟
#define Board_LED_GPIO_PORT    	GPIOC			                     // GPIO端口 
#define Board_LED_GPIO_CLK 	    RCC_APB2Periph_GPIOC	  	     // GPIO端口时钟  
#define Board_LED_GPIO_PIN		    GPIO_Pin_11			               // 连接到SCL时钟线的GPIO 


/** 定义管脚输出电平
  * 1 - 输出高电平
  * 0 - 输出低电平
  */
#define ON  1
#define OFF 0

/* 带参宏，可以像内联函数一样使用 */
#define Board_LED(x)	if (x)	\
					GPIO_SetBits(Board_LED_GPIO_PORT,Board_LED_GPIO_PIN);\
					else		\
					GPIO_ResetBits(Board_LED_GPIO_PORT,Board_LED_GPIO_PIN)

void Board_LED_GPIO_Config ( void );

#endif
