
#include "nrf_adc.h"
#include "ble_bas.h"

#define NRF_ADC_CONFIG_CT003	{ NRF_ADC_CONFIG_RES_10BIT,               \
                                 NRF_ADC_CONFIG_SCALING_INPUT_TWO_THIRDS, \
                                 NRF_ADC_CONFIG_REF_VBG }
#ifndef NRF_APP_PRIORITY_HIGH
#define NRF_APP_PRIORITY_HIGH 1
#endif
extern volatile int32_t adc_sample;
extern ble_bas_init_t bas_init;
/**
 * @brief ADC initialization.
 */
void adc_config(void)
{
    //const nrf_adc_config_t nrf_adc_config = NRF_ADC_CONFIG_DEFAULT;
		const nrf_adc_config_t nrf_adc_config = NRF_ADC_CONFIG_CT003;
    // Initialize and configure ADC
    nrf_adc_configure( (nrf_adc_config_t *)&nrf_adc_config);
    nrf_adc_input_select(NRF_ADC_CONFIG_INPUT_6);
    nrf_adc_int_enable(ADC_INTENSET_END_Enabled << ADC_INTENSET_END_Pos);
    NVIC_SetPriority(ADC_IRQn, NRF_APP_PRIORITY_HIGH);
    NVIC_EnableIRQ(ADC_IRQn);
		nrf_adc_start();
}
/**
 * @brief ADC interrupt handler.
 */
void ADC_IRQHandler(void)
{
    nrf_adc_conversion_event_clean();

    adc_sample = nrf_adc_result_get();
	  bas_init.initial_batt_level = adc_sample;
}
