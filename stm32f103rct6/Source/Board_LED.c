#include "Board_LED.h"


void Board_LED_GPIO_Config ( void )
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置引脚速率为50MHz    

	//配置STB引脚
	Board_LED_GPIO_APBxClock_FUN ( Board_LED_GPIO_CLK, ENABLE );  //开启GPIOB的外设时钟
	GPIO_InitStructure.GPIO_Pin = Board_LED_GPIO_PIN;	                //选择要控制的GPIOB引脚	
	GPIO_Init ( Board_LED_GPIO_PORT, & GPIO_InitStructure );	        //调用库函数，初始化GPIO
}

