#include <stdbool.h>
#include <stdint.h>
#include "nrf_drv_config.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_timer.h"

#include "rtc.h"
#include "motor.h"

extern uint32_t RTC0_TIME_BASE;
extern int16_t Minute_Hand_Clockwise_Step_Cnt;
extern int16_t Hour_Hand_Clockwise_Step_Cnt;
extern const nrf_drv_timer_t Hand_Control;  //创建一个定时器0的实例

/** @brief: Function for handling the RTC0 interrupts.
 * Triggered on TICK and COMPARE0 match.
 */
void rtc_handler(nrf_drv_rtc_int_type_t int_type)
{		
    if (int_type == NRF_DRV_RTC_INT_TICK)			
    {
        RTC0_CNT= RTC0_TIME_BASE + (NRF_RTC0->COUNTER/8) ;
			  if(NRF_RTC0->COUNTER % 8 == 0)
				{
					RTC_Get(RTC0_CNT);
					calculate_RTC_Position(calendar.hour,calendar.min,calendar.sec,&RTC_Hour_Hand_Position,&RTC_Minute_Hand_Position,&RTC_Second_Hand_Position);
					if(calendar.sec%20==0)
					{
					 Minute_Hand_Clockwise_Step_Cnt=0;
					 Minute_Hand_One_Step_flg = 1;
					}
					if(calendar.min%4==0 && calendar.sec==0)
					{
						Hour_Hand_Clockwise_Step_Cnt=0;
						Hour_Hand_One_Step_flg = 1;
					}
          if(Minute_Hand_One_Step_flg==1 || Hour_Hand_One_Step_flg==1)
					{
						nrf_drv_timer_enable(&Hand_Control);
					}
					//RTC0_CNT+=1;
				}
    }
}
