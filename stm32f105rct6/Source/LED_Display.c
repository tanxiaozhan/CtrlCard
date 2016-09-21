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
#include <string.h>

screen_para screen;        //显示屏参数
area_para area[MAX_AREA_NUMBER];           //显示区参数

uint8_t max_len=0;    // 各显示中显示字符最长的长度

//uint8_t * screen_dot;   //指向一个二维数组，该数组保存了当前显示的一屏点阵数据

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityAboveNormal, 1, 0);

/*----------------------------------------------------------------------------
 * 线程'LED_Dispaly'：LED显示屏扫描显示
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {

	uint8_t screen_width_bytes;     //屏宽、屏高（按字节为单位）
	screen_width_bytes=((screen.height % 8)==0)?(screen.height/8):(screen.height/8+1) ;
	uint8_t screen_dot[screen.height][screen_width_bytes];    //保存当前显示的一屏点阵数据
	uint8_t * pStr[screen.area_number];
	int i,j,current_area,area_no,row,col;
	for(i=0;i<screen.area_number;i++){
		pStr[i] = area[i].display_data;
		if(max_len < strlen((char *)area[i].display_data) )    //获取各显示区中字符数最多的
			max_len = strlen((char *)area[i].display_data);         
	}

	//按显示区获取点阵数据
	for(area_no=0;area_no<screen.area_number;area_no++){
		for(row=0;row<area[area_no].width;row++){
			for(col=0;col<area[area_no].height;col++){
				if( *pStr[area_no]<= 126){       //英文字符
					screen_dot[ area[area_no].x+row ][ area[area_no].y+col ] = ascii_Dot[ *pStr[area_no] - ' ' ][ row%16 ] ;
				}   //英文处理完毕
				else{         //汉字字符
					
				}    //汉字处理完毕
			}
		}
	}

	

	//填充一屏点阵数据，即给screen_dot数组赋值
	i=0;
	for( i=0; i<screen.height; i++ ){              
		for( j=0; j<screen.width ; j+=8 ){
			for( current_area=0;current_area<screen.area_number; current_area++){
				if( i<area[current_area].height && j<area[current_area].width ){           //边界检测，当前位于哪个显示区
					break;
				}
			}	
				if( ( *pStr[current_area] != '\0')){  
					if( *pStr[current_area] <= 126 ){                             //英文字母
						screen_dot[i][j/8] = ascii_Dot[ *pStr[current_area] - ' '][ i%16 ] ;
						if( (area[current_area].width-j)%8 != 0 ){                  //显示区宽度不是8的整数倍，其最几个点阵不够一字节，与其他显示区共用一个点阵字节
							screen_dot[i][j/8] &= 0xFF << ((area[current_area].width-j) % 8);
						}
						else if((area[current_area].x % 8) != 0){               //显示区起始X坐标不是8的整数倍，其起始的点阵与别的显示区共用点阵字节
							screen_dot[i][j/8] &= 0xFF >> (area[current_area].x %8);
						}
						
						pStr[current_area] ++;
						
					}  //英文字母处理完毕
					else{                                              //汉字字符
					uint16_t char_GBK_Code;                            //汉字的国标码
					uint8_t GBK_dot[2];                                  //汉字的点阵数据
					char_GBK_Code = * (uint16_t *)pStr[current_area];             //取一个汉字的国标码，二字节
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //国标码前后字节对调
					get_GBK_Code( GBK_dot, char_GBK_Code,12-current_area*4+i );   //读取汉字字模一行点阵数据2字节
					screen_dot[i][j/8]=GBK_dot[0];    
					j += 8  ;                                               
					screen_dot[i][j/8]=GBK_dot[1];
					pStr[current_area] += 2;                                      //一个汉字占二个字节
					}  //汉字处理完毕
				}
		}
	}
	//一屏点阵数据填充完成
	
	
	switch(screen.scan_type){
		case SCAN_16:                   //1/16扫描
			break;
		
		case SCAN_8_LINE_1FOR8ROW:          //1/8直线走线，一路数据带8行
			break;
		
		case SCAN_8_UP_TO_DOWN_1FOR16ROW:   //1/8上蛇行，一路数据带16行，8行折列
			break;
		
		case SCAN_8_DOWN_TO_UP_1FOR16ROW:   //1/8下蛇行，一路数据带16行，8行折列
			break;
		
		case SCAN_4_LINE_1FOR4_ROW:         //1/4直线走线，一路数据带4行
			break;

		case 	SCAN_4_UP_TO_DOWN_1FOR16ROW:   //1/4上蛇行，一路数据带16行，8行折列
			scan_4_up_to_down_1for16row((char *)screen_dot);
			break;
		
		case SCAN_4_DOWN_TO_UP_1FOR16ROW:   //1/4下蛇行，一路数据带16行
		break;
		
		case SCAN_4_UP_TO_DOWN_1FOR8ROW:    //1/4上蛇行，一路数据带8行
			break;
		
		case SCAN_4_DOWN_TO_UP_1FOR8ROW:   //1/4下蛇行，一路数据带8行
			break;
	}
}
	
void scan_4_up_to_down_1for16row(char * string_dot){	
	uint8_t row=4;   // 1/4扫描  
	//char charDot[2];    //保存当前显示字符的点阵信息，英文宽8位，汉字宽16位
	uint8_t i,j,k,len;
	uint16_t scan;    //列扫描
	uint8_t LED_width;   //屏宽
	char dot;
	
	EN(ON);
	while(1){
				//EN(OFF);
	while(i<screen.width){
		for(j=0;j<screen.height;j++){
			for(k=0;k<screen.area_number;k++){
			if( area[k].display_data[len]<=126 ){  //英文字符
				string_dot[len]= ;
				}
			}
			}
		}
		for(i=0;i<row;i++){
			for(LED_width=0;LED_width<screen.width;LED_width++){
					pStr[0]=area[0].display_data;
					if( *pStr[0]<=126){	//英文字符
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
							CLK(ON);       //594移位信号
						}
					}
					pStr[area] ++;
				}
				else{   //汉字字符
					uint16_t char_GBK_Code;    //汉字的国标码
					uint16_t GBK_Dot;          //汉字的点阵数据
					char_GBK_Code = * (uint16_t *)pStr[area];       //取一个汉字的国标码，二字节
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //国标码前后字节对调
					for(k=0;k<4;k++){
						get_GBK_Code( (uint8_t *)charDot, char_GBK_Code,12-k*4+i );   //读取汉字字模一行点阵数据2字节
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
							CLK(ON);       //594移位信号
					}
				}
					pStr[area] += 2;      //一个汉字占二个字节
				}
			}
			STB(OFF);
			STB(ON);     //锁存

			A( i & 0x01 );B( i & 0x02 );C( i & 0x04 );D( i & 0x08 );   //行扫描
			osDelay(5);
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

/*
	********************************************************
	*  LED显示屏初始化
	********************************************************
*/
void LED_Display_Init(void){

	Port_08_12_GPIO_Config();    //初始化控制卡08、12输出接口用到的GPIO引脚
	
	//显示屏参数初始化
	screen.area_number =1;    //分区数为1
	screen.width =64;
	screen.height=32;
	screen.color=SINGLE;
	screen.light=100;
	screen.scan_type=SCAN_4_UP_TO_DOWN_1FOR16ROW;
	
	//设置各显示区参数
	for(int i=0;i<screen.area_number;i++){
		area[i].id=i;
		area[i].width=64;
		area[i].height=32;
		area[i].content_type=TEXT;
		sprintf(area[i].display_data,"area%d",i);
	}
	
}
