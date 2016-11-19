
//#include "Spi_master.h"
#include "global.h"



void write_Sen_datas(uint8_t Addr,uint8_t data)  
{
    	uint32_t * p_spi_base_address;
	p_spi_base_address = spi_master_init(1, 0, false);

	uint8_t TempData[2];
	TempData[0]=Addr;
	TempData[1]=data;
	
	spi_master_tx_rx(p_spi_base_address, 2, TempData, TempData,1); 
	//spi_master_tx_rx(p_spi_base_address, 2, &data, NULL,1); 
}

void Read_Sen_datas(uint8_t Addr,uint8_t *buff,uint8_t Len)  
{
    	uint32_t * p_spi_base_address = spi_master_init(1, 0, false);
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
	write_Sen_datas(0x20, 0x6F);
}



