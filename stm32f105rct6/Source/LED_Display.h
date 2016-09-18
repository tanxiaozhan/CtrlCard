#include "cmsis_os.h"
#include "flash_SPI.h"
#include "stdbool.h"

#define MAX_AREA_NUMBER 3            //��ʾ��������󲻳���3����ʾ��
#define DISPLAY_STRING_LENGTH   100  //LED����ʾ������ַ�����

#define get_GBK_Code(pBuff, ch, rowNo) GetGBKCode_from_EXFlash( pBuff, ch, rowNo )	  //�����ȡ�����ַ���ģ����ĺ�������pBuffΪ�����ģ�������sChΪ�����ַ��������룩

typedef struct{
	uint8_t LED_area_numbers;              //LED��ʾ����������������ʾ�����ɷ�Ϊ3����ʾ����
	char str_data[MAX_AREA_NUMBER][DISPLAY_STRING_LENGTH];   //ָ�����ʾ����ʾ�ַ�����ָ�����飬�����±�Ϊ��ʾ����
	uint8_t color[MAX_AREA_NUMBER];        //��ʾ����ɫ
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

