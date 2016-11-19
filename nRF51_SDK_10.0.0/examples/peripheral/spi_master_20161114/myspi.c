#include "nrf_delay.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "nrf_drv_spi.h"
#include "bsp.h"
#include "app_timer.h"
#include "nordic_common.h"
#include "CT003.h"
#include "nrf_gpio.h"
#include "myio_def.h"
#include "myspi.h"
/*****************************************************************************
** 描  述：读出一个字节
** 入  参：无
** 返回值：读出的数据
******************************************************************************/
uint8_t LIS3DH_ReadOneByte(uint8_t Dat) 
{
    uint8_t retValue = 0;
		//Dat|=0x80;
    for(uint8_t BitCount = 0;BitCount < 8; BitCount++)
    {
				LIS3DH_Clr_SCLK;            //时钟线拉高，产生上升沿写入数据
				LIS3DH_Set_SCLK;
        if(LIS3DH_Get_DI==1) 
        {
          retValue<<=1;
					retValue++;
        }
				else
				{
					retValue<<=1;
				}
    }
    return (retValue);
}
/*****************************************************************************
** 描  述：读出一个字节
** 入  参：无
** 返回值：读出的数据
******************************************************************************/
void LIS3DH_WriteOneByte(uint8_t Dat) 
{
    //Dat|=0x80;
		for(uint8_t BitCount = 0;BitCount < 8; BitCount++)
    {
        if((Dat & 0x80)==0x80)
        {
            LIS3DH_Set_DO;
        }
        else
        {
            LIS3DH_Clr_DO;
        }
			  LIS3DH_Clr_SCLK;            //时钟线拉高，产生上升沿写入数据
				LIS3DH_Set_SCLK;
        Dat <<= 1;
    }
}

/*****************************************************************************
** 描  述：写入一个字节
** 入  参：Dat：待写入的数据
** 返回值：无
******************************************************************************/
void LIS3DH_Write_Reg(uint8_t Addr,uint8_t Dat)
{
	Addr&=0x3F;
	Addr|=0x80;
	LIS3DH_Enable_CS;
	LIS3DH_WriteOneByte(Addr);
	LIS3DH_WriteOneByte(Dat);
	LIS3DH_Enable_CS;
}

/*****************************************************************************
** 描  述：出一个字节
** 入  参：无
** 返回值：读出的数据
******************************************************************************/
uint8_t LIS3DH_Read_Reg(uint8_t Addr) 
{
		uint8_t retValue;
		Addr&=0x3F;
		Addr|=0x80;									//读寄存器地址时需要第一个Bit置1
	  LIS3DH_Enable_CS;
		LIS3DH_WriteOneByte(Addr);
		retValue=LIS3DH_ReadOneByte(Addr);
	  LIS3DH_Enable_CS;
    return (retValue);
}

/*****************************************************************************
** 描  述：出一个字节
** 入  参：无
** 返回值：读出的数据
******************************************************************************/
uint8_t LIS3DH_Multiple_Read_Reg(uint8_t Addr,uint8_t *buf,uint8_t len) 
{
		uint8_t retValue;
		uint8_t i;
		Addr&=0x3F;
		Addr|=0xC0;									//连续读寄存器地址时需要前2个Bit置1
	  LIS3DH_Enable_CS;
		LIS3DH_WriteOneByte(Addr);
	  for(i=0;i<len-1;i++)
		{
			retValue=LIS3DH_ReadOneByte(*buf+i);
			TestByte1=retValue;
		}
	  LIS3DH_Enable_CS;
    return (retValue);
}

/*****************************************************************************
** 描  述：写使能
** 入  参：无
** 返回值：无
******************************************************************************/
static void LIS3DH_Write_Enable(void)
{
    LIS3DH_Enable_CS;
    LIS3DH_WriteOneByte(0x0f);
    LIS3DH_Disable_CS;
}

void Enable_LIS3DH(void)
{
	LIS3DH_WriteOneByte(0x00);
	
}

/*****************************************************************************
** 描  述：从指定的地址读出指定长度的数据
** 入  参：pBuffer：指向存放读出数据的首地址       
**         ReadAddr：待读出数据的起始地址
**         ReadBytesNum：读出的字节数
** 返回值：
******************************************************************************/
uint8_t LIS3DH_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t ReadBytesNum)
{
    uint32_t i = 0;
    uint8_t pcmd[3] = {0};
    uint8_t ret = 0;

    LIS3DH_Enable_CS;              //打开片选

    LIS3DH_WriteOneByte(2);

    pcmd[0] = (uint8_t)((ReadAddr&0x00ff0000)>>16);
    pcmd[1] = (uint8_t)((ReadAddr&0x0000ff00)>>8);
    pcmd[2] = (uint8_t)ReadAddr;
    ret = LIS3DH_Write_CMD(pcmd);
    if(ret != 0)
    {
        return 1;
    }

    for(i = 0;i < ReadBytesNum; i++)
    {
       pBuffer[i] = LIS3DH_ReadOneByte(0x8f);  //读取LIS3DH中指定bytes字节数据

    }
    LIS3DH_Disable_CS;
    return 0;
}



/*****************************************************************************
** 描  述：写入命令
** 入  参：*CMD：指向待写入的命令
** 返回值：RET_SUCCESS
******************************************************************************/
uint8_t LIS3DH_Write_CMD(uint8_t *CMD)
{
    uint32_t i = 0;
    for(i = 0;i < 2; i++)
    {
        LIS3DH_WriteOneByte(CMD[i]);  //
    }

    return 0;
}

