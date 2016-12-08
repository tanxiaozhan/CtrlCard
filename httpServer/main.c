#include <stdio.h>
#include "cmsis_os.h"                   /* ARM::CMSIS:RTOS:Keil RTX */
#include "rl_net.h"                     /* Network definitions                */


int main (void) {

	
	int i,j;
	i=0;
	i=10;
	j=i*2;
	i=100;
	
		netInitialize ();    		//网络核心初始化
		netSLIP_Listen();      //开始SLIP监听
		osDelay(500);

//netHTTPs_LoginOnOff (false);
	

//		printf("\nLED CtrlCard is running...\n");

		while(1) {
			
		}
}
