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
#include "nrf_gpio.h"
#include "myspi.h"
#include "myio_def.h"
#define APP_TIMER_PRESCALER      0                      ///< Value of the RTC1 PRESCALER register.
#define APP_TIMER_OP_QUEUE_SIZE  2                      ///< Size of timer operation queues.

uint8_t TestByte;
uint8_t TestByte1;
uint8_t TestByte2;
uint8_t	Data_Moniter[6];

#define DELAY_MS                 1000                   ///< Timer Delay in milli-seconds.
/*
#define    SPI_G-Sensor_CS    12
#define    SPI_G-Sensor_CLK   13
#define    SPI_G-Sensor_MOSI  14
#define    SPI_G-Sensor_MISO  15

#define    SPI_G-Sensor_Enable_CS    nrf_gpio_pin_clear(SPI_FLASH_CS)   //片选有效
#define    SPI_G-Sensor_Disable_CS   nrf_gpio_pin_set(SPI_FLASH_CS)     //片选无效
#define    SPI_G-Sensor_Set_SCLK     nrf_gpio_pin_set(SPI_FLASH_CLK)    //时钟拉高
#define    SPI_G-Sensor_Clr_SCLK     nrf_gpio_pin_clear(SPI_FLASH_CLK)  //时钟置低
#define    SPI_G-Sensor_Set_DO       nrf_gpio_pin_set(SPI_FLASH_MOSI)   //MOSI拉高
#define    SPI_G-Sensor_Clr_DO       nrf_gpio_pin_clear(SPI_FLASH_MOSI) //MOSI置低
#define    SPI_G-Sensor_Get_DI       nrf_gpio_pin_read(SPI_FLASH_MISO)	

#define SPIS_MISO_PIN  28    // SPI MISO signal. 
#define SPIS_CSN_PIN   12    // SPI CSN signal. 
#define SPIS_MOSI_PIN  25    // SPI MOSI signal. 
#define SPIS_SCK_PIN   29    // SPI SCK signal. 

#define SPIM0_SCK_PIN       4     /< SPI clock GPIO pin number. 
#define SPIM0_MOSI_PIN      1     /< SPI Master Out Slave In GPIO pin number. 
#define SPIM0_MISO_PIN      3     /< SPI Master In Slave Out GPIO pin number. 
#define SPIM0_SS_PIN        2     /< SPI Slave Select GPIO pin number. 
*/

/*  SPI1 */

//#if 0
///#define SPI_PSELSCK1             05// 30//05//29   /*!< GPIO pin number for SPI clock              */
//#define SPI_PSELMOSI1           02// 02// 21   /*!< GPIO pin number for Master Out Slave In    */
//#define SPI_PSELMISO1            01//01// 23   /*!< GPIO pin number for Master In Slave Out    */
//#define SPI_PSELSS1              00// 00//28   /*!< GPIO pin number for Slave Select           */
//#else
//#define SPI_PSELSCK1             9 //13// 30//05//29   /*!< GPIO pin number for SPI clock              */
//#define SPI_PSELMOSI1            8 //12// 02// 21   /*!< GPIO pin number for Master Out Slave In    */
//#define SPI_PSELMISO1            3 //11//01// 23   /*!< GPIO pin number for Master In Slave Out    */
//#define SPI_PSELSS1              1 //10// 00//28   /*!< GPIO pin number for Slave Select           */
//#endif



/** @def  TX_RX_MSG_LENGTH
 * number of bytes to transmit and receive. This amount of bytes will also be tested to see that
 * the received bytes from slave are the same as the transmitted bytes from the master */
#define TX_RX_MSG_LENGTH         100

#if (SPI0_ENABLED == 1) || (SPI1_ENABLED == 1) || (SPI2_ENABLED == 1)

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

static uint8_t m_tx_data_spi[TX_RX_MSG_LENGTH]; ///< SPI master TX buffer.
static uint8_t m_rx_data_spi[TX_RX_MSG_LENGTH]; ///< SPI master RX buffer.

static volatile bool m_transfer_completed = true;
static spi_master_ex_state_t m_spi_master_ex_state = (spi_master_ex_state_t)0;

#if (SPI0_ENABLED == 1)
static const nrf_drv_spi_t m_spi_master_0 = NRF_DRV_SPI_INSTANCE(0);
#endif
#if (SPI1_ENABLED == 1)
static const nrf_drv_spi_t m_spi_master_1 = NRF_DRV_SPI_INSTANCE(1);
#endif
#if (SPI2_ENABLED == 1)
static const nrf_drv_spi_t m_spi_master_2 = NRF_DRV_SPI_INSTANCE(2);
#endif


/**@brief Function for error handling, which is called when an error has occurred. 
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name. 
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    UNUSED_VARIABLE(bsp_indication_set(BSP_INDICATE_FATAL_ERROR));

    for (;;)
    {
        // No implementation needed.
    }
}


/**@brief The function initializes TX buffer to values to be sent and clears RX buffer.
 *
 * @note Function initializes TX buffer to values from 0 to (len - 1).
 *       and clears RX buffer (fill by 0).
 *
 * @param[out] p_tx_data    A pointer to a buffer TX.
 * @param[out] p_rx_data    A pointer to a buffer RX.
 * @param[in] len           A length of the data buffers.
 */
static void init_buf(uint8_t * const p_tx_buf,
                     uint8_t * const p_rx_buf,
                     const uint16_t  len)
{
    uint16_t i;

    for (i = 0; i < len; i++)
    {
        p_tx_buf[i] = i;
        p_rx_buf[i] = 0;
    }
}


/**@brief Function for checking if buffers are equal.
 *
 * @note Function compares each element of p_tx_buf with p_rx_buf.
 *
 * @param[in] p_tx_data     A pointer to a buffer TX.
 * @param[in] p_rx_data     A pointer to a buffer RX.
 * @param[in] len           A length of the data buffers.
 *
 * @retval true     Buffers are equal.
 * @retval false    Buffers are different.
 */
static bool check_buf_equal(const uint8_t * const p_tx_buf,
                            const uint8_t * const p_rx_buf,
                            const uint16_t        len)
{
    uint16_t i;

    for (i = 0; i < len; i++)
    {
        if (p_tx_buf[i] != p_rx_buf[i])
        {
            return false;
        }
    }
    return true;
}


#if (SPI0_ENABLED == 1)
/**@brief Handler for SPI0 master events.
 *
 * @param[in] event SPI master event.
 */
void spi_master_0_event_handler(nrf_drv_spi_event_t event)
{
    uint32_t err_code = NRF_SUCCESS;
    bool result = false;

    switch (event)
    {
        case NRF_DRV_SPI_EVENT_DONE:
            // Check if received data is correct.
            result = check_buf_equal(m_tx_data_spi, m_rx_data_spi, TX_RX_MSG_LENGTH);
            APP_ERROR_CHECK_BOOL(result);

            nrf_drv_spi_uninit(&m_spi_master_0);

            err_code = bsp_indication_set(BSP_INDICATE_RCV_OK);
            APP_ERROR_CHECK(err_code);

            m_transfer_completed = true;
            break;

        default:
            // No implementation needed.
            break;
    }
}
#endif // (SPI0_ENABLED == 1)


#if (SPI1_ENABLED == 1)
/**@brief Handler for SPI1 master events.
 *
 * @param[in] event SPI master event.
 */
void spi_master_1_event_handler(nrf_drv_spi_event_t event)
{
    uint32_t err_code = NRF_SUCCESS;
    bool result = false;

    switch (event)
    {
        case NRF_DRV_SPI_EVENT_DONE:
            // Check if received data is correct.
            result = check_buf_equal(m_tx_data_spi, m_rx_data_spi, TX_RX_MSG_LENGTH);
            APP_ERROR_CHECK_BOOL(result);

            nrf_drv_spi_uninit(&m_spi_master_1);

            err_code = bsp_indication_set(BSP_INDICATE_RCV_OK);
            APP_ERROR_CHECK(err_code);

            m_transfer_completed = true;
            break;

        default:
            // No implementation needed.
            break;
    }
}
#endif // (SPI1_ENABLED == 1)


#if (SPI2_ENABLED == 1)
/**@brief Handler for SPI2 master events.
 *
 * @param[in] event SPI master event.
 */
void spi_master_2_event_handler(nrf_drv_spi_event_t event)
{
    uint32_t err_code = NRF_SUCCESS;
    bool result = false;

    switch (event)
    {
        case NRF_DRV_SPI_EVENT_DONE:
            // Check if received data is correct.
            result = check_buf_equal(m_tx_data_spi, m_rx_data_spi, TX_RX_MSG_LENGTH);
            APP_ERROR_CHECK_BOOL(result);

            nrf_drv_spi_uninit(&m_spi_master_2);

            err_code = bsp_indication_set(BSP_INDICATE_RCV_OK);
            APP_ERROR_CHECK(err_code);

            m_transfer_completed = true;
            break;

        default:
            // No implementation needed.
            break;
    }
}
#endif // (SPI2_ENABLED == 1)


/**@brief Function for initializing a SPI master driver.
 *
 * @param[in] p_instance    Pointer to SPI master driver instance.
 * @param[in] lsb           Bits order LSB if true, MSB if false.
 */
static void spi_master_init(nrf_drv_spi_t const * p_instance, bool lsb)
{
    uint32_t err_code = NRF_SUCCESS;

    nrf_drv_spi_config_t config =
    {
        .ss_pin       = NRF_DRV_SPI_PIN_NOT_USED,
        .irq_priority = APP_IRQ_PRIORITY_LOW,
        .orc          = 0xCC,
        .frequency    = NRF_DRV_SPI_FREQ_1M,
        .mode         = NRF_DRV_SPI_MODE_0,
        .bit_order    = (lsb ?
            NRF_DRV_SPI_BIT_ORDER_LSB_FIRST : NRF_DRV_SPI_BIT_ORDER_MSB_FIRST),
    };

    #if (SPI0_ENABLED == 1)
    if (p_instance == &m_spi_master_0)
    {
        config.sck_pin  = SPIM0_SCK_PIN;
        config.mosi_pin = SPIM0_MOSI_PIN;
        config.miso_pin = SPIM0_MISO_PIN;
        err_code = nrf_drv_spi_init(p_instance, &config,
            spi_master_0_event_handler);
    }
    else
    #endif // (SPI0_ENABLED == 1)

    #if (SPI1_ENABLED == 1)
    if (p_instance == &m_spi_master_1)
    {
        config.sck_pin  = SPIM1_SCK_PIN;
        config.mosi_pin = SPIM1_MOSI_PIN;
        config.miso_pin = SPIM1_MISO_PIN;
        err_code = nrf_drv_spi_init(p_instance, &config,
            spi_master_1_event_handler);
    }
    else
    #endif // (SPI1_ENABLED == 1)

    #if (SPI2_ENABLED == 1)
    if (p_instance == &m_spi_master_2)
    {
        config.sck_pin  = SPIM2_SCK_PIN;
        config.mosi_pin = SPIM2_MOSI_PIN;
        config.miso_pin = SPIM2_MISO_PIN;
        err_code = nrf_drv_spi_init(p_instance, &config,
            spi_master_2_event_handler);
    }
    else
    #endif // (SPI2_ENABLED == 1)

    {}

    APP_ERROR_CHECK(err_code);
}


/**@brief Function for sending and receiving data.
 *
 * @param[in]   p_instance   Pointer to SPI master driver instance.
 * @param[in]   p_tx_data    A pointer to a buffer TX.
 * @param[out]  p_rx_data    A pointer to a buffer RX.
 * @param[in]   len          A length of the data buffers.
 */
static void spi_send_recv(nrf_drv_spi_t const * p_instance,
                          uint8_t * p_tx_data,
                          uint8_t * p_rx_data,
                          uint16_t  len)
{
    // Initalize buffers.
    init_buf(p_tx_data, p_rx_data, len);

    uint32_t err_code = nrf_drv_spi_transfer(p_instance,
        p_tx_data, len, p_rx_data, len);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for executing and switching state.
 *
 */
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

#endif // (SPI0_ENABLED == 1) || (SPI1_ENABLED == 1) || (SPI2_ENABLED == 1)


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

//uint8_t LIS3DH_ReadOneByte(void);


void HW_Initial(void)
{
		nrf_gpio_cfg_output(SPI_LIS3DH_CS);
		nrf_gpio_cfg_output(SPI_LIS3DH_CLK);
		nrf_gpio_cfg_output(SPI_LIS3DH_MOSI);
		nrf_gpio_cfg_input(SPI_LIS3DH_MISO,NRF_GPIO_PIN_PULLUP);
		
		LIS3DH_Disable_CS;
		LIS3DH_Set_SCLK;
		LIS3DH_Set_DO;
		LIS3DH_Get_DI;
}


/** @brief Function for main application entry.
 */

int main(void)
{
    // Setup bsp module.
    bsp_configuration();
		HW_Initial();
		//LIS3DH_WriteOneByte(0x0f);
		TestByte=LIS3DH_Read_Reg(0x0f);			//获取ID
		
		
		
		//***LIS3DH初始化***//
		
		LIS3DH_Write_Reg(0x20,0x4f);
		LIS3DH_Write_Reg(0x22,0x00);
		LIS3DH_Write_Reg(0x23,0x00);
		LIS3DH_Write_Reg(0x24,0x00);
		LIS3DH_Write_Reg(0x21,0x04);
		
		//***读LIS3DH值，X,Y,Z三轴方向值，高位和低位***//
		//for(uint8_t i=0;i<6;i++)
		//	Data_Moniter[i]=LIS3DH_MultipleRead_Reg(0x28+i);
	  LIS3DH_Multiple_Read_Reg(0x28,&Data_Moniter[0],6);
		//Data_Moniter[0]=LIS3DH_Read_Reg(0x28);
		
		for(;;)
    {
			//TestByte=LIS3DH_ReadOneByte(0x0f);			//获取ID
    }
}

/** @} */
