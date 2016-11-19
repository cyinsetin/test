#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "motor.h"

#include "nrf_gpio.h"
#include "boards.h"


//static uint8_t Second_Hand_Input_direction_flg;						//Input direction = 0:Positive Input,1:Negative Input
//static uint8_t Minute_Hand_Input_direction_flg;						//Input direction = 0:Positive Input,1:Negative Input
//static uint8_t Hour_Hand_Input_direction_flg;							//Input direction = 0:Positive Input,1:Negative Input
//static uint8_t Small_Hand_Top_Input_direction_flg=0;  		//The Current Position of Status Hand
//static uint8_t Small_Hand_Buttom_Input_direction_flg=0;  	//The Current Position of Status Hand
//static uint8_t Small_Hand_Right_Input_direction_flg=0;  	//The Current Position of Status Hand
//static uint8_t Small_Hand_Left_Input_direction_flg=0;  		//The Current Position of Status Hand

//static uint8_t Hour_Hand_Input_PIN_flg=0;
//static uint8_t Minute_Hand_Input_PIN_flg=0;
//static uint8_t Second_Hand_Input_PIN_flg=0;

uint8_t Hour_Hand_Clockwise_Input_PIN_flg=0;
uint8_t Second_Hand_Clockwise_Input_PIN_flg=0;
uint8_t Minute_Hand_Clockwise_Input_PIN_flg=0;
uint8_t Hour_Hand_Counterclockwise_Input_PIN_flg=0;
uint8_t Minute_Hand_Counterclockwise_Input_PIN_flg=0;
uint8_t Second_Hand_Counterclockwise_Input_PIN_flg=0;

//uint8_t Pointer_Sec_Action_flg;
//uint8_t Pointer_Min_Action_flg;
//uint8_t Pointer_Hour_Action_flg;

//uint8_t Pointer_Sec_Zero_flg;
//uint8_t Pointer_Min_Zero_flg;
//uint8_t Pointer_Hour_Zero_flg;

uint8_t Hour_Hand_Counterclockwise_Step_Cnt=0;						//Ê±ÕëÄæÊ±Õë
uint8_t Minute_Hand_Counterclockwise_Step_Cnt=0;
uint8_t Second_Hand_Counterclockwise_Step_Cnt=0;



uint16_t Hour_Hand_Clockwise_Plus_Cnt=0;
uint16_t Minute_Hand_Clockwise_Plus_Cnt=0;
uint16_t Second_Hand_Clockwise_Plus_Cnt=0;

uint16_t Hour_Hand_Counterclockwise_Plus_Cnt=0;
uint16_t Minute_Hand_Counterclockwise_Plus_Cnt=0;
uint16_t Second_Hand_Counterclockwise_Plus_Cnt=0;

void motor_gpio_init(void)
{
  nrf_gpio_cfg_output(pin_motor_hour_p);
	nrf_gpio_cfg_output(pin_motor_hour_n);
  nrf_gpio_cfg_output(pin_motor_min_p);
  nrf_gpio_cfg_output(pin_motor_min_n);
  nrf_gpio_cfg_output(pin_motor_sec_p);
  nrf_gpio_cfg_output(pin_motor_sec_n);
	
  nrf_gpio_pin_clear(pin_motor_hour_p);
  nrf_gpio_pin_clear(pin_motor_hour_n);
  nrf_gpio_pin_clear(pin_motor_min_p);
  nrf_gpio_pin_clear(pin_motor_min_n);
  nrf_gpio_pin_clear(pin_motor_sec_p);
  nrf_gpio_pin_clear(pin_motor_sec_n);
}

void calculate_RTC_Position(uint8_t sys_hour,uint8_t sys_minute,uint8_t sys_second,int16_t *RTC_Hour_Position,int16_t *RTC_Minute_Position,int16_t *RTC_Second_Position)
{
        //uint8_t dif_hour;
//        uint32_t total_sys_hour_second,total_sys_min_second;
        *RTC_Hour_Position =(int16_t)((sys_hour % 12) * 5 * 3 + (sys_minute/4));
	      *RTC_Minute_Position = (int16_t)(sys_minute * 3 + sys_second/20);
				*RTC_Second_Position =(int16_t)(sys_second * 3);
}
/**
*/
void Second_Hand_Clockwise(int16_t *Position,uint8_t *Step_Cnt)
{
 switch(Second_Hand_Clockwise_Plus_Cnt){
	 case 0:case 1:case 52:case 102:
				if(Second_Hand_Clockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_set(pin_motor_sec_p);
						break;
					}
				else
					{
						nrf_gpio_pin_set(pin_motor_sec_n);
						break;
					}
   case 36:case 88:case 138:
				if(Second_Hand_Clockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_clear(pin_motor_sec_p);
						break;
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_sec_n);
						break;
					}
	case 625:
				if(Second_Hand_Clockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_clear(pin_motor_sec_p);
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_sec_n);
					}
        Second_Hand_Clockwise_Plus_Cnt = 0;
        //Second_Hand_Clockwise_Input_PIN_flg = ~Second_Hand_Clockwise_Input_PIN_flg;
															 
				if(Second_Hand_Clockwise_Input_PIN_flg==0)
					{
						Second_Hand_Clockwise_Input_PIN_flg=1;
					}
				else
					{
						Second_Hand_Clockwise_Input_PIN_flg=0;
					}
				//Second_Hand_Clockwise_Step_Cnt+=1;
				//Second_Hand_Position+=1;
				//if(Second_Hand_Position>=180) Second_Hand_Position=0;
				*Step_Cnt+=1;
				*Position+=1;
				if(*Position>=180) *Position=0;
        break;
  default:break;
  }
  Second_Hand_Clockwise_Plus_Cnt++;
}

void Second_Hand_Counterclockwise(int16_t *Position,uint8_t *Step_Cnt)
{
 switch(Second_Hand_Counterclockwise_Plus_Cnt){
	 case 0:case 16:case 308:case 348:case 396:case 424:
				if(Second_Hand_Counterclockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_set(pin_motor_sec_n);
						break;
					}
				else
					{
						nrf_gpio_pin_set(pin_motor_sec_p);
						break;
					}
   case 8:case 288:case 328:case 364:case 404:case 436:
				if(Second_Hand_Counterclockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_clear(pin_motor_sec_n);
						break;
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_sec_p);
						break;
					}	 
   case 56:
				if(Second_Hand_Counterclockwise_Input_PIN_flg==0)
					{
				    nrf_gpio_pin_clear(pin_motor_sec_n);
				    nrf_gpio_pin_set(pin_motor_sec_p);
						break;
					}
				else
					{
						nrf_gpio_pin_set(pin_motor_sec_n);
				    nrf_gpio_pin_clear(pin_motor_sec_p);
						break;
					}	 
   case 144:
				if(Second_Hand_Counterclockwise_Input_PIN_flg==0)
					{
				    nrf_gpio_pin_set(pin_motor_sec_n);
				    nrf_gpio_pin_clear(pin_motor_sec_p);
						break;
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_sec_n);
				    nrf_gpio_pin_set(pin_motor_sec_p);
						break;
					}
   case 1250:
				if(Second_Hand_Counterclockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_clear(pin_motor_sec_n);
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_sec_p);
					}	
        Second_Hand_Counterclockwise_Plus_Cnt = 0;
				if(Second_Hand_Counterclockwise_Input_PIN_flg==0)
					{
						Second_Hand_Counterclockwise_Input_PIN_flg = 1;
						break;
					}
				else
					{
						Second_Hand_Counterclockwise_Input_PIN_flg = 0;
						break;
					}	
				//Second_Hand_Counterclockwise_Step_Cnt+=1;
				//Second_Hand_Position-=1;
				//if(Second_Hand_Position<=0) Second_Hand_Position=0;
				*Step_Cnt+=1;
				*Position-=1;
				if(*Position<=0) *Position=180;
        break;
   default:break;
   }
   Second_Hand_Counterclockwise_Plus_Cnt++;
}
/********************************************************************************************/
void Minute_Hand_Clockwise(int16_t *Position,uint8_t *Step_Cnt)
{
 switch(Minute_Hand_Clockwise_Plus_Cnt){
	 case 0:case 1:case 52:case 102:
				if(Minute_Hand_Clockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_set(pin_motor_min_p);
						break;
					}
				else
					{
						nrf_gpio_pin_set(pin_motor_min_n);
						break;
					}
   case 36:case 88:case 138:
				if(Minute_Hand_Clockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_clear(pin_motor_min_p);
						break;
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_min_n);
						break;
					}
	case 625:
				if(Minute_Hand_Clockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_clear(pin_motor_min_p);
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_min_n);
					}
        Minute_Hand_Clockwise_Plus_Cnt = 0;
        //Minute_Hand_Clockwise_Input_PIN_flg = ~Minute_Hand_Clockwise_Input_PIN_flg;
															 
				if(Minute_Hand_Clockwise_Input_PIN_flg==0)
					{
						Minute_Hand_Clockwise_Input_PIN_flg=1;
					}
				else
					{
						Minute_Hand_Clockwise_Input_PIN_flg=0;
					}
				//Minute_Hand_Clockwise_Step_Cnt+=1;
				//Minute_Hand_Position+=1;
				*Step_Cnt+=1;
				*Position+=1;
				if(*Position>=180) *Position=0;
        break;
  default:break;
  }
  Minute_Hand_Clockwise_Plus_Cnt++;
}


void Minute_Hand_Counterclockwise(int16_t *Position,uint8_t *Step_Cnt)
{
 switch(Minute_Hand_Counterclockwise_Plus_Cnt){
	 case 0:case 16:case 308:case 348:case 396:case 424:
				if(Minute_Hand_Counterclockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_set(pin_motor_min_n);
						break;
					}
				else
					{
						nrf_gpio_pin_set(pin_motor_min_p);
						break;
					}
   case 8:case 288:case 328:case 364:case 404:case 436:
				if(Minute_Hand_Counterclockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_clear(pin_motor_min_n);
						break;
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_min_p);
						break;
					}	 
   case 56:
				if(Minute_Hand_Counterclockwise_Input_PIN_flg==0)
					{
				    nrf_gpio_pin_clear(pin_motor_min_n);
				    nrf_gpio_pin_set(pin_motor_min_p);
						break;
					}
				else
					{
						nrf_gpio_pin_set(pin_motor_min_n);
				    nrf_gpio_pin_clear(pin_motor_min_p);
						break;
					}	 
   case 144:
				if(Minute_Hand_Counterclockwise_Input_PIN_flg==0)
					{
				    nrf_gpio_pin_set(pin_motor_min_n);
				    nrf_gpio_pin_clear(pin_motor_min_p);
						break;
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_min_n);
				    nrf_gpio_pin_set(pin_motor_min_p);
						break;
					}
   case 1250:
				if(Minute_Hand_Counterclockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_clear(pin_motor_min_n);
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_min_p);
					}	
        Minute_Hand_Counterclockwise_Plus_Cnt = 0;
				if(Minute_Hand_Counterclockwise_Input_PIN_flg==0)
					{
						Minute_Hand_Counterclockwise_Input_PIN_flg = 1;
						break;
					}
				else
					{
						Minute_Hand_Counterclockwise_Input_PIN_flg = 0;
						break;
					}	
				//Minute_Hand_Counterclockwise_Step_Cnt+=1;
				//Minute_Hand_Position-=1;
				//if(Minute_Hand_Position<=0) Minute_Hand_Position=0;
				*Step_Cnt+=1;
				*Position-=1;
				if(*Position<=0) *Position=180;					
        break;
   default:break;
   }
   Minute_Hand_Counterclockwise_Plus_Cnt++;
}
/********************************************************************************************/
void Hour_Hand_Clockwise(int16_t *Position,uint8_t *Step_Cnt)
{
 switch(Hour_Hand_Clockwise_Plus_Cnt){
	 case 0:case 52:case 102:
				if(Hour_Hand_Clockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_set(pin_motor_hour_p);
						break;
					}
				else
					{
						nrf_gpio_pin_set(pin_motor_hour_n);
						break;
					}
   case 36:case 88:case 138:
				if(Hour_Hand_Clockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_clear(pin_motor_hour_p);
						break;
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_hour_n);
						break;
					}
	case 625:
				if(Hour_Hand_Clockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_clear(pin_motor_hour_p);
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_hour_n);
					}
        Hour_Hand_Clockwise_Plus_Cnt = 0;
        //Hour_Hand_Clockwise_Input_PIN_flg = ~Hour_Hand_Clockwise_Input_PIN_flg;
															 
				if(Hour_Hand_Clockwise_Input_PIN_flg==0)
					{
						Hour_Hand_Clockwise_Input_PIN_flg=1;
					}
				else
					{
						Hour_Hand_Clockwise_Input_PIN_flg=0;
					}
				*Step_Cnt+=1;
				*Position+=1;
				if(*Position>=180) *Position=0;
				//Hour_Hand_Clockwise_Step_Cnt+=1;
				//Hour_Hand_Position+=1;
				
				//if(Hour_Hand_Position>=180) Hour_Hand_Position=0;
        break;
  default:break;
  }
  Hour_Hand_Clockwise_Plus_Cnt++;
}

void Hour_Hand_Counterclockwise(int16_t *Position,uint8_t *Step_Cnt)
{
 switch(Hour_Hand_Counterclockwise_Plus_Cnt){
	 case 0:case 16:case 308:case 348:case 396:case 424:
				if(Hour_Hand_Counterclockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_set(pin_motor_hour_n);
						break;
					}
				else
					{
						nrf_gpio_pin_set(pin_motor_hour_p);
						break;
					}
   case 8:case 288:case 328:case 364:case 404:case 436:
				if(Hour_Hand_Counterclockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_clear(pin_motor_hour_n);
						break;
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_hour_p);
						break;
					}	 
   case 56:
				if(Hour_Hand_Counterclockwise_Input_PIN_flg==0)
					{
				    nrf_gpio_pin_clear(pin_motor_hour_n);
				    nrf_gpio_pin_set(pin_motor_hour_p);
						break;
					}
				else
					{
						nrf_gpio_pin_set(pin_motor_hour_n);
				    nrf_gpio_pin_clear(pin_motor_hour_p);
						break;
					}	 
   case 144:
				if(Hour_Hand_Counterclockwise_Input_PIN_flg==0)
					{
				    nrf_gpio_pin_set(pin_motor_hour_n);
				    nrf_gpio_pin_clear(pin_motor_hour_p);
						break;
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_hour_n);
				    nrf_gpio_pin_set(pin_motor_hour_p);
						break;
					}
   case 1250:
				if(Hour_Hand_Counterclockwise_Input_PIN_flg==0)
					{
						nrf_gpio_pin_clear(pin_motor_hour_n);
					}
				else
					{
						nrf_gpio_pin_clear(pin_motor_hour_p);
					}	
        Hour_Hand_Counterclockwise_Plus_Cnt = 0;
				if(Hour_Hand_Counterclockwise_Input_PIN_flg==0)
					{
						Hour_Hand_Counterclockwise_Input_PIN_flg = 1;
						break;
					}
				else
					{
						Hour_Hand_Counterclockwise_Input_PIN_flg = 0;
						break;
					}
				*Step_Cnt+=1;
				*Position-=1;
				if(*Position<=0) *Position=180;							
				//Hour_Hand_Counterclockwise_Step_Cnt+=1;
				//Hour_Hand_Position-=1;
				//if(Hour_Hand_Position<=0) Hour_Hand_Position=0;
        break;
   default:break;
   }
   Hour_Hand_Counterclockwise_Plus_Cnt++;
}






