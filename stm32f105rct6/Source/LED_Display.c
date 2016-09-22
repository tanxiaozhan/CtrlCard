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

//uint8_t * screen_dot;   //Ö¸ÏòÒ»¸ö¶şÎ¬Êı×é£¬¸ÃÊı×é±£´æÁËµ±Ç°ÏÔÊ¾µÄÒ»ÆÁµãÕóÊı¾İ

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityAboveNormal, 1, 0);

/*----------------------------------------------------------------------------
 * Ïß³Ì'LED_Dispaly'£ºLEDÏÔÊ¾ÆÁÉ¨ÃèÏÔÊ¾
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {

	uint8_t screen_width_bytes;     //ÆÁ¿í¡¢ÆÁ¸ß£¨°´×Ö½ÚÎªµ¥Î»£©
	uint8_t screen_dot[screen.height][screen_width_bytes];    //±£´æµ±Ç°ÏÔÊ¾µÄÒ»ÆÁµãÕóÊı¾İ
	uint8_t * pStr[screen.area_number];
	unsigned int col;
	uint8_t area_no,scan,row;
	
while(1){
	
	//°´ÏÔÊ¾Çø»ñÈ¡µãÕóÊı¾İ£¬ÍùÏÔ´æĞ´ÈëµãÕóÊı¾İ
	for( area_no=0;area_no<screen.area_number;area_no++ ){
		for( row=area[area_no].y;row<area[area_no].height;row++){
			pStr[area_no] = area[area_no].display_data;
			col = area[area_no].x;
			while( (col<area[area_no].width) && ( *pStr[area_no] != 0 ) ){
				if( *pStr[area_no]<= 126){       //Ó¢ÎÄ×Ö·û
					uint8_t char_dot;
					char_dot = ascii_Dot[ *pStr[area_no] - ' ' ][ (row-area[area_no].y)%16 ] ;
					scan = 0x80;
					while( col < area[area_no].width && scan > 0x00 ){
						fill_point((uint8_t *)screen_dot, screen.width/8, col, row, char_dot & scan );
						scan >>= 1;
						col ++;
					}
					pStr[area_no] ++ ;      //ÏÂÒ»¸ö×Ö·û

				}   //Ó¢ÎÄ´¦ÀíÍê±Ï
				
				else{         //ºº×Ö×Ö·û
					uint16_t char_GBK_Code;                            //ºº×ÖµÄ¹ú±êÂë
					uint8_t GBK_dot[2];                                  //ºº×ÖµÄµãÕóÊı¾İ
					char_GBK_Code = * (uint16_t *)pStr[area_no];             //È¡Ò»¸öºº×ÖµÄ¹ú±êÂë£¬¶ş×Ö½Ú
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //¹ú±êÂëÇ°ºó×Ö½Ú¶Ôµ÷
					get_GBK_Code( GBK_dot, char_GBK_Code,(row-area[area_no].y)%16 );   //¶ÁÈ¡ºº×Ö×ÖÄ£Ò»ĞĞµãÕóÊı¾İ2×Ö½Ú
					
					//´¦Àíºº×ÖÇ°8Î»µãÕó
					scan = 0x80;
					while( col < area[area_no].width && scan > 0x00 ){
						fill_point((uint8_t *)screen_dot, screen.width/8, col, row, GBK_dot[0]&scan );
						scan >>= 1;
						col ++ ;
					}
					//´¦Àíºº×Öºó8Î»µãÕó
					scan = 0x80;
					while( col < area[area_no].width && scan >0x00 ){
						fill_point((uint8_t *)screen_dot, screen.width/8, col, row, GBK_dot[1]&scan );
						scan >>= 1;
						col ++;
					}
						pStr[area_no] += 2;                                      //Ò»¸öºº×ÖÕ¼¶ş¸ö×Ö½Ú
					
				} //ºº×Ö´¦ÀíÍê±Ï
			}
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
osDelay(1);
}
		
/*	
	//°´ÏÔÊ¾Çø»ñÈ¡µãÕóÊı¾İ
	for( area_no=0;area_no<screen.area_number;area_no++ ){
		if( (area[area_no].x % 8) == 0 ){                 //ÏÔÊ¾ÇøÆğµãx×ø±êÊÇ8µÄ±¶Êı
			for( row=area[area_no].y;row<area[area_no].height;row++){
				pStr[area_no] = area[area_no].display_data;
				col = area[area_no].x;
				while( (col<area[area_no].width) && ( *pStr[area_no] != 0 ) ){
					if( *pStr[area_no]<= 126){       //Ó¢ÎÄ×Ö·û
						if( area[area_no].width-col < 8 ){     //ÏÔÊ¾Çø×îºóÒ»¶Î²»¹»Ò»¸ö×Ö½Ú
							screen_dot[row][col/8 + 1] &= 0xFF >> (area[area_no].width-col);
							screen_dot[row][col/8 + 1] |= (0xFF << (8-(area[area_no].width-col))) & ascii_Dot[*pStr[area_no]-' '][(row-area[area_no].y)%16] ;
						}
						else{
							screen_dot[row][col/8] = ascii_Dot[ *pStr[area_no] - ' ' ][ (row-area[area_no].y)%16 ] ;
						}
						pStr[area_no] ++ ;      //ÏÂÒ»¸ö×Ö·û
						col += 8 ;               //ÏÂÒ»¸öÏÔ´æ×Ö½Ú
					}   //Ó¢ÎÄ´¦ÀíÍê±Ï
					else{         //ºº×Ö×Ö·û
						uint16_t char_GBK_Code;                            //ºº×ÖµÄ¹ú±êÂë
						uint8_t GBK_dot[2];                                  //ºº×ÖµÄµãÕóÊı¾İ
						char_GBK_Code = * (uint16_t *)pStr[area_no];             //È¡Ò»¸öºº×ÖµÄ¹ú±êÂë£¬¶ş×Ö½Ú
						char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //¹ú±êÂëÇ°ºó×Ö½Ú¶Ôµ÷
						get_GBK_Code( GBK_dot, char_GBK_Code,(row-area[area_no].y)%16 );   //¶ÁÈ¡ºº×Ö×ÖÄ£Ò»ĞĞµãÕóÊı¾İ2×Ö½Ú
						if(area[area_no].width-col<8){
							screen_dot[row][col/8+1] &= 0xFF >> (area[area_no].width - col );
							screen_dot[row][col/8+1] |= (0xFF << (8-(area[area_no].width-col))) & GBK_dot[0];
						}
						else{
							screen_dot[row][col/8]=GBK_dot[0];    
						}
						col += 8 ;
						if( col > area[area_no].width ){ break;  }    //³¬³öÏÔÊ¾Çø¿í¶È
						if(area[area_no].width-col<8){
							screen_dot[row][col/8+1] &= 0xFF >> (area[area_no].width - col );
							screen_dot[row][col/8+1] |= (0xFF << (8-(area[area_no].width-col))) & GBK_dot[1];
						}
						else{
							screen_dot[row][col/8]=GBK_dot[1];    
						}
						col += 8 ;
						pStr[area_no] += 2;                                      //Ò»¸öºº×ÖÕ¼¶ş¸ö×Ö½Ú
					} 	   //ºº×Ö´¦ÀíÍê±Ï
				}
			}
		}
		else{      //ÏÔÊ¾ÇøÆğµãx×ø±ê²»ÊÇ8µÄ±¶Êı
			for( row=area[area_no].y;row<area[area_no].height;row++){
				pStr[area_no] = area[area_no].display_data;
				col = area[area_no].x;
				while( (col<area[area_no].width) && ( *pStr[area_no] != 0 ) ){
					if( *pStr[area_no]<= 126){       //Ó¢ÎÄ×Ö·û
						int char_dot;
						char_dot = ascii_Dot[*pStr[area_no]-' '][(row-area[area_no].y)%16];
						scan = 0x80;
						while(col<area[area_no].width && scan>0x00 ){
							fill_point((uint8_t *)screen_dot, screen.width/8, col, row, char_dot & scan );
							col ++;
							scan >>= 1;
						}
						pStr[area_no] ++ ;      //ÏÂÒ»¸ö×Ö·û
					}   //Ó¢ÎÄ´¦ÀíÍê±Ï
					else{         //ºº×Ö×Ö·û
						uint16_t char_GBK_Code;                            //ºº×ÖµÄ¹ú±êÂë
						uint8_t GBK_dot[2];                                  //ºº×ÖµÄµãÕóÊı¾İ
						char_GBK_Code = * (uint16_t *)pStr[area_no];             //È¡Ò»¸öºº×ÖµÄ¹ú±êÂë£¬¶ş×Ö½Ú
						char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //¹ú±êÂëÇ°ºó×Ö½Ú¶Ôµ÷
						get_GBK_Code( GBK_dot, char_GBK_Code,(row-area[area_no].y)%16 );   //¶ÁÈ¡ºº×Ö×ÖÄ£Ò»ĞĞµãÕóÊı¾İ2×Ö½Ú
						
						//´¦Àíºº×ÖÇ°8¸öµãÕó
						scan = 0x80;
						while(col<area[area_no].width && scan>0x00){
							fill_point((uint8_t *)screen_dot, screen.width/8, col, row, GBK_dot[0] & scan );
							col ++ ;
							scan >>= 1;
						}
						//´¦Àíºº×Öºó8¸öµãÕó
						scan = 0x80;
						while(col<area[area_no].width && scan>0x00){
							fill_point((uint8_t *)screen_dot, screen.width/8, col, row, GBK_dot[1] & scan );
							col ++ ;
							scan >>= 1;
						}
						pStr[area_no] += 2;                                      //Ò»¸öºº×ÖÕ¼¶ş¸ö×Ö½Ú
					} 	   //ºº×Ö´¦ÀíÍê±Ï
				}
			}
		}
	}
*/
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
	EN(ON);
	for(row=0;row<scan_rows;row++){
		for(col=0;col<screen_width_bytes;col++){
			for(i=0;i<4;i++){
				scan=0x01;
				for(j=0;j<8;j++){
					CLK(OFF);     
					PORT_12_1_R( *(pdot_buff + (12-4*i+row)*screen_width_bytes + col) & scan );
					PORT_12_2_R( *(pdot_buff + (12-4*i+row+16)*screen_width_bytes + col) & scan );
					PORT_12_3_R( *(pdot_buff + (12-4*i+row+32)*screen_width_bytes + col) & scan );
					PORT_12_4_R( *(pdot_buff + (12-4*i+row+48)*screen_width_bytes + col) & scan );
					scan <<= 1;
					CLK(ON);       //594ÒÆÎ»ĞÅºÅ
				}
			}
			A( i & 0x01 );B( i & 0x02 );C( i & 0x04 );D( i & 0x08 );   //ĞĞÉ¨Ãè
			osDelay(3);
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
	
	uint8_t i;
	Port_08_12_GPIO_Config();    //³õÊ¼»¯¿ØÖÆ¿¨08¡¢12Êä³ö½Ó¿ÚÓÃµ½µÄGPIOÒı½Å
	
	//ÏÔÊ¾ÆÁ²ÎÊı³õÊ¼»¯
	screen.area_number =1;    //·ÖÇøÊıÎª1
	screen.width =64;
	screen.height=32;
	screen.color=SINGLE;
	screen.light=100;
	screen.scan_type=SCAN_4_UP_TO_DOWN_1FOR16ROW;
	
	//ÉèÖÃ¸÷ÏÔÊ¾Çø²ÎÊı
	for(i=0;i<screen.area_number;i++){
		area[i].id=i;
		area[i].width=64;
		area[i].height=32;
		area[i].content_type=TEXT;
		sprintf((char *)area[i].display_data,"area%d",i);
	}
	
}
