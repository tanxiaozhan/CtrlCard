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

		uint8_t Rx_Buffer[20];
		
		USARTx_Config();	
		printf("\nUSART Config OK\r\n");

		FLASH_SPI_initialize();

		netInitialize ();    		//网络核心初始化
		netSLIP_Listen();      //开始SLIP监听
		osDelay(1000);

		netHTTPs_LoginOnOff (false);

		LED_display_init();    		//初始化LED显示屏
		//LED_display_start();      //启动显示
		Board_LED_GPIO_Config();
	
		//WIFI模块初始化、开启
		ESP8266_init();
		ESP8266_rst();
		
		printf("\r\nLED CtrlCard is running...\r\n");
		
		
printf("flash ID:%06x",SPI_FLASH_ReadID());
		SPI_FLASH_SectorErase(0);	 	 
		
		SPI_FLASH_BufferWrite((uint8_t *)"screen:64*32", 0, 12);
		printf("\r\n 写入的数据为：%s \r\n", "screen:64*32");
		SPI_FLASH_BufferRead(Rx_Buffer, 0, 12);
		printf("\r\n 读出的数据为：%s \r\n", Rx_Buffer);
		
	
		while(1) {
			
			Board_LED(ON);
			osDelay(1000);
			Board_LED(OFF);
			osDelay(1000);
		}
}
