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
	screen_width_bytes=((screen.height % 8)==0)?(screen.height/8):(screen.height/8+1) ;
	uint8_t screen_dot[screen.height][screen_width_bytes];    //���浱ǰ��ʾ��һ����������
	uint8_t * pStr[screen.area_number];
	int i,j,current_area,area_no,row,col;
	for(i=0;i<screen.area_number;i++){
		pStr[i] = area[i].display_data;
		if(max_len < strlen((char *)area[i].display_data) )    //��ȡ����ʾ�����ַ�������
			max_len = strlen((char *)area[i].display_data);         
	}

	//����ʾ����ȡ��������
	for(area_no=0;area_no<screen.area_number;area_no++){
		for(row=0;row<area[area_no].width;row++){
			for(col=0;col<area[area_no].height;col++){
				if( *pStr[area_no]<= 126){       //Ӣ���ַ�
					screen_dot[ area[area_no].x+row ][ area[area_no].y+col ] = ascii_Dot[ *pStr[area_no] - ' ' ][ row%16 ] ;
				}   //Ӣ�Ĵ������
				else{         //�����ַ�
					
				}    //���ִ������
			}
		}
	}

	

	//���һ���������ݣ�����screen_dot���鸳ֵ
	i=0;
	for( i=0; i<screen.height; i++ ){              
		for( j=0; j<screen.width ; j+=8 ){
			for( current_area=0;current_area<screen.area_number; current_area++){
				if( i<area[current_area].height && j<area[current_area].width ){           //�߽��⣬��ǰλ���ĸ���ʾ��
					break;
				}
			}	
				if( ( *pStr[current_area] != '\0')){  
					if( *pStr[current_area] <= 126 ){                             //Ӣ����ĸ
						screen_dot[i][j/8] = ascii_Dot[ *pStr[current_area] - ' '][ i%16 ] ;
						if( (area[current_area].width-j)%8 != 0 ){                  //��ʾ����Ȳ���8������������������󲻹�һ�ֽڣ���������ʾ������һ�������ֽ�
							screen_dot[i][j/8] &= 0xFF << ((area[current_area].width-j) % 8);
						}
						else if((area[current_area].x % 8) != 0){               //��ʾ����ʼX���겻��8��������������ʼ�ĵ���������ʾ�����õ����ֽ�
							screen_dot[i][j/8] &= 0xFF >> (area[current_area].x %8);
						}
						
						pStr[current_area] ++;
						
					}  //Ӣ����ĸ�������
					else{                                              //�����ַ�
					uint16_t char_GBK_Code;                            //���ֵĹ�����
					uint8_t GBK_dot[2];                                  //���ֵĵ�������
					char_GBK_Code = * (uint16_t *)pStr[current_area];             //ȡһ�����ֵĹ����룬���ֽ�
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //������ǰ���ֽڶԵ�
					get_GBK_Code( GBK_dot, char_GBK_Code,12-current_area*4+i );   //��ȡ������ģһ�е�������2�ֽ�
					screen_dot[i][j/8]=GBK_dot[0];    
					j += 8  ;                                               
					screen_dot[i][j/8]=GBK_dot[1];
					pStr[current_area] += 2;                                      //һ������ռ�����ֽ�
					}  //���ִ������
				}
		}
	}
	//һ����������������
	
	
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
			scan_4_up_to_down_1for16row((char *)screen_dot);
			break;
		
		case SCAN_4_DOWN_TO_UP_1FOR16ROW:   //1/4�����У�һ·���ݴ�16��
		break;
		
		case SCAN_4_UP_TO_DOWN_1FOR8ROW:    //1/4�����У�һ·���ݴ�8��
			break;
		
		case SCAN_4_DOWN_TO_UP_1FOR8ROW:   //1/4�����У�һ·���ݴ�8��
			break;
	}
}
	
void scan_4_up_to_down_1for16row(char * string_dot){	
	uint8_t row=4;   // 1/4ɨ��  
	//char charDot[2];    //���浱ǰ��ʾ�ַ��ĵ�����Ϣ��Ӣ�Ŀ�8λ�����ֿ�16λ
	uint8_t i,j,k,len;
	uint16_t scan;    //��ɨ��
	uint8_t LED_width;   //����
	char dot;
	
	EN(ON);
	while(1){
				//EN(OFF);
	while(i<screen.width){
		for(j=0;j<screen.height;j++){
			for(k=0;k<screen.area_number;k++){
			if( area[k].display_data[len]<=126 ){  //Ӣ���ַ�
				string_dot[len]= ;
				}
			}
			}
		}
		for(i=0;i<row;i++){
			for(LED_width=0;LED_width<screen.width;LED_width++){
					pStr[0]=area[0].display_data;
					if( *pStr[0]<=126){	//Ӣ���ַ�
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
							CLK(ON);       //594��λ�ź�
						}
					}
					pStr[area] ++;
				}
				else{   //�����ַ�
					uint16_t char_GBK_Code;    //���ֵĹ�����
					uint16_t GBK_Dot;          //���ֵĵ�������
					char_GBK_Code = * (uint16_t *)pStr[area];       //ȡһ�����ֵĹ����룬���ֽ�
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //������ǰ���ֽڶԵ�
					for(k=0;k<4;k++){
						get_GBK_Code( (uint8_t *)charDot, char_GBK_Code,12-k*4+i );   //��ȡ������ģһ�е�������2�ֽ�
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
							CLK(ON);       //594��λ�ź�
					}
				}
					pStr[area] += 2;      //һ������ռ�����ֽ�
				}
			}
			STB(OFF);
			STB(ON);     //����

			A( i & 0x01 );B( i & 0x02 );C( i & 0x04 );D( i & 0x08 );   //��ɨ��
			osDelay(5);
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

	Port_08_12_GPIO_Config();    //��ʼ�����ƿ�08��12����ӿ��õ���GPIO����
	
	//��ʾ��������ʼ��
	screen.area_number =1;    //������Ϊ1
	screen.width =64;
	screen.height=32;
	screen.color=SINGLE;
	screen.light=100;
	screen.scan_type=SCAN_4_UP_TO_DOWN_1FOR16ROW;
	
	//���ø���ʾ������
	for(int i=0;i<screen.area_number;i++){
		area[i].id=i;
		area[i].width=64;
		area[i].height=32;
		area[i].content_type=TEXT;
		sprintf((char *)area[i].display_data,"area%d",i);
	}
	
}
