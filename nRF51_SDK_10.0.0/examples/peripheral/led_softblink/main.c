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
#include "SEGGER_RTT.H"
#include "SEGGER_RTT_CONF.H"
#ifdef SOFTDEVICE_PRESENT
#include "softdevice_handler.h"
#endif

/*Timer initalization parameters*/   
#define OP_QUEUES_SIZE          3
#define APP_TIMER_PRESCALER     0 
#define SPI_OPERATING_FREQUENCY (0x02000000uL << (uint32_t)Freq_8Mbps/*Freq_1Mbps*/)  /**< Slave clock frequency. */
#define TIMEOUT_COUNTER         0x3000uL                                /**< Timeout for SPI transaction in units of loop iterations. */

typedef enum
{
    Freq_125Kbps = 0,        /*!< drive SClk with frequency 125Kbps */
    Freq_250Kbps,            /*!< drive SClk with frequency 250Kbps */
    Freq_500Kbps,            /*!< drive SClk with frequency 500Kbps */
    Freq_1Mbps,              /*!< drive SClk with frequency 1Mbps */
    Freq_2Mbps,              /*!< drive SClk with frequency 2Mbps */
    Freq_4Mbps,              /*!< drive SClk with frequency 4Mbps */
    Freq_8Mbps               /*!< drive SClk with frequency 8Mbps */
} SPIFrequency_t;

/**
 *  SPI master module number
 */
typedef enum
{
    SPI0 = 0,               /*!< SPI module 0 */
    SPI1                    /*!< SPI module 1 */
} SPIModuleNumber;

/**
 *  SPI mode
 */
typedef enum
{
    //------------------------Clock polarity 0, Clock starts with level 0-------------------------------------------
    SPI_MODE0 = 0,          /*!< Sample data at rising edge of clock and shift serial data at falling edge */
    SPI_MODE1,              /*!< sample data at falling edge of clock and shift serial data at rising edge */
    //------------------------Clock polarity 1, Clock starts with level 1-------------------------------------------
    SPI_MODE2,              /*!< sample data at falling edge of clock and shift serial data at rising edge */
    SPI_MODE3               /*!< Sample data at rising edge of clock and shift serial data at falling edge */
} SPIMode;




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


uint32_t* spi_master_init(SPIModuleNumber module_number, SPIMode mode, bool lsb_first)
{
    uint32_t config_mode;

    NRF_SPI_Type *spi_base_address = (SPI0 == module_number)? NRF_SPI0 : (NRF_SPI_Type *)NRF_SPI1;

    if(SPI0 == module_number)
    {
        /* Configure GPIO pins used for pselsck, pselmosi, pselmiso and pselss for SPI0 */
        nrf_gpio_cfg_output(SPI_PSELSCK0);
        nrf_gpio_cfg_output(SPI_PSELMOSI0);
        nrf_gpio_cfg_input(SPI_PSELMISO0, NRF_GPIO_PIN_NOPULL);
        nrf_gpio_cfg_output(SPI_PSELSS0);

        /* Configure pins, frequency and mode */
        spi_base_address->PSELSCK  = SPI_PSELSCK0;
        spi_base_address->PSELMOSI = SPI_PSELMOSI0;
        spi_base_address->PSELMISO = SPI_PSELMISO0;
        nrf_gpio_pin_set(SPI_PSELSS0); /* disable Set slave select (inactive high) */
    }
    else
    {
        /* Configure GPIO pins used for pselsck, pselmosi, pselmiso and pselss for SPI1*/
        nrf_gpio_cfg_output(SPI_PSELSCK1);
        nrf_gpio_cfg_output(SPI_PSELMOSI1);
        nrf_gpio_cfg_input(SPI_PSELMISO1, NRF_GPIO_PIN_NOPULL);
        nrf_gpio_cfg_output(SPI_PSELSS1);

        /* Configure pins, frequency and mode */
        spi_base_address->PSELSCK  = SPI_PSELSCK1;
        spi_base_address->PSELMOSI = SPI_PSELMOSI1;
        spi_base_address->PSELMISO = SPI_PSELMISO1;
        nrf_gpio_pin_set(SPI_PSELSS1);         /* disable Set slave select (inactive high) */
    }

    spi_base_address->FREQUENCY = (uint32_t) SPI_OPERATING_FREQUENCY;

    /*lint -e845 -save // A zero has been given as right argument to operator '!'" */
    /** @snippet [SPI Select mode] */
    switch (mode )
    {
       
        case SPI_MODE0:
            config_mode = (SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos) | (SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos);
            break;
        case SPI_MODE1:
            config_mode = (SPI_CONFIG_CPHA_Trailing << SPI_CONFIG_CPHA_Pos) | (SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos);
            break;
        case SPI_MODE2:
            config_mode = (SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos) | (SPI_CONFIG_CPOL_ActiveLow << SPI_CONFIG_CPOL_Pos);
            break;
        case SPI_MODE3:
            config_mode = (SPI_CONFIG_CPHA_Trailing << SPI_CONFIG_CPHA_Pos) | (SPI_CONFIG_CPOL_ActiveLow << SPI_CONFIG_CPOL_Pos);
            break;
        default:
            config_mode = 0;
            break;
    
    }
    /** @snippet [SPI Select mode] */
    /*lint -restore */

    /*lint -e845 -save // A zero has been given as right argument to operator '!'" */
    /** @snippet [SPI Select endianess] */
    if (lsb_first)
    {
        spi_base_address->CONFIG = (config_mode | (SPI_CONFIG_ORDER_LsbFirst << SPI_CONFIG_ORDER_Pos));
    }
    else
    {
        spi_base_address->CONFIG = (config_mode | (SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos));
    }
    /** @snippet [SPI Select endianess] */
    /*lint -restore */

    spi_base_address->EVENTS_READY = 0U;

    /* Enable */
    spi_base_address->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);

    return (uint32_t *)spi_base_address;
}

//type: 0:write and read  1:only write  2: only read
bool spi_master_tx_rx(uint32_t *spi_base_address, uint16_t transfer_size, const uint8_t *tx_data, uint8_t *rx_data,uint8_t type)
{
    uint32_t counter = 0;
    uint16_t number_of_txd_bytes = 0;
    uint32_t SEL_SS_PINOUT;
    uint8_t temp;
    /*lint -e{826} //Are too small pointer conversion */
    NRF_SPI_Type *spi_base = (NRF_SPI_Type *)spi_base_address;

    if( (uint32_t *)NRF_SPI0 == spi_base_address)
    {
        SEL_SS_PINOUT = SPI_PSELSS0;
    }
    else
    {
        SEL_SS_PINOUT = SPI_PSELSS1;
    }

    /* enable slave (slave select active low) */
    nrf_gpio_pin_clear(SEL_SS_PINOUT);

    while(number_of_txd_bytes < transfer_size)
    {
      //  if(type==0 ||type==1)
        spi_base->TXD = (uint32_t)(tx_data[number_of_txd_bytes]);
	//else
	//{
	//	if(number_of_txd_bytes==0)
	//        	spi_base->TXD = (uint32_t)(tx_data[number_of_txd_bytes]);
	//	else
	//        	spi_base->TXD = 0x0;
	//}
		
	
        /* Wait for the transaction complete or timeout (about 10ms - 20 ms) */
        while ((spi_base->EVENTS_READY == 0U) && (counter < TIMEOUT_COUNTER))
        {
            counter++;
        }

        if (counter == TIMEOUT_COUNTER)
        {
            /* timed out, disable slave (slave select active low) and return with error */
            nrf_gpio_pin_set(SEL_SS_PINOUT);

		//printMsg("SPITimeout0");
			
            return false;
        }
        else
        {   /* clear the event to be ready to receive next messages */
            spi_base->EVENTS_READY = 0U;
        }

        if(type==0 || type==1)
        	rx_data[number_of_txd_bytes] = (uint8_t)spi_base->RXD;
	else
	{
		if(number_of_txd_bytes==0)
		{
			temp=(uint8_t)spi_base->RXD;
			if(temp==0)
				temp=1;
		}
		else
        		rx_data[number_of_txd_bytes-1] = (uint8_t)spi_base->RXD;
			
		//temp=spi_base->RXD;
		//if(temp==0x98)
		//	temp+=1;
	}
	
        number_of_txd_bytes++;
    };

    /* disable slave (slave select active low) */
    nrf_gpio_pin_set(SEL_SS_PINOUT);

    return true;
}

void write_Sen_datas(uint8_t Addr,uint8_t data)  
{
    	uint32_t * p_spi_base_address = spi_master_init(SPI1, SPI_MODE0, false);

	uint8_t TempData[2];
	TempData[0]=Addr;
	TempData[1]=data;
	
	spi_master_tx_rx(p_spi_base_address, 2, TempData, TempData,1); 
	//spi_master_tx_rx(p_spi_base_address, 2, &data, NULL,1); 
}

void Read_Sen_datas(uint8_t Addr,uint8_t *buff,uint8_t Len)  
{
    	uint32_t * p_spi_base_address = spi_master_init(SPI1, SPI_MODE0, false);
	Addr|=0x80;
	if(Len>1)
		Addr|=0x40;
	spi_master_tx_rx(p_spi_base_address, Len+1, &Addr, buff,2); 
	//spi_master_tx_rx(p_spi_base_address, Len, NULL, buff,2); 
}




void Enable_GSensor(void)
{
	
	write_Sen_datas(0x20, 0x4F);
	write_Sen_datas(0x22, 0x00);
	
	write_Sen_datas(0x23, 0x00);
	write_Sen_datas(0x24, 0x00);

	write_Sen_datas(0x21, 0x04);

	

}
void Disable_GSensor(void)
{
	write_Sen_datas(0x20, 0x00);

    //////////////////////////////

    //write_Sen_datas(0x20, 0x08);   //test for dengguo.
    
    //////////////////////////////
    

}

void Enable_GSensor2(void)
{
	write_Sen_datas(0x20, 0x6F);
}



static unsigned char Test_buffer[6];
static unsigned char Test_Byte;

int main(void)
{
    uint32_t err_code;

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
		
		Read_Sen_datas(0x0f,&Test_Byte,1);
		
		//Enable_GSensor();
		
		Enable_GSensor();
		
    while (true)
    {
        //__WFE();
				//Enable_GSensor();
				Read_Sen_datas(0x28,Test_buffer,6);
			SEGGER_RTT_printf(0,"XH:%x ",Test_buffer[0]);
			SEGGER_RTT_printf(0,"XL:%x\r\n",Test_buffer[1]);
			SEGGER_RTT_printf(0,"YH:%x ",Test_buffer[2]);
			SEGGER_RTT_printf(0,"YL:%x\r\n",Test_buffer[3]);
			SEGGER_RTT_printf(0,"ZH:%x ",Test_buffer[4]);
			SEGGER_RTT_printf(0,"ZL:%x\r\n",Test_buffer[5]);			
				//Disable_GSensor();
				nrf_delay_ms(1000);
    }
}

/** @} */
