/**
  ******************************************************************************
  * @file    rtc.h
  * @version V1.0.0
  * @date    2016-10-6
  * @brief   stm32F103 实时钟san模块头文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "stm32f10x.h"

/*系统时间*/
struct rtc_time {
	int rt_sec;
	int rt_min;
	int rt_hour;
	int rt_date;
	int rt_mon;
	int rt_year;
	int rt_day;
};
    
void GregorianDay(struct rtc_time * tm);
uint32_t mktimev(struct rtc_time *tm);
void to_tm(uint32_t tim, struct rtc_time * tm);
void Time_Adjust(struct rtc_time *tm);
void RTC_Configuration(void);
void RTC_check(void);
