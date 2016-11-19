#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_drv_config.h"
#include "nrf_adc.h"
//#include "nrf_drv_timer.h"
#include "app_timer.h"
#include "boards.h"
#include "key.h"

extern uint16_t key_press_cnt;
extern uint8_t key_pause_3s_flg;
extern uint8_t key_pause_5s_flg;
extern uint8_t key_pause_10s_flg;
extern uint8_t One_step_flg;
extern uint8_t Hand_activity_mode;	
extern uint8_t key_pause_short_flg;

void Key_init(void)
{
	//nrf_gpio_range_cfg_output(LED_STOP,LED_START);
	//for(uint8_t i=0;i<LEDS_NUMBER;i++) nrf_gpio_pin_set(leds_list[i]);
	//for(uint8_t i=0;i<BUTTONS_NUMBER;i++)
	//{
/*
	//FOR EN-nRF51822DK & BLE400
		NRF_GPIO->PIN_CNF[KEY_IO]=
		(GPIO_PIN_CNF_DIR_Input<<GPIO_PIN_CNF_DIR_Pos)	|
		(GPIO_PIN_CNF_INPUT_Connect<<GPIO_PIN_CNF_INPUT_Pos)	|
		(GPIO_PIN_CNF_PULL_Pullup<<GPIO_PIN_CNF_PULL_Pos)	|
		(GPIO_PIN_CNF_DRIVE_S0S1<<GPIO_PIN_CNF_DRIVE_Pos)	|
		(GPIO_PIN_CNF_SENSE_Low<<GPIO_PIN_CNF_SENSE_Pos);
	*/

	//FOR CT-003 PCBA
    NRF_GPIO->PIN_CNF[KEY_IO] = (GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos)
                                        | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                                        | (GPIO_PIN_CNF_PULL_Pulldown << GPIO_PIN_CNF_PULL_Pos)	
                                        | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                                        | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);

	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Msk;
	NVIC_SetPriority(GPIOTE_IRQn,3);
	NVIC_EnableIRQ(GPIOTE_IRQn);
}

