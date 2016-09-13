/*
**************************************
* 
* øÿ÷∆ø®øÿ÷∆–≈∫≈÷˜≥Ã–
*  
* ¥¥Ω®»’∆⁄£∫2016.09.12 
*
*************************************
*/

#include "LED_Display.h"
#include "Port_08_12.h"

static void LED_Display(void const *arg);
osThreadDef(LED_Display, osPriorityNormal, 1, 0);

/*----------------------------------------------------------------------------
  œﬂ≥Ã'LED_Dispaly': «˝∂ØLEDœ‘ æ∆¡œ‘ æ–≈œ¢
 *---------------------------------------------------------------------------*/
static void LED_Display (void const *arg) {
		uint8_t row=16;  
		uint8_t col=64;
		uint8_t i,j;
		for(i=1;i<=row;i++){
			EN(ON);
			A( i & 0x01 );B( i & 0x02 );C( i & 0x04 );D( i & 0x08 );
			CLK(OFF);
			//PORT_08_1_R1();
			
			
		}
	
	
	
}

void output_ABCD(uint8_t data){
	
	
}



/*
	********************************************************
	*
	*  ¥¥Ω®LEDœ‘ æ∆¡øÿ÷∆–≈∫≈œﬂ≥Ã£¨øÿ÷∆ø®øÿ÷∆–≈∫≈ø™ º ‰≥ˆ
	*
	*
	********************************************************
*/

void LED_Display_Start(void){
	osThreadCreate (osThread(LED_Display), NULL);
}

