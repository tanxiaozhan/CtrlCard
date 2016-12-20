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

		uint8_t Rx_Buffer[20];
		
		USARTx_Config();	
		printf("\nUSART Config OK\r\n");

		FLASH_SPI_initialize();

		netInitialize ();    		//������ĳ�ʼ��
		netSLIP_Listen();      //��ʼSLIP����
		osDelay(1000);

		netHTTPs_LoginOnOff (false);

		LED_display_init();    		//��ʼ��LED��ʾ��
		//LED_display_start();      //������ʾ
		Board_LED_GPIO_Config();
	
		//WIFIģ���ʼ��������
		ESP8266_init();
		ESP8266_rst();
		
		printf("\r\nLED CtrlCard is running...\r\n");
		
		
printf("flash ID:%06x",SPI_FLASH_ReadID());
		SPI_FLASH_SectorErase(0);	 	 
		
		SPI_FLASH_BufferWrite((uint8_t *)"screen:64*32", 0, 12);
		printf("\r\n д�������Ϊ��%s \r\n", "screen:64*32");
		SPI_FLASH_BufferRead(Rx_Buffer, 0, 12);
		printf("\r\n ����������Ϊ��%s \r\n", Rx_Buffer);
		
	
		while(1) {
			
			Board_LED(ON);
			osDelay(1000);
			Board_LED(OFF);
			osDelay(1000);
		}
}
