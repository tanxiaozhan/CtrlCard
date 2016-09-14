/*
**************************************
* 
* 控制卡控制信号主程�
*  
* 创建日期：2016.09.12 
*
*************************************
*/

#include "cmsis_os.h"
#include "LED_Display.h"
#include "Port_08_12.h"
#include "Ascii.h"

char data[DISPLAY_STRING_LENGTH]="LED";     //LED屏显示的文字信息，通过浏览器设置修改

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityAboveNormal, 1, 0);

/*----------------------------------------------------------------------------
  线程'LED_Dispaly': 驱动LED显示屏显示信息
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {
		uint8_t row=4;   // 1/4扫描  
		char * pStr=data;
		uint8_t charDot[2];    //保存当前显示字符的点阵信息，英文宽8位，汉字宽16位
		uint8_t i,j,k;
		uint16_t scan;    //列扫描
		uint8_t dot;
		EN(ON);
		while(1){

			for(i=0;i<row;i++){
				EN(OFF);
				A( i & 0x01 );B( i & 0x02 );C( i & 0x04 );D( i & 0x08 );   //行扫描

				pStr=data;
				while(* pStr != '\0'){
					
					if( *pStr<=126){	//英文字符
						for(k=0;k<4;k++){
						charDot[0] = ~(unsigned char)ascii_Dot[ *pStr - ' '][12-k*4+i];
						scan=0x01;
						for(j=0;j<8;j++){
							CLK(OFF);     
							dot = charDot[0] & scan;
							PORT_08_1_R1(dot);
							PORT_12_1_R(dot);
							scan <<= 1;
							CLK(ON);       //594移位信号
						}
					}
					pStr ++;
				}
				else{   //汉字字符
					uint16_t char_GBK_Code;    //汉字的国标码
					uint16_t GBK_Dot;          //汉字的点阵数据
					char_GBK_Code = * (uint16_t *)pStr;       //取一个汉字的国标码，二字节
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //国标码前后字节对调
					for(k=0;k<4;k++){
						get_GBK_Code( charDot, char_GBK_Code,12-k*4+i );   //读取汉字字模一行点阵数据2字节
						GBK_Dot = ( charDot[0] << 8 ) + charDot[1];
						scan=0x0001;
						for(j=0;j<16;j++){
							CLK(OFF);
							dot=GBK_Dot & scan;
							PORT_08_1_R1(dot);
							PORT_12_1_R(dot);
							scan <<= 1;
							CLK(ON);       //594移位信号
					}
				}
					pStr += 2;      //一个汉字占二个字节
				}
			}
			STB(OFF);
			STB(ON);     //锁存
			EN(ON);
			osDelay(15);
		}
	}
}


/*
	********************************************************
	*
	*  创建LED显示屏控制信号线程，控制卡控制信号开始输出
	*
	*
	********************************************************
*/

void LED_Display_Start(void){
	osThreadCreate (osThread(LED_Display), NULL);
}

