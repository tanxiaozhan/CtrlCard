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

char data[DISPLAY_STRING_LENGTH]="LEDÆÁ";     //LEDÆÁÏÔÊ¾µÄÎÄ×ÖĞÅÏ¢£¬Í¨¹ıä¯ÀÀÆ÷ÉèÖÃĞŞ¸Ä

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityNormal, 1, 0);

/*----------------------------------------------------------------------------
  Ïß³Ì'LED_Dispaly': Çı¶¯LEDÏÔÊ¾ÆÁÏÔÊ¾ĞÅÏ¢
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {
		uint8_t row=16;  
		uint8_t row_no;  //µ±Ç°ĞĞºÅ
		char * pStr=data;
		uint8_t charDot[2];    //±£´æµ±Ç°ÏÔÊ¾×Ö·ûµÄµãÕóĞÅÏ¢£¬Ó¢ÎÄ¿í8Î»£¬ºº×Ö¿í16Î»
		uint8_t i,j;
		uint16_t scan;    //ÁĞÉ¨Ãè
		EN(ON);
		while(1){
			
			for(i=0;i<row;i++){
				//row_no=i+1;
				A( OFF );B( OFF );C( OFF );D( OFF );   //¹Ø±ÕĞĞ
				pStr=data;
				while(* pStr != '\0'){
					
					if( *pStr<=126){	//Ó¢ÎÄ×Ö·û
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
							CLK(ON);       //594ÒÆÎ»ĞÅºÅ
						}
					pStr ++;
				}
				else{   //ºº×Ö×Ö·û
					uint16_t char_GBK_Code;    //ºº×ÖµÄ¹ú±êÂë
					uint16_t GBK_Dot;          //ºº×ÖµÄµãÕóÊı¾İ
					char_GBK_Code = * (uint16_t *)pStr;       //È¡Ò»¸öºº×ÖµÄ¹ú±êÂë£¬¶ş×Ö½Ú
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //¹ú±êÂëÇ°ºó×Ö½Ú¶Ôµ÷
					get_GBK_Code( charDot, char_GBK_Code );   //È¡ºº×Ö×ÖÄ£Êı¾İ
					GBK_Dot = ( charDot[0] << 8 ) + charDot[1];
					scan=0x0001;
					for(j=0;j<16;j++){
							CLK(OFF);
							PORT_08_1_R1(GBK_Dot & scan);
							PORT_08_2_R1(GBK_Dot & scan);
							PORT_12_1_R(GBK_Dot & scan);
							PORT_12_2_R(GBK_Dot & scan);
							scan <<= 1;
							CLK(ON);       //594ÒÆÎ»ĞÅºÅ
					}
					
					pStr += 2;      //Ò»¸öºº×ÖÕ¼¶ş¸ö×Ö½Ú
				}
			}
			STB(OFF);
			osDelay(1000);  //ÑÓÊ±
			STB(ON);     //Ëø´æ
			row_no=i+1;
			A( row_no & 0x01 );B( row_no & 0x02 );C( row_no & 0x04 );D( row_no & 0x08 );   //ĞĞÉ¨Ãè
			osDelay(10);
		}
		osDelay(50);
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

