
#ifndef _SPI_GSENSOR_H_
#define _SPI_GSENSOR_H_

void write_Sen_datas(uint8_t Addr,uint8_t data);
void Read_Sen_datas(uint8_t Addr,uint8_t *buff,uint8_t Len);  
void Enable_GSensor(void);
void Disable_GSensor(void);
void Enable_GSensor2(void);


#endif

