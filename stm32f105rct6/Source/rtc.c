/**
  ******************************************************************************
  * @file    rtc.h
  * @version V1.0.0
  * @date    2016-10-6
  * @brief   stm32F103 实时钟模块
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include <stdio.h>
#include "rtc.h"

#define FEBRUARY		2
#define	STARTOFTIME		1970
#define SECDAY			86400L           /*  一天有多少s */
#define SECYR			(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define	days_in_month(a) 	(month_days[(a) - 1])

static int month_days[12] = {	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*
 * This only works for the Gregorian calendar - i.e. after 1752 (in the UK)
 */
 /*计算星期几*/
void GregorianDay(struct rtc_time * tm)
{
	int leapsToDate;
	int lastYear;
	int day;
	int MonthOffset[] = { 0,31,59,90,120,151,181,212,243,273,304,334 };

	lastYear=tm->rt_year-1;

	/*计算从公元元年到计数的前一年之中一共经历了多少个闰年*/
	leapsToDate = lastYear/4 - lastYear/100 + lastYear/400;      

     /*如若计数的这一年为闰年，且计数的月份在2月之后，则日数加1，否则不加1*/
	if((tm->rt_year%4==0) &&
	   ((tm->rt_year%100!=0) || (tm->rt_year%400==0)) &&
	   (tm->rt_mon>2)) {
		/*
		 * We are past Feb. 29 in a leap year
		 */
		day=1;
	} else {
		day=0;
	}

	day += lastYear*365 + leapsToDate + MonthOffset[tm->rt_mon-1] + tm->rt_date; /*计算从公元元年元旦到计数日期一共有多少天*/

	tm->rt_day=day%7;
}

/* Converts Gregorian date to seconds since 1970-01-01 00:00:00.
 * Assumes input in normal date format, i.e. 1980-12-31 23:59:59
 * => year=1980, mon=12, day=31, hour=23, min=59, sec=59.
 *
 * [For the Julian calendar (which was used in Russia before 1917,
 * Britain & colonies before 1752, anywhere else before 1582,
 * and is still in use by some communities) leave out the
 * -year/100+year/400 terms, and add 10.]
 *
 * This algorithm was first published by Gauss (I think).
 *
 * WARNING: this function will overflow on 2106-02-07 06:28:16 on
 * machines were long is 32-bit! (However, as time_t is signed, we
 * will already get problems at other places on 2038-01-19 03:14:08)

 *ADD by fire：本函数在工程中的输入参数为北京时间，
 所以在转换成时间戳时最后要从北京时间转换为标准时间的时间戳
 */
u32 mktimev(struct rtc_time *tm)
{
	if (0 >= (int) (tm->rt_mon -= 2)) {	/* 1..12 -> 11,12,1..10 */
		tm->rt_mon += 12;		/* Puts Feb last since it has leap day */
		tm->rt_year -= 1;
	}

	return (((
		(u32) (tm->rt_year/4 - tm->rt_year/100 + tm->rt_year/400 + 367*tm->rt_mon/12 + tm->rt_date) +
			tm->rt_year*365 - 719499
	    )*24 + tm->rt_hour /* now have hours */
	  )*60 + tm->rt_min /* now have minutes */
	)*60 + tm->rt_sec-8*60*60; /* finally seconds */
	/*Add by fire: -8*60*60 把输入的北京时间转换为标准时间，
	再写入计时器中，确保计时器的数据为标准的UNIX时间戳*/ 
}

void to_tm(u32 tim, struct rtc_time * tm)
{
	register u32    i;
	register long   hms, day;

	day = tim / SECDAY;			/* 有多少天 */
	hms = tim % SECDAY;			/* 今天的时间，单位s */

	/* Hours, minutes, seconds are easy */
	tm->rt_hour = hms / 3600;
	tm->rt_min = (hms % 3600) / 60;
	tm->rt_sec = (hms % 3600) % 60;

	/* Number of years in days */ /*算出当前年份，起始的计数年份为1970年*/
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	tm->rt_year = i;

	/* Number of months in days left */ /*计算当前的月份*/
	if (leapyear(tm->rt_year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	tm->rt_mon = i;

	/* Days are what is left over (+1) from all that. *//*计算当前日期*/
	tm->rt_date = day + 1;

	/*
	 * Determine the day of week 计算星期几，0-6，0-星期天
	 */
	GregorianDay(tm);
}

/*
 * 函数名：Time_Adjust
 * 描述  ：时间校正
 * 输入  ：用于读取RTC时间的结构体指针
 * 输出  ：无
 * 调用  ：外部调用
 */
void Time_Adjust(struct rtc_time *tm)
{

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);

	/* 修改当前RTC计数寄存器内容 */
  RTC_SetCounter(mktimev(tm));
	
  /* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

/*
 * 函数名：RTC_Configuration
 * 描述  ：配置RTC
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void RTC_Configuration(void)
{
	/* 第一步：通过设置寄存器RCC_APB1ENR的PWREN和BKPEN位来打开电源和后备接口的时钟调用库函数 */
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* 第二步：电源控制寄存器(PWR_CR)的DBP位来使能对后备寄存器和RTC的访问调用库函数 */
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	
	/* 第三步：初始化复位BKP寄存器 */
	/* Reset Backup Domain */
	BKP_DeInit();
	
	/* 第四步：设置RTCCLK */
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* 第五步：将RTC输入时钟选择为LSE时钟输入并使能RTC，等待RTC和APB时钟同步 */
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){
	}
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC registers synchronization 
	 * 因为RTC时钟是低速的，内环时钟是高速的，所以要同步
	 */
	RTC_WaitForSynchro();
	
	/* Wait until last write operation on RTC registers has finished */
	//RTC_WaitForLastTask();
	
	/* Enable the RTC Second */
	//RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

/*
 * 函数名：RTC_Check
 * 描述  ：启动时进行RTC检测
 * 输入  ：tm，系统时间结构指针
 * 输出  ：无
 * 调用  ：外部调用
 */
void RTC_check(void){

	/*在启动时检查备份寄存器BKP_DR1，如果内容不是0xD3EA（“雨”的国标码）
	  则需重新配置实时钟*/
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xD3EA){
		/* 实时钟配置 */
		RTC_Configuration();
		BKP_WriteBackupRegister(BKP_DR1, 0xD3EA);
	}
	else{    /*启动无需设置新时钟*/
		/*检查是否掉电重启*/
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET){
		  //printf("\r\n\r\n 掉电重启....");
		}
		/*检查是否Reset复位*/
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET){
			//printf("\r\n\r\n 系统复位....");
		}
	}
	
	/* Clear reset flags */
	RCC_ClearFlag();
}
