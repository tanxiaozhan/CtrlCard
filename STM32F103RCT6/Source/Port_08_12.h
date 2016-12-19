/*
**********************************************************
*
* 控制卡08、12输出接口定义
* 
* 创建日期：2016.09.11
*
**********************************************************
*/

#include "stm32f10x_gpio.h"

#ifndef  __PORT_08_12_H
#define	 __PORT_08_12_H


//控制卡08、12接口公共管脚定义
#define PORT_08_12_STB_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   // 开启GPIO的外设时钟
#define PORT_08_12_STB_GPIO_PORT    	GPIOC			                     // GPIO端口 
#define PORT_08_12_STB_GPIO_CLK 	    RCC_APB2Periph_GPIOC	  	     // GPIO端口时钟  
#define PORT_08_12_STB_GPIO_PIN		    GPIO_Pin_2			               // 连接到SCL时钟线的GPIO 

#define PORT_08_12_SCK_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   
#define PORT_08_12_SCK_GPIO_PORT    	GPIOC			              
#define PORT_08_12_SCK_GPIO_CLK 	    RCC_APB2Periph_GPIOC		
#define PORT_08_12_SCK_GPIO_PIN		    GPIO_Pin_1			        

#define PORT_08_12_EN_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   
#define PORT_08_12_EN_GPIO_PORT    	  GPIOC			           
#define PORT_08_12_EN_GPIO_CLK 	      RCC_APB2Periph_GPIOC	
#define PORT_08_12_EN_GPIO_PIN		 	  GPIO_Pin_0			        

#define PORT_08_12_A_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd  
#define PORT_08_12_A_GPIO_PORT    	  GPIOB			           
#define PORT_08_12_A_GPIO_CLK 	      RCC_APB2Periph_GPIOB
#define PORT_08_12_A_GPIO_PIN		 		  GPIO_Pin_13			        

#define PORT_08_12_B_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   
#define PORT_08_12_B_GPIO_PORT    	  GPIOC			           
#define PORT_08_12_B_GPIO_CLK 	      RCC_APB2Periph_GPIOC	
#define PORT_08_12_B_GPIO_PIN		 		  GPIO_Pin_5			        

#define PORT_08_12_C_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   
#define PORT_08_12_C_GPIO_PORT    	  GPIOC			           
#define PORT_08_12_C_GPIO_CLK 	      RCC_APB2Periph_GPIOC	
#define PORT_08_12_C_GPIO_PIN		 		  GPIO_Pin_4			        

#define PORT_08_12_D_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   
#define PORT_08_12_D_GPIO_PORT    	  GPIOC			           
#define PORT_08_12_D_GPIO_CLK 	      RCC_APB2Periph_GPIOC	
#define PORT_08_12_D_GPIO_PIN		 		  GPIO_Pin_3			        


//控制卡08-1接口管脚定义

#define PORT_08_1_R1_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   
#define PORT_08_1_R1_GPIO_PORT    	GPIOB			           
#define PORT_08_1_R1_GPIO_CLK 	    RCC_APB2Periph_GPIOB	
#define PORT_08_1_R1_GPIO_PIN		 		GPIO_Pin_14			        

#define PORT_08_1_G1_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd  
#define PORT_08_1_G1_GPIO_PORT    	GPIOC			           
#define PORT_08_1_G1_GPIO_CLK 	    RCC_APB2Periph_GPIOC	
#define PORT_08_1_G1_GPIO_PIN		 		GPIO_Pin_6			        

#define PORT_08_1_R2_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd  
#define PORT_08_1_R2_GPIO_PORT    	GPIOB			           
#define PORT_08_1_R2_GPIO_CLK 	    RCC_APB2Periph_GPIOB	
#define PORT_08_1_R2_GPIO_PIN		 		GPIO_Pin_15			        

#define PORT_08_1_G2_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   
#define PORT_08_1_G2_GPIO_PORT    	GPIOC			           
#define PORT_08_1_G2_GPIO_CLK 	    RCC_APB2Periph_GPIOC	
#define PORT_08_1_G2_GPIO_PIN		 		GPIO_Pin_7			        


//控制卡08-2接口管脚定义
#define PORT_08_2_R1_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   
#define PORT_08_2_R1_GPIO_PORT    	GPIOC			           
#define PORT_08_2_R1_GPIO_CLK 	    RCC_APB2Periph_GPIOC	
#define PORT_08_2_R1_GPIO_PIN		 		GPIO_Pin_8			        

#define PORT_08_2_G1_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd  
#define PORT_08_2_G1_GPIO_PORT    	GPIOC			           
#define PORT_08_2_G1_GPIO_CLK 	    RCC_APB2Periph_GPIOC	
#define PORT_08_2_G1_GPIO_PIN		 		GPIO_Pin_10			        

#define PORT_08_2_R2_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd  
#define PORT_08_2_R2_GPIO_PORT    	GPIOC			           
#define PORT_08_2_R2_GPIO_CLK 	    RCC_APB2Periph_GPIOC	
#define PORT_08_2_R2_GPIO_PIN		 		GPIO_Pin_9		        

#define PORT_08_2_G2_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   
#define PORT_08_2_G2_GPIO_PORT    	GPIOC			           
#define PORT_08_2_G2_GPIO_CLK 	    RCC_APB2Periph_GPIOC	
#define PORT_08_2_G2_GPIO_PIN		 		GPIO_Pin_12			        

//控制卡12-1接口管脚定义
#define PORT_12_1_R_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   
#define PORT_12_1_R_GPIO_PORT    	  GPIOB			           
#define PORT_12_1_R_GPIO_CLK 	      RCC_APB2Periph_GPIOB	
#define PORT_12_1_R_GPIO_PIN		 		GPIO_Pin_14			        

#define PORT_12_1_G_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd  
#define PORT_12_1_G_GPIO_PORT    	  GPIOC			           
#define PORT_12_1_G_GPIO_CLK 	      RCC_APB2Periph_GPIOC	
#define PORT_12_1_G_GPIO_PIN		 	  GPIO_Pin_6			        

//控制卡12-2接口管脚定义
#define PORT_12_2_R_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   
#define PORT_12_2_R_GPIO_PORT    	  GPIOB			           
#define PORT_12_2_R_GPIO_CLK 	      RCC_APB2Periph_GPIOB
#define PORT_12_2_R_GPIO_PIN		 		GPIO_Pin_15

#define PORT_12_2_G_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd  
#define PORT_12_2_G_GPIO_PORT    	  GPIOC			           
#define PORT_12_2_G_GPIO_CLK 	      RCC_APB2Periph_GPIOC	
#define PORT_12_2_G_GPIO_PIN		 		GPIO_Pin_7			        

//控制卡12-3接口管脚定义
#define PORT_12_3_R_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   
#define PORT_12_3_R_GPIO_PORT    	  GPIOC			           
#define PORT_12_3_R_GPIO_CLK 	      RCC_APB2Periph_GPIOC	
#define PORT_12_3_R_GPIO_PIN		 		GPIO_Pin_8		        

#define PORT_12_3_G_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd  
#define PORT_12_3_G_GPIO_PORT    	  GPIOC			           
#define PORT_12_3_G_GPIO_CLK 	      RCC_APB2Periph_GPIOC	
#define PORT_12_3_G_GPIO_PIN		 		GPIO_Pin_10			        

//控制卡12-4接口管脚定义
#define PORT_12_4_R_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd   
#define PORT_12_4_R_GPIO_PORT    	  GPIOC			           
#define PORT_12_4_R_GPIO_CLK 	      RCC_APB2Periph_GPIOC	
#define PORT_12_4_R_GPIO_PIN		 		GPIO_Pin_9			        

#define PORT_12_4_G_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd  
#define PORT_12_4_G_GPIO_PORT    	  GPIOC			           
#define PORT_12_4_G_GPIO_CLK 	      RCC_APB2Periph_GPIOC	
#define PORT_12_4_G_GPIO_PIN		 		GPIO_Pin_12		  

//控制卡接口使用的管脚定义结束

/** 定义管脚输出电平
  * 1 - 输出高电平
  * 0 - 输出低电平
  */
#define ON  1
#define OFF 0

/* 带参宏，可以像内联函数一样使用 */
#define STB(x)	if (x)	\
					GPIO_SetBits(PORT_08_12_STB_GPIO_PORT,PORT_08_12_STB_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_12_STB_GPIO_PORT,PORT_08_12_STB_GPIO_PIN)

#define CLK(x)	if (x)	\
					GPIO_SetBits(PORT_08_12_SCK_GPIO_PORT,PORT_08_12_SCK_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_12_SCK_GPIO_PORT,PORT_08_12_SCK_GPIO_PIN)

#define EN(x)	if (x)	\
					GPIO_SetBits(PORT_08_12_EN_GPIO_PORT,PORT_08_12_EN_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_12_EN_GPIO_PORT,PORT_08_12_EN_GPIO_PIN)
					
#define A(x)	if (x)	\
					GPIO_SetBits(PORT_08_12_A_GPIO_PORT,PORT_08_12_A_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_12_A_GPIO_PORT,PORT_08_12_A_GPIO_PIN)

#define B(x)	if (x)	\
					GPIO_SetBits(PORT_08_12_B_GPIO_PORT,PORT_08_12_B_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_12_B_GPIO_PORT,PORT_08_12_B_GPIO_PIN)
					
#define C(x)	if (x)	\
					GPIO_SetBits(PORT_08_12_C_GPIO_PORT,PORT_08_12_C_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_12_C_GPIO_PORT,PORT_08_12_C_GPIO_PIN)
					
#define D(x)	if (x)	\
					GPIO_SetBits(PORT_08_12_D_GPIO_PORT,PORT_08_12_D_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_12_D_GPIO_PORT,PORT_08_12_D_GPIO_PIN)

//08-1接口管脚输出电平定义
#define PORT_08_1_R1(x)	if (x)	\
					GPIO_SetBits(PORT_08_1_R1_GPIO_PORT,PORT_08_1_R1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_1_R1_GPIO_PORT,PORT_08_1_R1_GPIO_PIN)
#define PORT_08_1_G1(x)	if (x)	\
					GPIO_SetBits(PORT_08_1_G1_GPIO_PORT,PORT_08_1_G1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_1_G1_GPIO_PORT,PORT_08_1_G1_GPIO_PIN)
#define PORT_08_1_R2(x)	if (x)	\
					GPIO_SetBits(PORT_08_1_R2_GPIO_PORT,PORT_08_1_R2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_1_R2_GPIO_PORT,PORT_08_1_R2_GPIO_PIN)
#define PORT_08_1_G2(x)	if (x)	\
					GPIO_SetBits(PORT_08_1_G2_GPIO_PORT,PORT_08_1_G2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_1_G2_GPIO_PORT,PORT_08_1_G2_GPIO_PIN)

//08-2接口管脚输出电平定义
#define PORT_08_2_R1(x)	if (x)	\
					GPIO_SetBits(PORT_08_2_R1_GPIO_PORT,PORT_08_2_R1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_2_R1_GPIO_PORT,PORT_08_2_R1_GPIO_PIN)
#define PORT_08_2_G1(x)	if (x)	\
					GPIO_SetBits(PORT_08_2_G1_GPIO_PORT,PORT_08_2_G1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_2_G1_GPIO_PORT,PORT_08_2_G1_GPIO_PIN)
#define PORT_08_2_R2(x)	if (x)	\
					GPIO_SetBits(PORT_08_2_R2_GPIO_PORT,PORT_08_2_R2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_2_R2_GPIO_PORT,PORT_08_2_R2_GPIO_PIN)
#define PORT_08_2_G2(x)	if (x)	\
					GPIO_SetBits(PORT_08_2_G2_GPIO_PORT,PORT_08_2_G2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_08_2_G2_GPIO_PORT,PORT_08_2_G2_GPIO_PIN)

//12-1接口管脚输出电平定义
#define PORT_12_1_R(x)	if (x)	\
					GPIO_SetBits(PORT_12_1_R_GPIO_PORT,PORT_12_1_R_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_12_1_R_GPIO_PORT,PORT_12_1_R_GPIO_PIN)
#define PORT_12_1_G(x)	if (x)	\
					GPIO_SetBits(PORT_12_1_G_GPIO_PORT,PORT_12_1_G_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_12_1_G_GPIO_PORT,PORT_12_1_G_GPIO_PIN)

//12-2接口管脚输出电平定义
#define PORT_12_2_R(x)	if (x)	\
					GPIO_SetBits(PORT_12_2_R_GPIO_PORT,PORT_12_2_R_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_12_2_R_GPIO_PORT,PORT_12_2_R_GPIO_PIN)
#define PORT_12_2_G(x)	if (x)	\
					GPIO_SetBits(PORT_12_2_G_GPIO_PORT,PORT_12_2_G_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_12_2_G_GPIO_PORT,PORT_12_2_G_GPIO_PIN)

//12-3接口管脚输出电平定义
#define PORT_12_3_R(x)	if (x)	\
					GPIO_SetBits(PORT_12_3_R_GPIO_PORT,PORT_12_3_R_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_12_3_R_GPIO_PORT,PORT_12_3_R_GPIO_PIN)
#define PORT_12_3_G(x)	if (x)	\
					GPIO_SetBits(PORT_12_3_G_GPIO_PORT,PORT_12_3_G_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_12_3_G_GPIO_PORT,PORT_12_3_G_GPIO_PIN)

//12-4接口管脚输出电平定义
#define PORT_12_4_R(x)	if (x)	\
					GPIO_SetBits(PORT_12_4_R_GPIO_PORT,PORT_12_4_R_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_12_4_R_GPIO_PORT,PORT_12_4_R_GPIO_PIN)
#define PORT_12_4_G(x)	if (x)	\
					GPIO_SetBits(PORT_12_4_G_GPIO_PORT,PORT_12_4_G_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PORT_12_4_G_GPIO_PORT,PORT_12_4_G_GPIO_PIN)


					
//控制卡接口管脚初始化
void Port_08_12_GPIO_Config(void);
void Port_08_12_Comm_GPIO_Config(void); //08、12接口共用管脚初始化
void Port_08_1_GPIO_Config(void);       //08-1接口管脚初始化
void Port_08_2_GPIO_Config(void);
void Port_12_1_GPIO_Config(void);
void Port_12_2_GPIO_Config(void);
void Port_12_3_GPIO_Config(void);
void Port_12_4_GPIO_Config(void);

#endif

