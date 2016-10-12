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

osThreadId LED_display_thread_id;   //显示线程id，创建/中止此线程控制显示的开关

bool power_on=false;  //显示屏开机/关机

screen_para screen;        //显示屏参数
area_para area[MAX_AREA_NUMBER];           //显示区参数

uint8_t max_len=0;    // 各显示中显示字符最长的长度
uint8_t screen_dot[32][8];    //保存当前显示的一屏点阵数据

uint8_t current_row_dot[MAX_STRING_LENGTH];   //该数组保存了当前显示行的点阵数据

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityAboveNormal, 1, 0);

/*----------------------------------------------------------------------------
 * 线程'LED_Dispaly'：LED显示屏扫描显示
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {

	uint8_t * pStr;
	unsigned int col;
	uint8_t area_no,scan,row,dot;
	unsigned int startX[MAX_AREA_NUMBER],i,v_col;
	uint8_t speed[MAX_AREA_NUMBER];                //显示特效中点阵的移动速度

	for(i=0;i<screen.area_number;i++){
		speed[i]=area[i].speed ;
		startX[i] = 0;
	}
	
	while(1){
		//清空显存，cve即用0xFF给显存数组赋值
		for(row=0;row<32;row++){      
			for(scan=0;scan<8;scan++){
				screen_dot[row][scan]=0xFF;
			}
		}
	
	
	//按显示区获取一行点阵数据，保存到数组current_row_dot
	for( area_no=0;area_no<screen.area_number;area_no++ ){
		for( row=area[area_no].y; row<(area[area_no].y+area[area_no].height);row++){
			pStr = area[area_no].display_data;
			
			col=0;
			
			//获取当前显示行的点阵数据，将点阵数据保存到current_row_dot数组
			while( col<area[area_no].length ){
				if( *pStr <= 126 ){      //英文字符
					current_row_dot[col++] = ascii_Dot[ *pStr - ' ' ][row - area[area_no].y % 16] ;    //获取点阵
					pStr++ ;
				}
				else{    //汉字字符
					uint16_t char_GBK_Code;                            //汉字的国标码
					uint8_t GBK_dot[2];                                  //汉字的点阵数据
					char_GBK_Code = * (uint16_t *)pStr;             //取一个汉字的国标码，二字节
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //国标码前后字节对调
					get_GBK_Code( GBK_dot, char_GBK_Code,(row-area[area_no].y)%16 );   //读取汉字字模一行点阵数据2字节
					current_row_dot[col++] = GBK_dot[0];
					current_row_dot[col++] = GBK_dot[1];
					pStr += 2 ;
				}
			}
			
			
			//刷新显存，即用点阵数据给显存数组赋值
			col = area[area_no].x;
			while( col < (area[area_no].x + area[area_no].width) ){
				v_col = col - area[area_no].x;    //在虚拟显存中的X坐标
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
//到此，显存点阵数据写入完成

	
	//确定扫描类型	
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
			dispay_scan_4_up_to_down_1for16row((uint8_t *)screen_dot, screen.width, screen.height );
			break;
		
		case SCAN_4_DOWN_TO_UP_1FOR16ROW:   //1/4下蛇行，一路数据带16行
			break;
		
		case SCAN_4_UP_TO_DOWN_1FOR8ROW:    //1/4上蛇行，一路数据带8行
			break;
		
		case SCAN_4_DOWN_TO_UP_1FOR8ROW:   //1/4下蛇行，一路数据带8行
			break;
	}
//osDelay(1);
}

}



/*
 *------------------------------------------------------------------------------------
 *
 *	 在(x,y)处显示一个点，实际上在显存中(x,y)处设置一个比特位的值，0-显示，1-不显示
 * 参数：pbuff
 *       screen_width   屏宽，字节为单位，即屏宽像素占多少字节
 *       screen_height  屏高
 *       x,y            显示屏上的坐标，左上角坐标(0,0)
 *       point          像素点的值，0--显示，1--不显示
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
 * 将显存（即screen_dot数组） 的数据显示到显示屏
 * 参数：pbuff          显存指针，指向保存了一屏点阵数据的数组
 *       screen_width   屏宽多少像素
 *       screen_height  屏高多少像素
 *
 *------------------------------------------------------------------------------------
*/
void dispay_scan_4_up_to_down_1for16row(uint8_t * pdot_buff, uint8_t screen_width, uint8_t screen_height){	
	uint8_t scan_rows=4;   // 1/4扫描  
	uint8_t scan, i, j;
	uint8_t row, col;   //当前行、列
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
					CLK(ON);       //594移位信号
					scan >>= 1;
				}
			}
		}
		A( row & 0x01 );B( row & 0x02 );   //行扫描
		STB(0);
		STB(1);  //锁存
		EN(ON);
		osDelay(2);
		EN(OFF);
		osDelay(3);

	}
}

/*
	********************************************************
	*
	*  创建LED显示屏控制信号线程，控制卡控制信号开始输出
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
	*  关闭显示
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
	*  LED显示屏初始化
	********************************************************
*/
void LED_display_init(void){
	
	uint8_t i;
	Port_08_12_GPIO_Config();    //初始化控制卡08、12输出接口用到的GPIO引脚
	
	//显示屏参数初始化
	screen.area_number =3;    //显示分区数
	screen.width =64;
	screen.height=32;
	screen.color=SINGLE;
	screen.light=100;
	screen.scan_type=SCAN_4_UP_TO_DOWN_1FOR16ROW;
	


	sprintf((char *)area[0].display_data,"谭蔷雨");
	sprintf((char *)area[1].display_data,"素华");
	sprintf((char *)area[2].display_data,"! $123456ABcd");
	//设置各显示区参数
	for(i=0;i<screen.area_number;i++){
		area[i].id=i;
		area[i].width=64;
		area[i].height=16;
		area[i].speed=i*5+1;
		area[i].content_type=TEXT;
		area[i].length=strlen((char *)area[i].display_data);
	}
	area[0].x=0;
	area[0].y=0;
	area[1].x=0;
	area[1].width=32;
	area[1].y=16;	
	area[2].x=32;
	area[2].y=16;
	area[2].width=30;
	area[0].speed=3;
	area[2].speed=6;
}
