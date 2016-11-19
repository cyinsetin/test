/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup led_softblink_example_main main.c
 * @{
 * @ingroup led_softblink_example
 * @brief LED Soft Blink Example Application main file.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "led_softblink.h"
#include "app_error.h"
#include "sdk_errors.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "app_util_platform.h"
#include "LIS3DH_Driver.h"
#include "SEGGER_RTT.H"
#include "SEGGER_RTT_CONF.H"

#ifdef SOFTDEVICE_PRESENT
#include "softdevice_handler.h"
#endif

/*Timer initalization parameters*/   
#define OP_QUEUES_SIZE          3
#define APP_TIMER_PRESCALER     0 

#define BIT0		0x0001
#define BIT1		0x0002
#define BIT2		0x0004
#define BIT3		0x0008
#define BIT4		0x0010
#define BIT5		0x0020
#define BIT6		0x0040
#define BIT7		0x0080

#define BIT8		0x0100
#define BIT9		0x0200
#define BIT10		0x0400
#define BIT11		0x0800
#define BIT12		0x1000
#define BIT13		0x2000
#define BIT14		0x4000
#define BIT15		0x8000

#define Rising			1
#define Falling 		2
#define MinValidVpp	20//26

DWORD  Adresult[3];			//采样结果值，X，Y，Z
BYTE GensorTest=0;			//
DWORD  Adresult_S=0;		//通过采样结果值，算出的向量值
WORD PreAdresult_S;			//预设的向量结果值
BYTE StepData[6]={0};
BYTE TempWaveLow=0;
BYTE TempWavehigh=0;
WORD PreVpp=0;
WORD PreDc=0;
BYTE PreWaveFlag=0;
BYTE StepFlag=0;

BYTE SD_ForIdle[10]={0};
BYTE SD_ForSleepAwake[20]={0};

BYTE WaveFlag=0;
BYTE LowPosCounter=0;
BYTE HighPosCounter=0;
BYTE PreValidStepCounter=0;
WORD WaveHigh=0;
volatile WORD WaveLow=0;
volatile WORD PreHighPos=0;
volatile WORD PreLowPos=0;
volatile BYTE PreValidStep=0;

WORD _array0=0;//[3]={1,1,1};
WORD _array1=0;//[3]={1,1,1};
WORD _array2=0;//[3]={0,0,0};
WORD _array3=0;//[3]={0,0,0};

BYTE  sampling_counter=0;								
WORD _adresult;//[3];							
WORD _max=0;//[3]={0,0,0};
WORD _min=2048;//[3]={1000,1000,1000};
WORD _dc=500;//[3]={500,500,500};
WORD _vpp=30;//[3]={30,30,30};	
WORD  _precision=5;//[3]={5,5,5};	
WORD _old_fixed;//[3];
WORD _new_fixed;//[3];
BYTE _bad_flag;//[3];			

BYTE SensorPowerDownBit=0;		//
WORD SensorSleepDetTime=0;		//
BYTE SensorPowerUpCounter=0;	//

#define SensorPowerDownFlag						(SensorPowerDownBit==1)//(SleepModeBYTE&DoSleepModeChangeBit)
#define Set_SensorPowerDownFlag()					(SensorPowerDownBit=1)//(SleepModeBYTE|=DoSleepModeChangeBit)
#define Clr_SensorPowerDownFlag()					(SensorPowerDownBit=0)//(SleepModeBYTE&=~DoSleepModeChangeBit)
#define ABS(x,y)  ((x>y) ? (x-y) : (y-x))


/**
 * @brief Function for LEDs initialization.
 */
static void leds_init(void)
{
    ret_code_t           err_code;
    
    led_sb_init_params_t led_sb_init_params = LED_SB_INIT_DEFAULT_PARAMS(LEDS_MASK);
    
    err_code = led_softblink_init(&led_sb_init_params);

    APP_ERROR_CHECK(err_code);
}

#ifndef SOFTDEVICE_PRESENT
static void lfclk_init(void)
{
    uint32_t err_code;
    nrf_drv_clock_config_t nrf_drv_clock_config = NRF_DRV_CLOCK_DEAFULT_CONFIG;

    err_code = nrf_drv_clock_init(&nrf_drv_clock_config);
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request();
}
#endif

BYTE SensorDataAnalize(DWORD SensorData)
{
  	BYTE Data;
	BYTE RtValue=0;
	StepData[0]  = StepData[1];
	StepData[1]  = StepData[2];
	StepData[2]  = StepData[3];
	StepData[3]  = StepData[4];
	StepData[4]  = StepData[5];
	StepData[5]  = (SensorData>>4);//Sheet1.Cells(i, 4)
	if(StepData[5] > 200 || StepData[5] <20)
	{
		StepData[5]=StepData[4];
	}
	Data = StepData[0];
	if(WaveFlag != 0) //Then
	{
		if(LowPosCounter<255)
			LowPosCounter++;

		if(HighPosCounter<255)
			HighPosCounter++;

		if(PreValidStepCounter<255)
			PreValidStepCounter++;
	}

	if(LowPosCounter>90 ||HighPosCounter>90)
	{
		WaveFlag=0;
	}


	if(WaveFlag == 0) //Then
	{
		WaveFlag = Rising;
		WaveHigh = 74;//77;
		WaveLow = 54;//51;
		TempWavehigh = WaveHigh;
		TempWaveLow = WaveLow;
		PreVpp = MinValidVpp;
		PreDc = 64;
		PreLowPos = 0;
		PreHighPos = 0;

		LowPosCounter=0;
		HighPosCounter=0;

		PreWaveFlag=WaveFlag;
		
	}
	else if(WaveFlag == Rising) //Then '上升
	{
		if(Data >= WaveHigh) //Then
		{
			if((StepData[1] < Data) && (StepData[1] > WaveHigh) &&  (Data - WaveHigh) > PreVpp / 4) //Then
				;
			else
				WaveHigh = Data;
		}
		else
		{
			if((WaveHigh > WaveLow) 
				&& ((WaveHigh - WaveLow) > MinValidVpp) 
				&& ((WaveHigh - WaveLow) > PreVpp * 1 / 4) 
				&& (WaveHigh > PreDc) 
				&& (HighPosCounter>=9)// ((i - PreHighPos) >= 9)
			) 
			{
				if(StepData[1] > Data || StepData[2] > Data || StepData[3] > Data 
					|| (0 && (StepData[4] > Data || StepData[5] > Data)) 
				)
					;
				else
				{
					WaveFlag = Falling;
					PreVpp = WaveHigh - WaveLow;
					PreDc = WaveLow + PreVpp / 2;
					TempWaveLow = WaveLow;
					WaveLow = Data;
					HighPosCounter=0;//PreHighPos = i - 1
					PreWaveFlag=WaveFlag;

				}
			}
			else
			{
				if(LowPosCounter>5&& ( Data < WaveLow))
				{
					WaveHigh = TempWavehigh;
					WaveLow = Data;
					WaveFlag = Falling;
				}
				else
				{
				}
			}
		}
	}
	else if(WaveFlag == Falling) //Then
	{
		if(Data <= WaveLow)
		{
			if((StepData[1] > Data) && (StepData[1] < WaveLow) && (WaveLow - Data) > PreVpp / 4 )
				;
			else//Else
				WaveLow = Data;
		}
		else
		{
			if((WaveHigh > WaveLow)  && ((WaveHigh - WaveLow) > MinValidVpp) 
				&& ((WaveHigh - WaveLow) > PreVpp * 1 / 4) && (WaveLow < PreDc) 
				&& (LowPosCounter>=9)//((i - PreLowPos) >= 9)
			)
			{
				if( StepData[1] < Data || StepData[2] < Data || StepData[3] < Data 
					//|| (0 && (StepData[4] < Data || StepData[5]  < Data || StepData[6]  < Data)) 
				)
					;
				else
				{
					WaveFlag = Rising;
					PreVpp = WaveHigh - WaveLow;
					PreDc = WaveLow + PreVpp * 1 / 2;
					TempWavehigh = WaveHigh;
					WaveHigh = Data;
					if(PreWaveFlag!=WaveFlag)
						RtValue=1;//StepFlag = 1;
					LowPosCounter=0;
					PreValidStepCounter=0;
					PreValidStep = 0;
					PreWaveFlag=WaveFlag;
				}
			}
			else
			{
				if(/*i - PreHighPos > 5*/HighPosCounter>5 &&  Data > WaveHigh )
				{
					WaveLow = TempWaveLow;
					WaveHigh = Data;
					WaveFlag = Falling;
				}
				else
				{
				}
			}
		}
	}
	return RtValue;
}

BYTE SensorDataAnalize_2(DWORD SensorData)
{
	//BYTE i=0;  //modify by jack,open the sleep.
	BYTE RtValue=0;
	_array3=_array2;
	_array2=_array1;
	_array1=_array0;
	_array0 = SensorData;
	_adresult=_array0 + _array1 + _array2 + _array3;
	_adresult=_adresult >> 2;

	if (_adresult > _max) {
		_max = _adresult;
	}
	if (_adresult < _min) {
		_min = _adresult;
	}
	sampling_counter = sampling_counter + 1;
	if ((sampling_counter == 50))
	{               
		sampling_counter = 0;
		{
			#if 0   //modify by jack, open the sleep.
			if(SD_ForDeepLightSleep[0]<255)
			for(i=0;i<4;i++)
			{
				if(ABS(Vpp35Max[i], Vpp35Min[i]) > 85)
				{
					SD_ForDeepLightSleep[0]++;
					break;
				}
			}
			Vpp35Max[0]=0;
			Vpp35Max[1]=0;
			Vpp35Max[2]=0;
			Vpp35Max[3]=0;

			Vpp35Min[0]=2048;
			Vpp35Min[1]=2048;
			Vpp35Min[2]=2048;
			Vpp35Min[3]=2048;
			#endif
			_vpp = _max - _min;
			if(_vpp>50)
			{
				if(SD_ForSleepAwake[0]<255)
					SD_ForSleepAwake[0]++;
			}
			_dc = _min + (_vpp >> 1);
			_max = 0;
			_min = 4096;//1023;
			_bad_flag = 0;

			if (_vpp>=160)
			{
				_precision=_vpp/32;//8
			}
			else
			{ 
				_precision=2;
				_bad_flag=1;
			}
		}
	}
	_old_fixed=_new_fixed;
	if (_adresult>=_new_fixed)                         
	{   
		if((_adresult-_new_fixed)>=_precision)
		{_new_fixed=_adresult;}
	}
	if (_adresult<_new_fixed)
	{   
		if((_new_fixed-_adresult)>=_precision)
		{_new_fixed=_adresult;}
	}
    if ((((_old_fixed>=_dc)&&(_new_fixed<_dc)))&&(_bad_flag==0))        
    {
	RtValue=1;	
    }
    return RtValue;
}

void TimeWindow2(void)
{
  if(ReReg==2)//如果是新开始的第一步，直接在记步缓存中加1
  {
    //TempSteps++;
    //Interval2=0;
    //ReReg=1;
    InvalidSteps2=0;	
  }
  else//如果不是新开始的第一步
  {
    if((Interval2>=TIMEWINDOW_MIN)&&\
		((Interval2<TIMEWINDOW_MAX)))//||( (Interval<=TIMEWINDOW_MAX_S) && (TempSteps >= REGULATION_S))))	//如果时间间隔在有效的时间窗内
    {
	  InvalidSteps2=0;
	  Interval2=0;
    }
    else if(Interval2<TIMEWINDOW_MIN) //如果时间间隔小于时间窗下限
    {	
      if(ReReg==0 )//|| (ReReg==1 && TempSteps>=12))//如果已经找到规律
      {
        if(InvalidSteps2<255) 	
					InvalidSteps2++;	//无效步缓存加1
        if( (ReReg==0 && InvalidSteps2>=INVALID)
			)//||(ReReg==1 && TempSteps>=12 && InvalidSteps2>=2))//如果无效步达到所要求的数值，则重新寻找规律
        {	
          InvalidSteps2=0;
					InvalidSteps=0;
          ReReg=1;
          TempSteps=1;
          Interval=0;
					Interval2=0;
					WaveFlag=0;
          StepFlag=0;
        }
        else //否则，只丢弃这一次的记步，但是继续记步，不需要重新寻找规律
        {
          Interval2=0;
        }
      }
      else if(ReReg==1)	//如果还没有找到规律，则之前的寻找规律过程无效，重新寻找规律
      {
        InvalidSteps=0;	
        InvalidSteps2=0;	
        ReReg=1;
        TempSteps=1;
        Interval=0;
				Interval2=0;
				WaveFlag=0;
        StepFlag=0;
      }
    }
    else if(Interval2>TIMEWINDOW_MAX)//如果时间间隔大于时间窗上限，记步已经间断，重新寻找规律
    {
      InvalidSteps=0;	
      InvalidSteps2=0;	
      ReReg=1;						
      TempSteps=1;
      Interval=0;
      Interval2=0;
      WaveFlag=0;
      StepFlag=0;
    }
  }		
}

void TimeWindow()
{
   BYTE i=0;

  // if(StepIndex<800)
//   {
  // 	StepIntervalTime[StepIndex++]=Interval;
 //  }
   
   if(Interval >200)
   {
	   for(i=15;i>0;i--)
	   {
	   	IntervalTime[i]=IntervalTime[i-1];
	   }
	   IntervalTime[0]=Interval;
   }
  if(ReReg==2)//如果是新开始的第一步，直接在记步缓存中加1
  {
    TempSteps++;
    Interval=0;
    ReReg=1;
    InvalidSteps=0;	
  }
  else//如果不是新开始的第一步
  {
  
    if(Interval>TIMEWINDOW_MAX || Interval2>TIMEWINDOW_MAX)//如果时间间隔大于时间窗上限，记步已经间断，重新寻找规律
    {
      InvalidSteps=0;	
      ReReg=1;						
      TempSteps=1;
      Interval=0;
      //StepFlag=0;
      WaveFlag=0;
      InvalidSteps2=0;	
      Interval2=0;

    }
		else if((Interval>=TIMEWINDOW_MIN)&&\
		((Interval<TIMEWINDOW_MAX)))//||( (Interval<=TIMEWINDOW_MAX_S) && (TempSteps >= REGULATION_S))))	//如果时间间隔在有效的时间窗内
    {

			{
	      InvalidSteps=0;
	      if(ReReg==1)//如果还没有找到规律
	      {
	        TempSteps++;//记步缓存加1
					if(TempSteps>12) 
					{
						TempSteps+=StepAddCheck();
					}
	        if(TempSteps>=REGULATION)//如果记步缓存达到所要求的规律数
	        {
							{
		          ReReg=0;//已经找到规律
		          if((STEPS+TempSteps)>255)
								STEPS=255;
							else
		          	STEPS=STEPS+TempSteps;//更新显示

							AllSTEPS+=TempSteps;		  
		          TempSteps=0;
							}
	        }
	        Interval=0;
	      }
	      else if(ReReg==0)//如果已经找到规律，直接更新显示
	      {
					STEPS+=StepAddCheck();
					AllSTEPS+=StepAddCheck();
	      	if(STEPS<255)
						STEPS++;
					AllSTEPS++;
	        TempSteps=0;
	        Interval=0;
					printData("STEPS==%d",STEPS);
	      }
			}
    }
    else if(Interval<TIMEWINDOW_MIN) //如果时间间隔小于时间窗下限
    {	
      if(ReReg==0||(ReReg==1 && TempSteps>=12))//如果已经找到规律
      {
        if(InvalidSteps<255) 	
					InvalidSteps++;	//无效步缓存加1
        if( (ReReg==0 && InvalidSteps2>=INVALID)
			||(ReReg==1 && TempSteps>=12 && InvalidSteps2>=2)//如果无效步达到所要求的数值，则重新寻找规律
			)
        {	
          InvalidSteps=0;
          ReReg=1;
          TempSteps=1;
          Interval=0;
					InvalidSteps2=0;	
					Interval2=0;
					WaveFlag=0;
        }
        else //否则，只丢弃这一次的记步，但是继续记步，不需要重新寻找规律
        {
          Interval=0;
        }
        //StepFlag=0;
	  
      }
      else if(ReReg==1)	//如果还没有找到规律，则之前的寻找规律过程无效，重新寻找规律
      {
        InvalidSteps=0;	
        ReReg=1;
        TempSteps=1;
        Interval=0;
        //StepFlag=0;
        WaveFlag=0;

      }
    }
  }		
}


/**  开平方根算法  **/
float Sqrt_IM (float x) 
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i>>1);
	x = *(float*)&i;
	x = x*(1.5f - xhalf*x*x);
	return 1/x;
}


void GSensorDataSamplingHandle(void)
{
	uint16_t	x,y,z;
	BYTE tmp[6]={0};
	
	Read_Sen_datas(0x28,tmp,6);
	x=(tmp[0]>>4)|(WORD)tmp[1]<<4;
	y=(tmp[2]>>4)|(WORD)tmp[3]<<4;
	z=(tmp[4]>>4)|(WORD)tmp[5]<<4;
				
	SEGGER_RTT_printf(0,"X-Axis = :%x ",x);
	SEGGER_RTT_printf(0,"Y-Axis = :%x ",y);
	SEGGER_RTT_printf(0,"Z-Axis = :%x\r\n",z);
	  if(x&BIT11)
  	  Adresult[0] =  2048-(x&~BIT11)&0xFFF;
  else 
  	Adresult[0] = x;
  if(y&BIT11)
  	  Adresult[1] =  2048-(y&~BIT11)&0xFFF;
  else 
  	Adresult[1] = y;
  if(z&BIT11)
  	  Adresult[2] =  2048-(z&~BIT11)&0xFFF;
  else 
  	Adresult[2] = z;

	if(!(GensorTest &BIT0)&&Adresult[0]!=0)// &&Adresult[0]!=-1) 
		GensorTest|=BIT0;
	if(!(GensorTest &BIT1)&&Adresult[1]!=0)// &&Adresult[1]!=-1) 
		GensorTest|=BIT1;
	if(!(GensorTest &BIT2)&&Adresult[2]!=0)// &&Adresult[2]!=-1) 
		GensorTest|=BIT2;

	Adresult_S=Adresult[0]*Adresult[0] + Adresult[1]*Adresult[1] + Adresult[2]*Adresult[2];
	Adresult_S=(unsigned int)Sqrt_IM((float)Adresult_S);
//printData("Adresult_S==%d",Adresult_S);
	SEGGER_RTT_printf(0,"Adresult_S = :%x\r\n",Adresult_S);
	
	if(SensorPowerDownFlag )//SensorSleepCounter>SensorIntoSLTIME)
	{
		#if 0
		if(HW_KEY_IN())
		{
			Clr_SensorPowerDownFlag();
			SensorSleepDetTime=0;
			SensorPowerUpCounter=0;
		}
		else if(ABS(PreAdresult_S[3],Adresult_S)>80
				||ABS(PreAdresult_S[0],Adresult[0])>80
				||ABS(PreAdresult_S[1],Adresult[1])>80
				||ABS(PreAdresult_S[2],Adresult[2])>80
				)
		#endif
		if(ABS(PreAdresult_S, Adresult_S)>85)
		{
			SensorPowerUpCounter++;
			if(SensorPowerUpCounter>=2)
			{
				//BLE_Advertising_Start();		
				//Clr_SensorPowerDownFlag();
				//AppTimer_20MsRun_Start();			
				SensorSleepDetTime=0;
				SensorPowerUpCounter=0;
				//printData("Clr_SensorPowerDownFlag",1);
				
			}
		}
		else
		{
			Disable_GSensor();
			
		}
		PreAdresult_S=Adresult_S;	
		//PreAdresult_S[1]=Adresult[1];	
		//PreAdresult_S[2]=Adresult[2];	
		//PreAdresult_S[3]=Adresult_S;	
		return;
	}
	else
	{
	  if(SD_ForIdle[0]<255)
	  {
		  if(ABS(Adresult_S,PreAdresult_S)>70)
		  {
			if(SD_ForIdle[0]<255)
			  	SD_ForIdle[0]++;
		  }
	  }
	  PreAdresult_S=Adresult_S;
	}
	SEGGER_RTT_printf(0,"PreAdresult_S = :%x\r\n",PreAdresult_S);
	StepFlag=SensorDataAnalize(Adresult_S);
  //SensorDataAnalize_2(Adresult_S);
  if(SensorDataAnalize_2(Adresult_S))
  {
		TimeWindow2();	 	
  }
  if (StepFlag)      
  {
    TimeWindow();
    StepFlag=0;
  }
}

void pedometer_handler(void * p_context)
{
	Enable_GSensor();
	GSensorDataSamplingHandle();
}

void timer_initial(void)
{
	uint32_t err_code;
	 APP_TIMER_DEF(pedometer_timer);
	 err_code = app_timer_create(&pedometer_timer,
                                        APP_TIMER_MODE_REPEATED,
                                        pedometer_handler);
	err_code = app_timer_start(pedometer_timer, APP_TIMER_TICKS(100, APP_TIMER_PRESCALER), NULL);
	//err_code = app_timer_stop(pedometer_timer);
}



/**
 * @brief Function for application main entry.
 */
//static unsigned char Test_buffer[6];
//static unsigned char Test_Byte;

int main(void)
{
    uint32_t err_code;
		//uint16_t x,y,z;

    // We need the LFCK running for APP_TIMER. In case SoftDevice is in use
    // it will start the LFCK during initialization, otherwise we have to
    // start it manually.
#ifdef SOFTDEVICE_PRESENT
    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, false);
#else
    lfclk_init();
#endif

    // Start APP_TIMER to generate timeouts.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, OP_QUEUES_SIZE, NULL);
		//APP_TIMER_INIT(APP_TIMER_PRESCALER, OP_QUEUES_SIZE, NULL);
    timer_initial();
		
    leds_init();
    err_code = led_softblink_start(LEDS_MASK);
    APP_ERROR_CHECK(err_code);
		
    Enable_GSensor();
		//Read_Sen_datas(0x0f,&Test_Byte,1);
		
		//SEGGER_RTT_printf(0,"G-Sensor ID = :%x ",Test_Byte);
		while (true)
    {
        __WFE();
				/*
				if(BUTTON_1==0)
				{
					Enable_GSensor();
					//Enable_GSensor2();
					nrf_delay_ms(100);
					Read_Sen_datas(0x28,Test_buffer,6);
					x=(Test_buffer[0]>>4)|(WORD)Test_buffer[1]<<4;
					y=(Test_buffer[2]>>4)|(WORD)Test_buffer[3]<<4;
					z=(Test_buffer[4]>>4)|(WORD)Test_buffer[5]<<4;
				
					SEGGER_RTT_printf(0,"X-Axis = :%x ",x);
					SEGGER_RTT_printf(0,"Y-Axis = :%x ",y);
					SEGGER_RTT_printf(0,"Z-Axis = :%x\r\n",z);
					nrf_delay_ms(200);
					Disable_GSensor();					
				}
				else
				{
					Enable_GSensor2();
					nrf_delay_ms(100);
					Read_Sen_datas(0x28,Test_buffer,6);
					x=(Test_buffer[0]>>4)|(WORD)Test_buffer[1]<<4;
					y=(Test_buffer[2]>>4)|(WORD)Test_buffer[3]<<4;
					z=(Test_buffer[4]>>4)|(WORD)Test_buffer[5]<<4;
				
					SEGGER_RTT_printf(0,"X-Axis = :%x ",x);
					SEGGER_RTT_printf(0,"Y-Axis = :%x ",y);
					SEGGER_RTT_printf(0,"Z-Axis = :%x\r\n",z);
					nrf_delay_ms(200);
					Disable_GSensor();
				}
				nrf_delay_ms(100);
				*/
    }
}

/** @} */
