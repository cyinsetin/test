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
static void nus_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length)
{
		uint8_t i,ck_a,ck_b,len,err_f,cmd;
		uint8_t j;
	  uint32_t err_code;
	  static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
    static uint8_t index = 0;
	
    for (uint32_t i = 0; i < length; i++)
    {
        while(app_uart_put(p_data[i]) != NRF_SUCCESS);
    }
    while(app_uart_put('\n') != NRF_SUCCESS);
	


		err_f=false;
    for (uint32_t i = 0; i < length; i++)
    {
        while(app_uart_put(p_data[i]) != NRF_SUCCESS);
    }
			err_f=false;
			if (p_data[0]!=Watch_header_H) err_f=true;
			if (p_data[1]!=Watch_header_L) err_f=true;
			if (p_data[2]!=Watch_ID_H) err_f=true;
			if (!err_f) {
										len=BT_RxBuf[4];
										bt_rx_len=len+8;
										ck_a=ck_b=0;
										for (i=0;i<len+4;i++) 
											{
												ck_a+=BT_RxBuf[i+2];
												ck_b+=ck_a;
											}
										if ((ck_a!=BT_RxBuf[i+2])||(ck_b!=BT_RxBuf[i+3])) err_f=true;
									}
			if (err_f) {
										bt_rx_len=8;
										//SimpleProfile_SetParameter(SIMPLEPROFILE_CHAR4, 8, ANS_NACK);
										err_code = ble_nus_string_send(&m_nus, ANS_NACK, 8);
									} 
			else 
			{
				//bt_rx_len=8;
				//SimpleProfile_SetParameter(SIMPLEPROFILE_CHAR4, 8, ANS_ACK);
				bt_rx_len=len+8;
				cmd=BT_RxBuf[3];
				if (cmd!=0x07) {
					for(j=0;j<bt_rx_len;j++)
					{
						//Uart_TxBuf[j] = BT_RxBuf[j];
						err_code = ble_nus_string_send(&m_nus, ANS_NACK, index);
					}
				}
			}
				switch (cmd) {
											case 0x00:	//Get Device S/N
														
														bt_rx_len=20;
														err_code = ble_nus_string_send(&m_nus, ANS_SN, 20);
													break;
											case 0x01:	//Message
														for(j=0;j<bt_rx_len;j++)
															{
																//BT_RxBuf[j];
															}
																err_code = ble_nus_string_send(&m_nus, ANS_SN, 20);
													break;
											case 0x02:	//Set Watch Time
														for(j=0;j<bt_rx_len;j++)
															{
																//BT_RxBuf[j];
															}
															err_code = ble_nus_string_send(&m_nus, ANS_SN, 20);
													break;
											case 0x03:	//Set Watch Date
														for(j=0;j<bt_rx_len;j++)
															{
	
															}
														err_code = ble_nus_string_send(&m_nus, ANS_ACK, 8);
													break;
											case 0x04:	//Set Alarm Clock
										
												break;
											case 0x05:	//Get FW Ver
										
											
												break;
											case 0x06:	//ACK
										
												break;
											case 0x07:	//Set Device Name
												break;
											case 0x08:	//Get Device Name
												break;
										
											case 0x09: 	//System Type

												break;
										
											case 0x0B:	//Anti-lost
										
														err_code = ble_nus_string_send(&m_nus, ANS_ACK, 8);
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
												break;
											case 0x0F:	//NAK
												break;
											case 0x12:	//find cell phone
												break;
										
											case 0x13:	//Get BATT
														bt_rx_len=8;
														err_code = ble_nus_string_send(&m_nus, ANS_ACK, 8);
												break;
											case 0x14:	//Adj Hour Hand
														for(j=0;j<bt_rx_len;j++)
															{

															}
														err_code = ble_nus_string_send(&m_nus, ANS_ACK, 8);
												break;
											case 0x15:	//Adj Minute Hand
														for(j=0;j<bt_rx_len;j++)
															{

															}
														err_code = ble_nus_string_send(&m_nus, ANS_ACK, 8);
												break;
											case 0x16:	//Adj Second Hand

														err_code = ble_nus_string_send(&m_nus, ANS_ACK, 8);
											break;
										case 0x17:	//Adj Message Hand
														err_code = ble_nus_string_send(&m_nus, ANS_ACK, 8);
											break;
										case 0x18:	//Adj Pedometer Hand
											break;
										case 0x19:	//Enter Hand Adj Mode
											break;
										case 0x20:	//Erase Pedometer
				
											break;
										case 0x21:	//Erase Sleep Data
											break;
										case 0x81:	//Set Watch ID
				
											
											break;
										case 0xE0:	//Erase Sleep Data
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
/**@snippet [Handling the data received over BLE] */

