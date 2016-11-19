#ifndef __MYIO_DEF_H
#define __MYIO_DEF_H


#define    SPI_LIS3DH_CS    1
#define    SPI_LIS3DH_CLK   9
#define    SPI_LIS3DH_MOSI  8
#define    SPI_LIS3DH_MISO  3

#define    LIS3DH_Enable_CS    nrf_gpio_pin_clear(SPI_LIS3DH_CS)   //Ƭѡ��Ч
#define    LIS3DH_Disable_CS   nrf_gpio_pin_set(SPI_LIS3DH_CS)     //Ƭѡ��Ч
#define    LIS3DH_Set_SCLK     nrf_gpio_pin_set(SPI_LIS3DH_CLK)    //ʱ������
#define    LIS3DH_Clr_SCLK     nrf_gpio_pin_clear(SPI_LIS3DH_CLK)  //ʱ���õ�
#define    LIS3DH_Set_DO       nrf_gpio_pin_set(SPI_LIS3DH_MOSI)   //MOSI����
#define    LIS3DH_Clr_DO       nrf_gpio_pin_clear(SPI_LIS3DH_MOSI) //MOSI�õ�
#define    LIS3DH_Get_DI       nrf_gpio_pin_read(SPI_LIS3DH_MISO)


#endif // __MYIO_DEF_H
