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
#include <string.h>

screen_para screen;        //��ʾ������
area_para area[MAX_AREA_NUMBER];           //��ʾ������

uint8_t max_len=0;    // ����ʾ����ʾ�ַ���ĳ���

//uint8_t * screen_dot;   //ָ��һ����ά���飬�����鱣���˵�ǰ��ʾ��һ����������

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityAboveNormal, 1, 0);

/*----------------------------------------------------------------------------
 * �߳�'LED_Dispaly'��LED��ʾ��ɨ����ʾ
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {

	uint8_t screen_width_bytes;     //�������ߣ����ֽ�Ϊ��λ��
	uint8_t screen_dot[screen.height][screen_width_bytes];    //���浱ǰ��ʾ��һ����������
	uint8_t * pStr[screen.area_number];
	unsigned int col;
	uint8_t area_no,scan,row;
	
while(1){
	
	//����ʾ����ȡ�������ݣ����Դ�д���������
	for( area_no=0;area_no<screen.area_number;area_no++ ){
		for( row=area[area_no].y;row<area[area_no].height;row++){
			pStr[area_no] = area[area_no].display_data;
			col = area[area_no].x;
			while( (col<area[area_no].width) && ( *pStr[area_no] != 0 ) ){
				if( *pStr[area_no]<= 126){       //Ӣ���ַ�
					uint8_t char_dot;
					char_dot = ascii_Dot[ *pStr[area_no] - ' ' ][ (row-area[area_no].y)%16 ] ;
					scan = 0x80;
					while( col < area[area_no].width && scan > 0x00 ){
						fill_point((uint8_t *)screen_dot, screen.width/8, col, row, char_dot & scan );
						scan >>= 1;
						col ++;
					}
					pStr[area_no] ++ ;      //��һ���ַ�

				}   //Ӣ�Ĵ������
				
				else{         //�����ַ�
					uint16_t char_GBK_Code;                            //���ֵĹ�����
					uint8_t GBK_dot[2];                                  //���ֵĵ�������
					char_GBK_Code = * (uint16_t *)pStr[area_no];             //ȡһ�����ֵĹ����룬���ֽ�
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //������ǰ���ֽڶԵ�
					get_GBK_Code( GBK_dot, char_GBK_Code,(row-area[area_no].y)%16 );   //��ȡ������ģһ�е�������2�ֽ�
					
					//������ǰ8λ����
					scan = 0x80;
					while( col < area[area_no].width && scan > 0x00 ){
						fill_point((uint8_t *)screen_dot, screen.width/8, col, row, GBK_dot[0]&scan );
						scan >>= 1;
						col ++ ;
					}
					//�����ֺ�8λ����
					scan = 0x80;
					while( col < area[area_no].width && scan >0x00 ){
						fill_point((uint8_t *)screen_dot, screen.width/8, col, row, GBK_dot[1]&scan );
						scan >>= 1;
						col ++;
					}
						pStr[area_no] += 2;                                      //һ������ռ�����ֽ�
					
				} //���ִ������
			}
		}
	}
//���ˣ��Դ��������д�����
	
	
	//ȷ��ɨ������	
	switch(screen.scan_type){
		case SCAN_16:                   //1/16ɨ��
			break;
		
		case SCAN_8_LINE_1FOR8ROW:          //1/8ֱ�����ߣ�һ·���ݴ�8��
			break;
		
		case SCAN_8_UP_TO_DOWN_1FOR16ROW:   //1/8�����У�һ·���ݴ�16�У�8������
			break;
		
		case SCAN_8_DOWN_TO_UP_1FOR16ROW:   //1/8�����У�һ·���ݴ�16�У�8������
			break;
		
		case SCAN_4_LINE_1FOR4_ROW:         //1/4ֱ�����ߣ�һ·���ݴ�4��
			break;

		case 	SCAN_4_UP_TO_DOWN_1FOR16ROW:   //1/4�����У�һ·���ݴ�16�У�8������
			dispay_scan_4_up_to_down_1for16row((uint8_t *)screen_dot, screen.width, screen.height );
			break;
		
		case SCAN_4_DOWN_TO_UP_1FOR16ROW:   //1/4�����У�һ·���ݴ�16��
			break;
		
		case SCAN_4_UP_TO_DOWN_1FOR8ROW:    //1/4�����У�һ·���ݴ�8��
			break;
		
		case SCAN_4_DOWN_TO_UP_1FOR8ROW:   //1/4�����У�һ·���ݴ�8��
			break;
	}
osDelay(1);
}
		
/*	
	//����ʾ����ȡ��������
	for( area_no=0;area_no<screen.area_number;area_no++ ){
		if( (area[area_no].x % 8) == 0 ){                 //��ʾ�����x������8�ı���
			for( row=area[area_no].y;row<area[area_no].height;row++){
				pStr[area_no] = area[area_no].display_data;
				col = area[area_no].x;
				while( (col<area[area_no].width) && ( *pStr[area_no] != 0 ) ){
					if( *pStr[area_no]<= 126){       //Ӣ���ַ�
						if( area[area_no].width-col < 8 ){     //��ʾ�����һ�β���һ���ֽ�
							screen_dot[row][col/8 + 1] &= 0xFF >> (area[area_no].width-col);
							screen_dot[row][col/8 + 1] |= (0xFF << (8-(area[area_no].width-col))) & ascii_Dot[*pStr[area_no]-' '][(row-area[area_no].y)%16] ;
						}
						else{
							screen_dot[row][col/8] = ascii_Dot[ *pStr[area_no] - ' ' ][ (row-area[area_no].y)%16 ] ;
						}
						pStr[area_no] ++ ;      //��һ���ַ�
						col += 8 ;               //��һ���Դ��ֽ�
					}   //Ӣ�Ĵ������
					else{         //�����ַ�
						uint16_t char_GBK_Code;                            //���ֵĹ�����
						uint8_t GBK_dot[2];                                  //���ֵĵ�������
						char_GBK_Code = * (uint16_t *)pStr[area_no];             //ȡһ�����ֵĹ����룬���ֽ�
						char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //������ǰ���ֽڶԵ�
						get_GBK_Code( GBK_dot, char_GBK_Code,(row-area[area_no].y)%16 );   //��ȡ������ģһ�е�������2�ֽ�
						if(area[area_no].width-col<8){
							screen_dot[row][col/8+1] &= 0xFF >> (area[area_no].width - col );
							screen_dot[row][col/8+1] |= (0xFF << (8-(area[area_no].width-col))) & GBK_dot[0];
						}
						else{
							screen_dot[row][col/8]=GBK_dot[0];    
						}
						col += 8 ;
						if( col > area[area_no].width ){ break;  }    //������ʾ�����
						if(area[area_no].width-col<8){
							screen_dot[row][col/8+1] &= 0xFF >> (area[area_no].width - col );
							screen_dot[row][col/8+1] |= (0xFF << (8-(area[area_no].width-col))) & GBK_dot[1];
						}
						else{
							screen_dot[row][col/8]=GBK_dot[1];    
						}
						col += 8 ;
						pStr[area_no] += 2;                                      //һ������ռ�����ֽ�
					} 	   //���ִ������
				}
			}
		}
		else{      //��ʾ�����x���겻��8�ı���
			for( row=area[area_no].y;row<area[area_no].height;row++){
				pStr[area_no] = area[area_no].display_data;
				col = area[area_no].x;
				while( (col<area[area_no].width) && ( *pStr[area_no] != 0 ) ){
					if( *pStr[area_no]<= 126){       //Ӣ���ַ�
						int char_dot;
						char_dot = ascii_Dot[*pStr[area_no]-' '][(row-area[area_no].y)%16];
						scan = 0x80;
						while(col<area[area_no].width && scan>0x00 ){
							fill_point((uint8_t *)screen_dot, screen.width/8, col, row, char_dot & scan );
							col ++;
							scan >>= 1;
						}
						pStr[area_no] ++ ;      //��һ���ַ�
					}   //Ӣ�Ĵ������
					else{         //�����ַ�
						uint16_t char_GBK_Code;                            //���ֵĹ�����
						uint8_t GBK_dot[2];                                  //���ֵĵ�������
						char_GBK_Code = * (uint16_t *)pStr[area_no];             //ȡһ�����ֵĹ����룬���ֽ�
						char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //������ǰ���ֽڶԵ�
						get_GBK_Code( GBK_dot, char_GBK_Code,(row-area[area_no].y)%16 );   //��ȡ������ģһ�е�������2�ֽ�
						
						//������ǰ8������
						scan = 0x80;
						while(col<area[area_no].width && scan>0x00){
							fill_point((uint8_t *)screen_dot, screen.width/8, col, row, GBK_dot[0] & scan );
							col ++ ;
							scan >>= 1;
						}
						//�����ֺ�8������
						scan = 0x80;
						while(col<area[area_no].width && scan>0x00){
							fill_point((uint8_t *)screen_dot, screen.width/8, col, row, GBK_dot[1] & scan );
							col ++ ;
							scan >>= 1;
						}
						pStr[area_no] += 2;                                      //һ������ռ�����ֽ�
					} 	   //���ִ������
				}
			}
		}
	}
*/
}



/*
 *------------------------------------------------------------------------------------
 *
 *	 ��(x,y)����ʾһ���㣬ʵ�������Դ���(x,y)������һ������λ��ֵ��0-��ʾ��1-����ʾ
 * ������pbuff
 *       screen_width   �����ֽ�Ϊ��λ������������ռ�����ֽ�
 *       screen_height  ����
 *       x,y            ��ʾ���ϵ����꣬���Ͻ�����(0,0)
 *       point          ���ص��ֵ��0--��ʾ��1--����ʾ
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
 * ���Դ棨��screen_dot���飩 ��������ʾ����ʾ��
 * ������pbuff          �Դ�ָ�룬ָ�򱣴���һ���������ݵ�����
 *       screen_width   �����������
 *       screen_height  ���߶�������
 *
 *------------------------------------------------------------------------------------
*/
void dispay_scan_4_up_to_down_1for16row(uint8_t * pdot_buff, uint8_t screen_width, uint8_t screen_height){	
	uint8_t scan_rows=4;   // 1/4ɨ��  
	uint8_t scan, i, j;
	uint8_t row, col;   //��ǰ�С���
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
					CLK(ON);       //594��λ�ź�
				}
			}
			A( i & 0x01 );B( i & 0x02 );C( i & 0x04 );D( i & 0x08 );   //��ɨ��
			osDelay(3);
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

/*
	********************************************************
	*  LED��ʾ����ʼ��
	********************************************************
*/
void LED_Display_Init(void){
	
	uint8_t i;
	Port_08_12_GPIO_Config();    //��ʼ�����ƿ�08��12����ӿ��õ���GPIO����
	
	//��ʾ��������ʼ��
	screen.area_number =1;    //������Ϊ1
	screen.width =64;
	screen.height=32;
	screen.color=SINGLE;
	screen.light=100;
	screen.scan_type=SCAN_4_UP_TO_DOWN_1FOR16ROW;
	
	//���ø���ʾ������
	for(i=0;i<screen.area_number;i++){
		area[i].id=i;
		area[i].width=64;
		area[i].height=32;
		area[i].content_type=TEXT;
		sprintf((char *)area[i].display_data,"area%d",i);
	}
	
}
