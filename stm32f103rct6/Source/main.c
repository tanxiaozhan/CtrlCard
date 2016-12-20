/*------------------------------------------------------------------------------
 * 
 * LED显示屏控制卡项目
 * 
 * 2016-08-06
 *------------------------------------------------------------------------------
 * Name:    main.c
 * Purpose: main
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "cmsis_os.h"                   /* ARM::CMSIS:RTOS:Keil RTX */
#include "rl_net.h"                     /* Network definitions                */

#include "rl_net_lib.h"

#include "flash_SPI.h"
#include "usartX.h"
#include "esp8266.h"
#include "Board_LED.h"
#include "LED_Display.h"

/*----------------------------------------------------------------------------
  Main Thread 'main': 控制卡、网络初始化
 *---------------------------------------------------------------------------*/
int main (void) {

		USARTx_Config();	

		FLASH_SPI_initialize();   //初始化外部闪存，（25Q64
	
		netInitialize ();    		  //网络核心初始化
		netSLIP_Listen();         //开始SLIP监听
		osDelay(1000);

		netHTTPs_LoginOnOff (false);

		LED_display_init();    		 //初始化LED显示屏
		//LED_display_start();     //开启显示
		Board_LED_GPIO_Config();
	
		//WIFI模块初始化、开启
		ESP8266_init();
		ESP8266_rst();
		
		printf("\r\nLED CtrlCard is running...\r\n");
	
		while(1) {
			
			Board_LED(ON);
			osDelay(1000);
			Board_LED(OFF);
			osDelay(1000);
		}
}
