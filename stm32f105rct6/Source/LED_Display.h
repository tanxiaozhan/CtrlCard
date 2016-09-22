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


//ɨ�跽ʽ�����DOC��scan_type.jpg
typedef enum{
	SCAN_16 = 0,                       //1/16ɨ��
	SCAN_8_LINE_1FOR8ROW,          //1/8ֱ�����ߣ�һ·���ݴ�8��
	SCAN_8_UP_TO_DOWN_1FOR16ROW,   //1/8�����У�һ·���ݴ�16�У�8������
	SCAN_8_DOWN_TO_UP_1FOR16ROW,   //1/8�����У�һ·���ݴ�16�У�8������
	SCAN_4_LINE_1FOR4_ROW,         //1/4ֱ�����ߣ�һ·���ݴ�4��
	SCAN_4_UP_TO_DOWN_1FOR16ROW,   //1/4�����У�һ·���ݴ�16�У�8������
	SCAN_4_DOWN_TO_UP_1FOR16ROW,   //1/4�����У�һ·���ݴ�16��
	SCAN_4_UP_TO_DOWN_1FOR8ROW,    //1/4�����У�һ·���ݴ�8��
	SCAN_4_DOWN_TO_UP_1FOR8ROW,    //1/8�����У�һ·���ݴ�8��
}module_scan_type;

//��ʾ��ɫ������
typedef enum{
	SINGLE,      //��ɫ
	DOUBLE,      //˫ɫ
	COLOR        //��ɫ
}led_display_color;

//LED��ʾ������
typedef struct{
	uint8_t width;         //�������أ�
	uint8_t height;        //����
	uint8_t light;         //����
	led_display_color color;         //��ɫ/˫ɫ��
	module_scan_type scan_type;     //ɨ�跽ʽ��1/4��1/8��1/16ɨ�輰���ߣ���1/4ֱ�ߡ������ε�
	uint8_t area_number;   //��������������LED��ʾ����Ϊ�����������ֻ�ܷ�3����
}screen_para;

//��ʾ����ʾ��������
typedef enum{
	CLOCK,           //��ʾʱ������
	TEXT             //�ı�����
}area_content_type;

//��ʾ���������Ĳ���
typedef struct{
	uint8_t id;         //����
	uint8_t x;          //��������X����
	uint8_t y;          //��������Y����
	uint8_t width;      //�������
	uint8_t height;
	area_content_type content_type;    //������ʾ���ݵ����ͣ��ı���ʱ��
	uint8_t display_data[DISPLAY_STRING_LENGTH];    //������ʾ����������
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

