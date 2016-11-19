/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup led_softblink_example_main main.c
 * @{
 * @ingroup led_softblink_example
 * @brief LED Soft Blink Example Application main file.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "led_softblink.h"
#include "app_error.h"
#include "sdk_errors.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "app_util_platform.h"
#include "LIS3DH_Driver.h"
#include "SEGGER_RTT.H"
#include "SEGGER_RTT_CONF.H"

#ifdef SOFTDEVICE_PRESENT
#include "softdevice_handler.h"
#endif

/*Timer initalization parameters*/   
#define OP_QUEUES_SIZE          3
#define APP_TIMER_PRESCALER     0 


/**
 * @brief Function for LEDs initialization.
 */
static void leds_init(void)
{
    ret_code_t           err_code;
    
    led_sb_init_params_t led_sb_init_params = LED_SB_INIT_DEFAULT_PARAMS(LEDS_MASK);
    
    err_code = led_softblink_init(&led_sb_init_params);

    APP_ERROR_CHECK(err_code);
}

#ifndef SOFTDEVICE_PRESENT
static void lfclk_init(void)
{
    uint32_t err_code;
    nrf_drv_clock_config_t nrf_drv_clock_config = NRF_DRV_CLOCK_DEAFULT_CONFIG;

    err_code = nrf_drv_clock_init(&nrf_drv_clock_config);
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request();
}
#endif

/**
 * @brief Function for application main entry.
 */
static unsigned char Test_buffer[6];
static unsigned char Test_Byte;

int main(void)
{
    uint32_t err_code;
		uint16_t x,y,z;

    // We need the LFCK running for APP_TIMER. In case SoftDevice is in use
    // it will start the LFCK during initialization, otherwise we have to
    // start it manually.
#ifdef SOFTDEVICE_PRESENT
    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, false);
#else
    lfclk_init();
#endif

    // Start APP_TIMER to generate timeouts.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, OP_QUEUES_SIZE, NULL);

    leds_init();
    err_code = led_softblink_start(LEDS_MASK);
    APP_ERROR_CHECK(err_code);     

    //Enable_GSensor();
		Read_Sen_datas(0x0f,&Test_Byte,1);
		SEGGER_RTT_printf(0,"G-Sensor ID = :%x ",Test_Byte);
		while (true)
    {
        //__WFE();
				if(BUTTON_1==0)
				{
					Enable_GSensor();
					//Enable_GSensor2();
					nrf_delay_ms(100);
					Read_Sen_datas(0x28,Test_buffer,6);
					x=(Test_buffer[0]>>4)|(WORD)Test_buffer[1]<<4;
					y=(Test_buffer[2]>>4)|(WORD)Test_buffer[3]<<4;
					z=(Test_buffer[4]>>4)|(WORD)Test_buffer[5]<<4;
				
					SEGGER_RTT_printf(0,"X-Axis = :%x ",x);
					SEGGER_RTT_printf(0,"Y-Axis = :%x ",y);
					SEGGER_RTT_printf(0,"Z-Axis = :%x\r\n",z);
					nrf_delay_ms(200);
					Disable_GSensor();					
				}
				else
				{
					Enable_GSensor2();
					nrf_delay_ms(100);
					Read_Sen_datas(0x28,Test_buffer,6);
					x=(Test_buffer[0]>>4)|(WORD)Test_buffer[1]<<4;
					y=(Test_buffer[2]>>4)|(WORD)Test_buffer[3]<<4;
					z=(Test_buffer[4]>>4)|(WORD)Test_buffer[5]<<4;
				
					SEGGER_RTT_printf(0,"X-Axis = :%x ",x);
					SEGGER_RTT_printf(0,"Y-Axis = :%x ",y);
					SEGGER_RTT_printf(0,"Z-Axis = :%x\r\n",z);
					nrf_delay_ms(200);
					Disable_GSensor();
				}
				nrf_delay_ms(100);
				
    }
}

/** @} */
