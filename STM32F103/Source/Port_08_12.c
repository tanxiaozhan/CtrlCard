/*
*************************************************************
*
* 控制卡输出接口（08、12）管脚设置
* 创建日期：2016.09.11
*
*************************************************************
*/


#include "Port_08_12.h"


/*
	*************************************************
	*
  * @brief  初始化 08、12 端口共用的GPIO引脚
	*          即A、B、C、D、STB、CLK、EN 引脚
  * @param  无
  * @retval 无
	*
	*************************************************
*/
void Port_08_12_Comm_GPIO_Config ( void )
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置引脚速率为50MHz    

	//配置STB引脚
	PORT_08_12_STB_GPIO_APBxClock_FUN ( PORT_08_12_STB_GPIO_CLK, ENABLE );  //开启GPIOB和GPIOF的外设时钟
	GPIO_InitStructure.GPIO_Pin = PORT_08_12_STB_GPIO_PIN;	                //选择要控制的GPIOB引脚	
	GPIO_Init ( PORT_08_12_STB_GPIO_PORT, & GPIO_InitStructure );	        //调用库函数，初始化GPIO
	
	// 配置 SCK 引脚
	PORT_08_12_SCK_GPIO_APBxClock_FUN ( PORT_08_12_SCK_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_12_SCK_GPIO_PIN;	
	GPIO_Init ( PORT_08_12_SCK_GPIO_PORT, & GPIO_InitStructure );

	// 配置 EN 引脚
	PORT_08_12_EN_GPIO_APBxClock_FUN ( PORT_08_12_EN_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_12_EN_GPIO_PIN;	
	GPIO_Init ( PORT_08_12_EN_GPIO_PORT, & GPIO_InitStructure );

	// 配置 A 引脚
	PORT_08_12_A_GPIO_APBxClock_FUN ( PORT_08_12_A_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_12_A_GPIO_PIN;	
	GPIO_Init ( PORT_08_12_A_GPIO_PORT, & GPIO_InitStructure );
	
	// 配置 B 引脚
	PORT_08_12_B_GPIO_APBxClock_FUN ( PORT_08_12_B_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_12_B_GPIO_PIN;	
	GPIO_Init ( PORT_08_12_B_GPIO_PORT, & GPIO_InitStructure );
	
	// 配置 C 引脚
	PORT_08_12_C_GPIO_APBxClock_FUN ( PORT_08_12_C_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_12_C_GPIO_PIN;	
	GPIO_Init ( PORT_08_12_C_GPIO_PORT, & GPIO_InitStructure );
	
	// 配置 D 引脚
	PORT_08_12_D_GPIO_APBxClock_FUN ( PORT_08_12_D_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_12_D_GPIO_PIN;	
	GPIO_Init ( PORT_08_12_D_GPIO_PORT, & GPIO_InitStructure );
}

/*
	*************************************************
	*
  * @brief  初始化 08-1 端口用到的GPIO引脚
  * @param  无
  * @retval 无
	*************************************************
*/
void Port_08_1_GPIO_Config ( void )
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置引脚速率为50MHz    

	// 配置 R1 引脚
	PORT_08_1_R1_GPIO_APBxClock_FUN ( PORT_08_1_R1_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_1_R1_GPIO_PIN;	
	GPIO_Init ( PORT_08_1_R1_GPIO_PORT, & GPIO_InitStructure );
	
	// 配置 R2 引脚
	PORT_08_1_R2_GPIO_APBxClock_FUN ( PORT_08_1_R2_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_1_R2_GPIO_PIN;	
	GPIO_Init ( PORT_08_1_R2_GPIO_PORT, & GPIO_InitStructure );
	
	// 配置 G1 引脚
	PORT_08_1_G1_GPIO_APBxClock_FUN ( PORT_08_1_G1_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_1_G1_GPIO_PIN;	
	GPIO_Init ( PORT_08_1_G1_GPIO_PORT, & GPIO_InitStructure );
	
	// 配置 G2 引脚
	PORT_08_1_G2_GPIO_APBxClock_FUN ( PORT_08_1_G2_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_1_G2_GPIO_PIN;	
	GPIO_Init ( PORT_08_1_G2_GPIO_PORT, & GPIO_InitStructure );
}

/*
	*************************************************
	*
  * @brief  初始化 08-2 端口用到的GPIO引脚
  * @param  无
  * @retval 无
	*************************************************
*/
void Port_08_2_GPIO_Config ( void )
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置引脚速率为50MHz    

	// 配置 08-2接口GPIO引脚
	// 配置 R1 引脚
	PORT_08_2_R1_GPIO_APBxClock_FUN ( PORT_08_2_R1_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_2_R1_GPIO_PIN;	
	GPIO_Init ( PORT_08_2_R1_GPIO_PORT, & GPIO_InitStructure );
	
	// 配置 R2 引脚
	PORT_08_2_R2_GPIO_APBxClock_FUN ( PORT_08_2_R2_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_2_R2_GPIO_PIN;	
	GPIO_Init ( PORT_08_2_R2_GPIO_PORT, & GPIO_InitStructure );
	
	// 配置 G1 引脚
	PORT_08_2_G1_GPIO_APBxClock_FUN ( PORT_08_2_G1_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_2_G1_GPIO_PIN;	
	GPIO_Init ( PORT_08_2_G1_GPIO_PORT, & GPIO_InitStructure );
	
	// 配置 G2 引脚
	PORT_08_2_G2_GPIO_APBxClock_FUN ( PORT_08_2_G2_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_08_2_G2_GPIO_PIN;	
	GPIO_Init ( PORT_08_2_G2_GPIO_PORT, & GPIO_InitStructure );
}
	
/*
	*************************************************
	*
  * @brief  初始化 12-1 端口用到的GPIO引脚
  * @param  无
  * @retval 无
	*************************************************
*/
void Port_12_1_GPIO_Config ( void )
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置引脚速率为50MHz    

	// 配置 12-1接口GPIO引脚
	// 配置 R 引脚
	PORT_12_1_R_GPIO_APBxClock_FUN ( PORT_12_1_R_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_12_1_R_GPIO_PIN;	
	GPIO_Init ( PORT_12_1_R_GPIO_PORT, & GPIO_InitStructure );
	
	// 配置 G 引脚
	PORT_12_1_G_GPIO_APBxClock_FUN ( PORT_12_1_G_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_12_1_G_GPIO_PIN;	
	GPIO_Init ( PORT_12_1_G_GPIO_PORT, & GPIO_InitStructure );
}
	
/*
	*************************************************
	*
  * @brief  初始化 12-2 端口用到的GPIO引脚
  * @param  无
  * @retval 无
	*************************************************
*/
void Port_12_2_GPIO_Config ( void )
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置引脚速率为50MHz    

	// 配置 12-2 接口GPIO引脚
	// 配置 R 引脚
	PORT_12_2_R_GPIO_APBxClock_FUN ( PORT_12_2_R_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_12_2_R_GPIO_PIN;	
	GPIO_Init ( PORT_12_2_R_GPIO_PORT, & GPIO_InitStructure );
	
	// 配置 G1 引脚
	PORT_12_2_G_GPIO_APBxClock_FUN ( PORT_12_2_G_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_12_2_G_GPIO_PIN;	
	GPIO_Init ( PORT_12_2_G_GPIO_PORT, & GPIO_InitStructure );
}
	
/*
	*************************************************
	*
  * @brief  初始化 12-3 端口用到的GPIO引脚
  * @param  无
  * @retval 无
	*************************************************
*/
void Port_12_3_GPIO_Config ( void )
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置引脚速率为50MHz    

	// 配置 12-3接口GPIO引脚
	// 配置 R 引脚
	PORT_12_3_R_GPIO_APBxClock_FUN ( PORT_12_3_R_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_12_3_R_GPIO_PIN;	
	GPIO_Init ( PORT_12_3_R_GPIO_PORT, & GPIO_InitStructure );
	
	// 配置 G 引脚
	PORT_12_3_G_GPIO_APBxClock_FUN ( PORT_12_3_G_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_12_3_G_GPIO_PIN;	
	GPIO_Init ( PORT_12_3_G_GPIO_PORT, & GPIO_InitStructure );
}
	
/*
	*************************************************
	*
  * @brief  初始化 12-4 端口用到的GPIO引脚
  * @param  无
  * @retval 无
	*************************************************
*/
void Port_12_4_GPIO_Config ( void )
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置引脚速率为50MHz    

	// 配置 12-4 接口GPIO引脚
	// 配置 R1 引脚
	PORT_12_4_R_GPIO_APBxClock_FUN ( PORT_12_4_R_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_12_4_R_GPIO_PIN;	
	GPIO_Init ( PORT_12_4_R_GPIO_PORT, & GPIO_InitStructure );
	
	// 配置 G 引脚
	PORT_12_4_G_GPIO_APBxClock_FUN ( PORT_12_4_G_GPIO_CLK, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = PORT_12_4_G_GPIO_PIN;	
	GPIO_Init ( PORT_12_4_G_GPIO_PORT, & GPIO_InitStructure );
}
	

/*
	*************************************************
	*
  * @brief  初始化控制卡08、12端口用到的GPIO引脚
  * @param  无
  * @retval 无
	*************************************************
*/
void Port_08_12_GPIO_Config(void){
	
	Port_08_12_Comm_GPIO_Config();

	GPIO_SetBits(PORT_08_12_EN_GPIO_PORT, PORT_08_12_EN_GPIO_PIN);  		// 关闭端口EN管脚

	Port_08_1_GPIO_Config();
	Port_08_2_GPIO_Config();
	Port_12_1_GPIO_Config();
	Port_12_2_GPIO_Config();
	Port_12_3_GPIO_Config();
	Port_12_4_GPIO_Config();

}
