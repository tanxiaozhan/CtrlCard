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
#include "rtc.h"

/*时间结构体*/
struct rtc_time system_time;

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

		
	   /*  把标准时间转换为北京时间*/
	  to_tm(RTC_GetCounter() + 8*60*60, &system_time);/*把定时器的值转换为北京时间*/	
		printf("\ndate:%d-%d-%d  %d:%d:%d\n",system_time.tm_year,system_time.tm_mon,system_time.tm_mday,
		         system_time.tm_hour,system_time.tm_min,system_time.tm_sec );
		osDelay (2050);
		
  }
}



/*----------------------------------------------------------------------------
  Main Thread 'main': 控制卡、网络初始化
 *---------------------------------------------------------------------------*/
int main (void) {

		FLASH_SPI_initialize();
		USARTx_Config();	

		//LED指示灯
		LED_Initialize ();
		osThreadCreate (osThread(BlinkLed), NULL);
	
		LED_display_init();      //显示屏初始化
		LED_display_start();     //控制卡输出启动，LED显示屏开始显示
	
		netInitialize ();    		//网络核心初始化
		netSLIP_Listen();      //开始SLIP监听
		osDelay(500);

netHTTPs_LoginOnOff (false);
	
		ESP8266_init();      //初始化控制EPS8266用到的PGIO引脚
		ESP8266_ENABLE();   //打开WIFI

		printf("\nLED CtrlCard is running...\n");

		while(1) {
			
		}
}
