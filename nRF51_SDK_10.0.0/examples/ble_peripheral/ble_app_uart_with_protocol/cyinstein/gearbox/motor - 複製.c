#include <stdbool.h>
#include <stdint.h>
#include "motor.h"

#include "nrf_gpio.h"
#include "boards.h"

static uint8_t RTC_Second_Hand_Position=0;  									//The RTC Current Position of Second Hand
static uint8_t RTC_Minute_Hand_Position=0;										//The RTC Current Position of Minute Hand
static uint8_t RTC_Hour_Hand_Position=0;											//The RTC Current Position of Hour Hand
static uint8_t RTC_Small_Hand_Top_Position=0;  								//The RTC Current Position of Small Hand Hand
static uint8_t RTC_Small_Hand_Buttom_Position=0;  						//The RTC Current Position of Small Hand Hand
static uint8_t RTC_Small_Hand_Right_Position=0;  							//The RTC Current Position of Small Hand Hand
static uint8_t RTC_Small_Hand_Left_Position=0;  							//The RTC Current Position of Small Hand Hand


static uint8_t Second_Hand_Position=0;  									//The Current Position of Second Hand
static uint8_t Minute_Hand_Position=0;										//The Current Position of Minute Hand
static uint8_t Hour_Hand_Position=0;											//The Current Position of Hour Hand
static uint8_t Small_Hand_Top_Position=0;  								//The Current Position of Status Hand
static uint8_t Small_Hand_Buttom_Position=0;  						//The Current Position of Status Hand
static uint8_t Small_Hand_Right_Position=0;  							//The Current Position of Status Hand
static uint8_t Small_Hand_Left_Position=0;  							//The Current Position of Status Hand




static uint8_t Second_Hand_Input_direction_flg;						//Input direction = 0:Positive Input,1:Negative Input
static uint8_t Minute_Hand_Input_direction_flg;						//Input direction = 0:Positive Input,1:Negative Input
static uint8_t Hour_Hand_Input_direction_flg;							//Input direction = 0:Positive Input,1:Negative Input
static uint8_t Small_Hand_Top_Input_direction_flg=0;  		//The Current Position of Status Hand
static uint8_t Small_Hand_Buttom_Input_direction_flg=0;  	//The Current Position of Status Hand
static uint8_t Small_Hand_Right_Input_direction_flg=0;  	//The Current Position of Status Hand
static uint8_t Small_Hand_Left_Input_direction_flg=0;  		//The Current Position of Status Hand

static uint8_t Hour_Hand_Input_PIN_flg=0;
static uint8_t Minute_Hand_Input_PIN_flg=0;
static uint8_t Second_Hand_Input_PIN_flg=0;

uint8_t Hour_Hand_Counterclockwise_Input_PIN_flg=0;
uint8_t Minute_Hand_Counterclockwise_Input_PIN_flg=0;
uint8_t Second_Hand_Counterclockwise_Input_PIN_flg=0;

uint8_t Second_Hand_Clockwise_Input_PIN_flg=0;
uint8_t Minute_Hand_Clockwise_Input_PIN_flg=0;
uint8_t Hour_Hand_Clockwise_Input_PIN_flg=0;
//uint8_t Pointer_Sec_Action_flg;
//uint8_t Pointer_Min_Action_flg;
//uint8_t Pointer_Hour_Action_flg;

//uint8_t Pointer_Sec_Zero_flg;
//uint8_t Pointer_Min_Zero_flg;
//uint8_t Pointer_Hour_Zero_flg;

uint8_t Hour_Hand_Clockwise_Step_Cnt=0;										//
uint8_t Minute_Hand_Clockwise_Step_Cnt=0;
uint8_t Second_Hand_Clockwise_Step_Cnt=0;

uint8_t Hour_Hand_Counterclockwise_Step_Cnt=0;						//时针逆时针
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


void Second_Hand_Clockwise(void)
{
                if(Second_Hand_Clockwise_Input_PIN_flg == 0)
                {
                        // 第一个 1s
                        switch(Second_Hand_Clockwise_Plus_Cnt){
                        case 0:case 1:case 52:case 102:
														 nrf_gpio_pin_set(pin_motor_sec_p);
                             break;
                        case 36:case 88:case 138:
														 nrf_gpio_pin_clear(pin_motor_sec_p);
                             break;		
                        //case 52:
												//		 nrf_gpio_pin_set(pin_motor_sec_p);
                        //     break;		
                        //case 88:
												//		 nrf_gpio_pin_clear(pin_motor_sec_p);
                        //     break;												
                        //case 102:
												//		 nrf_gpio_pin_set(pin_motor_sec_p);
                        //     break;
                        //case 138:
												//		 nrf_gpio_pin_clear(pin_motor_sec_p);
                        //     break;			
                        case 625:
														 nrf_gpio_pin_clear(pin_motor_sec_p);
                             Second_Hand_Clockwise_Plus_Cnt = 0;
                             Second_Hand_Clockwise_Input_PIN_flg = 1;
												     Second_Hand_Clockwise_Step_Cnt+=1;

												     Second_Hand_Position+=1;
												     if(Second_Hand_Position>=180) Second_Hand_Position=0;
                             break;
                        default:break;
                        }
                }
                else if(Second_Hand_Clockwise_Input_PIN_flg == 1)
                {
                        // 第二个 1s
                        switch(Second_Hand_Clockwise_Plus_Cnt){
                        case 0: case 1:case 52:case 102:
														 nrf_gpio_pin_set(pin_motor_sec_n);
                             break;
                        case 36:case 88:case 138:
														 nrf_gpio_pin_clear(pin_motor_sec_n);
                             break;		
                        /*
												case 52:
														 nrf_gpio_pin_set(pin_motor_sec_n);
                             break;		
                        case 88:
														 nrf_gpio_pin_clear(pin_motor_sec_n);
                             break;												
                        case 102:
														 nrf_gpio_pin_set(pin_motor_sec_n);
                             break;
                        case 138:
														 nrf_gpio_pin_clear(pin_motor_sec_n);
                             break;
													*/
												case 625:
														 nrf_gpio_pin_clear(pin_motor_sec_n);
                             Second_Hand_Clockwise_Plus_Cnt = 0;
                             Second_Hand_Clockwise_Input_PIN_flg = 0;
												     Second_Hand_Clockwise_Step_Cnt+=1;
												     Second_Hand_Position+=1;
												     if(Second_Hand_Position>=180) Second_Hand_Position=0;
                             break;
                        default:break;
                        }
                }
                Second_Hand_Clockwise_Plus_Cnt++;
        }

void Second_Hand_Counterclockwise(void)
{
                if(Second_Hand_Counterclockwise_Input_PIN_flg == 0)
                {
                  switch(Second_Hand_Counterclockwise_Plus_Cnt){
                        case 0: case 1:case 16:case 308+62:case 344+62:case 392+62:case 432+62:
														 nrf_gpio_pin_set(pin_motor_sec_n);
                             break;
                        case 8:case 288+62:case 324+62:case 360+62:case 420+62:
														 nrf_gpio_pin_clear(pin_motor_sec_n);
                             break;
												/*
                        case 16:
														 nrf_gpio_pin_set(pin_motor_sec_n);
                             break;
												*/
                        case 56:
														 nrf_gpio_pin_clear(pin_motor_sec_n);
												     nrf_gpio_pin_set(pin_motor_sec_p);
                             break;
                        case 144:
														 nrf_gpio_pin_set(pin_motor_sec_n);
												     nrf_gpio_pin_clear(pin_motor_sec_p);
                             break;
												/*
                        case 288+62:
														 nrf_gpio_pin_clear(pin_motor_sec_n);
                             break;
                        case 308+62:
														 nrf_gpio_pin_set(pin_motor_sec_n);
                             break;
                        case 324+62:
														 nrf_gpio_pin_clear(pin_motor_sec_n);
                             break;
                        case 344+62:
														 nrf_gpio_pin_set(pin_motor_sec_n);
                             break;
                        case 360+62:
														 nrf_gpio_pin_clear(pin_motor_sec_n);
                             break;
                        case 392+62:
														 nrf_gpio_pin_set(pin_motor_sec_n);
                             break;
                        case 420+62:
														 nrf_gpio_pin_clear(pin_motor_sec_n);
                             break;
                        case 432+62:
														 nrf_gpio_pin_set(pin_motor_sec_n);
                             break;
														 */
                        case 625:
														 nrf_gpio_pin_clear(pin_motor_sec_n);
                             Second_Hand_Counterclockwise_Plus_Cnt = 0;
                             Second_Hand_Counterclockwise_Input_PIN_flg = 1;
												     Second_Hand_Counterclockwise_Step_Cnt+=1;
												     Second_Hand_Position-=1;
												     if(Second_Hand_Position>=180) Second_Hand_Position=0;
                             break;
                        default:break;
                        }
                }
                else if(Second_Hand_Counterclockwise_Input_PIN_flg == 1)
                {
                        // 第二个 1s
                  switch(Second_Hand_Counterclockwise_Plus_Cnt){
                        case 0: case 1:case 16:case 308+62:case 344+62:case 392+62:case 432+62:
														 nrf_gpio_pin_set(pin_motor_sec_p);
                             break;
                        case 8:
														 nrf_gpio_pin_clear(pin_motor_sec_p);
                             break;
												/*
                        case 16:
														 nrf_gpio_pin_set(pin_motor_sec_p);
                             break;
												*/
                        case 56:
														 nrf_gpio_pin_clear(pin_motor_sec_p);
												     nrf_gpio_pin_set(pin_motor_sec_n);
                             break;
                        case 144:
														 nrf_gpio_pin_set(pin_motor_sec_p);
												     nrf_gpio_pin_clear(pin_motor_sec_n);
                             break;
												/*
                        case 288+62:
                        //case 340:
														 nrf_gpio_pin_clear(pin_motor_sec_p);
                             break;
                        case 308+62:
														 nrf_gpio_pin_set(pin_motor_sec_p);
                             break;
                        case 324+62:
														 nrf_gpio_pin_clear(pin_motor_sec_p);
                             break;
                        case 344+62:
														 nrf_gpio_pin_set(pin_motor_sec_p);
                             break;
                        case 360+62:
														 nrf_gpio_pin_clear(pin_motor_sec_p);
                             break;
                        case 392+62:
														 nrf_gpio_pin_set(pin_motor_sec_p);
                             break;
                        case 420+62:
														 nrf_gpio_pin_clear(pin_motor_sec_p);
                             break;
                        case 432+62:
														 nrf_gpio_pin_set(pin_motor_sec_p);
                             break;			
*/														 
                        case 625:
														 nrf_gpio_pin_clear(pin_motor_sec_p);
                             Second_Hand_Counterclockwise_Plus_Cnt = 0;
                             Second_Hand_Counterclockwise_Input_PIN_flg = 0;
												     Second_Hand_Counterclockwise_Step_Cnt+=1;
												     Second_Hand_Position-=1;
												     if(Second_Hand_Position>=180) Second_Hand_Position=180;
                             break;
                        default:break;
                        }
                }
                Second_Hand_Counterclockwise_Plus_Cnt++;
}

void Minute_Hand_Clockwise(void)
{
                if(Minute_Hand_Clockwise_Input_PIN_flg == 0)
                {
                        // 第一个 1s
                        switch(Minute_Hand_Clockwise_Plus_Cnt){
                        case 0: case 1:case 52:case 102:
														 nrf_gpio_pin_set(pin_motor_min_p);
                             break;
                        case 36:case 88:case 138:
														 nrf_gpio_pin_clear(pin_motor_min_p);
                             break;	
/*												
                        case 52:
														 nrf_gpio_pin_set(pin_motor_min_p);
                             break;		
                        case 88:
														 nrf_gpio_pin_clear(pin_motor_min_p);
                             break;												
                        case 102:
														 nrf_gpio_pin_set(pin_motor_min_p);
                             break;
                        case 138:
														 nrf_gpio_pin_clear(pin_motor_min_p);
                             break;			
*/												
                        case 625:
														 nrf_gpio_pin_clear(pin_motor_min_p);
                             Minute_Hand_Clockwise_Plus_Cnt = 0;
                             Minute_Hand_Clockwise_Input_PIN_flg = 1;
												     Minute_Hand_Clockwise_Step_Cnt += 1;
												     Minute_Hand_Position+=1;
												     if(Minute_Hand_Position>=180) Minute_Hand_Position=0;
                             break;
                        default:break;
                        }
                }
                else if(Minute_Hand_Clockwise_Input_PIN_flg == 1)
                {
                        // 第二个 1s
                        switch(Minute_Hand_Clockwise_Plus_Cnt){
                        case 0: case 1:case 52:case 102:
														 nrf_gpio_pin_set(pin_motor_min_n);
                             break;
                        case 36:case 88:case 138:
														 nrf_gpio_pin_clear(pin_motor_min_n);
                             break;		
												/*
                        case 52:
														 nrf_gpio_pin_set(pin_motor_min_n);
                             break;		
                        case 88:
														  nrf_gpio_pin_clear(pin_motor_min_n);
                             break;												
                        case 102:
														nrf_gpio_pin_set(pin_motor_min_n);
                             break;
                        case 138:
														  nrf_gpio_pin_clear(pin_motor_min_n);
                             break;	
*/  											
                        case 625:
														 nrf_gpio_pin_clear(pin_motor_min_n);
                             Minute_Hand_Clockwise_Plus_Cnt = 0;
                             Minute_Hand_Clockwise_Input_PIN_flg = 0;
												     Minute_Hand_Clockwise_Step_Cnt += 1;
												     Minute_Hand_Position+=1;
												     if(Minute_Hand_Position>=180) Minute_Hand_Position=0;
                             break;
                        default:break;
                        }
                }
                Minute_Hand_Clockwise_Plus_Cnt++;
        }

void Minute_Hand_Counterclockwise(void)
        {
                if(Minute_Hand_Counterclockwise_Input_PIN_flg == 0)
                {
                        // 第一个 1s
                        switch(Minute_Hand_Clockwise_Plus_Cnt){
                        case 0: case 1:case 16:case 308+62:case 344+62:case 392+62:case 432+62:
														 nrf_gpio_pin_set(pin_motor_min_n);
                             break;
                        case 8:case 288+62:case 324+62:case 360+62:case 420+62:
														  nrf_gpio_pin_clear(pin_motor_min_n);
                             break;
												/*
                        case 16:
														 nrf_gpio_pin_set(pin_motor_min_n);
                             break;
												*/
                        case 56:
														  nrf_gpio_pin_clear(pin_motor_min_n);
												     nrf_gpio_pin_set(pin_motor_min_p);
                             break;
                        case 144:
														 nrf_gpio_pin_set(pin_motor_min_n);
												      nrf_gpio_pin_clear(pin_motor_min_n);
                             break;
												/*
                        case 288+62:
														  nrf_gpio_pin_clear(pin_motor_min_n);
                             break;
                        case 308+62:
														 nrf_gpio_pin_set(pin_motor_min_n);
                             break;
                        case 324+62:
														  nrf_gpio_pin_clear(pin_motor_min_n);
                             break;
                        case 344+62:
														 nrf_gpio_pin_set(pin_motor_min_n);
                             break;
                        case 360+62:
														  nrf_gpio_pin_clear(pin_motor_min_n);
                             break;
                        case 392+62:
														 nrf_gpio_pin_set(pin_motor_min_n);
                             break;
                        case 420+62:
														  nrf_gpio_pin_clear(pin_motor_min_n);
                             break;
                        case 432+62:
														 nrf_gpio_pin_set(pin_motor_min_n);
                             break;
												*/
                        case 625:
														  nrf_gpio_pin_clear(pin_motor_min_n);
                             Minute_Hand_Counterclockwise_Step_Cnt = 0;
                             Minute_Hand_Counterclockwise_Input_PIN_flg = 1;
												     Minute_Hand_Clockwise_Step_Cnt += 1;
												     Minute_Hand_Position-=1;
												     if(Minute_Hand_Position>=180) Minute_Hand_Position=180;
                             break;
                        default:break;
                        }
                }
                else if(Minute_Hand_Counterclockwise_Input_PIN_flg == 1)
                {
                        // 第二个 1s
                        switch(Minute_Hand_Clockwise_Plus_Cnt){
                        case 0: case 1:case 16:case 308+62:case 344+62:case 392+62:case 432+62:
														 nrf_gpio_pin_set(pin_motor_min_p);
                             break;
                        case 8:case 288+62:case 324+62:case 360+62:case 420+62:
														 nrf_gpio_pin_clear(pin_motor_min_p);
                             break;
												/*
                        case 16:
														 nrf_gpio_pin_set(pin_motor_min_p);
                             break;
*/
                        case 56:
														 nrf_gpio_pin_clear(pin_motor_min_p);
												     nrf_gpio_pin_set(pin_motor_min_n);
                             break;
                        case 144:
														 nrf_gpio_pin_set(pin_motor_min_p);
												     nrf_gpio_pin_clear(pin_motor_min_n);
                             break;
/*
                        case 288+62:
                        //case 340:
														 nrf_gpio_pin_clear(pin_motor_min_p);
                             break;
                        case 308+62:
														 nrf_gpio_pin_set(pin_motor_min_p);
                             break;
                        case 324+62:
														 nrf_gpio_pin_clear(pin_motor_min_p);
                             break;
                        case 344+62:
														 nrf_gpio_pin_set(pin_motor_min_p);
                             break;
                        case 360+62:
														 nrf_gpio_pin_clear(pin_motor_min_p);
                             break;
                        case 392+62:
														 nrf_gpio_pin_set(pin_motor_min_p);
                             break;
                        case 420+62:
														 nrf_gpio_pin_clear(pin_motor_min_p);
                             break;
                        case 432+62:
														 nrf_gpio_pin_set(pin_motor_min_p);
                             break;		
*/												
                        case 625:
														 nrf_gpio_pin_clear(pin_motor_min_p);
                             Minute_Hand_Counterclockwise_Step_Cnt = 0;
                             Minute_Hand_Counterclockwise_Input_PIN_flg = 0;
												     Minute_Hand_Clockwise_Step_Cnt += 1;
												     Minute_Hand_Position-=1;
												     if(Minute_Hand_Position>=180) Minute_Hand_Position=180;
                             break;
                        default:break;
                        }
                }
                Minute_Hand_Counterclockwise_Step_Cnt++;
        }

void Hour_Hand_Clockwise(void)
{
                if(Hour_Hand_Clockwise_Input_PIN_flg == 0)
                {
                        // 第一个 1s
                        switch(Hour_Hand_Clockwise_Plus_Cnt){
													case 0: case 52: case 102:
														 nrf_gpio_pin_set(pin_motor_hour_p);
                             break;
                        case 36:case 88:case 138:
														 nrf_gpio_pin_clear(pin_motor_hour_p);
                             break;		
                       // case 52:
											//			 nrf_gpio_pin_set(pin_motor_hour_p);
                      //       break;		
                       // case 88:
												//		 nrf_gpio_pin_clear(pin_motor_hour_p);
                       //      break;												
                       // case 102:
											//			 nrf_gpio_pin_set(pin_motor_hour_p);
                        //     break;
                       // case 138:
											//			 nrf_gpio_pin_clear(pin_motor_hour_p);
                        //     break;												
                        case 625:
														 nrf_gpio_pin_clear(pin_motor_hour_p);
                             Hour_Hand_Clockwise_Plus_Cnt = 0;
                             Hour_Hand_Clockwise_Input_PIN_flg = 1;
												     Hour_Hand_Clockwise_Step_Cnt += 1;
												     Hour_Hand_Position+=1;
												     if(Hour_Hand_Position>=180) Hour_Hand_Position=0;
                             break;
                        default:break;
                        }
                }
                else if(Hour_Hand_Clockwise_Input_PIN_flg == 1)
                {
                        // 第二个 1s
                        switch(Hour_Hand_Clockwise_Plus_Cnt){
                        case 0: case 1: case 52: case 102:
														 nrf_gpio_pin_set(pin_motor_hour_n);
                             break;
                        case 36: case 88: case 138:
														 nrf_gpio_pin_clear(pin_motor_hour_n);
                             break;		
                        //case 52:
												//		 nrf_gpio_pin_set(pin_motor_hour_n);
                        //     break;		
                        //case 88:
												//		 nrf_gpio_pin_clear(pin_motor_hour_n);
                        //     break;												
                        //case 102:
												//		 nrf_gpio_pin_set(pin_motor_hour_n);
                        //     break;
                        //case 138:
												//		 nrf_gpio_pin_clear(pin_motor_hour_n);
                        //     break;	  											
                        case 625:
														 nrf_gpio_pin_clear(pin_motor_hour_n);
                             Hour_Hand_Clockwise_Plus_Cnt = 0;
                             Hour_Hand_Clockwise_Input_PIN_flg = 0;
												     Hour_Hand_Clockwise_Step_Cnt += 1;
												     Hour_Hand_Position+=1;
												     if(Hour_Hand_Position>=180) Hour_Hand_Position=0;
                             break;
                        default:break;
                        }
                }
                Hour_Hand_Clockwise_Plus_Cnt++;
        }
void Hour_Hand_Counterclockwise(void)
        {
                if(Hour_Hand_Counterclockwise_Input_PIN_flg == 0)
                {
                        switch(Hour_Hand_Counterclockwise_Plus_Cnt){
                        case 0: case 1: case 16: case 308+62: case 392+62: case 432+62:
														 nrf_gpio_pin_set(pin_motor_hour_n);
                             break;
                        case 8: case 288+62: case 324+62: case 360+62: case 420+62:
														 nrf_gpio_pin_clear(pin_motor_hour_n);
                             break;
                        //case 16:
												//		 nrf_gpio_pin_set(pin_motor_hour_n);
                        //     break;
                        case 56:
														 nrf_gpio_pin_clear(pin_motor_hour_n);
												     nrf_gpio_pin_set(pin_motor_hour_p);
                             break;
                        case 144:
														 nrf_gpio_pin_set(pin_motor_hour_n);
												     nrf_gpio_pin_clear(pin_motor_hour_p);
                             break;
                        //case 288+62:
												//		 nrf_gpio_pin_clear(pin_motor_hour_n);
                        //     break;
                        //case 308+62:
												//		 nrf_gpio_pin_set(pin_motor_hour_n);
                        //     break;
                        //case 324+62:
												//		 nrf_gpio_pin_clear(pin_motor_hour_n);
                        //     break;
                        //case 344+62:
												//		 nrf_gpio_pin_set(pin_motor_hour_n);
                        //     break;
                        //case 360+62:
												//		 nrf_gpio_pin_clear(pin_motor_hour_n);
                        //     break;
                        //case 392+62:
												//		 nrf_gpio_pin_set(pin_motor_hour_n);
                        //     break;
                        //case 420+62:
												//		 nrf_gpio_pin_clear(pin_motor_hour_n);
                        //     break;
                        //case 432+62:
												//		 nrf_gpio_pin_set(pin_motor_hour_n);
                        //     break;
                        case 625:
														 nrf_gpio_pin_clear(pin_motor_hour_n);
                             Hour_Hand_Counterclockwise_Plus_Cnt = 0;
                             Hour_Hand_Counterclockwise_Input_PIN_flg = 1;
												     Hour_Hand_Clockwise_Step_Cnt += 1;
												     Hour_Hand_Position-=1;
												     if(Hour_Hand_Position>=180) Hour_Hand_Position=180;
                             break;
                        default:break;
                        }
                }
                else if(Hour_Hand_Counterclockwise_Input_PIN_flg == 1)
                {
                        // 第二个 1s
                        switch(Hour_Hand_Counterclockwise_Plus_Cnt){
                        case 0: case 1: case 16: case 308+62: case 344+62: case 392+62: case 432+62:
														 nrf_gpio_pin_set(pin_motor_hour_p);
                             break;
                        case 8: case 288+62:
														 nrf_gpio_pin_clear(pin_motor_hour_p);
                             break;
												/*
                        case 16:
														 nrf_gpio_pin_set(pin_motor_hour_p);
                             break;
												*/
                        case 56:
														 nrf_gpio_pin_clear(pin_motor_hour_p);
												     nrf_gpio_pin_set(pin_motor_hour_n);
                             break;
                        case 144:
														 nrf_gpio_pin_set(pin_motor_hour_p);
												     nrf_gpio_pin_clear(pin_motor_hour_n);
                             break;
												/*
                        case 288+62:
                        //case 340:
														 nrf_gpio_pin_clear(pin_motor_hour_p);
                             break;
                        case 308+62:
														 nrf_gpio_pin_set(pin_motor_hour_p);
                             break;
                        case 324+62:
														 nrf_gpio_pin_clear(pin_motor_hour_p);
                             break;
                        case 344+62:
														 nrf_gpio_pin_set(pin_motor_hour_p);
                             break;
                        case 360+62:
														 nrf_gpio_pin_clear(pin_motor_hour_p);
                             break;
                        case 392+62:
														 nrf_gpio_pin_set(pin_motor_hour_p);
                             break;
                        case 420+62:
														 nrf_gpio_pin_clear(pin_motor_hour_p);
                             break;
                        case 432+62:
														 nrf_gpio_pin_set(pin_motor_hour_p);
                             break;		
*/														 
                        case 625:
														 nrf_gpio_pin_clear(pin_motor_hour_p);
                             Hour_Hand_Counterclockwise_Plus_Cnt = 0;
                             Hour_Hand_Counterclockwise_Input_PIN_flg = 0;
												     Hour_Hand_Clockwise_Step_Cnt += 1;
												     Hour_Hand_Position-=1;
												     if(Hour_Hand_Position>=180) Hour_Hand_Position=180;
                             break;
                        default:break;
                        }
                }
                Hour_Hand_Counterclockwise_Plus_Cnt++;
        }


void hand_clockwise_Positive()
{
	
}




