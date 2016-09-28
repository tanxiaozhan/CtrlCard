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
uint8_t screen_dot[32][8];    //±£´æµ±Ç°ÏÔÊ¾µÄÒ»ÆÁµãÕóÊı¾İ

uint8_t current_row_dot[MAX_STRING_LENGTH];   //¸ÃÊı×é±£´æÁËµ±Ç°ÏÔÊ¾ĞĞµÄµãÕóÊı¾İ

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityAboveNormal, 1, 0);

/*----------------------------------------------------------------------------
 * Ïß³Ì'LED_Dispaly'£ºLEDÏÔÊ¾ÆÁÉ¨ÃèÏÔÊ¾
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {

	uint8_t * pStr;
	unsigned int col;
	uint8_t area_no,scan,row,dot;
	unsigned int startX[MAX_AREA_NUMBER],i,v_col;
	uint8_t speed[MAX_AREA_NUMBER];                //ÏÔÊ¾ÌØĞ§ÖĞµãÕóµÄÒÆ¶¯ËÙ¶È

	for(i=0;i<screen.area_number;i++){
		speed[i]=area[i].speed ;
		startX[i] = 0;
	}
	
	while(1){

		//Çå¿ÕÏÔ´æ£¬cve¼´ÓÃ0xFF¸øÏÔ´æÊı×é¸³Öµ
		for(row=0;row<32;row++){      
			for(scan=0;scan<8;scan++){
				screen_dot[row][scan]=0xFF;
			}
		}
	
	
	//°´ÏÔÊ¾Çø»ñÈ¡Ò»ĞĞµãÕóÊı¾İ£¬±£´æµ½Êı×écurrent_row_dot
	for( area_no=0;area_no<screen.area_number;area_no++ ){
		for( row=area[area_no].y; row<(area[area_no].y+area[area_no].height);row++){
			pStr = area[area_no].display_data;
			col=0;
			//»ñÈ¡µ±Ç°ÏÔÊ¾ĞĞµÄµãÕóÊı¾İ£¬½«µãÕóÊı¾İ±£´æµ½current_row_dotÊı×é
			while( col<area[area_no].length ){
				if( *pStr <= 126 ){      //Ó¢ÎÄ×Ö·û
					current_row_dot[col++] = ascii_Dot[ *pStr - ' ' ][row - area[area_no].y % 16] ;
					pStr++ ;
				}
				else{    //ºº×Ö×Ö·û
					uint16_t char_GBK_Code;                            //ºº×ÖµÄ¹ú±êÂë
					uint8_t GBK_dot[2];                                  //ºº×ÖµÄµãÕóÊı¾İ
					char_GBK_Code = * (uint16_t *)pStr;             //È¡Ò»¸öºº×ÖµÄ¹ú±êÂë£¬¶ş×Ö½Ú
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //¹ú±êÂëÇ°ºó×Ö½Ú¶Ôµ÷
					get_GBK_Code( GBK_dot, char_GBK_Code,(row-area[area_no].y)%16 );   //¶ÁÈ¡ºº×Ö×ÖÄ£Ò»ĞĞµãÕóÊı¾İ2×Ö½Ú
					current_row_dot[col++] = GBK_dot[0];
					current_row_dot[col++] = GBK_dot[1];
					pStr += 2 ;
				}
			}
			
			
			//Ë¢ĞÂÏÔ´æ£¬¼´ÓÃµãÕóÊı¾İ¸øÏÔ´æÊı×é¸³Öµ
			col = area[area_no].x;
			while( col < (area[area_no].x + area[area_no].width) ){
				v_col = col - area[area_no].x;    //ÔÚĞéÄâÏÔ´æÖĞµÄX×ø±ê
				if( startX[area_no]+v_col >= area[area_no].width && startX[area_no]+v_col < area[area_no].width+area[area_no].length*8 ){
					dot = startX[area_no]+v_col-area[area_no].width ;
					dot = current_row_dot[dot/8] & ( 0x80 >> (dot%8)) ;
					fill_point( (uint8_t *)screen_dot, screen.width/8, col, row ,dot ) ;
				}
				col++;
			}
		}
		speed[area_no]--;
		if( speed[area_no] < 1 ){
			speed[area_no] = area[area_no].speed;
			startX[area_no]++;
			if( startX[area_no] >= area[area_no].width+area[area_no].length*8 )
				startX[area_no] = 0;
		}
		
	}
//µ½´Ë£¬ÏÔ´æµãÕóÊı¾İĞ´ÈëÍê³É

	
	//È·¶¨É¨ÃèÀàĞÍ	
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
			dispay_scan_4_up_to_down_1for16row((uint8_t *)screen_dot, screen.width, screen.height );
			break;
		
		case SCAN_4_DOWN_TO_UP_1FOR16ROW:   //1/4ÏÂÉßĞĞ£¬Ò»Â·Êı¾İ´ø16ĞĞ
			break;
		
		case SCAN_4_UP_TO_DOWN_1FOR8ROW:    //1/4ÉÏÉßĞĞ£¬Ò»Â·Êı¾İ´ø8ĞĞ
			break;
		
		case SCAN_4_DOWN_TO_UP_1FOR8ROW:   //1/4ÏÂÉßĞĞ£¬Ò»Â·Êı¾İ´ø8ĞĞ
			break;
	}
//osDelay(1);
}

}



/*
 *------------------------------------------------------------------------------------
 *
 *	 ÔÚ(x,y)´¦ÏÔÊ¾Ò»¸öµã£¬Êµ¼ÊÉÏÔÚÏÔ´æÖĞ(x,y)´¦ÉèÖÃÒ»¸ö±ÈÌØÎ»µÄÖµ£¬0-ÏÔÊ¾£¬1-²»ÏÔÊ¾
 * ²ÎÊı£ºpbuff
 *       screen_width   ÆÁ¿í£¬×Ö½ÚÎªµ¥Î»£¬¼´ÆÁ¿íÏñËØÕ¼¶àÉÙ×Ö½Ú
 *       screen_height  ÆÁ¸ß
 *       x,y            ÏÔÊ¾ÆÁÉÏµÄ×ø±ê£¬×óÉÏ½Ç×ø±ê(0,0)
 *       point          ÏñËØµãµÄÖµ£¬0--ÏÔÊ¾£¬1--²»ÏÔÊ¾
 *
 *------------------------------------------------------------------------------------
*/
void fill_point(uint8_t * pbuff, uint8_t screen_width, uint8_t x, uint8_t y, bool point){
	pbuff += screen_width*y + x/8;
	point = !point;
	if( point ){
		*pbuff |= 0x80 >> x%8 ;
	}
	else{
		*pbuff &= ~( 0x80 >> x%8);
	}
}


/*
 *------------------------------------------------------------------------------------
 *
 * ½«ÏÔ´æ£¨¼´screen_dotÊı×é£© µÄÊı¾İÏÔÊ¾µ½ÏÔÊ¾ÆÁ
 * ²ÎÊı£ºpbuff          ÏÔ´æÖ¸Õë£¬Ö¸Ïò±£´æÁËÒ»ÆÁµãÕóÊı¾İµÄÊı×é
 *       screen_width   ÆÁ¿í¶àÉÙÏñËØ
 *       screen_height  ÆÁ¸ß¶àÉÙÏñËØ
 *
 *------------------------------------------------------------------------------------
*/
void dispay_scan_4_up_to_down_1for16row(uint8_t * pdot_buff, uint8_t screen_width, uint8_t screen_height){	
	uint8_t scan_rows=4;   // 1/4É¨Ãè  
	uint8_t scan, i, j;
	uint8_t row, col;   //µ±Ç°ĞĞ¡¢ÁĞ
	unsigned int screen_width_bytes;
	
	screen_width_bytes = screen_width /8;
	//EN(ON);
	for(row=0;row<scan_rows;row++){
		for(col=0;col<screen_width_bytes;col++){
			for(i=0;i<4;i++){
				scan=0x80;
				for(j=0;j<8;j++){
					CLK(OFF);     
					PORT_12_1_R( *(pdot_buff + (12-4*i+row)*screen_width_bytes + col) & scan );
					PORT_12_2_R( *(pdot_buff + (12-4*i+row+16)*screen_width_bytes + col) & scan );
					PORT_12_3_R( *(pdot_buff + (12-4*i+row+32)*screen_width_bytes + col) & scan );
					PORT_12_4_R( *(pdot_buff + (12-4*i+row+48)*screen_width_bytes + col) & scan );
					CLK(ON);       //594ÒÆÎ»ĞÅºÅ
					scan >>= 1;
				}
			}
		}
		A( row & 0x01 );B( row & 0x02 );   //ĞĞÉ¨Ãè
		STB(0);
		STB(1);  //Ëø´æ
		EN(ON);
		osDelay(2);
		EN(OFF);
		osDelay(3);

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
	
	uint8_t i;
	Port_08_12_GPIO_Config();    //³õÊ¼»¯¿ØÖÆ¿¨08¡¢12Êä³ö½Ó¿ÚÓÃµ½µÄGPIOÒı½Å
	
	//ÏÔÊ¾ÆÁ²ÎÊı³õÊ¼»¯
	screen.area_number =3;    //ÏÔÊ¾·ÖÇøÊı
	screen.width =64;
	screen.height=32;
	screen.color=SINGLE;
	screen.light=100;
	screen.scan_type=SCAN_4_UP_TO_DOWN_1FOR16ROW;
	


	sprintf((char *)area[0].display_data,"Ì·Ç¾Óê");
	sprintf((char *)area[1].display_data,"ËØ»ª");
	sprintf((char *)area[2].display_data,"LED");
	//ÉèÖÃ¸÷ÏÔÊ¾Çø²ÎÊı
	for(i=0;i<screen.area_number;i++){
		area[i].id=i;
		area[i].width=64;
		area[i].height=16;
		area[i].speed=i*5+1;
		area[i].content_type=TEXT;
		area[i].length=strlen((char *)area[0].display_data);
	}
	area[0].x=0;
	area[0].y=0;
	area[1].x=0;
	area[1].width=32;
	area[1].y=16;	
	area[2].x=32;
	area[2].y=16;
	area[2].width=30;
	
}
