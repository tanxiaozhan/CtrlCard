#include "esp8266.h"
#include "cmsis_os.h"                   /* ARM::CMSIS:RTOS:Keil RTX */




/*
 * 函数名：ESP8266_rst
 * 描述  ：重启WF-ESP8266模块
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被 main() 调用
 */
void ESP8266_rst ( void )
{
	 ESP8266_RST_LOW_LEVEL();    
	 osDelay ( 500 ); 
	 ESP8266_RST_HIGH_LEVEL();

}

/**
  * @brief  ESP8266初始化函数
  * @param  无
  * @retval 无
  */
void ESP8266_init ( void )
{
	ESP8266_GPIO_Config (); 
	ESP8266_RST_HIGH_LEVEL();
	ESP8266_DISABLE();    //关闭WIFI，ESP8266第7管脚高电平使能，低电平进入休眠

}


/**
  * @brief  初始化ESP8266用到的GPIO引脚
  * @param  无
  * @retval 无
  */
static void ESP8266_GPIO_Config ( void )
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 配置 ENABLE 引脚*/
	ESP8266_CH_PD_APBxClock_FUN ( ESP8266_CH_PD_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = ESP8266_CH_PD_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init ( ESP8266_CH_PD_PORT, & GPIO_InitStructure );	 
	
	/* 配置 RST 引脚*/
	ESP8266_RST_APBxClock_FUN ( ESP8266_RST_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = ESP8266_RST_PIN;	
	GPIO_Init ( ESP8266_RST_PORT, & GPIO_InitStructure );	 
}
