/*
**************************************************
*
*
* ͨ��Usart1���ֿ��ļ���д���ⲿFlash
* ������
* 1���ڴ��ڵ�������з���дָ���ʽ���£�{��д�ֿ���ʼ������ַ,�ֿ�������С���ֽڣ�} ��{0002,001234567},
*		// {��} ---Ϊ�ָ�����0002 -- 1-4λ��ʾ�ֿ���ʼ������ַ���̶�4λ���ȣ������ǰ����0��䣬
*		// 0012345 --- 6-14λ���ݱ�ʾ�ֿ�����������ֽ�Ϊ��λ��
*
*
**************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "UsartX.h"
#include "SysTick.h"
#include "Flash_SPI.h"
#include <stdbool.h>

char CK_flag;
char USARTx_RX_CNT;
uint32_t falsh_write_size;
char USARTx_RX_BUF[16];
uint32_t addr;
uint32_t addr_Stop;
uint8_t pBuffer[256];
uint32_t HZK_size;      //�ֿ��������ֽڣ�

int main(void)
{ 
	uint32_t Erase_flag=0;
	uint32_t Erase_adrr=0;
	uint16_t i,j,k;
	bool isErase;
	
	SysTick_Init();
	FLASH_SPI_initialize();
	USARTx_Config();
	NVIC_Configuration();

	printf("ͨ��Usart1���ֿ��ļ���д���ⲿFlash����\r\n");
	printf("��ʮ�����Ʒ�ʽ���Ͳ��������2A 23 00 50 00 01 23 2A---Ϊһ������ָ��\r\n");
	printf("2A 23 Ϊ��β�жϱ�ʶ---��3,4��ʾ��ʼ��ַ--05 4096��\r\n");
	printf("��5,6���ݱ�ʾ����ʼ��ַ��Ҫ�������ٸ��������������д����--0002��\r\n");
	printf("\r\n");
	printf("Usart��ʼ����ɣ�\r\n\r\n");

  while(1)
  {
		printf("�ȴ���д����...\r\n");
		CK_flag=0;   //���ڽ��������Ƿ��ǵ�ַ�ĵı�ʶ�ж�λ---Ϊ0 ��ʾ������յ��ǵ�ַ
    USARTx_RX_CNT=0; //���ڽ��յ���ַ����ʱ�����ã�ÿ�ε����ﴮ�ڽ��ջ��������±�Ϊ0����ʾ���潫���յ�ַ
    falsh_write_size=0;    //���ڽ��յ��ǵ�ַ���ݵĸ��������ı仯ʵ�ڴ��ڽ����жϺ����е��������������ʾ�ٴ���д
    Delay_ms(1000);
		//�������whileѭ������Ŀ���ǵȴ����ڽ��յ�ַָ��---��ʽ���£�{��д�ֿ���ʼ������ַ,�ֿ�������С���ֽڣ�} ��{0002,001234567},
		// {��} ---Ϊ�ָ�����0002 -- 1-4λ��ʾ�ֿ���ʼ������ַ���̶�4λ���ȣ������ǰ����0��䣬
		// 0012345 --- 6-14λ���ݱ�ʾ�ֿ�����������ֽ�Ϊ��λ��
        
		while(!(USARTx_RX_BUF[0]=='{') || !(USARTx_RX_BUF[15]=='}'));     
	
		addr=(USARTx_RX_BUF[1]-48)*1000 + (USARTx_RX_BUF[2]-48)*100 + (USARTx_RX_BUF[3]-48)*10 + USARTx_RX_BUF[4]-48;  //addrΪ���յ�����ʼ��ַ����������Ҳ����˵�ǵڼ�������
    //addr_Stop  = addr*4096;//�ڼ���������Ӧ��������ַ
    HZK_size=(USARTx_RX_BUF[6]-48)*100000000L + (USARTx_RX_BUF[7]-48)*10000000L + (USARTx_RX_BUF[8]-48)*1000000L + (USARTx_RX_BUF[9]-48)*100000L + (USARTx_RX_BUF[10]-48)*10000L +
		       (USARTx_RX_BUF[10]-48)*1000 + (USARTx_RX_BUF[12]-48)*100 + (USARTx_RX_BUF[12]-48)*10 + USARTx_RX_BUF[14]-48 ;
		printf("HZK size:%d\r\n",HZK_size);
		Erase_flag=HZK_size / 4096 ;  //��д�ֿ���Ҫ����������д��ǰ����������в���
		printf("erase sector:%d\r\n",Erase_flag);
		printf("begin addr=%d\r\n",addr);
			printf("erase num=%d\r\n",Erase_flag);
			for(i=0;i<Erase_flag;i++){
				Erase_adrr=addr+i;//������ַ����
				printf("���ڼ��� %d ����...\r\n",Erase_adrr);
				isErase=true;  //�ⲿflash�Ƿ��Ѳ���
				j=0;    
				while(j<16 && isErase ){
					SPI_FLASH_BufferRead(pBuffer, Erase_adrr*4096+j*256, 256);
					k=0;
					while(k<256 && isErase ){
						if(pBuffer[k] != 0xFF){     //flash��ֵ������0xFF�����Ȳ������������д������
							isErase=false;
							printf("���ڲ����� %d ����...\r\n",Erase_adrr);
							SPI_FLASH_SectorErase(4096*Erase_adrr);  //��������
						}
						k++;
					}
					j++;
				}
        Delay_ms(50);
      }
			addr*=4096;
			CK_flag=10; //���ڽ��������Ƿ��ǵ�ַ�ĵı�ʶ�ж�λ ---Ϊ10 ��ʾ������յ�������
			printf("��ѡ��һ���ֿ��ļ������͡�\r\n");
		
			while(falsh_write_size < HZK_size) ;   //�ȴ��ֿ⴫�͡���д���
			printf("�ֿ���д��ɡ�д���ֿ�������%d\r\n\r\n",falsh_write_size);
			i=16;
			SPI_FLASH_BufferRead(pBuffer,addr-i,16);
			printf("��ʾ��д��Flash�е����16���ֽڵ�����\r\n");
			for(i=0;i<16;i++){
				printf("%02x ",pBuffer[i]);
			}
			printf("\r\n\r\n");
			Delay_ms(2000);
  }
}

//--------------�����жϺ���------------
void macUSART_INT_FUN(void)
{        
  uint8_t res;
  if(USART_GetITStatus(macUSARTx, USART_IT_RXNE) != RESET)  //���յ�����  
  {        
        //USART_ClearITPendingBit(macUSARTx, USART_IT_RXNE); 
   res=USART_ReceiveData(macUSARTx);  //��ȡ���յ������� 
   USARTx_RX_BUF[USARTx_RX_CNT++]=res;  //�������ݣ���Ҫ�����ַָ��
	 if(CK_flag==10){  //���ڽ�������
      SPI_FLASH_PageWrite(&res,addr++,1);  //��д���յ������ݵ�FLASH
			falsh_write_size++;  //��д��ַ����---�������н�����д��ɺ���ʾ
   }
   if( USARTx_RX_CNT >= 20)
			USARTx_RX_CNT=0;  
  }
} 

