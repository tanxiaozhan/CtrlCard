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

osThreadId LED_display_thread_id;   //��ʾ�߳�id������/��ֹ���߳̿�����ʾ�Ŀ���

bool power_on=false;  //��ʾ������/�ػ�
bool check_on=false;  //��Ļ�����

screen_para screen;        //��ʾ������
area_para area[MAX_AREA_NUMBER];           //��ʾ������

uint8_t max_len=0;    // ����ʾ����ʾ�ַ���ĳ���
uint8_t screen_dot_red[32][8];    //���浱ǰ��ʾ��һ����������
uint8_t screen_dot_green[32][8];   //���浱ǰ��ʾ��һ����ɫ��������

uint8_t current_row_dot[MAX_STRING_LENGTH];   //�����鱣���˵�ǰ��ʾ�еĵ�������

//��ʾ���߳�
static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityAboveNormal, 1, 0);

//��Ļ���
static void check_Display(void const *arg);
osThreadDef(check_Display, osPriorityAboveNormal, 1, 0);


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
				screen_dot_red[row][scan]=0xFF;
				screen_dot_green[row][scan]=0xFF;
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
					current_row_dot[col++] = ascii_Dot[ *pStr - ' ' ][row - area[area_no].y % 16] ;    //��ȡ����
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
					if(area[area_no].red) fill_point( (uint8_t *)screen_dot_red, screen.width/8, col, row ,dot ) ;
					if(area[area_no].green) fill_point( (uint8_t *)screen_dot_green, screen.width/8, col, row ,dot ) ;
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

	//��ʾһ�������Դ��е�������ʾ����Ļ��
	buffer_to_display();

	}

}

/*----------------------------------------------------------------------------
 * �߳�'check_Dispaly'����Ļ���
 *---------------------------------------------------------------------------*/
static void check_Display (void const *arg) {
	while(1){	
		buffer_to_display();
	}
}

/*
 *------------------------------------------------------------------------------------
 *
 * ��(x,y)����ʾһ���㣬ʵ�������Դ���(x,y)������һ������λ��ֵ��0-��ʾ��1-����ʾ
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
 * ��ʾһ���������Դ����Ϣ��ʾ����Ļ�� 
 * ������pbuff
 *       screen_width   �����ֽ�Ϊ��λ������������ռ�����ֽ�
 *       screen_height  ����
 *
 *------------------------------------------------------------------------------------
*/
void buffer_to_display(void){
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
			dispay_scan_4_up_to_down_1for16row((uint8_t *)screen_dot_red, (uint8_t *)screen_dot_green,screen.width, screen.height);
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
void dispay_scan_4_up_to_down_1for16row(uint8_t * pdot_buff_red, uint8_t * pdot_buff_green, uint8_t screen_width, uint8_t screen_height){	
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
					PORT_12_1_R( *(pdot_buff_red + (12-4*i+row)*screen_width_bytes + col) & scan );
					PORT_08_1_R1( *(pdot_buff_red + (12-4*i+row)*screen_width_bytes + col) & scan );
					PORT_12_2_R( *(pdot_buff_red + (12-4*i+row+16)*screen_width_bytes + col) & scan );
					PORT_08_1_R2( *(pdot_buff_red + (12-4*i+row+16)*screen_width_bytes + col) & scan );
					PORT_12_3_R( *(pdot_buff_red + (12-4*i+row+32)*screen_width_bytes + col) & scan );
					PORT_08_2_R1( *(pdot_buff_red + (12-4*i+row+32)*screen_width_bytes + col) & scan );
					PORT_12_4_R( *(pdot_buff_red + (12-4*i+row+48)*screen_width_bytes + col) & scan );
					PORT_08_2_R2( *(pdot_buff_red + (12-4*i+row+48)*screen_width_bytes + col) & scan );
					
					PORT_12_1_G( *(pdot_buff_green + (12-4*i+row)*screen_width_bytes + col) & scan );
					PORT_08_1_G1( *(pdot_buff_green + (12-4*i+row)*screen_width_bytes + col) & scan );
					PORT_12_2_G( *(pdot_buff_green + (12-4*i+row+16)*screen_width_bytes + col) & scan );
					PORT_08_1_G2( *(pdot_buff_green + (12-4*i+row+16)*screen_width_bytes + col) & scan );
					PORT_12_3_G( *(pdot_buff_green + (12-4*i+row+32)*screen_width_bytes + col) & scan );
					PORT_08_2_G1( *(pdot_buff_green + (12-4*i+row+32)*screen_width_bytes + col) & scan );
					PORT_12_4_G( *(pdot_buff_green + (12-4*i+row+48)*screen_width_bytes + col) & scan );
					PORT_08_2_G2( *(pdot_buff_green + (12-4*i+row+48)*screen_width_bytes + col) & scan );

					CLK(ON);       //594��λ�ź�
					scan >>= 1;
				}
			}
		}
		A( row & 0x01 );B( row & 0x02 );C( row & 0x04 );D( row & 0x08 );   //��ɨ��
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
	********************************************************
*/

void LED_display_start(void){
	if( !power_on ){
		LED_display_thread_id = osThreadCreate (osThread(LED_Display), NULL);
		if( LED_display_thread_id !=NULL ){
			power_on=true;
		}
	}
}

/*
	********************************************************
	*
	*  �ر���ʾ
	*
	********************************************************
*/
void LED_display_power_off(void){
	if(osThreadTerminate(LED_display_thread_id) == osOK ){
		EN(OFF);
		power_on=false;
	}
}

/*
	********************************************************
	*
	*  ����/�˳���Ļ���
	*
	********************************************************
*/
void check_screen(bool check, bool check_red, bool check_green){

	uint8_t row, col;
	uint8_t red, green;
	if(check_red)
		red=0x0;
	else
		red=0xFF;
	
	if(check_green)
		green=0x0;
	else
		green=0xFF;
	for(row=0;row<screen.height;row++){      
		for(col=0;col<screen.width/8 ;col++){
			screen_dot_red[row][col]=red;
			screen_dot_green[row][col]=green;
		}
	}
	
	//������Ļ���
	if(check && !check_on){      
		if(osThreadTerminate(LED_display_thread_id) == osOK ){
			LED_display_thread_id=osThreadCreate(osThread(check_Display),NULL);
			check_on=true;
		}
	}
	
	//�˳���Ļ���
	if(!check && check_on){     
		if(osThreadTerminate(LED_display_thread_id) == osOK){
			LED_display_thread_id = osThreadCreate(osThread(LED_Display), NULL);
			check_on=false;
		}
	}
}



/*
	********************************************************
	*  LED��ʾ����ʼ��
	********************************************************
*/
void LED_display_init(void){
	
	uint8_t i,j;
	uint8_t pBuff[100];
	Port_08_12_GPIO_Config();    //��ʼ�����ƿ�08��12����ӿ��õ���GPIO����

	//���ⲿ�����ȡ��ʾ������
	SPI_FLASH_BufferRead(pBuff,0,9);
	if(pBuff[0]==0xFF){
		screen.width=64;
		screen.height=32;
		screen.light=100;
		screen.color=SINGLE;
		screen.area_number=MAX_AREA_NUMBER;
		screen.scan_type=SCAN_4_UP_TO_DOWN_1FOR16ROW;
	}
	else{
		screen.width =(((uint16_t)pBuff[0]) <<16) + (uint16_t)pBuff[1] ;
		screen.height=(((uint16_t)pBuff[2]) <<16) + (uint16_t)pBuff[3] ;
		screen.light= (((uint16_t)pBuff[4]) <<16) + (uint16_t)pBuff[5] ;
		if(pBuff[6]==0)
			screen.color=SINGLE;
			else if(pBuff[6]==1)
				screen.color=DOUBLE;
			else
				screen.color=COLOR;
	
		screen.scan_type=SCAN_4_UP_TO_DOWN_1FOR16ROW;
		screen.area_number =pBuff[8];    //��ʾ������
		if(screen.area_number>MAX_AREA_NUMBER)
			screen.area_number=MAX_AREA_NUMBER;
	}
	
	
	//���ⲿ�����ȡ����ʾ������
	for(i=0;i<MAX_AREA_NUMBER;i++){
		//��ʾ�������������ⲿ���棬��ַ��0100��ʼ��ÿ����������ռ��100�ֽ�
		SPI_FLASH_BufferRead(pBuff, (i+1)*100, 100);
		
		if(pBuff[0]<MAX_AREA_NUMBER){  //��ʾ��������ȷ����ʼ��������������֮�������������
			area[i].id=pBuff[0];
			area[i].x=(((uint16_t)pBuff[1]) <<16) + (uint16_t)pBuff[2];
			area[i].y=(((uint16_t)pBuff[3]) <<16) + (uint16_t)pBuff[4];
			area[i].width=(((uint16_t)pBuff[5]) <<16) + (uint16_t)pBuff[6];
			area[i].height=(((uint16_t)pBuff[7]) <<16) + (uint16_t)pBuff[8];
			area[i].red=pBuff[9];
			area[i].red=1;
			area[i].green =pBuff[10];
			area[i].green=1;
			area[i].ani_in=pBuff[11];
			area[i].ani_out=pBuff[12];
			area[i].speed=pBuff[13];
			area[i].speed=i*2+4;
			area[i].content_type=TEXT;
			area[i].length=pBuff[15];
			//area[i].length=strlen((char *)area[i].display_data);
			if(area[i].length>MAX_STRING_LENGTH)
				area[i].length=MAX_STRING_LENGTH;
			
			for(j=0;j<area[i].length;j++)
					area[i].display_data[j]=pBuff[j+16];
		}
		else
			del_area(i);
	}
	
}

//ɾ����ʾ����
//������area_no ��ʾ�����ţ�0-2
void del_area(uint8_t area_no){
	area[area_no].width=0;
	area[area_no].height=0;
	area[area_no].x=0;
	area[area_no].y=0;
	area[area_no].content_type=TEXT;
	area[area_no].ani_in=0;
	area[area_no].ani_out=0;
	area[area_no].length=0;
	area[area_no].display_data[0]='\0';
}
