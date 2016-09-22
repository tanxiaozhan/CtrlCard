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
uint8_t screen_dot[32][8];    //保存当前显示的一屏点阵数据

//uint8_t * screen_dot;   //指向一个二维数组，该数组保存了当前显示的一屏点阵数据

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityAboveNormal, 1, 0);

/*----------------------------------------------------------------------------
 * 线程'LED_Dispaly'：LED显示屏扫描显示
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {

	//uint8_t screen_dot[screen.height][screen_width_bytes];    //保存当前显示的一屏点阵数据
	//uint8_t * pStr[screen.area_number];
	uint8_t * pStr[3];

	unsigned int col;
	uint8_t area_no,scan,row;
	
while(1){
	
	//按显示区获取点阵数据，往显存写入点阵数据
	for( area_no=0;area_no<screen.area_number;area_no++ ){
		for( row=area[area_no].y;row<area[area_no].height;row++){
			pStr[area_no] = area[area_no].display_data;
			col = area[area_no].x;
			while( (col<area[area_no].width) && ( *pStr[area_no] != 0 ) ){
				if( *pStr[area_no]<= 126){       //英文字符
					uint8_t char_dot;
					char_dot = ascii_Dot[ *pStr[area_no] - ' ' ][ (row-area[area_no].y)%16 ] ;
					scan = 0x80;
					while( col < area[area_no].width && scan > 0x00 ){
						fill_point((uint8_t *)screen_dot, screen.width/8, col, row, char_dot & scan );
						scan >>= 1;
						col ++;
					}
					pStr[area_no] ++ ;      //下一个字符

				}   //英文处理完毕
				
				else{         //汉字字符
					uint16_t char_GBK_Code;                            //汉字的国标码
					uint8_t GBK_dot[2];                                  //汉字的点阵数据
					char_GBK_Code = * (uint16_t *)pStr[area_no];             //取一个汉字的国标码，二字节
					char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //国标码前后字节对调
					get_GBK_Code( GBK_dot, char_GBK_Code,(row-area[area_no].y)%16 );   //读取汉字字模一行点阵数据2字节
					
					//处理汉字前8位点阵
					scan = 0x80;
					while( col < area[area_no].width && scan > 0x00 ){
						fill_point((uint8_t *)screen_dot, screen.width/8, col, row, GBK_dot[0]&scan );
						scan >>= 1;
						col ++ ;
					}
					//处理汉字后8位点阵
					scan = 0x80;
					while( col < area[area_no].width && scan >0x00 ){
						fill_point((uint8_t *)screen_dot, screen.width/8, col, row, GBK_dot[1]&scan );
						scan >>= 1;
						col ++;
					}
						pStr[area_no] += 2;                                      //一个汉字占二个字节
					
				} //汉字处理完毕
			}
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
		
/*	
	//按显示区获取点阵数据
	for( area_no=0;area_no<screen.area_number;area_no++ ){
		if( (area[area_no].x % 8) == 0 ){                 //显示区起点x坐标是8的倍数
			for( row=area[area_no].y;row<area[area_no].height;row++){
				pStr[area_no] = area[area_no].display_data;
				col = area[area_no].x;
				while( (col<area[area_no].width) && ( *pStr[area_no] != 0 ) ){
					if( *pStr[area_no]<= 126){       //英文字符
						if( area[area_no].width-col < 8 ){     //显示区最后一段不够一个字节
							screen_dot[row][col/8 + 1] &= 0xFF >> (area[area_no].width-col);
							screen_dot[row][col/8 + 1] |= (0xFF << (8-(area[area_no].width-col))) & ascii_Dot[*pStr[area_no]-' '][(row-area[area_no].y)%16] ;
						}
						else{
							screen_dot[row][col/8] = ascii_Dot[ *pStr[area_no] - ' ' ][ (row-area[area_no].y)%16 ] ;
						}
						pStr[area_no] ++ ;      //下一个字符
						col += 8 ;               //下一个显存字节
					}   //英文处理完毕
					else{         //汉字字符
						uint16_t char_GBK_Code;                            //汉字的国标码
						uint8_t GBK_dot[2];                                  //汉字的点阵数据
						char_GBK_Code = * (uint16_t *)pStr[area_no];             //取一个汉字的国标码，二字节
						char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //国标码前后字节对调
						get_GBK_Code( GBK_dot, char_GBK_Code,(row-area[area_no].y)%16 );   //读取汉字字模一行点阵数据2字节
						if(area[area_no].width-col<8){
							screen_dot[row][col/8+1] &= 0xFF >> (area[area_no].width - col );
							screen_dot[row][col/8+1] |= (0xFF << (8-(area[area_no].width-col))) & GBK_dot[0];
						}
						else{
							screen_dot[row][col/8]=GBK_dot[0];    
						}
						col += 8 ;
						if( col > area[area_no].width ){ break;  }    //超出显示区宽度
						if(area[area_no].width-col<8){
							screen_dot[row][col/8+1] &= 0xFF >> (area[area_no].width - col );
							screen_dot[row][col/8+1] |= (0xFF << (8-(area[area_no].width-col))) & GBK_dot[1];
						}
						else{
							screen_dot[row][col/8]=GBK_dot[1];    
						}
						col += 8 ;
						pStr[area_no] += 2;                                      //一个汉字占二个字节
					} 	   //汉字处理完毕
				}
			}
		}
		else{      //显示区起点x坐标不是8的倍数
			for( row=area[area_no].y;row<area[area_no].height;row++){
				pStr[area_no] = area[area_no].display_data;
				col = area[area_no].x;
				while( (col<area[area_no].width) && ( *pStr[area_no] != 0 ) ){
					if( *pStr[area_no]<= 126){       //英文字符
						int char_dot;
						char_dot = ascii_Dot[*pStr[area_no]-' '][(row-area[area_no].y)%16];
						scan = 0x80;
						while(col<area[area_no].width && scan>0x00 ){
							fill_point((uint8_t *)screen_dot, screen.width/8, col, row, char_dot & scan );
							col ++;
							scan >>= 1;
						}
						pStr[area_no] ++ ;      //下一个字符
					}   //英文处理完毕
					else{         //汉字字符
						uint16_t char_GBK_Code;                            //汉字的国标码
						uint8_t GBK_dot[2];                                  //汉字的点阵数据
						char_GBK_Code = * (uint16_t *)pStr[area_no];             //取一个汉字的国标码，二字节
						char_GBK_Code = ( char_GBK_Code << 8 ) + ( char_GBK_Code >> 8 );    //国标码前后字节对调
						get_GBK_Code( GBK_dot, char_GBK_Code,(row-area[area_no].y)%16 );   //读取汉字字模一行点阵数据2字节
						
						//处理汉字前8个点阵
						scan = 0x80;
						while(col<area[area_no].width && scan>0x00){
							fill_point((uint8_t *)screen_dot, screen.width/8, col, row, GBK_dot[0] & scan );
							col ++ ;
							scan >>= 1;
						}
						//处理汉字后8个点阵
						scan = 0x80;
						while(col<area[area_no].width && scan>0x00){
							fill_point((uint8_t *)screen_dot, screen.width/8, col, row, GBK_dot[1] & scan );
							col ++ ;
							scan >>= 1;
						}
						pStr[area_no] += 2;                                      //一个汉字占二个字节
					} 	   //汉字处理完毕
				}
			}
		}
	}
*/
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
	EN(ON);
	for(row=0;row<scan_rows;row++){
		for(col=0;col<screen_width_bytes;col++){
			for(i=0;i<4;i++){
				//printf("row=%d, dot=%2x   \n",12-4*i+row, *(pdot_buff+ (12-4*i+row)*screen_width_bytes + col));
				
				scan=0x01;
				for(j=0;j<8;j++){
					CLK(OFF);     
					PORT_12_1_R( *(pdot_buff + (12-4*i+row)*screen_width_bytes + col) & scan );
					PORT_12_2_R( *(pdot_buff + (12-4*i+row+16)*screen_width_bytes + col) & scan );
					PORT_12_3_R( *(pdot_buff + (12-4*i+row+32)*screen_width_bytes + col) & scan );
					PORT_12_4_R( *(pdot_buff + (12-4*i+row+48)*screen_width_bytes + col) & scan );
					CLK(ON);       //594移位信号
					scan <<= 1;
				}
			}
		}
		A( row & 0x01 );B( row & 0x02 );   //行扫描
		STB(0);
		EN(ON);  //延时
		STB(1);  //锁存
		osDelay(1);
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
	
	uint8_t i;
	Port_08_12_GPIO_Config();    //初始化控制卡08、12输出接口用到的GPIO引脚
	
	//显示屏参数初始化
	screen.area_number =1;    //分区数为1
	screen.width =64;
	screen.height=32;
	screen.color=SINGLE;
	screen.light=100;
	screen.scan_type=SCAN_4_UP_TO_DOWN_1FOR16ROW;
	
	//设置各显示区参数
	for(i=0;i<screen.area_number;i++){
		area[i].id=i;
		area[i].width=64;
		area[i].height=16;
		area[i].content_type=TEXT;
		sprintf((char *)area[i].display_data,"area%d",i);
	}
	area[0].x=0;
	area[0].y=0;
	area[1].x=0;
	area[1].y=16;
	printf("area 0:%s\n",area[0].display_data);
	printf("area 1:%s\n",area[1].display_data);
	
	
}
