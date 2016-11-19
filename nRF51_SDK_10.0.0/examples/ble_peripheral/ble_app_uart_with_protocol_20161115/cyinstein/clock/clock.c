#include "nrf_drv_clock.h"

// Function starting the internal LFCLK oscillator.
// This is needed by RTC1 which is used by the application timer
// (When SoftDevice is enabled the LFCLK is always running and this is not needed).
/** @brief Function starting the internal LFCLK XTAL oscillator.
 */

void lfclk_config(void)
{

    //ret_code_t err_code = nrf_drv_clock_init();
    //APP_ERROR_CHECK(err_code);
    //nrf_drv_clock_lfclk_request(NULL);

	NRF_CLOCK->LFCLKSRC = (CLOCK_LFCLKSRC_SRC_Xtal>>CLOCK_LFCLKSRC_SRC_Pos);
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_LFCLKSTART = 1;
	while(NRF_CLOCK->EVENTS_LFCLKSTARTED == 0);
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
}

