#include "ets_sys.h"
#include "user_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "os_type.h"
#include "osapi.h"
#include "driver/uart.h"
#include "uart_transceivers.h"

//����AP SSIDΪesp8266_XXXXXX,XΪMAC��ַ��������ַ
#define ESP_AP_SSID      "esp8266_%02x%02x%02x"
#define ESP_AP_PASSWORD  "12345678"

#define SOFTAP_IF       0x01

/*
*******************************************************
 ����ESP8266ΪAPģʽ��������IP��DHCP��������ַ��
��������
���أ���
*******************************************************
*/
bool wifi_softap_setup( void ){

    bool ap_init_result;   //��¼wifi_softap��ʼ�����

	ap_init_result = wifi_set_opmode(SOFTAP_MODE);

    ap_init_result &= wifi_softap_dhcps_stop();

    struct ip_info info;
    IP4_ADDR(&info.ip, 192, 168, 10, 254);  // set IP
    IP4_ADDR(&info.gw, 192, 168, 10, 254);    // set gateway
    IP4_ADDR(&info.netmask, 255, 255, 255, 0); // set netmask
    ap_init_result &= wifi_set_ip_info(SOFTAP_IF, &info);

    struct dhcps_lease esp_dhcps_lease;
    IP4_ADDR(&esp_dhcps_lease.start_ip,192,168,10,1);
    IP4_ADDR(&esp_dhcps_lease.end_ip,192,168,10,5);
    ap_init_result &= wifi_softap_set_dhcps_lease(&esp_dhcps_lease);

    ap_init_result &= wifi_softap_dhcps_start();


    char softap_mac[6];
    wifi_get_macaddr(STATION_IF, softap_mac);

    struct softap_config *config = (struct softap_config *)os_zalloc(sizeof(struct softap_config ));
    wifi_softap_get_config(config); // Get soft-AP config first.
    os_sprintf(config->ssid, ESP_AP_SSID,softap_mac[3],softap_mac[4],softap_mac[5]);
    os_sprintf(config->password, ESP_AP_PASSWORD);
    config->authmode = AUTH_OPEN;
    config->ssid_len = 0;        // or its actual SSID length
    config->max_connection = 2;

    ap_init_result &= wifi_softap_set_config(config); // Set ESP8266 soft-AP config

    os_free(config);


    if(ap_init_result)
    	os_printf("softap setup success!\n");
    else
    	os_printf("softap setup fail!\n");

    return ap_init_result;
}


void ICACHE_FLASH_ATTR
user_init(void)
{

	uart_init(BIT_RATE_115200, BIT_RATE_115200);

	init_data_buffer();     //��ʼ������SLIP���ݰ��Ļ�����

    for(;;){
    	if( wifi_softap_setup() )     //����ESP8266Ϊapģʽ������ap��IP������DHCP��ַ��Χ
    		break;
    	else
    		os_delay_us(5000);
    }

    os_delay_us(5000);
    os_delay_us(5000);
    os_delay_us(5000);
    os_delay_us(5000);
    os_delay_us(5000);

    uart_tx_one_char(UART0,0x0d);   //��������з�(0x0D,0x0A)�������CLIENT�����ӵ�SLIP��������
    uart_tx_one_char(UART0,0x0a);
    uart0_sendStr("CLIENT");
}
