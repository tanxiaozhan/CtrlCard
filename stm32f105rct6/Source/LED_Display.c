/*
**************************************
* 
* ���ƿ������ź������
*  
* �������ڣ�2016.09.12 
*
*************************************
*/

#include "cmsis_os.h"
#include "LED_Display.h"
#include "Port_08_12.h"
#include "Ascii.h"

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityNormal, 1, 0);

/*----------------------------------------------------------------------------
  �߳�'LED_Dispaly': ����LED��ʾ����ʾ��Ϣ
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {
		uint8_t row=16;  
		uint8_t col=64;
		uint8_t crow;  //��ǰ��
		char data[]="LED";
		char * pdata=data;
		uint8_t charDot;
		uint8_t i,j;
		uint8_t scan;    //��ɨ��
		EN(ON);
		while(1){
			
			for(i=0;i<row;i++){
				crow=i+1;
				A( crow & 0x01 );B( crow & 0x02 );C( crow & 0x04 );D( crow & 0x08 );   //��ɨ��
				while(* pdata){
						charDot = ascii_Dot[ *pdata - ' ',i];
						scan=0x01;
						for(j=0;j<8;j++){
							CLK(OFF);     
							PORT_08_1_R1(charDot & scan);
							PORT_08_2_R1(charDot & scan);
							PORT_12_1_R(charDot & scan);
							PORT_12_2_R(charDot & scan);
							scan <<= 1;
							CLK(ON);       //594��λ�ź�
						}
					pdata ++;
				}
				STB(OFF);
				osDelay(1);  //��ʱ
				STB(ON);
			}
		osDelay(50);
		}
}

/*
	********************************************************
	*
	*  ����LED��ʾ�������ź��̣߳����ƿ������źſ�ʼ���
	*
	*
	********************************************************
*/

void LED_Display_Start(void){
	osThreadCreate (osThread(LED_Display), NULL);
}

