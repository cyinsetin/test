/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
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
* @defgroup spi_master_example_main main.c
* @{
* @ingroup spi_master_example
*
* @brief SPI Master Loopback Example Application main file.
*
* This file contains the source code for a sample application using SPI.
*
*/

#include "nrf_delay.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "nrf_drv_spi.h"
#include "bsp.h"
#include "app_timer.h"
#include "nordic_common.h"
#include "bd_lis3dh_driver.h"
#include <nrf_assert.h>
#include "nrf_gpio.h"

#define APP_TIMER_PRESCALER      0                      ///< Value of the RTC1 PRESCALER register.
#define APP_TIMER_OP_QUEUE_SIZE  2                      ///< Size of timer operation queues.

//#define SPIM0_SCK_PIN       9     /**< SPI clock GPIO pin number. */
//#define SPIM0_MOSI_PIN      8     /**< SPI Master Out Slave In GPIO pin number. */
//#define SPIM0_MISO_PIN      3     /**< SPI Master In Slave Out GPIO pin number. */
//#define SPIM0_SS_PIN        1     /**< SPI Slave Select GPIO pin number. */

//#if 0
//#define SPI_PSELSCK1             05// 30//05//29   /*!< GPIO pin number for SPI clock              */
//#define SPI_PSELMOSI1           02// 02// 21   /*!< GPIO pin number for Master Out Slave In    */
//#define SPI_PSELMISO1            01//01// 23   /*!< GPIO pin number for Master In Slave Out    */
//#define SPI_PSELSS1              00// 00//28   /*!< GPIO pin number for Slave Select           */
//#else
//#define SPI_PSELSCK1             9 //13// 30//05//29   /*!< GPIO pin number for SPI clock              */
//#define SPI_PSELMOSI1            8 //12// 02// 21   /*!< GPIO pin number for Master Out Slave In    */
//#define SPI_PSELMISO1            3 //11//01// 23   /*!< GPIO pin number for Master In Slave Out    */
//#define SPI_PSELSS1              1 //10// 00//28   /*!< GPIO pin number for Slave Select           */
//#endif
//#define	SPI1_ENABLED	1;
#define SPI_BUFSIZE 8
volatile uint8_t   SPIMasterBuffer[SPI_BUFSIZE];
volatile uint8_t   SPISlaveBuffer[SPI_BUFSIZE];
volatile uint8_t  SPIReadLength, SPIWriteLength;
uint32_t SEL_SS_PINOUT;

//static volatile bool m_transfer_completed = true;


uint8_t		Test_byte;

/*
typedef enum
{
    #if (SPI0_ENABLED == 1)
    TEST_STATE_SPI0_LSB,    ///< Test SPI0, bits order LSB
    TEST_STATE_SPI0_MSB,    ///< Test SPI0, bits order MSB
    #endif
    #if (SPI1_ENABLED == 1)
    TEST_STATE_SPI1_LSB,    ///< Test SPI1, bits order LSB
    TEST_STATE_SPI1_MSB,    ///< Test SPI1, bits order MSB
    #endif
    #if (SPI2_ENABLED == 1)
    TEST_STATE_SPI2_LSB,    ///< Test SPI2, bits order LSB
    TEST_STATE_SPI2_MSB,    ///< Test SPI2, bits order MSB
    #endif
    END_OF_TEST_SEQUENCE
} spi_master_ex_state_t;
*/

//static spi_master_ex_state_t m_spi_master_ex_state = (spi_master_ex_state_t)0;
/*
#if (SPI0_ENABLED == 1)
static const nrf_drv_spi_t m_spi_master_0 = NRF_DRV_SPI_INSTANCE(0);
#endif
#if (SPI1_ENABLED == 1)
static const nrf_drv_spi_t m_spi_master_1 = NRF_DRV_SPI_INSTANCE(1);
#endif
#if (SPI2_ENABLED == 1)
static const nrf_drv_spi_t m_spi_master_2 = NRF_DRV_SPI_INSTANCE(2);
#endif
*/


/**
 *  SPI master operating frequency
 */
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



#define DELAY_MS                 1000                   ///< Timer Delay in milli-seconds.
#define SPI_OPERATING_FREQUENCY  ( 0x02000000UL << (uint32_t)Freq_8Mbps)  /*!< Slave clock frequency. */
#define TIMEOUT_COUNTER          0x3000UL  /*!< timeout for getting rx bytes from slave */

/** @def  TX_RX_MSG_LENGTH
 * number of bytes to transmit and receive. This amount of bytes will also be tested to see that
 * the received bytes from slave are the same as the transmitted bytes from the master */
#define TX_RX_MSG_LENGTH         100

#define LIS3DH_ODR_FREQ LIS3DH_ODR_100Hz


uint32_t* spi_master_init(SPIModuleNumber module_number, SPIMode mode, bool lsb_first)
{
    uint32_t config_mode;

    NRF_SPI_Type *spi_base_address = (SPI0 == module_number)? NRF_SPI0 : (NRF_SPI_Type *)NRF_SPI1;

    if(SPI0 == module_number) {
        /* Configure pins, frequency and mode */
        spi_base_address->PSELSCK  = SPIM0_SCK_PIN;
        spi_base_address->PSELMOSI = SPIM0_MOSI_PIN;
        spi_base_address->PSELMISO = SPIM0_MISO_PIN;
        nrf_gpio_pin_set(SPIM0_SS_PIN); /* disable Set slave select (inactive high) */
        SEL_SS_PINOUT = SPIM0_SS_PIN;
    }
//#ifdef SPI1
    else {
        /* Configure pins, frequency and mode */
        spi_base_address->PSELSCK  = SPIM1_SCK_PIN;
        spi_base_address->PSELMOSI = SPIM1_MOSI_PIN;
        spi_base_address->PSELMISO = SPIM1_MISO_PIN;
        nrf_gpio_pin_set(SPIM1_SS_PIN);         /* disable Set slave select (inactive high) */
        SEL_SS_PINOUT = SPIM1_SS_PIN;
    }
//#endif
    spi_base_address->FREQUENCY = (uint32_t) SPI_OPERATING_FREQUENCY;

    switch (mode ) {
            /*lint -e845 -save // A zero has been given as right argument to operator '!'" */
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
            /*lint -restore */
    }
    if (lsb_first) {
        /*lint -e{845} // A zero has been given as right argument to operator '|'" */
        spi_base_address->CONFIG = (config_mode | (SPI_CONFIG_ORDER_LsbFirst << SPI_CONFIG_ORDER_Pos));
    } else {
        /*lint -e{845} // A zero has been given as right argument to operator '|'" */
        spi_base_address->CONFIG = (config_mode | (SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos));
    }

    spi_base_address->EVENTS_READY = 0U;

    return (uint32_t *)spi_base_address;
}


bool spi_master_tx_rx(uint32_t *spi_base_address, uint8_t transfer_size, const uint8_t *tx_data, uint8_t *rx_data)
{
    uint32_t counter = 0;
    uint16_t number_of_txd_bytes = 0;
    uint32_t SEL_SS_PINOUT = SPIM0_SS_PIN;
    /*lint -e{826} //Are too small pointer conversion */
    NRF_SPI_Type *spi_base = (NRF_SPI_Type *)spi_base_address;

    if( (uint32_t *)NRF_SPI0 == spi_base_address) {
        SEL_SS_PINOUT = SPIM0_SS_PIN;
    }
//#ifdef SPI1
    else {
        SEL_SS_PINOUT = SPIM1_SS_PIN;
    }
//#endif
    /* enable slave (slave select active low) */
    nrf_gpio_pin_clear(SEL_SS_PINOUT);

    while(number_of_txd_bytes < transfer_size) {
        spi_base->TXD = (uint32_t)(tx_data[number_of_txd_bytes]);

        /* Wait for the transaction complete or timeout (about 10ms - 20 ms) */
        while ((spi_base->EVENTS_READY == 0U) && (counter < TIMEOUT_COUNTER)) {
            counter++;
        }

        if (counter == TIMEOUT_COUNTER) {
            /* timed out, disable slave (slave select active low) and return with error */
            nrf_gpio_pin_set(SEL_SS_PINOUT);
            return false;
        } else {   /* clear the event to be ready to receive next messages */
            spi_base->EVENTS_READY = 0U;
        }

        rx_data[number_of_txd_bytes] = (uint8_t)spi_base->RXD;
        number_of_txd_bytes++;
    };

    /* disable slave (slave select active low) */
    nrf_gpio_pin_set(SEL_SS_PINOUT);

    return true;
}

void spi_master_enable(SPIModuleNumber module_number)
{

    NRF_SPI_Type *spi_base_address = (SPI0 == module_number)? NRF_SPI0 : (NRF_SPI_Type *)NRF_SPI1;

    if(SPI0 == module_number) {
        /* Configure GPIO pins used for pselsck, pselmosi, pselmiso and pselss for SPI0 */
        nrf_gpio_cfg_output(SPIM0_SCK_PIN);
        nrf_gpio_cfg_output(SPIM0_MOSI_PIN);
        nrf_gpio_cfg_input(SPIM0_MISO_PIN, NRF_GPIO_PIN_NOPULL);
        nrf_gpio_cfg_output(SPIM0_SS_PIN);
    }
//#ifdef SPI1
    else {
        /* Configure GPIO pins used for pselsck, pselmosi, pselmiso and pselss for SPI1*/
        nrf_gpio_cfg_output(SPIM1_SCK_PIN);
        nrf_gpio_cfg_output(SPIM1_MOSI_PIN);
        nrf_gpio_cfg_input(SPIM1_MISO_PIN, NRF_GPIO_PIN_NOPULL);
        nrf_gpio_cfg_output(SPIM1_SS_PIN);
    }
//#endif
    spi_base_address->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);
}

void spi_master_disable(SPIModuleNumber module_number)
{
    NRF_SPI_Type *spi_base_address = (SPI0 == module_number)? NRF_SPI0 : (NRF_SPI_Type *)NRF_SPI1;

    if(SPI0 == module_number) {
        NRF_GPIO->PIN_CNF[SPIM0_SCK_PIN] =  (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos);
        NRF_GPIO->PIN_CNF[SPIM0_MOSI_PIN] =  (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos);
        NRF_GPIO->PIN_CNF[SPIM0_MISO_PIN] =  (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos);
        NRF_GPIO->PIN_CNF[SPIM0_SS_PIN] =  (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos);
    }
//#ifdef SPI1
    else {
        NRF_GPIO->PIN_CNF[SPIM1_SCK_PIN] =  (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos);
        NRF_GPIO->PIN_CNF[SPIM1_MOSI_PIN] =  (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos);
        NRF_GPIO->PIN_CNF[SPIM1_MISO_PIN] =  (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos);
        NRF_GPIO->PIN_CNF[SPIM1_SS_PIN] =  (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos);
    }
//#endif
    spi_base_address->ENABLE = (SPI_ENABLE_ENABLE_Disabled << SPI_ENABLE_ENABLE_Pos);

}

static uint8_t LIS3DH_Init(void)
{
    // bool init_succed;
    uint8_t whoami;

    /* Initialise SPI */
    /*!< Sample data at rising edge of clock and shift serial data at falling edge */
    /*!< MSb first */
    uint32_t *spi_base_address = spi_master_init(SPI1,SPI_MODE0,false);
    if (spi_base_address == 0) {
        return false;
    }
    spi_master_enable(SPI1);

    /* get device WHO_AM_I first to see if it exists! */
    ASSERT(LIS3DH_ReadReg(LIS3DH_WHO_AM_I,&whoami));
    Test_byte = whoami;
		ASSERT(whoami == 0x33);
		//Test_byte = whoami;
		
    ASSERT(LIS3DH_WriteReg(LIS3DH_CTRL_REG1, 0xaa));
    ASSERT(LIS3DH_WriteReg(LIS3DH_CTRL_REG2, 0xaa));
    ASSERT(LIS3DH_WriteReg(LIS3DH_CTRL_REG3, 0xaa));
    ASSERT(LIS3DH_WriteReg(LIS3DH_CTRL_REG4, 0xaa));
    ASSERT(LIS3DH_WriteReg(LIS3DH_CTRL_REG5, 0xaa));
    ASSERT(LIS3DH_WriteReg(LIS3DH_CTRL_REG6, 0xaa));
    ASSERT(LIS3DH_FIFOModeEnable(LIS3DH_FIFO_DISABLE));


    ASSERT(LIS3DH_SetAxis(LIS3DH_X_ENABLE|LIS3DH_Y_ENABLE|LIS3DH_Z_ENABLE));//
    ASSERT(LIS3DH_SetMode(LIS3DH_NORMAL));

    ASSERT(LIS3DH_SetODR(LIS3DH_ODR_FREQ));
    LIS3DH_SetFullScale(LIS3DH_FULLSCALE_4);
    ASSERT(LIS3DH_SetBDU(MEMS_ENABLE));

#ifdef FIFO_STREAM_MODE
    LIS3DH_FIFOModeEnable(LIS3DH_FIFO_STREAM_MODE);
#else

    LIS3DH_FIFOModeEnable(LIS3DH_FIFO_MODE);
#endif

    LIS3DH_SetClickCFG(0);
    LIS3DH_SetIntConfiguration(0);
    spi_master_disable(SPI1);
    return true;
}
/*
static void switch_state(void)
{
    nrf_drv_spi_t const * p_instance;

    switch (m_spi_master_ex_state)
    {
        #if (SPI0_ENABLED == 1)
        case TEST_STATE_SPI0_LSB:
            p_instance = &m_spi_master_0;
            spi_master_init(p_instance, true);
            break;

        case TEST_STATE_SPI0_MSB:
            p_instance = &m_spi_master_0;
            spi_master_init(p_instance, false);
            break;
        #endif // (SPI0_ENABLED == 1)

        #if (SPI1_ENABLED == 1)
        case TEST_STATE_SPI1_LSB:
            p_instance = &m_spi_master_1;
            spi_master_init(p_instance, true);
            break;

        case TEST_STATE_SPI1_MSB:
            p_instance = &m_spi_master_1;
            spi_master_init(p_instance, false);
            break;
        #endif // (SPI1_ENABLED == 1)

        #if (SPI2_ENABLED == 1)
        case TEST_STATE_SPI2_LSB:
            p_instance = &m_spi_master_2;
            spi_master_init(p_instance, true);
            break;

        case TEST_STATE_SPI2_MSB:
            p_instance = &m_spi_master_2;
            spi_master_init(p_instance, false);
            break;
        #endif // (SPI2_ENABLED == 1)

        default:
            return;
    }
    if (++m_spi_master_ex_state >= END_OF_TEST_SEQUENCE)
    {
        m_spi_master_ex_state = (spi_master_ex_state_t)0;
    }

    spi_send_recv(p_instance, m_tx_data_spi, m_rx_data_spi, TX_RX_MSG_LENGTH);
}

*/

/**@brief Function for initializing bsp module.
 */
void bsp_configuration()
{
    uint32_t err_code = NRF_SUCCESS;

    NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART    = 1;

    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
        // Do nothing.
    }

    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, NULL);

    err_code = bsp_init(BSP_INIT_LED, APP_TIMER_TICKS(100, APP_TIMER_PRESCALER), NULL);
    APP_ERROR_CHECK(err_code);
}


/** @brief Function for main application entry.
 */


int main(void)
{
    
//		uint8_t	test;	
		// Setup bsp module.
		
    bsp_configuration();
		nrf_gpio_cfg_output(SPIM1_SCK_PIN);
		nrf_gpio_cfg_output(SPIM1_SS_PIN);
	  nrf_gpio_cfg_output(SPIM1_MOSI_PIN);
		//nrf_gpio_cfg_input(SPIM1_MISO_PIN,1);
		nrf_gpio_pin_clear(BSP_LED_0);
		#if (SPI0_ENABLED == 1) || (SPI1_ENABLED == 1) || (SPI2_ENABLED == 1)
		{
			nrf_delay_ms(DELAY_MS);
			LIS3DH_Init();
		}
		#endif
		nrf_delay_ms(DELAY_MS);
		//nrf_gpio_pin_set(BSP_LED_0);
		//spi_master_enable(SPI0);
		//LIS3DH_Init();
		//ASSERT(LIS3DH_ReadReg(LIS3DH_WHO_AM_I,&whoami));
		//LIS3DH_ReadReg(LIS3DH_WHO_AM_I,&Test_byte);
		nrf_delay_ms(DELAY_MS);
		//spi_master_disable(SPI0);
		
	
    for (;;)
    {
        /*
				#if (SPI0_ENABLED == 1) || (SPI1_ENABLED == 1) || (SPI2_ENABLED == 1)
        if (m_transfer_completed)
        {
            m_transfer_completed = false;

            switch_state();
            nrf_delay_ms(DELAY_MS);
        }
        #endif // (SPI0_ENABLED == 1) || (SPI1_ENABLED == 1) || (SPI2_ENABLED == 1)
				*/
				LIS3DH_Init();
				//nrf_delay_ms(10);
				
				//nrf_gpio_pin_set(SPIM1_SS_PIN);
				//nrf_gpio_pin_set(SPI1_CONFIG_MOSI_PIN);
				nrf_gpio_pin_clear(BSP_LED_0);
				nrf_delay_ms(100);
				nrf_gpio_pin_set(BSP_LED_0);
				//nrf_gpio_pin_clear(SPIM1_SS_PIN);
				//nrf_gpio_pin_clear(SPI1_CONFIG_MOSI_PIN);
				nrf_delay_ms(100);
				
    }
}

/** @} */
