#ifndef __MYSPI_H
#define __MYSPI_H
#include "nrf.h"

extern uint8_t TestByte1;

uint8_t LIS3DH_ReadOneByte(uint8_t Dat); 
void LIS3DH_WriteOneByte(uint8_t Dat);
void LIS3DH_Write_Reg(uint8_t Addr,uint8_t Dat);
uint8_t LIS3DH_Read_Reg(uint8_t Addr);
uint8_t LIS3DH_Write_CMD(uint8_t *CMD);
uint8_t LIS3DH_MultipleRead_Reg(uint8_t Addr);
uint8_t LIS3DH_Multiple_Read_Reg(uint8_t Addr,uint8_t *buf,uint8_t len) ;
#endif //MYSPI.H
