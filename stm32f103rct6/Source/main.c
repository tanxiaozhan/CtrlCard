/*------------------------------------------------------------------------------
 * 
 * LED��ʾ�����ƿ���Ŀ
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
  Main Thread 'main': ���ƿ��������ʼ��
 *---------------------------------------------------------------------------*/
int main (void) {

		USARTx_Config();	

		FLASH_SPI_initialize();   //��ʼ���ⲿ���棬��25Q64
	
		netInitialize ();    		  //������ĳ�ʼ��
		netSLIP_Listen();         //��ʼSLIP����
		osDelay(1000);

		netHTTPs_LoginOnOff (false);

		LED_display_init();    		 //��ʼ��LED��ʾ��
		//LED_display_start();     //������ʾ
		Board_LED_GPIO_Config();
	
		//WIFIģ���ʼ��������
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
