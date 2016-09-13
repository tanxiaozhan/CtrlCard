#include "lwip/ip_addr.h"
#include "lwip/ip.h"

#include "c_types.h"
#include "driver/uart.h"
#include "osapi.h"
#include "mem.h"
#include "lwip/memp.h"

#include "slip_end_esc.h"

#define SLIP_PACK_SIZE 1600           //SLIP数据包最大长度

//接收slip数据的结构
struct slip_pack{
	uint8 * buff;
	uint32 len;
};

struct slip_pack * pslip_pack;    //SLIP数据报文指针

enum slipif_recv_state receive_state = SLIP_RECV_NORMAL;     //slip 之前接收到的字符状态，
															//之前收到0xDB的状态为SLIP_RECV_ESCAPE

/*
 * 初始化数据缓冲区
 */
void ICACHE_FLASH_ATTR
init_data_buffer(void)
{
	pslip_pack = (struct slip_pack *)os_zalloc(sizeof(struct slip_pack));     //初始化SLIP数据报文指针
	pslip_pack->buff = (uint8 *)os_zalloc(sizeof(uint8) * SLIP_PACK_SIZE);    //给接收串口数据的缓冲区分配内存
	pslip_pack->len = 0;													 //已接收的数据长度（字节）
}

LOCAL void ICACHE_FLASH_ATTR
slip_output(struct slip_pack * p){

	struct pbuf * pslip_pbuf=pbuf_alloc( PBUF_LINK, p->len, PBUF_RAM);   //定义PBUF_RAM类型的pbuf

	os_memcpy((uint8 *)pslip_pbuf->payload,p->buff,p->len);    //把串口接收的数据复制到pbuf

	ip_addr_t dest;          //目标IP地址
	u8_t dest_offset=16;	//ip包中目标地址的偏移

	IP4_ADDR(&dest, p->buff[dest_offset], p->buff[dest_offset+1], p->buff[dest_offset+2], p->buff[dest_offset+3]);

	//ip_output(struct pbuf *p, ip_addr_t *src, ip_addr_t *dest, u8_t ttl, u8_t tos, u8_t proto)
	ip_output(pslip_pbuf, &dest, &dest,0, 0, 0);      //把串口接收的IP包发送到下一层（网络层）
}


/*
 *从ESP8266 uart0接收的数据，接收到一个完整的slip包后，将数据包交给网络接口发送
 */
void ICACHE_FLASH_ATTR
slip_input(uint8 c){

	uint8 i,j;
	switch( receive_state ){
	case SLIP_RECV_NORMAL:
		switch(c){
			case SLIP_END:   //0xC0, slip数据包的结构：0xC0数据0xC0
				if( pslip_pack->len >0){   //接收到一个slip数据包，如果是TCP包则发到网络接口

					if( pslip_pack->len>=20 ){     //ip数据包>=20字节
						if( ( (pslip_pack->buff[0] & 0xf0) == 0x40 ) && (pslip_pack->buff[9]==6) ){    //如果数据包第一字节前四位等于4，即IPV4数据包，并且为TCP协议(IP包第9个字节），否则不向下层提交处理。
						slip_output(pslip_pack);

						}
					}
					pslip_pack->len  = 0;     //清除缓冲区

				}
				break;

			case SLIP_ESC:					//0xDB
				receive_state = SLIP_RECV_ESCAPE;
				break;

			default:
				pslip_pack->buff[ pslip_pack->len++ ]=c;
		}
		break;

	case SLIP_RECV_ESCAPE:
		switch(c){
		case SLIP_ESC_END:				//是0xDC，转为0xC0
			pslip_pack->buff[ pslip_pack->len++ ]=SLIP_END;
			break;

		case SLIP_ESC_ESC:				//0xDD，转为0xDB
			pslip_pack->buff[ pslip_pack->len++ ]=SLIP_ESC;

		}
		receive_state=SLIP_RECV_NORMAL;
	}


	//uart_tx_one_char(UART0,c);
	//uart_tx_one_char(UART0,pslip_pack->buff[pslip_pack->len-1]);

	if( pslip_pack->len >=SLIP_PACK_SIZE){     //接收的数据大于缓冲区，丢弃重新接收。
		 pslip_pack->len =0;
		os_printf("slip buffer overflow!!");
	}

}
