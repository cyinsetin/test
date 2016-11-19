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
 *
 * @defgroup ble_sdk_uart_over_ble_main main.c
 * @{
 * @ingroup  ble_sdk_app_nus_eval
 * @brief    UART over BLE application main file.
 *
 * This file contains the source code for a sample application that uses the Nordic UART service.
 * This application uses the @ref srvlib_conn_params module.
 */

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "app_button.h"

#include "ble_nus.h"
#include "ble_bas.h"
#include "ble_dis.h"
#include "app_uart.h"
#include "app_util_platform.h"
#include "bsp.h"
#include "bsp_btn_ble.h"

/**********cyinstein******************/
#include "global.h"
#include "motor.h"
//#include "uart.h"
#include "key.h"
#include "adc.h"
#include "rtc.h"
//#include "clock.h"
//uint8_t SERIAL_NUMBER[]="0958230966";
//const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(1); /**< Declaring an instance of nrf_drv_rtc for RTC1. */
//const nrf_drv_timer_t Hand_Control = NRF_DRV_TIMER_INSTANCE(1);  //创建一个定时器1的实例
//const nrf_drv_timer_t Button_Control = NRF_DRV_TIMER_INSTANCE(2);  //创建一个定时器2的实例
#define BOOTLOADER_DFU_START  0xB1
/*************************************/

#define IS_SRVC_CHANGED_CHARACT_PRESENT 0                                           /**< Include the service_changed characteristic. If not enabled, the server's database cannot be changed for the lifetime of the device. */

#define DEVICE_NAME                     "CT-003V001"                               /**< Name of device. Will be included in the advertising data. */
//uint8_t DEVICE_NAME[] = "ST-003V001";
#define MANUFACTURER_NAME               "Cyinstein"                      /**< Manufacturer. Will be passed to Device Information Service. */
#define HW_VERSION											"V0.30"
//#define FW_VERSION											"V0.10"
uint8_t FW_VERSION[] = "ST-003V001";
#define SW_VERSION											"V0.10"
#define SERIAL_NUMBER										"1234567890"
#define MODULE_NUMBER										"CT-003/004"
#define AUTHOR_NAME											"Stanley Huang"
#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS      180                                         /**< The advertising timeout (in units of seconds). */

//#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
//#define APP_TIMER_OP_QUEUE_SIZE         4                                           /**< Size of timer operation queues. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(75, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)  /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER) /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define START_STRING                    "Start...\n"                                /**< The string that will be sent over the UART when the application starts. */

#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define UART_TX_BUF_SIZE                256                                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE                256                                         /**< UART RX buffer size. */

static ble_nus_t                        m_nus;   
static ble_bas_t                        m_bas;                                     /**< Structure used to identify the battery service. */

static uint16_t                         m_conn_handle = BLE_CONN_HANDLE_INVALID;    /**< Handle of the current connection. */

static ble_uuid_t                       m_adv_uuids[] = {{BLE_UUID_NUS_SERVICE, NUS_SERVICE_UUID_TYPE}};  /**< Universally unique service identifier. */
ble_bas_init_t bas_init;
//static void on_bas_evt(ble_bas_t * p_bas, ble_bas_evt_t * p_evt);

/**@brief Function for assert macro callback.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyse 
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num    Line number of the failing ASSERT call.
 * @param[in] p_file_name File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


/**@brief Function for the GAP initialization.
 *
 * @details This function will set up all the necessary GAP (Generic Access Profile) parameters of 
 *          the device. It also sets the permissions and appearance.
 */
static void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    
    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *) DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling the data from the Nordic UART Service.
 *
 * @details This function will process the data received from the Nordic UART BLE Service and send
 *          it to the UART module.
 *
 * @param[in] p_nus    Nordic UART Service structure.
 * @param[in] p_data   Data to be send to UART module.
 * @param[in] length   Length of the data.
 */
static void send_data_check_sum(uint8_t * p1_data,uint8_t datalen,uint8_t * sum1,uint8_t * sum2)
{
	uint8_t i,len,ck_a,ck_b;
	len=datalen;
	ck_a=0;
	ck_b=0;
	for (i=0;i<len;i++) 
	{
		ck_a+=p1_data[i];
		ck_b+=ck_a;
	}
	*sum1=ck_a;
	*sum2=ck_b;
}
static void RX_data_check_sum(uint8_t * p1_data,uint8_t * sum1,uint8_t * sum2)
{
	uint8_t i,len,ck_a,ck_b;
	len=p1_data[4];
	ck_a=0;
	ck_b=0;
	for (i=0;i<len+4;i++) 
	{
		ck_a=ck_a + p1_data[i+2];
		ck_b=(uint8_t)ck_b + (uint8_t)ck_a;
	}
	*sum1=ck_a;
	*sum2=ck_b;
}

static void cmd_ack(void)
{
	uint32_t err_code;
	BT_TxBuf[1]=0x06;
	bt_tx_len=4;
	send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
	err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
}
/**@brief Function for handling the data from the Nordic UART Service.
 *
 * @details This function will process the data received from the Nordic UART BLE Service and send
 *          it to the UART module.
 *
 * @param[in] p_nus    Nordic UART Service structure.
 * @param[in] p_data   Data to be send to UART module.
 * @param[in] length   Length of the data.
 */
/**@snippet [Handling the data received over BLE] */
/*
static void nus_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
        while(app_uart_put(p_data[i]) != NRF_SUCCESS);
    }
    while(app_uart_put('\n') != NRF_SUCCESS);
}
*/
static void ble_stack_disable(void)
{
	uint32_t err_code;
	err_code = sd_nvic_DisableIRQ(SWI2_IRQn);
	APP_ERROR_CHECK(err_code);
	err_code=softdevice_handler_sd_disable();
	APP_ERROR_CHECK(err_code);
}
void reset_prepare(void)
{
	uint32_t err_code;
	err_code = sd_ble_gap_disconnect(m_conn_handle,BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
	APP_ERROR_CHECK(err_code);
	err_code=ble_conn_params_stop();
	APP_ERROR_CHECK(err_code);
	sd_ble_gap_adv_stop();
	ble_stack_disable();
	nrf_delay_ms(1000);
}
static void nus_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length)
{
		uint8_t i,err_f,len,cmd;
	  uint8_t ck_a;
	  uint8_t ck_b;
	  uint8_t tmp_cka;
	  uint8_t tmp_ckb;
		//uint8_t len;
		uint8_t j;
	  uint32_t err_code;	

//		uint32_t Tx_data_length;
//	  static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
//    static uint8_t index = 0;
/*	
    for (uint32_t i = 0; i < length; i++)
    {
        while(app_uart_put(p_data[i]) != NRF_SUCCESS);
    }
    while(app_uart_put('\n') != NRF_SUCCESS);
	*/
	
		err_f=false;

    for (uint32_t i = 0; i < length; i++)
    {
        while(app_uart_put(p_data[i]) != NRF_SUCCESS);
    }
		//while(app_uart_put('\n') != NRF_SUCCESS);
			//err_code = ble_nus_string_send(&m_nus, p_data, length);
			err_f=false;
			if (p_data[0]!=Watch_header_H) err_f=true;
			if (p_data[1]!=Watch_header_L) err_f=true;
			if (p_data[2]!=Watch_ID_H) err_f=true;
		  len=length;
		  tmp_cka=p_data[len-2];
		  tmp_ckb=p_data[len-1];
			if (err_f==false) {
										RX_data_check_sum(p_data,&ck_a,&ck_b);	
                    if(ck_a==tmp_cka)
										  {
												if(ck_b==tmp_ckb)
												{
													err_f=false;
													//err_code = ble_nus_string_send(&m_nus, ANS_NAK, 7);
												}
												else
											  {
												 err_f=true;
												 //err_code = ble_nus_string_send(&m_nus, ANS_NAK, 7);
												 BT_TxBuf[0]=0x07; 
												 BT_TxBuf[1]=0x0F;
												 bt_tx_len=4;
												 //send_data_check_sum(SN,&BT_TxBuf[bt_tx_len-2],&BT_TxBuf[bt_tx_len-1]);
												 send_data_check_sum(ANS_NAK,2,&BT_TxBuf[bt_tx_len-2],&BT_TxBuf[bt_tx_len-1]);
												 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
											  }
											}
											else
											{
												err_f=true;
											}

									}
			else 
			{
						err_code = ble_nus_string_send(&m_nus, ANS_NAK, 7);
			}
			if(err_f==false)
			{
/*******command ok,run function***************************/
				//BT_TxBuf[0]=Watch_header_H;
				//BT_TxBuf[1]=Watch_header_L;
				//BT_TxBuf[2]=Watch_ID_H;
				BT_TxBuf[0]=0x07;
				//BT_TxBuf[1]=Watch_header_L;
				//BT_TxBuf[2]=Watch_ID_H;
			  cmd=p_data[3];
				switch (cmd) {
											case 0x00:	//Get Device S/N
														BT_TxBuf[1]=0x00;
														bt_tx_len=20;
														for(i=0;i<bt_tx_len-4;i++)
															{
																BT_TxBuf[i+2]=SN[i];
															}
														send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
											      err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
													break;
											case 0x01:	//Send Message
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
													break;
											case 0x02:	//Set Watch Time
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
													break;
											case 0x03:	//Set Watch Date
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
													break;
											case 0x04:	//Set Watch Clock
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
												break;
											case 0x05:	//Get FW Ver
														//FW_VER
													 BT_TxBuf[1]=5;
											     bt_tx_len=9;
													 //bt_tx_len=2+strlen((const char *)FW_VERSION)+2;
													 for(i=0;i<strlen((const char *)FW_VERSION);i++)
													 {
													 	BT_TxBuf[i+2]=FW_VERSION[i];
													 }
													 send_data_check_sum(FW_VERSION,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
											     err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
												break;
											case 0x06:	//ACK
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
												break;
											case 0x07:	//Set Device Name
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
												break;
											case 0x08:	//Get Device Name
														BT_TxBuf[1]=8;
														bt_tx_len=2+strlen(DEVICE_NAME)+2;
														for(i=0;i<strlen(DEVICE_NAME);i++)
															{
																BT_TxBuf[i+2]=DEVICE_NAME[i];
															}
														send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
											      err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
												break;
										
											case 0x09: 	//System Type
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, 8);
												break;
											case 0x0B:	//Anti-lost
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
												break;
											case 0x0C:	//Get Pedometer Data
														for(j=0;j<bt_rx_len;j++)
															{
																//BT_RxBuf[j];
															}
														err_code = ble_nus_string_send(&m_nus, BT_RxBuf, bt_rx_len);
												break;
											case 0x0D:	//Get Sleep Data

												break;
											case 0x0E:	//Reset Watch Data
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
												break;
											case 0x0F:	//NAK
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
												break;
											case 0x12:	//find cell phone
												
												break;
										
											case 0x13:	//Get BATT

												   BT_TxBuf[1]=0x13;
											     BT_TxBuf[2]=80;
													 bt_tx_len=5;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
												break;
											case 0x14:	//Adj Hour Hand
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
												break;
											case 0x15:	//Adj Minute Hand
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
												break;
											case 0x16:	//Adj Second Hand
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
											break;
										case 0x17:	//Adj Message Hand
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
											break;
										case 0x18:	//Adj Pedometer Hand
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
											break;
										case 0x19:	//Enter Hand Adj Mode
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
											break;
										case 0x20:	//Erase Pedometer
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
											break;
										case 0x21:	//Erase Sleep Data
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
											break;
										case 0x81:	//Set Watch ID
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
											break;
										case 0xDF:
													err_code=sd_power_gpregret_clr(0xff);
													APP_ERROR_CHECK(err_code);
													err_code=sd_power_gpregret_set(p_data[0]);
													APP_ERROR_CHECK(err_code);
													reset_prepare();
													NVIC_SystemReset();
											break;
										case 0xE0:	//Erase Sleep Data
												   BT_TxBuf[1]=0x06;
													 bt_tx_len=4;
												   send_data_check_sum(SN,2,&BT_TxBuf[bt_tx_len-1],&BT_TxBuf[bt_tx_len-2]);
													 err_code = ble_nus_string_send(&m_nus, BT_TxBuf, bt_tx_len);
											break;
										case 0xEE:	//Terminate BLE
										
													// Disconnect
													//GAPRole_TerminateConnection();
													//adv_enabled = FALSE;
													// Stop advertising
													//GAPRole_SetParameter( GAPROLE_ADVERT_ENABLED, sizeof( uint8 ), &adv_enabled );				
											break;
										case 0xF0:
											break;
										case 0xF1:
											break;
										default:
											break;

									}
								}
}
/**@snippet [Handling the data received over BLE] */


/**@brief Function for initializing the Battery Service.
 */
/*
static void bas_init(void)
{
    uint32_t       err_code;
    ble_bas_init_t bas_init_obj;

    memset(&bas_init_obj, 0, sizeof(bas_init_obj));

    bas_init_obj.evt_handler          = on_bas_evt;
    bas_init_obj.support_notification = true;
    bas_init_obj.p_report_ref         = NULL;
    bas_init_obj.initial_batt_level   = 100;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init_obj.battery_level_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init_obj.battery_level_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&bas_init_obj.battery_level_char_attr_md.write_perm);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init_obj.battery_level_report_read_perm);

    err_code = ble_bas_init(&m_bas, &bas_init_obj);
    APP_ERROR_CHECK(err_code);
}
*/

/**@brief Function for initializing services that will be used by the application.
 */
static void services_init(void)
{
    uint32_t       err_code;
    ble_nus_init_t nus_init;
    //ble_bas_init_t bas_init;
    ble_dis_init_t dis_init;
	
    memset(&nus_init, 0, sizeof(nus_init));

    nus_init.data_handler = nus_data_handler;
    
    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);

    // Initialize Battery Service.
		//bas_init();

    memset(&bas_init, 0, sizeof(bas_init));

    // Here the sec level for the Battery Service can be changed/increased.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&bas_init.battery_level_char_attr_md.write_perm);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_report_read_perm);

    bas_init.evt_handler          = NULL;
    bas_init.support_notification = true;
    bas_init.p_report_ref         = NULL;
    bas_init.initial_batt_level   = 50;

    err_code = ble_bas_init(&m_bas, &bas_init);
    APP_ERROR_CHECK(err_code);

    // Initialize Device Information Service.
		
    memset(&dis_init, 0, sizeof(dis_init));

    ble_srv_ascii_to_utf8(&dis_init.manufact_name_str, (char *)MANUFACTURER_NAME);
    ble_srv_ascii_to_utf8(&dis_init.hw_rev_str, (char *)HW_VERSION);
    ble_srv_ascii_to_utf8(&dis_init.fw_rev_str, (char *)FW_VERSION);
    ble_srv_ascii_to_utf8(&dis_init.sw_rev_str, (char *)SW_VERSION);
    ble_srv_ascii_to_utf8(&dis_init.serial_num_str, (char *)SERIAL_NUMBER);
    ble_srv_ascii_to_utf8(&dis_init.model_num_str, (char *)MODULE_NUMBER);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dis_init.dis_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&dis_init.dis_attr_md.write_perm);

    err_code = ble_dis_init(&dis_init);
    APP_ERROR_CHECK(err_code);
    //memset(&nus_init, 0, sizeof(nus_init));

    //nus_init.data_handler = nus_data_handler;
    
    //err_code = ble_nus_init(&m_nus, &nus_init);
    //APP_ERROR_CHECK(err_code);
		
}


/**@brief Function for handling an event from the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module
 *          which are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply setting
 *       the disconnect_on_fail config parameter, but instead we use the event handler
 *       mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;
    
    if(p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief Function for handling errors from the Connection Parameters module.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;
    
    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;
    
    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
static void sleep_mode_enter(void)
{
    uint32_t err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);

    // Prepare wakeup buttons.
    err_code = bsp_btn_ble_sleep_mode_prepare();
    APP_ERROR_CHECK(err_code);

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    err_code = sd_power_system_off();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    uint32_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);
            break;
        case BLE_ADV_EVT_IDLE:
            sleep_mode_enter();
            break;
        default:
            break;
    }
}


/**@brief Function for the Application's S110 SoftDevice event handler.
 *
 * @param[in] p_ble_evt S110 SoftDevice event.
 */
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t                         err_code;
    
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            APP_ERROR_CHECK(err_code);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break;
            
        case BLE_GAP_EVT_DISCONNECTED:
            err_code = bsp_indication_set(BSP_INDICATE_IDLE);
            APP_ERROR_CHECK(err_code);
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for dispatching a S110 SoftDevice event to all modules with a S110 SoftDevice 
 *        event handler.
 *
 * @details This function is called from the S110 SoftDevice event interrupt handler after a S110 
 *          SoftDevice event has been received.
 *
 * @param[in] p_ble_evt  S110 SoftDevice event.
 */
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    ble_conn_params_on_ble_evt(p_ble_evt);
    ble_nus_on_ble_evt(&m_nus, p_ble_evt);
    on_ble_evt(p_ble_evt);
    ble_advertising_on_ble_evt(p_ble_evt);
    bsp_btn_ble_on_ble_evt(p_ble_evt);
    
}


/**@brief Function for the S110 SoftDevice initialization.
 *
 * @details This function initializes the S110 SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    uint32_t err_code;
    
    // Initialize SoftDevice.
    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, NULL);

    // Enable BLE stack.
    ble_enable_params_t ble_enable_params;
    memset(&ble_enable_params, 0, sizeof(ble_enable_params));
#if (defined(S130) || defined(S132))
    ble_enable_params.gatts_enable_params.attr_tab_size   = BLE_GATTS_ATTR_TAB_SIZE_DEFAULT;
#endif
    ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;
    err_code = sd_ble_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);
    
    // Subscribe for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling events from the BSP module.
 *
 * @param[in]   event   Event generated by button press.
 */
void bsp_event_handler(bsp_event_t event)
{
    uint32_t err_code;
    switch (event)
    {
        case BSP_EVENT_SLEEP:
            sleep_mode_enter();
            break;

        case BSP_EVENT_DISCONNECT:
            err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            if (err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BSP_EVENT_WHITELIST_OFF:
            err_code = ble_advertising_restart_without_whitelist();
            if (err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
            break;

        default:
            break;
    }
}


/**@brief   Function for handling app_uart events.
 *
 * @details This function will receive a single character from the app_uart module and append it to 
 *          a string. The string will be be sent over BLE when the last character received was a 
 *          'new line' i.e '\n' (hex 0x0D) or if the string has reached a length of 
 *          @ref NUS_MAX_DATA_LENGTH.
 */
/**@snippet [Handling the data received over UART] */
void uart_event_handle(app_uart_evt_t * p_event)
{
    static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
    static uint8_t index = 0;
    uint32_t       err_code;

    switch (p_event->evt_type)
    {
        case APP_UART_DATA_READY:
            UNUSED_VARIABLE(app_uart_get(&data_array[index]));
            index++;

            if ((data_array[index - 1] == '\n') || (index >= (BLE_NUS_MAX_DATA_LEN)))
            {
                err_code = ble_nus_string_send(&m_nus, data_array, index);
                if (err_code != NRF_ERROR_INVALID_STATE)
                {
                    //APP_ERROR_CHECK(err_code);
                }
                
                index = 0;
            }
            break;

        case APP_UART_COMMUNICATION_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_communication);
            break;

        case APP_UART_FIFO_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_code);
            break;

        default:
            break;
    }
}
/**@snippet [Handling the data received over UART] */


/**@brief  Function for initializing the UART module.
 */
/**@snippet [UART Initialization] */

static void uart_init(void)
{
    uint32_t                     err_code;
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

    APP_UART_FIFO_INIT( &comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       uart_event_handle,
                       APP_IRQ_PRIORITY_LOW,
                       err_code);
    APP_ERROR_CHECK(err_code);
}

/**@snippet [UART Initialization] */


/**@brief Function for initializing the Advertising functionality.
 */
static void advertising_init(void)
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;

    // Build advertising data struct to pass into @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = false;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;

    memset(&scanrsp, 0, sizeof(scanrsp));
    scanrsp.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    scanrsp.uuids_complete.p_uuids  = m_adv_uuids;

    ble_adv_modes_config_t options = {0};
    options.ble_adv_fast_enabled  = BLE_ADV_FAST_ENABLED;
    options.ble_adv_fast_interval = APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;

    err_code = ble_advertising_init(&advdata, &scanrsp, &options, on_adv_evt, NULL);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing buttons and leds.
 *
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */
static void buttons_leds_init(bool * p_erase_bonds)
{
    bsp_event_t startup_event;

    uint32_t err_code = bsp_init(BSP_INIT_LED | BSP_INIT_BUTTONS,
                                 APP_TIMER_TICKS(100, APP_TIMER_PRESCALER), 
                                 bsp_event_handler);
    APP_ERROR_CHECK(err_code);

    err_code = bsp_btn_ble_init(NULL, &startup_event);
    APP_ERROR_CHECK(err_code);

    *p_erase_bonds = (startup_event == BSP_EVENT_CLEAR_BONDING_DATA);
}


/**@brief Function for placing the application in low power state while waiting for events.
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

void copystr(uint8_t *str1,uint8_t *str2)
{
	uint8_t i;
	for(i=0;i<strlen((char*)str2);i++)
	{
		str1[i]=str2[i];
	}
}

static void hand_control_event_handler(nrf_timer_event_t event_type, void* p_context)
{
	//uint32_t err_code;
	switch(Hand_activity_mode)
	{
		case Time_Mode_index:					//0:Time Mode
					if(Minute_Hand_One_Step_flg==1)
					{
						Minute_Hand_Clockwise(&Minute_Hand_Position,&Minute_Hand_Clockwise_Step_Cnt);
						if(Minute_Hand_Clockwise_Step_Cnt>=1)
						{
							Minute_Hand_One_Step_flg=0;
							Minute_Hand_Clockwise_Step_Cnt = 0;
						}
					}
					if(Hour_Hand_One_Step_flg==1)
					{
						Hour_Hand_Clockwise(&Hour_Hand_Position,&Hour_Hand_Clockwise_Step_Cnt);
						if(Hour_Hand_Clockwise_Step_Cnt>=1)
						{
							Hour_Hand_One_Step_flg=0;
							Hour_Hand_Clockwise_Step_Cnt = 0;
						}
					}
					if(Hour_Hand_One_Step_flg==0 && Minute_Hand_One_Step_flg==0)
					{
						nrf_drv_timer_disable(&Hand_Control);
						//Disable_Hand_Ctrl();
					}
			break;
		case Show_Date_BAT_index:					//1:Show Date & Battery Capacity
			switch(Show_Date_Batt_mode_step)
			{
				case 0:
							Hour_Hand_Clockwise_Step_Cnt=0;
							Minute_Hand_Clockwise_Step_Cnt=0;
							Second_Hand_Clockwise_Step_Cnt=0;
							Show_Date_Batt_mode_step=1;		
				case 1:		//Minute Hand Show Date
			      if(Minute_Hand_Position!=date_position[calendar.w_date])							
						{
						  Minute_Hand_Clockwise(&Minute_Hand_Position,&Minute_Hand_Clockwise_Step_Cnt);
							if(Minute_Hand_Clockwise_Step_Cnt>=1)
							{
								Minute_Hand_Clockwise_Step_Cnt=0;
								if(Minute_Hand_Position>=180)
								{
									Minute_Hand_Position = 0;
								}
							}
						}						 
						else
						{
							Minute_Hand_Clockwise_Step_Cnt = 0;
							//Show_Date_Batt_mode_step=2;
						}
					//break;
				case 2:		//Hour Hnad Show Week

 		        if(Hour_Hand_Position!=weeks_position[calendar.week])
					 	{
							Hour_Hand_Clockwise(&Hour_Hand_Position,&Hour_Hand_Clockwise_Step_Cnt);
							if(Hour_Hand_Clockwise_Step_Cnt>=1)
							{
								Hour_Hand_Clockwise_Step_Cnt=0;
								if(Hour_Hand_Position>=180)
								{
									Hour_Hand_Position = 0;
								}
							}
						}		
						else
						{
							Hour_Hand_Clockwise_Step_Cnt = 0;
							//Show_Date_Batt_mode_step=3;
						}
						if((Minute_Hand_Position==date_position[calendar.w_date]) && (Hour_Hand_Position==weeks_position[calendar.week]))
						{
							Show_Date_Batt_mode_step=3;
						}
					break;
				case 3:		//Small Hand Show Battery Level

					Start_Show_value=NRF_RTC1->COUNTER;
				  //Show_Date_Batt_mode_step++;
					Show_Date_Batt_mode_step=4;
					break;
				case 4:		//Delay 5'S and Then Goto Time Mode
							if((NRF_RTC1->COUNTER-Start_Show_value)>40)
							{
								Show_Date_Batt_mode_step=0;
								Hand_activity_mode=8;
							}
					break;
				default:
					break;
			}			
			break;
		case BLE_On_Mode_index:					//2:BLE On Mode
			switch(BLE_On_Mode_step)
			{
				case 0:

					break;
				case 1:
					break;
				case 2:
					break;
				default:
					break;
			}			
			break;
		case BLE_Off_Mode_index:					//3:BLE Off Mode
			switch(BLE_Off_Mode_step)
			{
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
				default:
					break;
			}			
			break;
		case BAT_Low_Vol_Mode_index:					//4:Battery Low Voltage Mode
			switch(Battery_Low_Voltage_Mode_step)
			{
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
				default:
					break;
			}			
			break;
		case Hand_Calibration_Mode_index:					//5:Pointer Calibration Mode
			switch(Pointer_Calibration_mode_step)
			{

				case 0:			//2:Minute hand running 64hz
							//nrf_drv_rtc_tick_enable(&rtc,true);
//							nrf_drv_rtc_tick_disable(&rtc);
						  Pointer_Calibration_mode_step=1;
					break;
				case 1:
							Minute_Hand_Clockwise(&Minute_Hand_Position,&Minute_Hand_Clockwise_Step_Cnt);
							if(key_pause_short_flg)
							{
								key_pause_short_flg=0;								
								Pointer_Calibration_mode_step=2;
							}
					break;
				case 2:			//3:Minute hand running 1 step/key
							if(key_pause_short_flg)
							{
								key_pause_short_flg=0;								
								One_step_flg=1;
							}
							else if(key_pause_3s_flg)
							{
								key_pause_3s_flg=0;								
								Pointer_Calibration_mode_step=3;
							}
							if(One_step_flg==1)
							{
								Minute_Hand_Clockwise(&Minute_Hand_Position,&Minute_Hand_Clockwise_Step_Cnt);
								if(Minute_Hand_Clockwise_Step_Cnt>=1)
								{
									Minute_Hand_Clockwise_Step_Cnt=0;
									One_step_flg=0;
								}
							}							
					break;
				case 3:			//4:Hour hand running 64hz
							Hour_Hand_Clockwise(&Hour_Hand_Position,&Hour_Hand_Clockwise_Step_Cnt);
							if(key_pause_short_flg)
							{
								key_pause_short_flg=0;								
								Pointer_Calibration_mode_step=4;
							}				
					break;
				case 4:			//5:Hour hand running 1 step/key
							if(key_pause_short_flg)
							{
								key_pause_short_flg=0;
								One_step_flg=1;
							}
							else if(key_pause_3s_flg)
							{
								key_pause_3s_flg=0;
								Pointer_Calibration_mode_step=5;
							}
							if(One_step_flg==1)
							{
								Hour_Hand_Clockwise(&Hour_Hand_Position,&Hour_Hand_Clockwise_Step_Cnt);
								if(Hour_Hand_Clockwise_Step_Cnt>=1)
								{
									Hour_Hand_Clockwise_Step_Cnt=0;
									One_step_flg=0;
								}
							}									
					break;		
				case 5:			//0:Small hand running 64hz
							Second_Hand_Clockwise(&Second_Hand_Position,&Second_Hand_Clockwise_Step_Cnt);
							if(key_pause_short_flg==1)
							{
								key_pause_short_flg=0;
								Pointer_Calibration_mode_step=6;
							}
					break;
				case 6:			//1:Small hand running 1 step/key
							if(key_pause_short_flg)
							{
								key_pause_short_flg=0;								
								//Second_Hand_Clockwise();
								One_step_flg=1;
							}
							else if(key_pause_3s_flg)
							{
								key_pause_3s_flg=0;								
								Pointer_Calibration_mode_step=7;
							}
							if(One_step_flg==1)
							{
								Second_Hand_Clockwise(&Second_Hand_Position,&Second_Hand_Clockwise_Step_Cnt);
								if(Second_Hand_Clockwise_Step_Cnt>=1)
								{
									Second_Hand_Clockwise_Step_Cnt=0;
									One_step_flg=0;
								}
							}
					break;
				case 7:			//6:All Hand Show Time And Status,Set Step to zero	
					    key_pause_short_flg=0;
							key_pause_3s_flg=0;

							Second_Hand_Position=0;  									//The Current Position of Second Hand
							Minute_Hand_Position=0;										//The Current Position of Minute Hand
							Hour_Hand_Position=0;											//The Current Position of Hour Hand
							Small_Hand_Top_Position=0;  								//The Current Position of Status Hand
							Small_Hand_Buttom_Position=0;  						//The Current Position of Status Hand
							Small_Hand_Right_Position=0;  							//The Current Position of Status Hand
							Small_Hand_Left_Position=0;  							//The Current Position of Status Hand
					    Pointer_Calibration_mode_step=0;
				
							Hand_activity_mode=Chasing_Time_index;
				      Chasing_Time_step=0;
				      Show_Date_Batt_mode_step=0;
							//nrf_drv_timer_disable(&Hand_Control);
				      //nrf_drv_rtc_tick_enable(&rtc,true);
					break;						
				default:
					break;
																									
			}			
			break;
		case Power_Off_Mode_index:					//6:Power Off Mode
			switch(Power_Off_Mode_step){
				case 0:		//Hour Hand Show Time
							if(Hour_Hand_Position!=0)
							{
								Hour_Hand_Clockwise(&Hour_Hand_Position,&Hour_Hand_Clockwise_Step_Cnt);
								if(Hour_Hand_Clockwise_Step_Cnt>=1)
								{
									Hour_Hand_Clockwise_Step_Cnt=0;
									if(Hour_Hand_Position>=180)
									{
										Hour_Hand_Position=0;
									}
								}
							}
				      if(Hour_Hand_Position==0)
							{
								Hour_Hand_Clockwise_Step_Cnt = 0;
								Power_Off_Mode_step=1;
							}
					break;
				case 1:		//Minute Hand Show Time
					   if(Minute_Hand_Position!=0)
						 {
								Minute_Hand_Clockwise(&Minute_Hand_Position,&Minute_Hand_Clockwise_Step_Cnt);
								if(Minute_Hand_Clockwise_Step_Cnt>=1)
								{
									Minute_Hand_Clockwise_Step_Cnt=0;
									if(Minute_Hand_Position>=180)
									{
										Minute_Hand_Position=0;
									}									
								}
						  }
				      if(Minute_Hand_Position==0)
							{
								Minute_Hand_Clockwise_Step_Cnt = 0;
								Power_Off_Mode_step=2;
							}
					break;
				case 2:		//Small Hand Show Pedometer
							if(Second_Hand_Position!=0)
							{
								Second_Hand_Clockwise(&Second_Hand_Position,&Second_Hand_Clockwise_Step_Cnt);
								if(Second_Hand_Clockwise_Step_Cnt>=1)
								{
									Second_Hand_Clockwise_Step_Cnt=0;
									if(Second_Hand_Position>=180)
									{
										Second_Hand_Position=0;
									}	
								}
							}
				      if(Second_Hand_Position==0)
							{
								Second_Hand_Clockwise_Step_Cnt = 0;
								Power_Off_Mode_step=3;
							}
					break;				
				case 3:
							//Hand_activity_mode=Time_Mode_index;
							Power_Off_Mode_step=0;
							nrf_drv_timer_disable(&Hand_Control);
//							nrf_drv_rtc_tick_disable(&rtc);
					break;		
        default:
				  break;
	    }			
	
			break;
		case Power_On_Mode_index:					//7:Power On Mode
			switch(Power_On_Mode_step)
			{
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
				default:
					break;
			}
			break;
		case Chasing_Time_index:					//8:Chasing Time Mode
			switch(Chasing_Time_step)
			{
				case 0:
							RTC_CNT= RTC_TIME_BASE + (NRF_RTC1->COUNTER/8) ;
							RTC_Get(RTC_CNT);
							calculate_RTC_Position(calendar.hour,calendar.min,calendar.sec,&RTC_Hour_Hand_Position,&RTC_Minute_Hand_Position,&RTC_Second_Hand_Position);									
							Hour_Hand_Clockwise_Step_Cnt=0;
							Minute_Hand_Clockwise_Step_Cnt=0;
							Second_Hand_Clockwise_Step_Cnt=0;
							Chasing_Time_step=1;
				case 1:		//Hour Hand Show Time
							if(Hour_Hand_Position!=RTC_Hour_Hand_Position)
							{
								Hour_Hand_Clockwise(&Hour_Hand_Position,&Hour_Hand_Clockwise_Step_Cnt);
								if(Hour_Hand_Clockwise_Step_Cnt>=1)
								{
									Hour_Hand_Clockwise_Step_Cnt=0;
									if(Hour_Hand_Position>=180)
									{
										Hour_Hand_Position=0;
									}
								}
							}
				      else if(Hour_Hand_Position==RTC_Hour_Hand_Position)
							{
								Hour_Hand_Clockwise_Step_Cnt = 0;
								//Chasing_Time_step=2;
							}
					//break;
				case 2:		//Minute Hand Show Time
							if(Minute_Hand_Position!=RTC_Minute_Hand_Position)
							{
								Minute_Hand_Clockwise(&Minute_Hand_Position,&Minute_Hand_Clockwise_Step_Cnt);
								if(Minute_Hand_Clockwise_Step_Cnt>=1)
								{
									Minute_Hand_Clockwise_Step_Cnt=0;
									if(Minute_Hand_Position>=180)
									{
										Minute_Hand_Position=0;
									}								
								}
							}
				      else if(Minute_Hand_Position==RTC_Minute_Hand_Position)
							{
								Minute_Hand_Clockwise_Step_Cnt = 0;
								//Chasing_Time_step=3;
							}
					//break;
				case 3:		//Small Hand Show Pedometer
						 if(Second_Hand_Position!=Pedometer_Position)
						 {
							 Second_Hand_Clockwise(&Second_Hand_Position,&Second_Hand_Clockwise_Step_Cnt);
				      if(Second_Hand_Clockwise_Step_Cnt>=1)
							{
								Second_Hand_Clockwise_Step_Cnt=0;
								if(Second_Hand_Position>=180)
								{
									Second_Hand_Position=0;
								}	
							}	
						 }
				     else if(Second_Hand_Position==RTC_Second_Hand_Position)
							{
								Second_Hand_Clockwise_Step_Cnt = 0;
								//Chasing_Time_step=4;
							}
							if((Hour_Hand_Position==RTC_Hour_Hand_Position)&&(Minute_Hand_Position==RTC_Minute_Hand_Position)&&(Second_Hand_Position==Pedometer_Position))
							{
								Chasing_Time_step=4;
							}
					break;				
				case 4:
							Hand_activity_mode=Time_Mode_index;
							Chasing_Time_step=0;
							nrf_drv_timer_disable(&Hand_Control);
//							nrf_drv_rtc_tick_enable(&rtc,true);
					break;
				default:
					break;
			}
			break;
			
		case Hand_To_Zero_Mode_index:					//9:Hand To Zero Mode
			switch(Hand_To_Zero_Mode_step){
				case 0:		//Hour Hand Show Time
							if(Hour_Hand_Position!=0)
							{
								Hour_Hand_Clockwise(&Hour_Hand_Position,&Hour_Hand_Clockwise_Step_Cnt);
								if(Hour_Hand_Clockwise_Step_Cnt>=1)
								{
									Hour_Hand_Clockwise_Step_Cnt=0;
									if(Hour_Hand_Position>=180)
									{
										Hour_Hand_Position=0;
									}
								}
							}
				      if(Hour_Hand_Position==0)
							{
								Hour_Hand_Clockwise_Step_Cnt = 0;
								Power_Off_Mode_step=1;
							}
					break;
				case 1:		//Minute Hand Show Time
					   if(Minute_Hand_Position!=0)
						 {
								Minute_Hand_Clockwise(&Minute_Hand_Position,&Minute_Hand_Clockwise_Step_Cnt);
								if(Minute_Hand_Clockwise_Step_Cnt>=1)
								{
									Minute_Hand_Clockwise_Step_Cnt=0;
									if(Minute_Hand_Position>=180)
									{
										Minute_Hand_Position=0;
									}									
								}
						  }
				      if(Minute_Hand_Position==0)
							{
								Minute_Hand_Clockwise_Step_Cnt = 0;
								Power_Off_Mode_step=2;
							}
					break;
				case 2:		//Small Hand Show Pedometer
							if(Second_Hand_Position!=0)
							{
								Second_Hand_Clockwise(&Second_Hand_Position,&Second_Hand_Clockwise_Step_Cnt);
								if(Second_Hand_Clockwise_Step_Cnt>=1)
								{
									Second_Hand_Clockwise_Step_Cnt=0;
									if(Second_Hand_Position>=180)
									{
										Second_Hand_Position=0;
									}	
								}
							}
				      if(Second_Hand_Position==0)
							{
								Second_Hand_Clockwise_Step_Cnt = 0;
								Power_Off_Mode_step=3;
							}
					break;				
				case 3:
							Hand_activity_mode=Time_Mode_index;
							Hand_To_Zero_Mode_step=0;
							nrf_drv_timer_disable(&Hand_Control);
//							nrf_drv_rtc_tick_disable(&rtc);
					break;		
        default:
				  break;
	    }
			break;
		case Hand_Calibration_Mode_for_BLE:
			//switch(Hand_To_Zero_Mode_step){
			switch(Hand_Calibration_Mode_for_BLE_step)
			{
				case 0:		//Hour Hand Show Time
							if(Hour_Hand_Position!=0)
							{
								Hour_Hand_Clockwise(&Hour_Hand_Position,&Hour_Hand_Clockwise_Step_Cnt);
								if(Hour_Hand_Clockwise_Step_Cnt>=1)
								{
									Hour_Hand_Clockwise_Step_Cnt=0;
									if(Hour_Hand_Position>=180)
									{
										Hour_Hand_Position=0;
									}
								}
							}
				      if(Hour_Hand_Position==0)
							{
								Hour_Hand_Clockwise_Step_Cnt = 0;
								Hand_Calibration_Mode_for_BLE_step=1;
							}
					break;
				case 1:		//Minute Hand Show Time
					   if(Minute_Hand_Position!=0)
						 {
								Minute_Hand_Clockwise(&Minute_Hand_Position,&Minute_Hand_Clockwise_Step_Cnt);
								if(Minute_Hand_Clockwise_Step_Cnt>=1)
								{
									Minute_Hand_Clockwise_Step_Cnt=0;
									if(Minute_Hand_Position>=180)
									{
										Minute_Hand_Position=0;
									}									
								}
						  }
				      if(Minute_Hand_Position==0)
							{
								Minute_Hand_Clockwise_Step_Cnt = 0;
								Hand_Calibration_Mode_for_BLE_step=2;
							}
					break;
				case 2:	
							if(Second_Hand_Position!=0)
							{
								Second_Hand_Clockwise(&Second_Hand_Position,&Second_Hand_Clockwise_Step_Cnt);
								if(Second_Hand_Clockwise_Step_Cnt>=1)
								{
									Second_Hand_Clockwise_Step_Cnt=0;
									if(Second_Hand_Position>=180)
									{
										Second_Hand_Position=0;
									}	
								}
							}
				      if(Second_Hand_Position==0)
							{
								Second_Hand_Clockwise_Step_Cnt = 0;
								Hand_Calibration_Mode_for_BLE_step=3;
							}
					break;				
				case 3:
							Hand_activity_mode=Chasing_Time_index;
							Hand_Calibration_Mode_for_BLE_step=0;
							nrf_drv_timer_disable(&Hand_Control);
//							nrf_drv_rtc_tick_disable(&rtc);
					break;		
        default:
				  break;
	    }
			break;
		default:
			break;
	}
}

//static void Key_Scan_event_handler(void* p_context)

static void Key_Scan_event_handler(nrf_timer_event_t event_type, void* p_context)
{
		key_press_cnt++;
		if(nrf_gpio_pin_read(KEY_IO)==1)
		{
			//key_press_flg=1;
			//BT_CNT[0]++;
			if(key_press_cnt>10 && key_press_cnt<50)
			{
				Vibrator_off();
			}
			else if(key_press_cnt>50 && key_press_cnt<300)
			{
				Vibrator_off();
			}
			else if(key_press_cnt>300 && key_press_cnt<320)
			{
				Vibrator_on();
			}
			else if(key_press_cnt>320 && key_press_cnt<500)
			{
				Vibrator_off();
			}			
			else if(key_press_cnt>500 && key_press_cnt<520)
			{
				Vibrator_on();
			}
			else if(key_press_cnt>520 && key_press_cnt<1000)
			{
				Vibrator_off();
			}		
			else if(key_press_cnt>1000 && key_press_cnt<1020)
			{
				Vibrator_on();
			}
			else if(key_press_cnt>1020 && key_press_cnt<1500)
			{
				Vibrator_off();
			}
		}
		else if(nrf_gpio_pin_read(KEY_IO)==0)
		{
			Vibrator_off();
		 if(key_press_cnt>5 && key_press_cnt<25)
			 {
				key_pause_short_flg=1;
				key_pause_3s_flg=0;
				key_pause_5s_flg=0;
				key_pause_10s_flg=0;
				if(Hand_activity_mode==Time_Mode_index)
				{
					key_press_times_cnt++;
					if(key_press_times_cnt>=2)
					{
						
					}
				}
			 }
			 else if(key_press_cnt>25 && key_press_cnt<300)
			 {
				key_pause_short_flg=1;
				key_pause_3s_flg=0;
				key_pause_5s_flg=0;
				key_pause_10s_flg=0;
				if(Hand_activity_mode==Time_Mode_index)
				{
					nrf_adc_start();
			    adc_sample=adc_sample;
					Hand_activity_mode=Show_Date_BAT_index;
					nrf_drv_timer_enable(&Hand_Control);
				}
			 }			 
			 else if(key_press_cnt>300 && key_press_cnt<500)
			 {
				key_pause_short_flg=0;
				key_pause_3s_flg=1;
				key_pause_5s_flg=0;
				key_pause_10s_flg=0;
				if(Hand_activity_mode==Time_Mode_index)
				{
					Hand_activity_mode=Hand_Calibration_Mode_index;
					nrf_drv_timer_enable(&Hand_Control);
				}
			 }
			 else if(key_press_cnt>500 && key_press_cnt<1000)
			 {
				key_pause_short_flg=0;
				key_pause_3s_flg=0;
				key_pause_5s_flg=1;
				key_pause_10s_flg=0;
				 
				//if(Hand_activity_mode==Time_Mode_index)
				//{
			//		Hand_activity_mode=5;
				//}			 
			 }
			 else if(key_press_cnt>1000 && key_press_cnt<1500)
			 {
				key_pause_short_flg=0;
				key_pause_3s_flg=0;
				key_pause_5s_flg=0;
				key_pause_10s_flg=1;
				if(Hand_activity_mode==Time_Mode_index)
				{
					Hand_activity_mode=Power_Off_Mode_index;
					nrf_drv_timer_enable(&Hand_Control);
				}					 
			 }
			 key_press_cnt=0;
				//nrf_drv_timer_disable(&Key_Scan);
			  //uint32_t err_code = app_timer_stop(m_button_a_timer_id);
			  nrf_drv_timer_disable(&Button_Control);
        //APP_ERROR_CHECK(err_code);
				//key_press_cnt=NRF_RTC0->COUNTER-key_press_time;
				//key_press_cnt=0;
			  //key_press_flg=0;
		}

	//nrf_drv_timer_enable(&Key_Scan);

}

/**@brief Application main function.
 */
int main(void)
{
    uint32_t err_code;
//		uint8_t s,i;
    bool erase_bonds;
    uint8_t start_string[] = START_STRING;
//    uint8_t ck_a,ck_b;
	  //copystr(DEVICE_NAME,start_string);
    //sprintf((char *)DEVICE_NAME,*start_string);
    // Initialize.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
    uart_init();
    buttons_leds_init(&erase_bonds);
    ble_stack_init();
    gap_params_init();
    services_init();
    advertising_init();
    conn_params_init();
    adc_config();
	  nrf_adc_start();
	
	  nrf_delay_ms(1000);

    printf("%s",start_string);

    //err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
    //APP_ERROR_CHECK(err_code);
    //check_sum(&ANS_NAK[0],&ck_a,&ck_b);	
    // Enter main loop.
    err_code = nrf_drv_timer_init(&Hand_Control, NULL, hand_control_event_handler);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_timer_init(&Button_Control, NULL, Key_Scan_event_handler);
    APP_ERROR_CHECK(err_code);
		//lfclk_config();
		//rtc_config();
    err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
		    APP_ERROR_CHECK(err_code);
    for (;;)
    {
        power_manage();
    }
}


/** 
 * @}
 */
