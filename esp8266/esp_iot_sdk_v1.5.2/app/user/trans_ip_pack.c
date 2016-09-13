#include "lwip/ip_addr.h"
#include "lwip/ip.h"

#include "c_types.h"
#include "driver/uart.h"
#include "osapi.h"
#include "mem.h"
#include "lwip/memp.h"

#include "slip_end_esc.h"

#define SLIP_PACK_SIZE 1600           //SLIP���ݰ���󳤶�

//����slip���ݵĽṹ
struct slip_pack{
	uint8 * buff;
	uint32 len;
};

struct slip_pack * pslip_pack;    //SLIP���ݱ���ָ��

enum slipif_recv_state receive_state = SLIP_RECV_NORMAL;     //slip ֮ǰ���յ����ַ�״̬��
															//֮ǰ�յ�0xDB��״̬ΪSLIP_RECV_ESCAPE

/*
 * ��ʼ�����ݻ�����
 */
void ICACHE_FLASH_ATTR
init_data_buffer(void)
{
	pslip_pack = (struct slip_pack *)os_zalloc(sizeof(struct slip_pack));     //��ʼ��SLIP���ݱ���ָ��
	pslip_pack->buff = (uint8 *)os_zalloc(sizeof(uint8) * SLIP_PACK_SIZE);    //�����մ������ݵĻ����������ڴ�
	pslip_pack->len = 0;													 //�ѽ��յ����ݳ��ȣ��ֽڣ�
}

LOCAL void ICACHE_FLASH_ATTR
slip_output(struct slip_pack * p){

	struct pbuf * pslip_pbuf=pbuf_alloc( PBUF_LINK, p->len, PBUF_RAM);   //����PBUF_RAM���͵�pbuf

	os_memcpy((uint8 *)pslip_pbuf->payload,p->buff,p->len);    //�Ѵ��ڽ��յ����ݸ��Ƶ�pbuf

	ip_addr_t dest;          //Ŀ��IP��ַ
	u8_t dest_offset=16;	//ip����Ŀ���ַ��ƫ��

	IP4_ADDR(&dest, p->buff[dest_offset], p->buff[dest_offset+1], p->buff[dest_offset+2], p->buff[dest_offset+3]);

	//ip_output(struct pbuf *p, ip_addr_t *src, ip_addr_t *dest, u8_t ttl, u8_t tos, u8_t proto)
	ip_output(pslip_pbuf, &dest, &dest,0, 0, 0);      //�Ѵ��ڽ��յ�IP�����͵���һ�㣨����㣩
}


/*
 *��ESP8266 uart0���յ����ݣ����յ�һ��������slip���󣬽����ݰ���������ӿڷ���
 */
void ICACHE_FLASH_ATTR
slip_input(uint8 c){

	uint8 i,j;
	switch( receive_state ){
	case SLIP_RECV_NORMAL:
		switch(c){
			case SLIP_END:   //0xC0, slip���ݰ��Ľṹ��0xC0����0xC0
				if( pslip_pack->len >0){   //���յ�һ��slip���ݰ��������TCP���򷢵�����ӿ�

					if( pslip_pack->len>=20 ){     //ip���ݰ�>=20�ֽ�
						if( ( (pslip_pack->buff[0] & 0xf0) == 0x40 ) && (pslip_pack->buff[9]==6) ){    //������ݰ���һ�ֽ�ǰ��λ����4����IPV4���ݰ�������ΪTCPЭ��(IP����9���ֽڣ����������²��ύ����
						slip_output(pslip_pack);

						}
					}
					pslip_pack->len  = 0;     //���������

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
		case SLIP_ESC_END:				//��0xDC��תΪ0xC0
			pslip_pack->buff[ pslip_pack->len++ ]=SLIP_END;
			break;

		case SLIP_ESC_ESC:				//0xDD��תΪ0xDB
			pslip_pack->buff[ pslip_pack->len++ ]=SLIP_ESC;

		}
		receive_state=SLIP_RECV_NORMAL;
	}


	//uart_tx_one_char(UART0,c);
	//uart_tx_one_char(UART0,pslip_pack->buff[pslip_pack->len-1]);

	if( pslip_pack->len >=SLIP_PACK_SIZE){     //���յ����ݴ��ڻ��������������½��ա�
		 pslip_pack->len =0;
		os_printf("slip buffer overflow!!");
	}

}
