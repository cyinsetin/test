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
 * @defgroup uart_example_main main.c
 * @{
 * @ingroup uart_example
 * @brief UART Example Application main file.
 *
 * This file contains the source code for a sample application using UART.
 * 
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "bsp.h"
//#include <spi_master.h>

//#define ENABLE_LOOPBACK_TEST  /**< if defined, then this example will be a loopback test, which means that TX should be connected to RX to get data loopback. */

#define MAX_TEST_DATA_BYTES     (15U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 1                           /**< UART RX buffer size. */

uint32_t spi_success;

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}



#ifdef ENABLE_LOOPBACK_TEST
/** @brief Function for setting the @ref ERROR_PIN high, and then enter an infinite loop.
 */
static void show_error(void)
{
    
    LEDS_ON(LEDS_MASK);
    while(true)
    {
        // Do nothing.
    }
}


/** @brief Function for testing UART loop back. 
 *  @details Transmitts one character at a time to check if the data received from the loopback is same as the transmitted data.
 *  @note  @ref TX_PIN_NUMBER must be connected to @ref RX_PIN_NUMBER)
 */
static void uart_loopback_test()
{
    uint8_t * tx_data = (uint8_t *)("\n\rLOOPBACK_TEST\n\r");
    uint8_t   rx_data;

    // Start sending one byte and see if you get the same
    for (uint32_t i = 0; i < MAX_TEST_DATA_BYTES; i++)
    {
        uint32_t err_code;
        while(app_uart_put(tx_data[i]) != NRF_SUCCESS);

        nrf_delay_ms(10);
        err_code = app_uart_get(&rx_data);

        if ((rx_data != tx_data[i]) || (err_code != NRF_SUCCESS))
        {
            show_error();
        }
    }
    return;
}


#endif


/**
 * @brief Function for main application entry.
 */


uint8_t CTRL_REG1_DATA[2]={0x20,0xA7};
uint8_t CTRL_REG2_DATA[2]={0x21,0x00};
uint8_t CTRL_REG3_DATA[2]={0x22,0x40};
uint8_t CTRL_REG4_DATA[2]={0x23,0x00};
uint8_t CTRL_REG5_DATA[2]={0x24,0x00};
uint8_t INT1_THS_DATA[2]={0x32,0x10};
uint8_t INT1_DURATION_DATA[2]={0x33,0x10};
uint8_t INT1_CFG_DATA[2]={0x30,0x0A};
uint8_t rx_data[2]={0x00,0x00};

uint8_t Test[2]={0x8F,0xFF};
uint8_t Test_Res[2]={0x00,0x00};

uint8_t tx_x1[2]={0xA8,0xFF};
uint8_t tx_x2[2]={0xA9,0xFF};
uint8_t tx_y1[2]={0xAA,0xFF};
uint8_t tx_y2[2]={0xAB,0xFF};
uint8_t tx_z1[2]={0xAC,0xFF};
uint8_t tx_z2[2]={0xAD,0xFF};
uint8_t rx_x1[2]={0x00,0x00};
uint8_t rx_x2[2]={0x00,0x00};
uint8_t rx_y1[2]={0x00,0x00};
uint8_t rx_y2[2]={0x00,0x00};
uint8_t rx_z1[2]={0x00,0x00};
uint8_t rx_z2[2]={0x00,0x00};


int main(void)
{
    
	
		uint32_t * p_spi_base_address;
		//uint32_t spi_success;

		/*	
		uint8_t CTRL_REG1_DATA[2]={0x20,0xA7};
		uint8_t CTRL_REG2_DATA[2]={0x21,0x00};
		uint8_t CTRL_REG3_DATA[2]={0x22,0x40};
		uint8_t CTRL_REG4_DATA[2]={0x23,0x00};
		uint8_t CTRL_REG5_DATA[2]={0x24,0x00};
		uint8_t INT1_THS_DATA[2]={0x32,0x10};
		uint8_t INT1_DURATION_DATA[2]={0x33,0x10};
		uint8_t INT1_CFG_DATA[2]={0x30,0x0A};
		uint8_t rx_data[2]={0x00,0x00};
		*/

		
		//p_spi_base_address = spi_master_init(SPI0, SPI_MODE0, false);
	
		//spi_success = spi_master_tx_rx(p_spi_base_address, 2, Test, Test_Res,2);
		
		/*
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, CTRL_REG1_DATA, rx_data,1);
		nrf_delay_ms(10);
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, CTRL_REG2_DATA, rx_data,1);
		nrf_delay_ms(10);
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, CTRL_REG3_DATA, rx_data,1);
		nrf_delay_ms(10);
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, CTRL_REG4_DATA, rx_data,1);
		nrf_delay_ms(10);
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, CTRL_REG5_DATA, rx_data,1);
		nrf_delay_ms(10);
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, INT1_THS_DATA, rx_data,1);
		nrf_delay_ms(10);
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, INT1_DURATION_DATA, rx_data,1);
		nrf_delay_ms(10);
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, INT1_CFG_DATA, rx_data,1);
		nrf_delay_ms(10);
		*/
		/*
		uint8_t tx_x1[2]={0xA8,0xFF};
		uint8_t tx_x2[2]={0xA9,0xFF};
		uint8_t tx_y1[2]={0xAA,0xFF};
		uint8_t tx_y2[2]={0xAB,0xFF};
		uint8_t tx_z1[2]={0xAC,0xFF};
		uint8_t tx_z2[2]={0xAD,0xFF};
		uint8_t rx_x1[2]={0x00,0x00};
		uint8_t rx_x2[2]={0x00,0x00};
		uint8_t rx_y1[2]={0x00,0x00};
		uint8_t rx_y2[2]={0x00,0x00};
		uint8_t rx_z1[2]={0x00,0x00};
		uint8_t rx_z2[2]={0x00,0x00};
		*/
		
		/*
		
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, tx_x1, rx_x1,1);
		nrf_delay_ms(10);
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, tx_x2, rx_x2,1);
		nrf_delay_ms(10);
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, tx_y1, rx_y1,1);
		nrf_delay_ms(10);
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, tx_y2, rx_y2,1);
		nrf_delay_ms(10);
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, tx_z1, rx_z1,1);
		nrf_delay_ms(10);
		spi_success = spi_master_tx_rx(p_spi_base_address, 2, tx_z2, rx_z2,1);
		nrf_delay_ms(10);
			*/
	
	
		
		LEDS_CONFIGURE(LEDS_MASK);
    LEDS_OFF(LEDS_MASK);
    uint32_t err_code;
    const app_uart_comm_params_t comm_params =
      {
          RX_PIN_NUMBER,
          TX_PIN_NUMBER,
          RTS_PIN_NUMBER,
          CTS_PIN_NUMBER,
          APP_UART_FLOW_CONTROL_ENABLED,
          false,
          UART_BAUDRATE_BAUDRATE_Baud38400
      };

    APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_error_handle,
                         APP_IRQ_PRIORITY_LOW,
                         err_code);

    APP_ERROR_CHECK(err_code);

#ifndef ENABLE_LOOPBACK_TEST
    printf("\n\rStart: \n\r");				
			
			
    while (true)
    {
        uint8_t cr;
        while(app_uart_get(&cr) != NRF_SUCCESS);
        while(app_uart_put(cr) != NRF_SUCCESS);

        if (cr == 'q' || cr == 'Q')
        {
            printf(" \n\rExit!\n\r");

            while (true)
            {
                // Do nothing.
            }
        }
    }
#else

    // This part of the example is just for testing the loopback .
    while (true)
    {
        uart_loopback_test();
    }
#endif
}


/** @} */
