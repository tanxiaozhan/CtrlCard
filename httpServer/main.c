#include <stdio.h>
#include "cmsis_os.h"                   /* ARM::CMSIS:RTOS:Keil RTX */
#include "rl_net.h"                     /* Network definitions                */


int main (void) {

	
	int i,j;
	i=0;
	i=10;
	j=i*2;
	i=100;
	
		netInitialize ();    		//������ĳ�ʼ��
		netSLIP_Listen();      //��ʼSLIP����
		osDelay(500);

//netHTTPs_LoginOnOff (false);
	

//		printf("\nLED CtrlCard is running...\n");

		while(1) {
			
		}
}
