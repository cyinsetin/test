#ifndef _MOTOR_H
#define _MOTOR_H
#include <stdint.h>

// definitions for CT003
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

#define pin_motor_hour_p 	HOUR_HAND_P 	//00
#define pin_motor_hour_n 	HOUR_HAND_N 	//01
#define pin_motor_min_p 	MINUTE_HAND_P //02
#define pin_motor_min_n 	MINUTE_HAND_N //03
#define pin_motor_sec_p 	PD_HAND_P 		//04
#define pin_motor_sec_n 	PD_HAND_N 		//05

void motor_gpio_init(void);
void calculate_RTC_Position(uint8_t sys_hour, uint8_t sys_minute, uint8_t sys_second, uint8_t* RTC_Hour_Position, uint8_t* RTC_Min_Position, uint8_t* RTC_Second_Position);
void Second_Hand_Clockwise(void);
void Second_Hand_Counterclockwise(void);
void Minute_Hand_Clockwise(void);
void Minute_Hand_Counterclockwise(void);
void Hour_Hand_Clockwise(void);
void Hour_Hand_Counterclockwise(void);
#endif
