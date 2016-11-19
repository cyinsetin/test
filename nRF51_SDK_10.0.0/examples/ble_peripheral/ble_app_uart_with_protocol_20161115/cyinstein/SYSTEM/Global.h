#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "boards.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_timer.h"
#include "nrf_delay.h"
#include "nrf_adc.h"
//#include "Spi_master.h"


//#define COMPARE_COUNTERTIME  (1UL)                                        /**< Get Compare event COMPARE_TIME seconds after the counter starts from 0. */
//#define LFCLK_FREQUENCY	(32768UL)
//#define RTC_FREQUENCY		(8UL)
//#define COUNTER_PRESCALER	((LFCLK_FREQUENCY / RTC_FREQUENCY) - 1 )
//#define TICK_EVENT_LED LED_1
//#define COMPARE_EVENT_LED LED_2
// General application timer settings.
#define APP_TIMER_PRESCALER             10    // Value of the RTC1 PRESCALER register.
#define APP_TIMER_MAX_TIMERS            2     // Maximum number of timers in this application.
#define APP_TIMER_OP_QUEUE_SIZE         5     // Size of timer operation queues.
#define Watch_header_H									0x48
#define Watch_header_L									0x59
#define Watch_ID_H											0x07
#define Ble_RX_TX_LEN           				20 
static uint8_t FW_VER[] = {'C','T','-','0','0','3','V','0','.','1','0'};
static uint8_t ANS_ACK[] = {0x48,0x59,0x07,0x06,0x00,0x00,0x0D,0x2E};
static uint8_t ANS_NAK[] = {0x48,0x59,0x07,0x0f,0x00,0x00,0x16,0x49};
//static uint8 ANS_REJ[] = {0x48,0x59,0x07,0x10,0x00,0x00,0x17,0x4C};
static uint8_t ANS_SN[] = {0x48,0x59,0x07,0x00,0x10,'T','R','E','K','-','E','M','0','0','6','V','0','.','1','0'};
	     uint8_t     SN[] = {2,0,1,6,1,1,0,4,'C','T','-','0','0','3','0','1'};
static uint8_t ANS_PN[] = {0x48,0x59,0x07,0x00,0x10,'T','R','E','K','-','E','M','0','0','6','V','0','.','1','0'};
	     uint8_t     PN[] = {'P','N',2,0,1,6,1,1,0,4,0,0,0,0,0,1};
	uint8_t bt_rx_len;
	uint8_t bt_tx_len;
uint8_t BT_RxBuf[Ble_RX_TX_LEN];
uint8_t BT_TxBuf[Ble_RX_TX_LEN];
//#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
//#define UART_RX_BUF_SIZE 1                           /**< UART RX buffer size. */
int16_t RTC_Second_Hand_Position=0;  									//The RTC Current Position of Second Hand
int16_t RTC_Minute_Hand_Position=0;										//The RTC Current Position of Minute Hand
int16_t RTC_Hour_Hand_Position=0;											//The RTC Current Position of Hour Hand

int16_t RTC_Small_Hand_Top_Position=0;  								//The RTC Current Position of Small Hand Hand
int16_t RTC_Small_Hand_Buttom_Position=0;  						//The RTC Current Position of Small Hand Hand
int16_t RTC_Small_Hand_Right_Position=0;  							//The RTC Current Position of Small Hand Hand
int16_t RTC_Small_Hand_Left_Position=0;  							//The RTC Current Position of Small Hand Hand

int16_t Second_Hand_Position=0;  									//The Current Position of Second Hand
int16_t Minute_Hand_Position=0;										//The Current Position of Minute Hand
int16_t Hour_Hand_Position=0;											//The Current Position of Hour Hand
int16_t Small_Hand_Top_Position=0;  								//The Current Position of Status Hand
int16_t Small_Hand_Buttom_Position=0;  						//The Current Position of Status Hand
int16_t Small_Hand_Right_Position=0;  							//The Current Position of Status Hand
int16_t Small_Hand_Left_Position=0;  							//The Current Position of Status Hand
int16_t Pedometer_Position=0;
uint8_t Hour_Hand_Clockwise_Step_Cnt;										//时针顺时针计步Cnt值
uint8_t Minute_Hand_Clockwise_Step_Cnt;								//分针顺时针计步Cnt值
uint8_t Second_Hand_Clockwise_Step_Cnt;								//秒钟顺时针计步Cnt值


uint32_t Pedometer_Value=0;											//计步数
uint8_t date_position_index=0;									//
uint8_t weeks_position_index=0;
//static uint8_t key_press_flg=0;
uint32_t key_press_time=0;
uint16_t key_press_cnt=0;
//static uint8_t second=0;
//static char str[30];
uint32_t RTC_CNT;
uint32_t RTC_TIME_BASE;
uint16_t BAT_Value;

uint8_t Hand_activity_mode;													//0:Time Mode
																										//1:Show Date & Battery Capacity
																										//2:BLE On Mode
																										//3:BLE Off Mode
																										//4:Battery Low Voltage Mode
																										//5:Hand Calibration Mode
																										//6:Power Off Mode
																										//7:Power On Mode
																										//8:Chasing Time Mode
																										//9:Hand To Zero Mode index
																										//10:Hand Calibration Mode for BLE
#define Time_Mode_index									0
#define Show_Date_BAT_index							1
#define BLE_On_Mode_index								2
#define BLE_Off_Mode_index							3
#define BAT_Low_Vol_Mode_index					4
#define Hand_Calibration_Mode_index			5
#define Power_Off_Mode_index						6
#define Power_On_Mode_index							7
#define Chasing_Time_index							8
#define Hand_To_Zero_Mode_index					9
#define Hand_Calibration_Mode_for_BLE		10

volatile int32_t adc_sample;
uint8_t Show_Date_Batt_mode_step;										//0:Hour Hand Show Week
																										//1:Minute Hand Show Day
																										//2:Small Hand Down Show Battery Capacity
																										//3:Small Hand Down Show Pedometer Value,Set Step to zero,Set Hand_activity mode to Time Mode

uint8_t BLE_On_Mode_step;														//0:Small Hand Down Show BLE On
																										//1:Turn On BLE Advertising
																										//2:Small Hand Down Show Pedometer Value,Set Step to zero,Set Hand_activity mode to Time Mode
																										
uint8_t BLE_Off_Mode_step;													//0:Small Hand Down Show BLE Off
																										//1:Turn Off BLE Advertising
																										//2:Small Hand Down Show Pedometer Value,Set Step to zero,Set Hand_activity mode to Time Mode

uint8_t Battery_Low_Voltage_Mode_step;							//0:Small Hand Down Show Battery Low Voltage
																										//1:Set Step to zero,Set Hand_activity mode to Time Mode

uint8_t Pointer_Calibration_mode_step;							//0:Small hand running 64hz
																										//1:Small hand running 1 step/key
																										//2:Minute hand running 64hz
																										//3:Minute hand running 1 step/key
																										//4:Hour hand running 64hz
																										//5:Hour hand running 1 step/key
																										//6:All Hand Show Time And Status,Set Step to zero
																										
uint8_t Power_Off_Mode_step;												//0:Small hand running 64hz

uint8_t Power_On_Mode_step;													//0:Small hand running 64hz
uint8_t	Chasing_Time_step;
uint8_t Hand_To_Zero_Mode_step;
uint8_t Hand_Calibration_Mode_for_BLE_step;

uint8_t key_pause_short_flg=0;

uint8_t key_pause_3s_flg=0;
uint8_t key_pause_5s_flg=0;
uint8_t key_pause_10s_flg=0;
uint8_t One_step_flg=0;

uint32_t Start_Show_value;
uint8_t Second_Hand_One_Step_flg = 0;
uint8_t Minute_Hand_One_Step_flg = 0;
uint8_t Hour_Hand_One_Step_flg = 0;

unsigned int* spi_master_init(int module_number, int mode, bool lsb_first);

const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(1); /**< Declaring an instance of nrf_drv_rtc for RTC1. */
const nrf_drv_timer_t Hand_Control = NRF_DRV_TIMER_INSTANCE(1);  //创建一个定时器1的实例
const nrf_drv_timer_t Button_Control = NRF_DRV_TIMER_INSTANCE(2);  //创建一个定时器2的实例


const uint8_t buttons[BUTTONS_NUMBER]={BUTTON_1,BUTTON_2};
const uint8_t leds_list[LEDS_NUMBER]={LED_1,LED_2,LED_3,LED_4};
const uint8_t date_position[32]={0,6,11,17,21,26,31,36,42,48,54,60,65,72,78,84,90,96,102,108,114,120,126,132,138,144,150,157,162,168,175,0};
const uint8_t weeks_position[7]={0,26,52,77,105,129,155};
const uint8_t small_hand_position[4]={0,134,158,170};
const uint8_t Bat_Low_position = 134;
const uint8_t BLE_off_position = 158;
const uint8_t BLE_on_position = 170;
uint8_t key_press_times_cnt;
/*
static int16_t RTC_Second_Hand_Position=0;  									//The RTC Current Position of Second Hand
static int16_t RTC_Minute_Hand_Position=0;										//The RTC Current Position of Minute Hand
static int16_t RTC_Hour_Hand_Position=0;											//The RTC Current Position of Hour Hand

static int16_t RTC_Small_Hand_Top_Position=0;  								//The RTC Current Position of Small Hand Hand
static int16_t RTC_Small_Hand_Buttom_Position=0;  						//The RTC Current Position of Small Hand Hand
static int16_t RTC_Small_Hand_Right_Position=0;  							//The RTC Current Position of Small Hand Hand
static int16_t RTC_Small_Hand_Left_Position=0;  							//The RTC Current Position of Small Hand Hand

static int16_t Second_Hand_Position=0;  									//The Current Position of Second Hand
static int16_t Minute_Hand_Position=0;										//The Current Position of Minute Hand
static int16_t Hour_Hand_Position=0;											//The Current Position of Hour Hand
static int16_t Small_Hand_Top_Position=0;  								//The Current Position of Status Hand
static int16_t Small_Hand_Buttom_Position=0;  						//The Current Position of Status Hand
static int16_t Small_Hand_Right_Position=0;  							//The Current Position of Status Hand
static int16_t Small_Hand_Left_Position=0;  							//The Current Position of Status Hand
*/
#endif // GLOBAL_H
