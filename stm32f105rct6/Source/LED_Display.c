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

char data[DISPLAY_STRING_LENGTH]="LED��";     //LED����ʾ��������Ϣ��ͨ������������޸�

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityNormal, 1, 0);

/*----------------------------------------------------------------------------
  �߳�'LED_Dispaly': ����LED��ʾ����ʾ��Ϣ
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {
		uint8_t row=16;  
		uint8_t row_no;  //��ǰ�к�
		char * pStr=data;
		uint8_t charDot[2];    //���浱ǰ��ʾ�ַ��ĵ�����Ϣ��Ӣ�Ŀ�8λ�����ֿ�16λ
		uint8_t i,j;
		uint16_t scan;    //��ɨ��
		EN(ON);
		while(1){
			
			for(i=0;i<row;i++){
				//row_no=i+1;
				A( OFF );B( OFF );C( OFF );D( OFF );   //�ر���
				pStr=data;
				while(* pStr != '\0'){
					
					if( *pStr<=126){	//Ӣ���ַ�
						printf("disp:%c  \n\n",*pStr);
						charDot[0] = (unsigned char)ascii_Dot[ *pStr - ' '][i];
						printf("DOt:%02x,  \n",charDot[0]);
						scan=0x01;
						for(j=0;j<8;j++){
							printf("j=%d, 08_12_R=%d   \n\n",j,charDot[0] & scan);
							CLK(OFF);     
							PORT_08_1_R1(charDot[0] & scan);
							PORT_08_2_R1(charDot[0] & scan);
							PORT_12_1_R(charDot[0] & scan);
							PORT_12_2_R(charDot[0] & scan);
							scan <<= 1;
							CLK(ON);       //594��λ�ź�
						}
					pStr ++;
				}
				else{   //�����ַ�
					uint16_t char_GBK_Code;    //���ֵĹ�����
					uint16_t GBK_Dot;          //���ֵĵ�������
					char_GBK_Code = * (uint16_t *)pStr;       //ȡһ�����ֵĹ����룬���ֽ�
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //������ǰ���ֽڶԵ�
					get_GBK_Code( charDot, char_GBK_Code );   //ȡ������ģ����
					GBK_Dot = ( charDot[0] << 8 ) + charDot[1];
					scan=0x0001;
					for(j=0;j<16;j++){
							CLK(OFF);
							PORT_08_1_R1(GBK_Dot & scan);
							PORT_08_2_R1(GBK_Dot & scan);
							PORT_12_1_R(GBK_Dot & scan);
							PORT_12_2_R(GBK_Dot & scan);
							scan <<= 1;
							CLK(ON);       //594��λ�ź�
					}
					
					pStr += 2;      //һ������ռ�����ֽ�
				}
			}
			STB(OFF);
			osDelay(1000);  //��ʱ
			STB(ON);     //����
			row_no=i+1;
			A( row_no & 0x01 );B( row_no & 0x02 );C( row_no & 0x04 );D( row_no & 0x08 );   //��ɨ��
			osDelay(10);
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

