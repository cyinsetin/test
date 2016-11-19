#ifndef __MOTOR_H
#define __MOTOR_H
#include <stdint.h>

#include "boards.h"
//#include "global.h"

extern int16_t RTC_Second_Hand_Position;  									//The RTC Current Position of Second Hand
extern int16_t RTC_Minute_Hand_Position;										//The RTC Current Position of Minute Hand
extern int16_t RTC_Hour_Hand_Position;											//The RTC Current Position of Hour Hand

extern int16_t RTC_Small_Hand_Top_Position;  								//The RTC Current Position of Small Hand Hand
extern int16_t RTC_Small_Hand_Buttom_Position;  						//The RTC Current Position of Small Hand Hand
extern int16_t RTC_Small_Hand_Right_Position;  							//The RTC Current Position of Small Hand Hand
extern int16_t RTC_Small_Hand_Left_Position;  							//The RTC Current Position of Small Hand Hand

extern int16_t Second_Hand_Position;  									//The Current Position of Second Hand
extern int16_t Minute_Hand_Position;										//The Current Position of Minute Hand
extern int16_t Hour_Hand_Position;											//The Current Position of Hour Hand
extern int16_t Small_Hand_Top_Position;  								//The Current Position of Status Hand
extern int16_t Small_Hand_Buttom_Position;  						//The Current Position of Status Hand
extern int16_t Small_Hand_Right_Position;  							//The Current Position of Status Hand
extern int16_t Small_Hand_Left_Position;  							//The Current Position of Status Hand

// definitions for CT003
/*
#define HOUR_HAND_P	   00
#define HOUR_HAND_N	   02
#define MINUTE_HAND_P  21
#define MINUTE_HAND_N  22
#define PD_HAND_P		   23
#define PD_HAND_N		   24
#define CS_G_SENSOR	   01
#define SDO_G_SENSOR	 03
#define VBAT_DETECT		 05
#define V5_DETECT		   06
#define SDI_G_SENSOR	 08
#define CLK_G_SENSOR	 09
#define VIBRATION_IO	 11
#define TX_BLE				 14
#define RX_BLE				 15
#define KEY_IO				 16
#define INT2_G_SENSOR	 28
#define INT1_G_SENSOR	 29
*/

#define pin_motor_hour_p 	00
#define pin_motor_hour_n 	01
#define pin_motor_min_p 	02
#define pin_motor_min_n 	03
#define pin_motor_sec_p 	04
#define pin_motor_sec_n 	05

//extern uint8_t Hour_Hand_Clockwise_Step_Cnt;										//
//extern uint8_t Minute_Hand_Clockwise_Step_Cnt;
//extern uint8_t Second_Hand_Clockwise_Step_Cnt;

extern uint8_t Second_Hand_One_Step_flg;
extern uint8_t Minute_Hand_One_Step_flg;
extern uint8_t Hour_Hand_One_Step_flg;

void motor_gpio_init(void);

//void Second_Hand_Clockwise(void);
void Second_Hand_Clockwise(int16_t *Position,uint8_t *Step_Cnt);
void Second_Hand_Counterclockwise(int16_t *Position,uint8_t *Step_Cnt);
//void Second_Hand_Counterclockwise(int16_t *Position,uint8_t *Clockwise_Step_Cnt);
//void Second_Hand_Counterclockwise(void);

//void Minute_Hand_Clockwise(void);
void Minute_Hand_Clockwise(int16_t *Position,uint8_t *Step_Cnt);
void Minute_Hand_Counterclockwise(int16_t *Position,uint8_t *Step_Cnt);
//void Minute_Hand_Counterclockwise(void);
//void Hour_Hand_Clockwise(void);
void Hour_Hand_Clockwise(int16_t *Position,uint8_t *Step_Cnt);
void Hour_Hand_Counterclockwise(int16_t *Position,uint8_t *Step_Cnt);
//void Hour_Hand_Counterclockwise(void);
//void calculate_RTC_Position(volatile uint8_t sys_hour, volatile uint8_t sys_minute, volatile uint8_t sys_second);
void calculate_RTC_Position(uint8_t sys_hour,uint8_t sys_minute,uint8_t sys_second,int16_t *RTC_Hour_Position,int16_t *RTC_Minute_Position,int16_t *RTC_Second_Position);
#endif
