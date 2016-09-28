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
uint8_t screen_dot[32][8];    //���浱ǰ��ʾ��һ����������

uint8_t current_row_dot[MAX_STRING_LENGTH];   //�����鱣���˵�ǰ��ʾ�еĵ�������

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityAboveNormal, 1, 0);

/*----------------------------------------------------------------------------
 * �߳�'LED_Dispaly'��LED��ʾ��ɨ����ʾ
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {

	uint8_t * pStr;
	unsigned int col;
	uint8_t area_no,scan,row,dot;
	unsigned int startX[MAX_AREA_NUMBER],i,v_col;
	uint8_t speed[MAX_AREA_NUMBER];                //��ʾ��Ч�е�����ƶ��ٶ�

	for(i=0;i<screen.area_number;i++){
		speed[i]=area[i].speed ;
		startX[i] = 0;
	}
	
	while(1){

		//����Դ棬cve����0xFF���Դ����鸳ֵ
		for(row=0;row<32;row++){      
			for(scan=0;scan<8;scan++){
				screen_dot[row][scan]=0xFF;
			}
		}
	
	
	//����ʾ����ȡһ�е������ݣ����浽����current_row_dot
	for( area_no=0;area_no<screen.area_number;area_no++ ){
		for( row=area[area_no].y; row<(area[area_no].y+area[area_no].height);row++){
			pStr = area[area_no].display_data;
			col=0;
			//��ȡ��ǰ��ʾ�еĵ������ݣ����������ݱ��浽current_row_dot����
			while( col<area[area_no].length ){
				if( *pStr <= 126 ){      //Ӣ���ַ�
					current_row_dot[col++] = ascii_Dot[ *pStr - ' ' ][row - area[area_no].y % 16] ;
					pStr++ ;
				}
				else{    //�����ַ�
					uint16_t char_GBK_Code;                            //���ֵĹ�����
					uint8_t GBK_dot[2];                                  //���ֵĵ�������
					char_GBK_Code = * (uint16_t *)pStr;             //ȡһ�����ֵĹ����룬���ֽ�
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //������ǰ���ֽڶԵ�
					get_GBK_Code( GBK_dot, char_GBK_Code,(row-area[area_no].y)%16 );   //��ȡ������ģһ�е�������2�ֽ�
					current_row_dot[col++] = GBK_dot[0];
					current_row_dot[col++] = GBK_dot[1];
					pStr += 2 ;
				}
			}
			
			
			//ˢ���Դ棬���õ������ݸ��Դ����鸳ֵ
			col = area[area_no].x;
			while( col < (area[area_no].x + area[area_no].width) ){
				v_col = col - area[area_no].x;    //�������Դ��е�X����
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
//osDelay(1);
}

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
					CLK(ON);       //594��λ�ź�
					scan >>= 1;
				}
			}
		}
		A( row & 0x01 );B( row & 0x02 );   //��ɨ��
		STB(0);
		STB(1);  //����
		EN(ON);
		osDelay(2);
		EN(OFF);
		osDelay(3);

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
	screen.area_number =3;    //��ʾ������
	screen.width =64;
	screen.height=32;
	screen.color=SINGLE;
	screen.light=100;
	screen.scan_type=SCAN_4_UP_TO_DOWN_1FOR16ROW;
	


	sprintf((char *)area[0].display_data,"̷Ǿ��");
	sprintf((char *)area[1].display_data,"�ػ�");
	sprintf((char *)area[2].display_data,"LED");
	//���ø���ʾ������
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
