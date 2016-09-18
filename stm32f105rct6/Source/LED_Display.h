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


void Port_08_1_GPIO_Config(void);
void Port_08_2_GPIO_Config(void);
void Port_12_1_GPIO_Config(void);
void Port_12_2_GPIO_Config(void);
void Port_12_3_GPIO_Config(void);
void Port_12_4_GPIO_Config(void);
void Port_08_12_GPIO_Config(void);
void LED_Display_Start(void);
bool get_display_on_LED_data(LED_data * data);

