#include "cmsis_os.h"
#include "flash_SPI.h"
#include "stdbool.h"

#define MAX_AREA_NUMBER 3            //显示屏划分最大不超过3个显示区
#define DISPLAY_STRING_LENGTH   100  //LED屏显示的最大字符长度

#define get_GBK_Code(pBuff, ch, rowNo) GetGBKCode_from_EXFlash( pBuff, ch, rowNo )	  //定义获取中文字符字模数组的函数名，pBuff为存放字模数组名Ch为中文字符（国标码）

typedef struct{
	uint8_t LED_area_numbers;              //LED显示屏分区数，整块显示屏最多可分为3个显示区域
	char str_data[MAX_AREA_NUMBER][DISPLAY_STRING_LENGTH];   //指向各显示区显示字符串的指针数组，数组下标为显示区号
	uint8_t color[MAX_AREA_NUMBER];        //显示的颜色
}LED_data;


//扫描方式，详见DOC中scan_type.jpg
typedef enum{
	SCAN_16 = 0,                       //1/16扫描
	SCAN_8_LINE_1FOR8ROW,          //1/8直线走线，一路数据带8行
	SCAN_8_UP_TO_DOWN_1FOR16ROW,   //1/8上蛇行，一路数据带16行，8行折列
	SCAN_8_DOWN_TO_UP_1FOR16ROW,   //1/8下蛇行，一路数据带16行，8行折列
	SCAN_4_LINE_1FOR4_ROW,         //1/4直线走线，一路数据带4行
	SCAN_4_UP_TO_DOWN_1FOR16ROW,   //1/4上蛇行，一路数据带16行，8行折列
	SCAN_4_DOWN_TO_UP_1FOR16ROW,   //1/4下蛇行，一路数据带16行
	SCAN_4_UP_TO_DOWN_1FOR8ROW,    //1/4上蛇行，一路数据带8行
	SCAN_4_DOWN_TO_UP_1FOR8ROW,    //1/8下蛇行，一路数据带8行
}module_scan_type;

//显示屏色彩类型
typedef enum{
	SINGLE,      //单色
	DOUBLE,      //双色
	COLOR        //彩色
}led_display_color;

//LED显示屏参数
typedef struct{
	uint8_t width;         //屏宽（像素）
	uint8_t height;        //屏高
	uint8_t light;         //亮度
	led_display_color color;         //单色/双色屏
	module_scan_type scan_type;     //扫描方式，1/4、1/8、1/16扫描及走线，如1/4直线、上蛇形等
	uint8_t area_number;   //分区数，即整个LED显示屏分为几个区，最多只能分3个区
}screen_para;

//显示区显示内容类型
typedef enum{
	CLOCK,           //显示时钟类型
	TEXT             //文本类型
}area_content_type;

//显示屏各个区的参数
typedef struct{
	uint8_t id;         //区号
	uint8_t x;          //分区起点的X坐标
	uint8_t y;          //分区起点的Y坐标
	uint8_t width;      //分区宽度
	uint8_t height;
	area_content_type content_type;    //分区显示内容的类型，文本或时钟
	uint8_t display_data[DISPLAY_STRING_LENGTH];    //分区显示的内容数据
}area_para;


void LED_Display_Init(void);
void Port_08_1_GPIO_Config(void);
void Port_08_2_GPIO_Config(void);
void Port_12_1_GPIO_Config(void);
void Port_12_2_GPIO_Config(void);
void Port_12_3_GPIO_Config(void);
void Port_12_4_GPIO_Config(void);
void Port_08_12_GPIO_Config(void);
void LED_Display_Start(void);

void fill_point(uint8_t * pbuff, uint8_t screen_width, uint8_t x, uint8_t y, bool point);
void dispay_scan_4_up_to_down_1for16row(uint8_t * pdot_buff, uint8_t screen_width, uint8_t screen_height);

