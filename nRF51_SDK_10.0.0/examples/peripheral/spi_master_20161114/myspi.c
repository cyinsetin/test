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
** ��  ��������һ���ֽ�
** ��  �Σ���
** ����ֵ������������
******************************************************************************/
uint8_t LIS3DH_ReadOneByte(uint8_t Dat) 
{
    uint8_t retValue = 0;
		//Dat|=0x80;
    for(uint8_t BitCount = 0;BitCount < 8; BitCount++)
    {
				LIS3DH_Clr_SCLK;            //ʱ�������ߣ�����������д������
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
** ��  ��������һ���ֽ�
** ��  �Σ���
** ����ֵ������������
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
			  LIS3DH_Clr_SCLK;            //ʱ�������ߣ�����������д������
				LIS3DH_Set_SCLK;
        Dat <<= 1;
    }
}

/*****************************************************************************
** ��  ����д��һ���ֽ�
** ��  �Σ�Dat����д�������
** ����ֵ����
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
** ��  ������һ���ֽ�
** ��  �Σ���
** ����ֵ������������
******************************************************************************/
uint8_t LIS3DH_Read_Reg(uint8_t Addr) 
{
		uint8_t retValue;
		Addr&=0x3F;
		Addr|=0x80;									//���Ĵ�����ַʱ��Ҫ��һ��Bit��1
	  LIS3DH_Enable_CS;
		LIS3DH_WriteOneByte(Addr);
		retValue=LIS3DH_ReadOneByte(Addr);
	  LIS3DH_Enable_CS;
    return (retValue);
}

/*****************************************************************************
** ��  ������һ���ֽ�
** ��  �Σ���
** ����ֵ������������
******************************************************************************/
uint8_t LIS3DH_Multiple_Read_Reg(uint8_t Addr,uint8_t *buf,uint8_t len) 
{
		uint8_t retValue;
		uint8_t i;
		Addr&=0x3F;
		Addr|=0xC0;									//�������Ĵ�����ַʱ��Ҫǰ2��Bit��1
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
** ��  ����дʹ��
** ��  �Σ���
** ����ֵ����
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
** ��  ������ָ���ĵ�ַ����ָ�����ȵ�����
** ��  �Σ�pBuffer��ָ���Ŷ������ݵ��׵�ַ       
**         ReadAddr�����������ݵ���ʼ��ַ
**         ReadBytesNum���������ֽ���
** ����ֵ��
******************************************************************************/
uint8_t LIS3DH_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t ReadBytesNum)
{
    uint32_t i = 0;
    uint8_t pcmd[3] = {0};
    uint8_t ret = 0;

    LIS3DH_Enable_CS;              //��Ƭѡ

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
       pBuffer[i] = LIS3DH_ReadOneByte(0x8f);  //��ȡLIS3DH��ָ��bytes�ֽ�����

    }
    LIS3DH_Disable_CS;
    return 0;
}



/*****************************************************************************
** ��  ����д������
** ��  �Σ�*CMD��ָ���д�������
** ����ֵ��RET_SUCCESS
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

