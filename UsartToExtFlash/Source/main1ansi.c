/*
**************************************************
*
*
* ͨ��Usart1���ֿ��ļ���д���ⲿFlash
* ������
* 1���ڴ��ڵ�������з���дָ���16���Ʒ�ʽ���͡�����2A 23 00 50 00 01 23 2A---Ϊһ������ָ��
* 2A 23 Ϊ��β�жϱ�ʶ---��3,4��ʾ��ʼ��ַ--0x05*4096��
* ��5,6���ݱ�ʾ����ʼ��ַ��Ҫ�������ٸ��������������д����--0x0002��
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
char RS232_RX_CNT;
uint16_t addr_i;
char RS232_RX_BUF[12];
uint16_t addr;
uint32_t addr_Stop;
uint8_t pBuffer[256];

int main(void)
{ 
	uint16_t Erase_flag=0;
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
        RS232_RX_CNT=0; //���ڽ��յ���ַ����ʱ�����ã�ÿ�ε����ﴮ�ڽ��ջ��������±�Ϊ0����ʾ���潫���յ�ַ
        addr_i=0;    //���ڽ��յ��ǵ�ַ���ݵĸ��������ı仯ʵ�ڴ��ڽ����жϺ����е��������������ʾ�ٴ���д
        Delay_ms(1000);
        //GUI_SetFont(&GUI_Font32B_1);
        //GUI_SetColor(GUI_YELLOW);
        //GUI_DispStringAtCEOL("Wait Send Addr",50,10);
        //GUI_DispStringAt("Waits Addr",100,160);
        //GUI_DispHexAt(addr_Stop,200,50,6);//��¼��ɺ������ʵ��¼���ֽ����������ֹ��ַ��ʾ����
//�������whileѭ������Ŀ���ǵȴ����ڽ��յ�ַָ��---ָ����Ҫ16���Ʒ���---����2A 23 00 50 00 01 23 2A---Ϊһ������ָ��
//2A 23 ��Ϊ�жϱ�ʶ---��3,4��ʾ��ʼ��ַ--0x05*4096---��5,6���ݱ�ʾ����ʼ��ַ��Ҫ�������ٸ��������������д����--0x0002��
        
printf("1�����?�꨺?after Delay");
		while(!(RS232_RX_BUF[0]==0x2A)||!(RS232_RX_BUF[1]==0x23)||!(RS232_RX_BUF[6]==0x23)||!(RS232_RX_BUF[7]==0x2A));
	
        addr=RS232_RX_BUF[2]*256+RS232_RX_BUF[3];  //addrΪ���յ�����ʼ��ַ����������Ҳ����˵�ǵڼ�������
        addr_Stop  = addr*4096;//�ڼ���������Ӧ��������ַ
        Erase_adrr = addr;   //��ʼ��ַҲ����Ҫ�����������ǵڼ���
        Erase_flag=RS232_RX_BUF[4]*256+RS232_RX_BUF[5];//Ҫ������������������

        //GUI_SetFont(&GUI_Font32B_1);
        //GUI_SetColor(GUI_CYAN);  
        //GUI_DispHexAt(addr*4096,100,50,6);//��ʾ��ʼ��ַ
        //GUI_DispDecAt(Erase_flag,100,85,4); 
//��ʾҪ��������������
        //GUI_DispStringAt("Start Erase",100,160);
		
		for(i=0;i<Erase_flag;i++)
        {
            Erase_adrr=addr+i;//������ַ����
			
			printf("���ڼ��� %d ����...\r\n",Erase_adrr);
			isErase=true;  //�ⲿflash�Ƿ��Ѳ���
			j=0;    
			while(j<16 && isErase ){
					SPI_FLASH_BufferRead(pBuffer, Erase_adrr+j*256, 256);
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
				
 
			//GUI_SetColor(GUI_WHITE);
            //GUI_DispDecAt(i+1,255,160,4);// ��ʾʵʱ��������״̬---�ڼ���
            Delay_ms(50);
        }
        //GUI_SetColor(GUI_CYAN); 
        //GUI_DispStringAt("Finis Erase",100,160);
        //GUI_SetFont(&GUI_FontHZ16);
        //GUI_SetColor(GUI_DARKMAGENTA);
        //GUI_DispStringAt("�����ε����������� #*aA/�ֿ�",100,210);//��������������ǰ��Ĳ����Ƿ����---����ǰ�������ASCII����ֿ��ַ�����е�FLASH�Ͳ�����ʾ
        CK_flag=10; //���ڽ��������Ƿ��ǵ�ַ�ĵı�ʶ�ж�λ ---Ϊ10 ��ʾ������յ�������
		
				printf("��ѡ��һ���ֿ��ļ������͡�\r\n");
		
        //GUI_SetFont(&GUI_Font32B_1);
        //GUI_SetColor(GUI_GREEN);
        //GUI_DispStringAtCEOL("Wait Send Data",50,10);

        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0);   //���������һ������---�����ȴ����ڴ�����ɣ������ٴ����Ͽ����Ƿ������
    
        //GUI_SetColor(GUI_WHITE);
        //GUI_DispDecAt(addr_i,100,120,7);
        //GUI_SetFont(&GUI_FontHZ16);
        //GUI_SetColor(GUI_BLUE);
        //GUI_DispStringAt("�����ε����������� #*aA/�ֿ�",100,210);//��������������ǰ�����д�Ƿ����---����ǰ�������ASCII����ֿ��ַ������������¼��ASCII�룬���е�FLASH�ͻ��ٴ���ʾ
        Delay_ms(100);

  }
}

//--------------�����жϺ���------------
void USART1_IRQHandler(void)
{        
  uint8_t res;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //���յ�����  
  {        
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); 
        res=USART_ReceiveData(USART1);  //��ȡ���յ������� 
        RS232_RX_BUF[RS232_RX_CNT++]=res;  //�������ݣ���Ҫ�����ַָ��
		if(CK_flag==10)  //���ڽ�������
        {  
                addr_i++;  //��д��ַ����---�������н�����д��ɺ���ʾ
                SPI_FLASH_BufferWrite(&res,addr++,1);  //��д���յ������ݵ�FLASH
        }
        if( RS232_RX_CNT==12)//��Ӱ��
                RS232_RX_CNT=0;  
  }
} 
