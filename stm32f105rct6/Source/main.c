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
#include "Usart1.h"
#include "cmsis_os.h"                   /* ARM::CMSIS:RTOS:Keil RTX */
#include "rl_net.h"                     /* Network definitions                */

#include "Board_LED.h"
#include "esp8266.h"
#include "rl_net_lib.h"
#include "Net_Config_TCP.h"

#include "Port_08_12.h"
#include "esp8266.h"
#include "LED_Display.h"



bool LEDrun;

static void BlinkLed (void const *arg);
osThreadDef(BlinkLed, osPriorityNormal, 1, 0);

/*----------------------------------------------------------------------------
  Thread 'BlinkLed': Blink the LEDs on an eval board
 *---------------------------------------------------------------------------*/
static void BlinkLed (void const *arg) {
  const uint8_t led_val[8] = { 0xff,0x0,0xff,0x0,0xff,0x0,0xff,0x0 };
  char cnt = 0;
  LEDrun = true;
  while(1) {
    // Every 3000 ms
    if (LEDrun == true) {
      LED_SetOut (led_val[cnt]);
      if (++cnt >= sizeof(led_val)) {
        cnt = 0;
      }
    }
		
		osDelay (1050);
		
  }
}



/*----------------------------------------------------------------------------
  Main Thread 'main': ���ƿ��������ʼ��
 *---------------------------------------------------------------------------*/
int main (void) {

		Port_08_12_GPIO_Config();    //��ʼ�����ƿ�08��12����ӿ��õ���GPIO����
		USARTx_Config();	

		//LEDָʾ��
		LED_Initialize ();
		osThreadCreate (osThread(BlinkLed), NULL);
	
		LED_Display_Start();     //���ƿ����������LED��ʾ����ʼ��ʾ
	
		netInitialize ();    		//������ĳ�ʼ��
		netSLIP_Listen();      //��ʼSLIP����
		osDelay(500);

		ESP8266_init();      //��ʼ������EPS8266�õ���PGIO����
		ESP8266_ENABLE();   //��WIFI

		printf("\nLED CtrlCard is running...\n");

		while(1) {
			
		}
}