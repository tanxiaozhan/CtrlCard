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
#include <string.h>

screen_para screen;        //ÏÔÊ¾ÆÁ²ÎÊı
area_para area[MAX_AREA_NUMBER];           //ÏÔÊ¾Çø²ÎÊı

uint8_t max_len=0;    // ¸÷ÏÔÊ¾ÖĞÏÔÊ¾×Ö·û×î³¤µÄ³¤¶È


static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityAboveNormal, 1, 0);

/*----------------------------------------------------------------------------
 * Ïß³Ì'LED_Dispaly'£ºLEDÏÔÊ¾ÆÁÉ¨ÃèÏÔÊ¾
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {

	uint8_t screen_dot[screen.height][screen.width];    //±£´æÕû¸öÆÁµÄµãÕóÊı¾İ	
	int i;
	for(i=0;i<screen.area_number;i++){
		if(max_len < strlen(area[i].display_data) )    //»ñÈ¡¸÷ÏÔÊ¾ÇøÖĞ×Ö·ûÊı×î¶àµÄ
			max_len = strlen(area[i].display_data);         
	}
	
	switch(screen.scan_type){
		case SCAN_16:                   //1/16É¨Ãè
			break;
		
		case SCAN_8_LINE_1FOR8ROW:          //1/8Ö±Ïß×ßÏß£¬Ò»Â·Êı¾İ´ø8ĞĞ
			break;
		
		case SCAN_8_UP_TO_DOWN_1FOR16ROW:   //1/8ÉÏÉßĞĞ£¬Ò»Â·Êı¾İ´ø16ĞĞ£¬8ĞĞÕÛÁĞ
			break;
		
		case SCAN_8_DOWN_TO_UP_1FOR16ROW:   //1/8ÏÂÉßĞĞ£¬Ò»Â·Êı¾İ´ø16ĞĞ£¬8ĞĞÕÛÁĞ
			break;
		
		case SCAN_4_LINE_1FOR4_ROW:         //1/4Ö±Ïß×ßÏß£¬Ò»Â·Êı¾İ´ø4ĞĞ
			break;

		case 	SCAN_4_UP_TO_DOWN_1FOR16ROW:   //1/4ÉÏÉßĞĞ£¬Ò»Â·Êı¾İ´ø16ĞĞ£¬8ĞĞÕÛÁĞ
			scan_4_up_to_down_1for16row((char *)screen_dot);
			break;
		
		case SCAN_4_DOWN_TO_UP_1FOR16ROW:   //1/4ÏÂÉßĞĞ£¬Ò»Â·Êı¾İ´ø16ĞĞ
		break;
		
		case SCAN_4_UP_TO_DOWN_1FOR8ROW:    //1/4ÉÏÉßĞĞ£¬Ò»Â·Êı¾İ´ø8ĞĞ
			break;
		
		case SCAN_4_DOWN_TO_UP_1FOR8ROW:   //1/4ÏÂÉßĞĞ£¬Ò»Â·Êı¾İ´ø8ĞĞ
			break;
	}
}
	
void scan_4_up_to_down_1for16row(char * string_dot){	
	uint8_t row=4;   // 1/4É¨Ãè  
	//char charDot[2];    //±£´æµ±Ç°ÏÔÊ¾×Ö·ûµÄµãÕóĞÅÏ¢£¬Ó¢ÎÄ¿í8Î»£¬ºº×Ö¿í16Î»
	uint8_t i,j,k,len;
	uint16_t scan;    //ÁĞÉ¨Ãè
	uint8_t LED_width;   //ÆÁ¿í
	char dot;
	
	EN(ON);
	while(1){
				//EN(OFF);
	while(i<screen.width){
		for(j=0;j<screen.height;j++){
			for(k=0;k<screen.area_number;k++){
			if( area[k].display_data[len]<=126 ){  //Ó¢ÎÄ×Ö·û
				string_dot[len]= ;
				}
			}
			}
		}
		for(i=0;i<row;i++){
			for(LED_width=0;LED_width<screen.width;LED_width++){
					pStr[0]=area[0].display_data;
					if( *pStr[0]<=126){	//Ó¢ÎÄ×Ö·û
						for(k=0;k<4;k++){
						charDot[0] = ~ascii_Dot[ *pStr[area] - ' '][12-k*4+i];
						scan=0x01;
						for(j=0;j<8;j++){
							CLK(OFF);     
							dot = charDot[0] & scan;
							if(area==0){
								//PORT_08_1_R1(dot);
								PORT_12_1_R(dot);
							}
							else{
								//PORT_08_2_R1(dot);
								PORT_12_2_R(dot);
							}
							
							scan <<= 1;
							CLK(ON);       //594ÒÆÎ»ĞÅºÅ
						}
					}
					pStr[area] ++;
				}
				else{   //ºº×Ö×Ö·û
					uint16_t char_GBK_Code;    //ºº×ÖµÄ¹ú±êÂë
					uint16_t GBK_Dot;          //ºº×ÖµÄµãÕóÊı¾İ
					char_GBK_Code = * (uint16_t *)pStr[area];       //È¡Ò»¸öºº×ÖµÄ¹ú±êÂë£¬¶ş×Ö½Ú
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //¹ú±êÂëÇ°ºó×Ö½Ú¶Ôµ÷
					for(k=0;k<4;k++){
						get_GBK_Code( (uint8_t *)charDot, char_GBK_Code,12-k*4+i );   //¶ÁÈ¡ºº×Ö×ÖÄ£Ò»ĞĞµãÕóÊı¾İ2×Ö½Ú
						GBK_Dot = ( charDot[0] << 8 ) + charDot[1];
						scan=0x0001;
						for(j=0;j<16;j++){
							CLK(OFF);
							dot=GBK_Dot & scan;
							PORT_08_1_R1(dot);
							if(area==0)
								PORT_12_1_R(dot);
							else
								PORT_12_2_R(dot);
							
							scan <<= 1;
							CLK(ON);       //594ÒÆÎ»ĞÅºÅ
					}
				}
					pStr[area] += 2;      //Ò»¸öºº×ÖÕ¼¶ş¸ö×Ö½Ú
				}
			}
			STB(OFF);
			STB(ON);     //Ëø´æ

			A( i & 0x01 );B( i & 0x02 );C( i & 0x04 );D( i & 0x08 );   //ĞĞÉ¨Ãè
			osDelay(5);
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

/*
	********************************************************
	*  LEDÏÔÊ¾ÆÁ³õÊ¼»¯
	********************************************************
*/
void LED_Display_Init(void){

	Port_08_12_GPIO_Config();    //³õÊ¼»¯¿ØÖÆ¿¨08¡¢12Êä³ö½Ó¿ÚÓÃµ½µÄGPIOÒı½Å
	
	//ÏÔÊ¾ÆÁ²ÎÊı³õÊ¼»¯
	screen.area_number =1;    //·ÖÇøÊıÎª1
	screen.width =64;
	screen.height=32;
	screen.color=SINGLE;
	screen.light=100;
	screen.scan_type=SCAN_4_UP_TO_DOWN_1FOR16ROW;
	
	//ÉèÖÃ¸÷ÏÔÊ¾Çø²ÎÊı
	for(int i=0;i<screen.area_number;i++){
		area[i].id=i;
		area[i].width=64;
		area[i].height=32;
		area[i].content_type=TEXT;
		sprintf(area[i].display_data,"area%d",i);
	}
	
}
