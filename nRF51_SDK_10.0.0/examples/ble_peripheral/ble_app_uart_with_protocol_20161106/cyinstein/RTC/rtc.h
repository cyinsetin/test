#ifndef __RTC_H
#define __RTC_H	    
//Mini STM32������
//RTCʵʱʱ�� ��������			 
//����ԭ��@ALIENTEK
//2010/6/6
#include <stdint.h>
//#define Enable_Hand_Ctrl() nrf_drv_timer_enable(&Hand_Control);
//#define Disable_Hand_Ctrl nrf_drv_timer_disable(&Hand_Control);
//ʱ��ṹ��
extern uint32_t RTC_CNT;
extern uint32_t RTC_TIME_BASE;
typedef struct 
{
	volatile uint8_t hour;
	volatile uint8_t min;
	volatile uint8_t sec;			
	//������������
	volatile uint16_t w_year;
	volatile uint8_t  w_month;
	volatile uint8_t  w_date;
	volatile uint8_t  week;		 
}_calendar_obj;					 



extern _calendar_obj calendar;	//�����ṹ��

extern uint8_t const mon_table[12];	//�·��������ݱ�
void Disp_Time(uint8_t x,uint8_t y,uint8_t size);//���ƶ�λ�ÿ�ʼ��ʾʱ��
void Disp_Week(uint8_t x,uint8_t y,uint8_t size,uint8_t lang);//��ָ��λ����ʾ����
//uint8_t RTC_Init(void);        //��ʼ��RTC,����0,ʧ��;1,�ɹ�;
uint8_t Is_Leap_Year(uint16_t year);//ƽ��,�����ж�
//uint8_t RTC_Get(void);         //����ʱ��   
//uint8_t RTC_Get(uint32_t RTC_Count);
void RTC_Get(uint32_t RTC_Count);
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day);
uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);//����ʱ��			 
//uint32_t RTC_GetCounter(void);

void RTC_SetCounter(uint32_t CounterValue);
//void calculate_RTC_Position(uint16_t sys_hour, uint16_t sys_minute, uint16_t sys_second, int16_t * Hour_Position, int16_t* Min_Position, int16_t* Second_Position);
void rtc_config(void);
#endif

