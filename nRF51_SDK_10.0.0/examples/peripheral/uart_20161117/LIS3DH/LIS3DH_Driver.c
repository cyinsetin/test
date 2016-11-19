
#include "LIS3DH_Driver.h"
#include "nrf51.h"
#include "nrf_gpio.h"
#include "boards.h"

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
	write_Sen_datas(0x20, 0x67);
}

