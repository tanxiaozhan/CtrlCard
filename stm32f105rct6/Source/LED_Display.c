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

char data[DISPLAY_STRING_LENGTH]="LED";     //LED����ʾ��������Ϣ��ͨ������������޸�

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityAboveNormal, 1, 0);

/*----------------------------------------------------------------------------
  �߳�'LED_Dispaly': ����LED��ʾ����ʾ��Ϣ
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {
		uint8_t row=4;   // 1/4ɨ��  
		char * pStr=data;
		uint8_t charDot[2];    //���浱ǰ��ʾ�ַ��ĵ�����Ϣ��Ӣ�Ŀ�8λ�����ֿ�16λ
		uint8_t i,j,k;
		uint16_t scan;    //��ɨ��
		uint8_t dot;
		EN(ON);
		while(1){

			for(i=0;i<row;i++){
				EN(OFF);
				A( i & 0x01 );B( i & 0x02 );C( i & 0x04 );D( i & 0x08 );   //��ɨ��

				pStr=data;
				while(* pStr != '\0'){
					
					if( *pStr<=126){	//Ӣ���ַ�
						for(k=0;k<4;k++){
						charDot[0] = ~(unsigned char)ascii_Dot[ *pStr - ' '][12-k*4+i];
						scan=0x01;
						for(j=0;j<8;j++){
							CLK(OFF);     
							dot = charDot[0] & scan;
							PORT_08_1_R1(dot);
							PORT_12_1_R(dot);
							scan <<= 1;
							CLK(ON);       //594��λ�ź�
						}
					}
					pStr ++;
				}
				else{   //�����ַ�
					uint16_t char_GBK_Code;    //���ֵĹ�����
					uint16_t GBK_Dot;          //���ֵĵ�������
					char_GBK_Code = * (uint16_t *)pStr;       //ȡһ�����ֵĹ����룬���ֽ�
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //������ǰ���ֽڶԵ�
					for(k=0;k<4;k++){
						get_GBK_Code( charDot, char_GBK_Code,12-k*4+i );   //��ȡ������ģһ�е�������2�ֽ�
						GBK_Dot = ( charDot[0] << 8 ) + charDot[1];
						scan=0x0001;
						for(j=0;j<16;j++){
							CLK(OFF);
							dot=GBK_Dot & scan;
							PORT_08_1_R1(dot);
							PORT_12_1_R(dot);
							scan <<= 1;
							CLK(ON);       //594��λ�ź�
					}
				}
					pStr += 2;      //һ������ռ�����ֽ�
				}
			}
			STB(OFF);
			STB(ON);     //����
			EN(ON);
			osDelay(15);
		}
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

