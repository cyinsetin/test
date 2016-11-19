#include "nrf_drv_config.h"
#include "boards.h"
#include "motor.h"
#include "adc.h"
#include "key.h"
#include "clock.h"
#include "rtc.h"


void Hw_initial(void)
{
	motor_gpio_init();
	adc_config();
	Key_init();
	lfclk_config();
	rtc_config();
	nrf_gpio_cfg_output(VIBRATION_IO);
	Vibrator_off();
}

void Sw_initial(void)
{

}
