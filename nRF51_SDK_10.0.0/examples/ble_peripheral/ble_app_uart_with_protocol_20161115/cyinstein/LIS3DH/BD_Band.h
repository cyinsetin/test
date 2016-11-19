
#ifndef _BD_BAND_H__
#define _BD_BAND_H__





#define SPI_OPERATING_FREQUENCY (0x02000000uL << (uint32_t)6/*Freq_1Mbps*/)  /**< Slave clock frequency. */

#define SPI_PSELSCK0              31   /*!< GPIO pin number for SPI clock (note that setting this to 31 will only work for loopback purposes as it not connected to a pin) */
#define SPI_PSELMOSI0             20   /*!< GPIO pin number for Master Out Slave In    */
#define SPI_PSELMISO0             22   /*!< GPIO pin number for Master In Slave Out    */
#define SPI_PSELSS0               30   /*!< GPIO pin number for Slave Select           */

/*  SPI1 */
#if 0
#define SPI_PSELSCK1             05// 30//05//29   /*!< GPIO pin number for SPI clock              */
#define SPI_PSELMOSI1           02// 02// 21   /*!< GPIO pin number for Master Out Slave In    */
#define SPI_PSELMISO1            01//01// 23   /*!< GPIO pin number for Master In Slave Out    */
#define SPI_PSELSS1              00// 00//28   /*!< GPIO pin number for Slave Select           */
#else
#define SPI_PSELSCK1             9 //13// 30//05//29   /*!< GPIO pin number for SPI clock              */
#define SPI_PSELMOSI1            8 //12// 02// 21   /*!< GPIO pin number for Master Out Slave In    */
#define SPI_PSELMISO1            3 //11//01// 23   /*!< GPIO pin number for Master In Slave Out    */
#define SPI_PSELSS1              1 //10// 00//28   /*!< GPIO pin number for Slave Select           */
#endif

#define TIMEOUT_COUNTER         0x3000uL                                /**< Timeout for SPI transaction in units of loop iterations. */


#define hw_Set_DecoderPower()	//	(nrf_gpio_pin_set(01),Set_DecoderPowerOnFlag())
#define hw_Clr_DecoderPower()	//	(nrf_gpio_pin_clear(01),Clr_DecoderPowerOnFlag())
#define Init_hw_DecoderPower()	//	(nrf_gpio_cfg_output(01),nrf_gpio_pin_clear(01),Clr_DecoderPowerOnFlag())


#define hw_SetLed0()		//nrf_gpio_pin_set(LED_0)
#define hw_ClrLed0()		//nrf_gpio_pin_clear(LED_0)
#define Init_hw_Led0()	//(nrf_gpio_cfg_output(LED_0),nrf_gpio_pin_clear(LED_0))

#define hw_SetLed1()		///nrf_gpio_pin_set(LED_1)
#define hw_ClrLed1()		//nrf_gpio_pin_clear(LED_1)
#define Init_hw_Led1()	//(nrf_gpio_cfg_output(LED_1),nrf_gpio_pin_clear(LED_1))

#define hw_SetLed2()		
#define hw_ClrLed2()		
#define Init_hw_Led2()		

#define hw_SetLed3()		
#define hw_ClrLed3()		
#define Init_hw_Led3()		

#define hw_SetLed4()		
#define hw_ClrLed4()		
#define Init_hw_Led4()		

#define hw_SetLed5()		
#define hw_ClrLed5()		
#define Init_hw_Led5()		

#define hw_SetLed6()		
#define hw_ClrLed6()		
#define Init_hw_Led6()		

#define hw_SetLed7()		
#define hw_ClrLed7()		
#define Init_hw_Led7()		

#define hw_SetPowerLed()		
#define hw_ClrPowerLed()		
#define Init_hw_PowerLed()	

#define hw_Set_VirbrateMotor()		nrf_gpio_pin_set(VIRBRATE_MOTOR)   //pin is used as virbrate motor.
#define hw_Clr_VirbrateMotor()		nrf_gpio_pin_clear(VIRBRATE_MOTOR)     //pin is used as virbrate motor.
#define Init_hw_VirbrateMotor()	(nrf_gpio_cfg_output(VIRBRATE_MOTOR),nrf_gpio_pin_clear(VIRBRATE_MOTOR))  //pin is used as virbrate motor.

#define vibrator_test()                      \
    do                                       \
    {                                        \
        nrf_gpio_pin_set(VIRBRATE_MOTOR);    \
        nrf_delay_ms(100);                   \
        nrf_gpio_pin_clear(VIRBRATE_MOTOR);  \
    } while (0)

#define hw_Set_SwitchAudio()		//nrf_gpio_pin_clear(05)   //pin is not used by watch.
#define hw_Clr_SwitchAudio()		//nrf_gpio_pin_set(05)     //pin is not used by watch.
#define Init_hw_SwitchAudio()		//(nrf_gpio_cfg_output(05),nrf_gpio_pin_set(05))  //pin is not used by watch.

//#define Init_hw_Key()		       	nrf_gpio_cfg_input(02, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pulldown)  //modify by jack.  pin Of xinjin = 02.
//#define HW_KEY_IN()				(nrf_gpio_pin_read(02)==1)     //modify by jack. pin Of xinjin = 02.

#define Init_hw_Key()		    nrf_gpio_cfg_input(16, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pulldown)  //modify by jack.  pin Of xinjin = 02.
#define HW_KEY_IN()				(nrf_gpio_pin_read(16)==1)     //modify by jack. pin Of xinjin = 02. 目前的实现，按下是高电平

#define Init_hw_SensorInt1()		//nrf_gpio_cfg_input(8, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Disabled)//nrf_gpio_cfg_input(03, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Disabled)
#define Init_hw_SensorInt2()		//nrf_gpio_cfg_input(9, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Disabled)//nrf_gpio_cfg_input(30, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Disabled)

#define Init_ADC_Recharge_Pin()		 nrf_gpio_cfg_input(6, (nrf_gpio_pin_pull_t)NRF_GPIO_PIN_NOPULL)
#define HW_ReCharge_IN()	        (nrf_gpio_pin_read(6)==1)  //recharge, the value is high.

#define PIN_TX		14	//23

//ADC recharge 06 pin

//15 print
#endif

