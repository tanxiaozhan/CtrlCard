#include "cmsis_os.h"
#include "flash_SPI.h"

#define DISPLAY_STRING_LENGTH   100  //LED屏显示的最大字符长度
#define get_GBK_Code(pBuff, ch, rowNo) GetGBKCode_from_EXFlash( pBuff, ch, rowNo )	  //定义获取中文字符字模数组的函数名，pBuff为存放字模数组名Ch为中文字符（国标码）


void Port_08_1_GPIO_Config(void);
void Port_08_2_GPIO_Config(void);
void Port_12_1_GPIO_Config(void);
void Port_12_2_GPIO_Config(void);
void Port_12_3_GPIO_Config(void);
void Port_12_4_GPIO_Config(void);
void Port_08_12_GPIO_Config(void);
void LED_Display_Start(void);

