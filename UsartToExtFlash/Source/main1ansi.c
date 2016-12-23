/*
**************************************************
*
*
* 通过Usart1把字库文件烧写到外部Flash
* 操作：
* 1、在串口调试软件中发送写指令，以16进制方式发送。比如2A 23 00 50 00 01 23 2A---为一个完整指令
* 2A 23 为首尾判断标识---第3,4表示起始地址--0x05*4096，
* 第5,6数据表示从起始地址需要擦除多少个扇区来方便后面写数据--0x0002个
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
        RS232_RX_CNT=0; //串口接收到地址数据时有作用，每次到这里串口接收缓存数组下表为0，表示后面将接收地址
        addr_i=0;    //串口接收到非地址数据的个数，它的变化实在串口接收中断函数中递增，这里清零表示再次烧写
        Delay_ms(1000);
        //GUI_SetFont(&GUI_Font32B_1);
        //GUI_SetColor(GUI_YELLOW);
        //GUI_DispStringAtCEOL("Wait Send Addr",50,10);
        //GUI_DispStringAt("Waits Addr",100,160);
        //GUI_DispHexAt(addr_Stop,200,50,6);//烧录完成后根据真实烧录的字节数计算出终止地址显示出来
//下面这个while循环，其目的是等待串口接收地址指令---指令需要16进制发送---比如2A 23 00 50 00 01 23 2A---为一个完整指令
//2A 23 都为判断标识---第3,4表示起始地址--0x05*4096---第5,6数据表示从起始地址需要擦除多少个扇区来方便后面写数据--0x0002个
        
printf("1¤×÷?￡ê?after Delay");
		while(!(RS232_RX_BUF[0]==0x2A)||!(RS232_RX_BUF[1]==0x23)||!(RS232_RX_BUF[6]==0x23)||!(RS232_RX_BUF[7]==0x2A));
	
        addr=RS232_RX_BUF[2]*256+RS232_RX_BUF[3];  //addr为接收到的起始地址所在扇区，也就是说是第几个扇区
        addr_Stop  = addr*4096;//第几个扇区对应的扇区地址
        Erase_adrr = addr;   //起始地址也就是要擦除的扇区是第几个
        Erase_flag=RS232_RX_BUF[4]*256+RS232_RX_BUF[5];//要擦出的连续扇区个数

        //GUI_SetFont(&GUI_Font32B_1);
        //GUI_SetColor(GUI_CYAN);  
        //GUI_DispHexAt(addr*4096,100,50,6);//显示起始地址
        //GUI_DispDecAt(Erase_flag,100,85,4); 
//显示要擦出的扇区个数
        //GUI_DispStringAt("Start Erase",100,160);
		
		for(i=0;i<Erase_flag;i++)
        {
            Erase_adrr=addr+i;//扇区地址自增
			
			printf("正在检查第 %d 扇区...\r\n",Erase_adrr);
			isErase=true;  //外部flash是否已擦除
			j=0;    
			while(j<16 && isErase ){
					SPI_FLASH_BufferRead(pBuffer, Erase_adrr+j*256, 256);
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
				
 
			//GUI_SetColor(GUI_WHITE);
            //GUI_DispDecAt(i+1,255,160,4);// 显示实时擦除扇区状态---第几个
            Delay_ms(50);
        }
        //GUI_SetColor(GUI_CYAN); 
        //GUI_DispStringAt("Finis Erase",100,160);
        //GUI_SetFont(&GUI_FontHZ16);
        //GUI_SetColor(GUI_DARKMAGENTA);
        //GUI_DispStringAt("啊波次的乐我许与在 #*aA/字库",100,210);//这里是用来测试前面的擦出是否完成---比如前面擦出了ASCII码的字库地址，其中的FLASH就不会显示
        CK_flag=10; //串口接收数据是否是地址的的标识判断位 ---为10 表示后面接收的是数据
		
				printf("请选择一个字库文件并发送。\r\n");
		
        //GUI_SetFont(&GUI_Font32B_1);
        //GUI_SetColor(GUI_GREEN);
        //GUI_DispStringAtCEOL("Wait Send Data",50,10);

        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0);   //这里等于是一个开关---用来等待串口传输完成，可以再串口上看到是否传输完成
    
        //GUI_SetColor(GUI_WHITE);
        //GUI_DispDecAt(addr_i,100,120,7);
        //GUI_SetFont(&GUI_FontHZ16);
        //GUI_SetColor(GUI_BLUE);
        //GUI_DispStringAt("啊波次的乐我许与在 #*aA/字库",100,210);//这里是用来测试前面的烧写是否完成---比如前面擦出了ASCII码的字库地址这里又重新烧录了ASCII码，其中的FLASH就会再次显示
        Delay_ms(100);

  }
}

//--------------串口中断函数------------
void USART1_IRQHandler(void)
{        
  uint8_t res;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收到数据  
  {        
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); 
        res=USART_ReceiveData(USART1);  //读取接收到的数据 
        RS232_RX_BUF[RS232_RX_CNT++]=res;  //接收数据，主要保存地址指令
		if(CK_flag==10)  //串口接收数据
        {  
                addr_i++;  //烧写地址自增---主函数中将在烧写完成后显示
                SPI_FLASH_BufferWrite(&res,addr++,1);  //烧写接收到的数据到FLASH
        }
        if( RS232_RX_CNT==12)//无影响
                RS232_RX_CNT=0;  
  }
} 
