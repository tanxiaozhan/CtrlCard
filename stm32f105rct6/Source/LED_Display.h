#include "cmsis_os.h"
#include "flash_SPI.h"
#include "stdbool.h"

#define MAX_AREA_NUMBER 3            //显示屏划分最大不超过3个显示区
#define MAX_STRING_LENGTH   30  //LED屏显示的最大字符长度

#define get_GBK_Code(pBuff, ch, rowNo) GetGBKCode_from_EXFlash( pBuff, ch, rowNo )	  //定义获取中文字符字模数组的函数名，pBuff为存放字模数组名Ch为中文字符（国标码）


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
	uint16_t width;         //屏宽（像素）
	uint16_t height;        //屏高
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

//显示屏各显示区参数
typedef struct{
	uint8_t id;         //区号
	uint8_t x;          //分区起点的X坐标
	uint8_t y;          //分区起点的Y坐标
	uint8_t width;      //分区宽度
	uint8_t height;
	uint8_t red;        //颜色，1-红色，0-不显示
	uint8_t green;      //颜色，1-绿色，0-不显示
	uint8_t ani_in;         //显示进入的特效
	uint8_t ani_out;        //显示退出的特效
	uint8_t speed;      //显示效果中点阵的移动速度
	area_content_type content_type;    //分区显示内容的类型，文本或时钟
	uint8_t display_data[MAX_STRING_LENGTH];    //分区显示的内容数据
	uint8_t length;       //显示的内容的长度
}area_para;


void Port_08_1_GPIO_Config(void);
void Port_08_2_GPIO_Config(void);
void Port_12_1_GPIO_Config(void);
void Port_12_2_GPIO_Config(void);
void Port_12_3_GPIO_Config(void);
void Port_12_4_GPIO_Config(void);
void Port_08_12_GPIO_Config(void);

void LED_display_init(void);
void LED_display_start(void);
void LED_display_power_off(void);

void fill_point(uint8_t * pbuff, uint8_t screen_width, uint8_t x, uint8_t y, bool point);
void dispay_scan_4_up_to_down_1for16row(uint8_t * pdot_buff_red, uint8_t * pdot_buff_green, uint8_t screen_width, uint8_t screen_height);
void check_screen(bool check, bool check_red, bool check_green);  //进入/退出屏幕检测
void buffer_to_display(void);
void del_area(uint8_t area_no);
