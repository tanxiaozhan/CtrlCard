/**
  ******************************************************************************
  * @file    rtc.h
  * @author  txz
  * @version V1.0.0
  * @date    2016-10-6
  * @brief   stm32F103 实时钟san模块头文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */


#include "stm32f10x_rtc.h"
struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
};
    
void GregorianDay(struct rtc_time * tm);
uint32_t mktimev(struct rtc_time *tm);
void to_tm(uint32_t tim, struct rtc_time * tm);
void Time_Adjust(struct rtc_time *tm);
