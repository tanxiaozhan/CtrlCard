#include "ets_sys.h"
#include "user_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "os_type.h"
#include "osapi.h"
#include "driver/uart.h"
#include "uart_transceivers.h"

//设置AP SSID为esp8266_XXXXXX,X为MAC地址后三个地址
#define ESP_AP_SSID      "esp8266_%02x%02x%02x"
#define ESP_AP_PASSWORD  "12345678"

#define SOFTAP_IF       0x01

/*
*******************************************************
 设置ESP8266为AP模式，并设置IP及DHCP服务器地址池
参数：无
返回：无
*******************************************************
*/
bool wifi_softap_setup( void ){

    bool ap_init_result;   //记录wifi_softap初始化结果

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

	init_data_buffer();     //初始化接收SLIP数据包的缓冲区

    for(;;){
    	if( wifi_softap_setup() )     //设置ESP8266为ap模式，设置ap的IP，分配DHCP地址范围
    		break;
    	else
    		os_delay_us(5000);
    }

    os_delay_us(5000);
    os_delay_us(5000);
    os_delay_us(5000);
    os_delay_us(5000);
    os_delay_us(5000);

    uart_tx_one_char(UART0,0x0d);   //先输出换行符(0x0D,0x0A)，再输出CLIENT，连接到SLIP服务器。
    uart_tx_one_char(UART0,0x0a);
    uart0_sendStr("CLIENT");
}
