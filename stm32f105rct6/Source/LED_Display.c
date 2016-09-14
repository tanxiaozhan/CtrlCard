/*
**************************************
* 
* ¿ØÖÆ¿¨¿ØÖÆĞÅºÅÖ÷³ÌĞ
*  
* ´´½¨ÈÕÆÚ£º2016.09.12 
*
*************************************
*/

#include "cmsis_os.h"
#include "LED_Display.h"
#include "Port_08_12.h"
#include "Ascii.h"

char data[DISPLAY_STRING_LENGTH]="LED";     //LEDÆÁÏÔÊ¾µÄÎÄ×ÖĞÅÏ¢£¬Í¨¹ıä¯ÀÀÆ÷ÉèÖÃĞŞ¸Ä

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityAboveNormal, 1, 0);

/*----------------------------------------------------------------------------
  Ïß³Ì'LED_Dispaly': Çı¶¯LEDÏÔÊ¾ÆÁÏÔÊ¾ĞÅÏ¢
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {
		uint8_t row=4;   // 1/4É¨Ãè  
		char * pStr=data;
		uint8_t charDot[2];    //±£´æµ±Ç°ÏÔÊ¾×Ö·ûµÄµãÕóĞÅÏ¢£¬Ó¢ÎÄ¿í8Î»£¬ºº×Ö¿í16Î»
		uint8_t i,j,k;
		uint16_t scan;    //ÁĞÉ¨Ãè
		uint8_t dot;
		EN(ON);
		while(1){

			for(i=0;i<row;i++){
				EN(OFF);
				A( i & 0x01 );B( i & 0x02 );C( i & 0x04 );D( i & 0x08 );   //ĞĞÉ¨Ãè

				pStr=data;
				while(* pStr != '\0'){
					
					if( *pStr<=126){	//Ó¢ÎÄ×Ö·û
						for(k=0;k<4;k++){
						charDot[0] = ~(unsigned char)ascii_Dot[ *pStr - ' '][12-k*4+i];
						scan=0x01;
						for(j=0;j<8;j++){
							CLK(OFF);     
							dot = charDot[0] & scan;
							PORT_08_1_R1(dot);
							PORT_12_1_R(dot);
							scan <<= 1;
							CLK(ON);       //594ÒÆÎ»ĞÅºÅ
						}
					}
					pStr ++;
				}
				else{   //ºº×Ö×Ö·û
					uint16_t char_GBK_Code;    //ºº×ÖµÄ¹ú±êÂë
					uint16_t GBK_Dot;          //ºº×ÖµÄµãÕóÊı¾İ
					char_GBK_Code = * (uint16_t *)pStr;       //È¡Ò»¸öºº×ÖµÄ¹ú±êÂë£¬¶ş×Ö½Ú
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //¹ú±êÂëÇ°ºó×Ö½Ú¶Ôµ÷
					for(k=0;k<4;k++){
						get_GBK_Code( charDot, char_GBK_Code,12-k*4+i );   //¶ÁÈ¡ºº×Ö×ÖÄ£Ò»ĞĞµãÕóÊı¾İ2×Ö½Ú
						GBK_Dot = ( charDot[0] << 8 ) + charDot[1];
						scan=0x0001;
						for(j=0;j<16;j++){
							CLK(OFF);
							dot=GBK_Dot & scan;
							PORT_08_1_R1(dot);
							PORT_12_1_R(dot);
							scan <<= 1;
							CLK(ON);       //594ÒÆÎ»ĞÅºÅ
					}
				}
					pStr += 2;      //Ò»¸öºº×ÖÕ¼¶ş¸ö×Ö½Ú
				}
			}
			STB(OFF);
			STB(ON);     //Ëø´æ
			EN(ON);
			osDelay(15);
		}
	}
}


/*
	********************************************************
	*
	*  ´´½¨LEDÏÔÊ¾ÆÁ¿ØÖÆĞÅºÅÏß³Ì£¬¿ØÖÆ¿¨¿ØÖÆĞÅºÅ¿ªÊ¼Êä³ö
	*
	*
	********************************************************
*/

void LED_Display_Start(void){
	osThreadCreate (osThread(LED_Display), NULL);
}

