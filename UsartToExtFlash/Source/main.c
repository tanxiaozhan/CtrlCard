/*
**************************************************
*
*
* 通过Usart1把字库文件烧写到外部Flash
* 操作：
* 1、在串口调试软件中发送写指令，格式如下：{烧写字库起始扇区地址,字库容量大小（字节）} ，{0002,001234567},
*		// {和} ---为分隔符，0002 -- 1-4位表示字库起始扇区地址，固定4位长度，不足的前面用0填充，
*		// 0012345 --- 6-14位数据表示字库的容量，以字节为单位。
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
uint32_t HZK_size;      //字库容量（字节）

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

	printf("通过Usart1把字库文件烧写到外部Flash程序\r\n");
	printf("以十六进制方式发送操作命令。如2A 23 00 50 00 01 23 2A---为一个完整指令\r\n");
	printf("2A 23 为首尾判断标识---第3,4表示起始地址--05 4096，\r\n");
	printf("第5,6数据表示从起始地址需要擦除多少个扇区来方便后面写数据--0002个\r\n");
	printf("\r\n");
	printf("Usart初始化完成！\r\n\r\n");

  while(1)
  {
		printf("等待烧写命令...\r\n");
		CK_flag=0;   //串口接收数据是否是地址的的标识判断位---为0 表示后面接收的是地址
    USARTx_RX_CNT=0; //串口接收到地址数据时有作用，每次到这里串口接收缓存数组下表为0，表示后面将接收地址
    falsh_write_size=0;    //串口接收到非地址数据的个数，它的变化实在串口接收中断函数中递增，这里清零表示再次烧写
    Delay_ms(1000);
		//下面这个while循环，其目的是等待串口接收地址指令---格式如下：{烧写字库起始扇区地址,字库容量大小（字节）} ，{0002,001234567},
		// {和} ---为分隔符，0002 -- 1-4位表示字库起始扇区地址，固定4位长度，不足的前面用0填充，
		// 0012345 --- 6-14位数据表示字库的容量，以字节为单位。
        
		while(!(USARTx_RX_BUF[0]=='{') || !(USARTx_RX_BUF[15]=='}'));     
	
		addr=(USARTx_RX_BUF[1]-48)*1000 + (USARTx_RX_BUF[2]-48)*100 + (USARTx_RX_BUF[3]-48)*10 + USARTx_RX_BUF[4]-48;  //addr为接收到的起始地址所在扇区，也就是说是第几个扇区
    //addr_Stop  = addr*4096;//第几个扇区对应的扇区地址
    HZK_size=(USARTx_RX_BUF[6]-48)*100000000L + (USARTx_RX_BUF[7]-48)*10000000L + (USARTx_RX_BUF[8]-48)*1000000L + (USARTx_RX_BUF[9]-48)*100000L + (USARTx_RX_BUF[10]-48)*10000L +
		       (USARTx_RX_BUF[10]-48)*1000 + (USARTx_RX_BUF[12]-48)*100 + (USARTx_RX_BUF[12]-48)*10 + USARTx_RX_BUF[14]-48 ;
		printf("HZK size:%d\r\n",HZK_size);
		Erase_flag=HZK_size / 4096 ;  //烧写字库需要的扇区数，写入前根据情况进行擦除
		printf("erase sector:%d\r\n",Erase_flag);
		printf("begin addr=%d\r\n",addr);
			printf("erase num=%d\r\n",Erase_flag);
			for(i=0;i<Erase_flag;i++){
				Erase_adrr=addr+i;//扇区地址自增
				printf("正在检查第 %d 扇区...\r\n",Erase_adrr);
				isErase=true;  //外部flash是否已擦除
				j=0;    
				while(j<16 && isErase ){
					SPI_FLASH_BufferRead(pBuffer, Erase_adrr*4096+j*256, 256);
					k=0;
					while(k<256 && isErase ){
						if(pBuffer[k] != 0xFF){     //flash的值不等于0xFF，需先擦除，后面才能写入数据
							isErase=false;
							printf("正在擦除第 %d 扇区...\r\n",Erase_adrr);
							SPI_FLASH_SectorErase(4096*Erase_adrr);  //擦出扇区
						}
						k++;
					}
					j++;
				}
        Delay_ms(50);
      }
			addr*=4096;
			CK_flag=10; //串口接收数据是否是地址的的标识判断位 ---为10 表示后面接收的是数据
			printf("请选择一个字库文件并发送。\r\n");
		
			while(falsh_write_size < HZK_size) ;   //等待字库传送、烧写完成
			printf("字库烧写完成。写入字库容量：%d\r\n\r\n",falsh_write_size);
			i=16;
			SPI_FLASH_BufferRead(pBuffer,addr-i,16);
			printf("显示烧写到Flash中的最后16个字节的数据\r\n");
			for(i=0;i<16;i++){
				printf("%02x ",pBuffer[i]);
			}
			printf("\r\n\r\n");
			Delay_ms(2000);
  }
}

//--------------串口中断函数------------
void macUSART_INT_FUN(void)
{        
  uint8_t res;
  if(USART_GetITStatus(macUSARTx, USART_IT_RXNE) != RESET)  //接收到数据  
  {        
        //USART_ClearITPendingBit(macUSARTx, USART_IT_RXNE); 
   res=USART_ReceiveData(macUSARTx);  //读取接收到的数据 
   USARTx_RX_BUF[USARTx_RX_CNT++]=res;  //接收数据，主要保存地址指令
	 if(CK_flag==10){  //串口接收数据
      SPI_FLASH_PageWrite(&res,addr++,1);  //烧写接收到的数据到FLASH
			falsh_write_size++;  //烧写地址自增---主函数中将在烧写完成后显示
   }
   if( USARTx_RX_CNT >= 20)
			USARTx_RX_CNT=0;  
  }
} 

