#include "cmsis_os.h"
#include "flash_SPI.h"
#include "stdbool.h"

#define MAX_AREA_NUMBER 3            //��ʾ��������󲻳���3����ʾ��
#define MAX_STRING_LENGTH   30  //LED����ʾ������ַ�����

#define get_GBK_Code(pBuff, ch, rowNo) GetGBKCode_from_EXFlash( pBuff, ch, rowNo )	  //�����ȡ�����ַ���ģ����ĺ�������pBuffΪ�����ģ�������sChΪ�����ַ��������룩


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
	uint16_t width;         //�������أ�
	uint16_t height;        //����
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

//��ʾ������ʾ������
typedef struct{
	uint8_t id;         //����
	uint8_t x;          //��������X����
	uint8_t y;          //��������Y����
	uint8_t width;      //�������
	uint8_t height;
	uint8_t red;        //��ɫ��1-��ɫ��0-����ʾ
	uint8_t green;      //��ɫ��1-��ɫ��0-����ʾ
	uint8_t ani_in;         //��ʾ�������Ч
	uint8_t ani_out;        //��ʾ�˳�����Ч
	uint8_t speed;      //��ʾЧ���е�����ƶ��ٶ�
	area_content_type content_type;    //������ʾ���ݵ����ͣ��ı���ʱ��
	uint8_t display_data[MAX_STRING_LENGTH];    //������ʾ����������
	uint8_t length;       //��ʾ�����ݵĳ���
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
void check_screen(bool check, bool check_red, bool check_green);  //����/�˳���Ļ���
void buffer_to_display(void);
void del_area(uint8_t area_no);
