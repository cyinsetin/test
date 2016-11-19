
#include "Global.h"


#define TIMEWINDOW_MIN  200               //ʱ�䴰����0.02s��0.2s
#define TIMEWINDOW_MAX  2000	              //ʱ�䴰����0.02s��2s
#define REGULATION    11 //10//ValidStepStart//15      //��Ϊ�ҵ��ȶ���������Ҫ�Ĳ���
#define INVALID		  3  //checklater  2               //��Ϊʧȥ�ȶ���������Ҫ�Ĳ���

#define TIMEWINDOW_MAX_S  20              //ʱ�䴰����0.02s��2s


//#define TWI_ADDRESS 0x88		//TWI/I2C�豸��ַ
int STEPS;
int SD_ForSleepAwake[20]={0};

//unsigned char itemp,jtemp,temp;								
char _bad_flag;//[3];									
//unsigned char Receive;	    				
//unsigned char ResultIndex;										
long  Adresult[3];											
char  sampling_counter=0;								
int _adresult;//[3];							
int _max=0;//[3]={0,0,0};
int _min=2048;//[3]={1000,1000,1000};
int _dc=500;//[3]={500,500,500};
int _vpp=30;//[3]={30,30,30};	
int  _precision=5;//[3]={5,5,5};	
int _old_fixed;//[3];
int _new_fixed;//[3];

int Vpp35Max[4]={0};//[3]={0,0,0};
int Vpp35Min[4]={2048,2048,2048,2048};//[3]={1000,1000,1000};

int Now8BitSensor[4]={0};
int Pre8BitSensor[4]={0};

  

int  AllSTEPS=0;


//unsigned long int STEPS_Temp;
int _array0=0;//[3]={1,1,1};
int _array1=0;//[3]={1,1,1};
int _array2=0;//[3]={0,0,0};
int _array3=0;//[3]={0,0,0};

int Interval=0;        //��¼ʱ������
char TempSteps=0;       //�ǲ�����
char InvalidSteps=0;    //��Ч������
char ReReg=2;           //��¼�Ƿ����¿�ʼѰ�ҹ���
								 
int Interval2=0;        //��¼ʱ������
char InvalidSteps2=0;    //��Ч������
								 
extern char WaveFlag;
extern char StepFlag;


void Init_Pedometer(void)
{
	Interval=0;
	Interval2=0;
	//Start_flag = 0;//ֹͣ
	sampling_counter = 0;
	STEPS = 0;//for pedometer
}

int IntervalTime[18]={0};
char IntervalTimeIndex=0;

int Aver_Data(char Start,char num)
{
	char i=0;
	int sum=0;
	for(i=Start;i<Start+num;i++)
	{
		sum+=IntervalTime[i];
	}
	return sum/num;
}

char StepAddCheck(void)
{
	char RtValue=0;
	
	if((Aver_Data(0,2)<600) && IntervalTime[1]<800 && IntervalTime[2]>=800 &&IntervalTime[3]<800 && (Aver_Data(3,2)<600))
	{
		RtValue=1;
	}
	else if((Aver_Data(0,2)<600) && IntervalTime[1]<800 && IntervalTime[2]>=800 && IntervalTime[3]>=800 &&IntervalTime[4]<800 &&(Aver_Data(4,2)<600))
	{
		RtValue=2;
	}
	else if((Aver_Data(0,3)<600) && IntervalTime[2]<800 && IntervalTime[3]>=800 && IntervalTime[4]>=800 && IntervalTime[5]>=800 && IntervalTime[6]<800&&(Aver_Data(6,3)<600))
	{
		RtValue=3;
	}
	else if((Aver_Data(0,4)<600) && IntervalTime[3]<800 && IntervalTime[4]>=800 && IntervalTime[5]>=800 && IntervalTime[6]>=800 && IntervalTime[7]>=800  && IntervalTime[8]<800 &&(Aver_Data(8,4)<600))
	{
		RtValue=4;
	}
	else if((Aver_Data(0,5)<600) && IntervalTime[4]<800 && IntervalTime[5]>=800 && IntervalTime[6]>=800 && IntervalTime[7]>=800&& IntervalTime[8]>=800 && IntervalTime[9]>=800  && IntervalTime[10]<800 &&(Aver_Data(10,5)<600))
	{
		RtValue=5;
	}
	return RtValue;
}

void TimeWindow2(void)
{
  if(ReReg==2)//������¿�ʼ�ĵ�һ����ֱ���ڼǲ������м�1
  {
    //TempSteps++;
    //Interval2=0;
    //ReReg=1;
    InvalidSteps2=0;	
  }
  else//��������¿�ʼ�ĵ�һ��
  {
    if((Interval2>=TIMEWINDOW_MIN)&&\
		((Interval2<TIMEWINDOW_MAX)))//||( (Interval<=TIMEWINDOW_MAX_S) && (TempSteps >= REGULATION_S))))	//���ʱ��������Ч��ʱ�䴰��
    {
	  InvalidSteps2=0;
	  Interval2=0;
    }
    else if(Interval2<TIMEWINDOW_MIN) //���ʱ����С��ʱ�䴰����
    {	
      if(ReReg==0 )//|| (ReReg==1 && TempSteps>=12))//����Ѿ��ҵ�����
      {
        if(InvalidSteps2<255) 	
		InvalidSteps2++;	//��Ч�������1
        if( (ReReg==0 && InvalidSteps2>=INVALID)
			)//||(ReReg==1 && TempSteps>=12 && InvalidSteps2>=2))//�����Ч���ﵽ��Ҫ�����ֵ��������Ѱ�ҹ���
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
        else //����ֻ������һ�εļǲ������Ǽ����ǲ�������Ҫ����Ѱ�ҹ���
        {
          Interval2=0;
        }
      }
      else if(ReReg==1)	//�����û���ҵ����ɣ���֮ǰ��Ѱ�ҹ��ɹ�����Ч������Ѱ�ҹ���
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
    else if(Interval2>TIMEWINDOW_MAX)//���ʱ��������ʱ�䴰���ޣ��ǲ��Ѿ���ϣ�����Ѱ�ҹ���
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
   char i=0;

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
  if(ReReg==2)//������¿�ʼ�ĵ�һ����ֱ���ڼǲ������м�1
  {
    TempSteps++;
    Interval=0;
    ReReg=1;
    InvalidSteps=0;	
  }
  else//��������¿�ʼ�ĵ�һ��
  {
  
    if(Interval>TIMEWINDOW_MAX || Interval2>TIMEWINDOW_MAX)//���ʱ��������ʱ�䴰���ޣ��ǲ��Ѿ���ϣ�����Ѱ�ҹ���
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
		((Interval<TIMEWINDOW_MAX)))//||( (Interval<=TIMEWINDOW_MAX_S) && (TempSteps >= REGULATION_S))))	//���ʱ��������Ч��ʱ�䴰��
    {

	{
	      InvalidSteps=0;
	      if(ReReg==1)//�����û���ҵ�����
	      {
	        TempSteps++;//�ǲ������1
		 if(TempSteps>12) 
		 {
		 	TempSteps+=StepAddCheck();
		 }
	        if(TempSteps>=REGULATION)//����ǲ�����ﵽ��Ҫ��Ĺ�����
	        {
		   	{
		          ReReg=0;//�Ѿ��ҵ�����
		          if((STEPS+TempSteps)>255)
				STEPS=255;
			   else
		          	STEPS=STEPS+TempSteps;//������ʾ

			   AllSTEPS+=TempSteps;		  
		          TempSteps=0;
		   	}
	        }
	        Interval=0;
	      }
	      else if(ReReg==0)//����Ѿ��ҵ����ɣ�ֱ�Ӹ�����ʾ
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
    else if(Interval<TIMEWINDOW_MIN) //���ʱ����С��ʱ�䴰����
    {	
      if(ReReg==0||(ReReg==1 && TempSteps>=12))//����Ѿ��ҵ�����
      {
        if(InvalidSteps<255) 	
		InvalidSteps++;	//��Ч�������1
        if( (ReReg==0 && InvalidSteps2>=INVALID)
			||(ReReg==1 && TempSteps>=12 && InvalidSteps2>=2)//�����Ч���ﵽ��Ҫ�����ֵ��������Ѱ�ҹ���
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
        else //����ֻ������һ�εļǲ������Ǽ����ǲ�������Ҫ����Ѱ�ҹ���
        {
          Interval=0;
        }
        //StepFlag=0;
	  
      }
      else if(ReReg==1)	//�����û���ҵ����ɣ���֮ǰ��Ѱ�ҹ��ɹ�����Ч������Ѱ�ҹ���
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


float Sqrt_IM (float x) 
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i>>1);
	x = *(float*)&i;
	x = x*(1.5f - xhalf*x*x);
	return 1/x;
}


long  Adresult_S=0;											

volatile int WaveLow=0;
volatile int PreHighPos=0;
volatile int PreLowPos=0;
int WaveHigh=0;

#define Rising			1
#define Falling 		2

char WaveFlag=0;

int PreVpp=0;
int PreDc=0;

#define MinValidVpp	20//26

int SampleCounter=0;

volatile char PreValidStep=0;

char StepData[6]={0};

char TempWaveLow=0;
char TempWavehigh=0;

char LowPosCounter=0;
char HighPosCounter=0;

char PreValidStepCounter=0;

char PreWaveFlag=0;

char StepFlag=0;
char SenSlIndex=0;
int SenSlData[4][10];

#define ABS(x,y)  ((x>y) ? (x-y) : (y-x))

int SensorSleepDetTime=0;
char SensorPowerUpCounter=0;
//int PreAdresult_S[4];
int PreAdresult_S;


char SensorDataAnalize(long SensorData)
{
  	char Data;
	char RtValue=0;
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
	else if(WaveFlag == Rising) //Then '����
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
					|| (0 && (StepData[4] > Data || StepData[5] > Data || StepData[6] > Data)) 
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

char SensorDataAnalize_2(long SensorData)
{
	//char i=0;  //modify by jack,open the sleep.
	char RtValue=0;
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

#if 0   //modify by jack. open sleep...
void SensorDataAnalizeForSleep(long SensorData,long X)
{
	char i;
	  if(SD_ForIdle[0]<255)
	  {
		  if(ABS(X,1024)>150)
		  {
			if(SD_ForIdle[0]<255)
			  	SD_ForIdle[0]++;
		  }
	  }
	  SenSlData[0][SenSlIndex]=Adresult[0];
	  SenSlData[1][SenSlIndex]=Adresult[1];
	  SenSlData[2][SenSlIndex]=Adresult[2];
	  SenSlData[3][SenSlIndex]=SensorData;
	  if(SenSlIndex>=1)//>=2)
	  {
		Now8BitSensor[0]=(SenSlData[0][0]+SenSlData[0][1])/2;//+SenSlData[2]+SenSlData[3])/4;
		Now8BitSensor[1]=(SenSlData[1][0]+SenSlData[1][1])/2;//+SenSlData[2]+SenSlData[3])/4;
		Now8BitSensor[2]=(SenSlData[2][0]+SenSlData[2][1])/2;//+SenSlData[2]+SenSlData[3])/4;
		Now8BitSensor[3]=(SenSlData[3][0]+SenSlData[3][1])/2;//+SenSlData[2]+SenSlData[3])/4;

		for(i=0;i<4;i++)
		{
			if(Now8BitSensor[i]>Vpp35Max[i])
				Vpp35Max[i]=Now8BitSensor[i];
			if(Now8BitSensor[i]<Vpp35Min[i])
				Vpp35Min[i]=Now8BitSensor[i];
		}
		if(ABS(Now8BitSensor[3],Pre8BitSensor[3])>64)
		{
			if(SD_ForDeepLightSleep[0]<255)
				SD_ForDeepLightSleep[0]++;
		}
		else if(ABS(Now8BitSensor[0],Pre8BitSensor[0])>64)
		{
			if(SD_ForDeepLightSleep[0]<255)
				SD_ForDeepLightSleep[0]++;
		}
		else if(ABS(Now8BitSensor[1],Pre8BitSensor[1])>64)
		{
			if(SD_ForDeepLightSleep[0]<255)
				SD_ForDeepLightSleep[0]++;
		}
		else if(ABS(Now8BitSensor[2],Pre8BitSensor[2])>64)
		{
			if(SD_ForDeepLightSleep[0]<255)
				SD_ForDeepLightSleep[0]++;
		}

		Pre8BitSensor[0]=Now8BitSensor[0];
		Pre8BitSensor[1]=Now8BitSensor[1];
		Pre8BitSensor[2]=Now8BitSensor[2];
		Pre8BitSensor[3]=Now8BitSensor[3];
		SenSlIndex=0;
	  }
	  else
		  SenSlIndex++;
}
#endif	  

char GensorTest=0;
/*
void GSensorDataSamplingHandle(void)
{
  //----------------------------------------------ADC����----------------------//
  uint16_t x,y,z;
  char tmp[6]={0};

   if(1)//SensorSleepCounter>SensorIntoSLTIME)
  {
  	SensorSleepDetTime+=100;
	if(SensorSleepDetTime>=1000)
	{
		SensorSleepDetTime=0;
	}
	else
	{
		if(SensorSleepDetTime==900)
		{
			Enable_GSensor2();
		}
		return;
	}
  }
  if(Interval<20000)
  	Interval+=20;//for pedometer
  if(Interval2<20000)
  	Interval2+=20;

  
  Read_Sen_datas(0x28,tmp,6);

  x=(tmp[0]>>4)|(int)tmp[1]<<4;
  y=(tmp[2]>>4)|(int)tmp[3]<<4;
  z=(tmp[4]>>4)|(int)tmp[5]<<4;

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
			BLE_Advertising_Start();		
			Clr_SensorPowerDownFlag();
			AppTimer_20MsRun_Start();			
			SensorSleepDetTime=0;
			SensorPowerUpCounter=0;
			printData("Clr_SensorPowerDownFlag",1);
			
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

  //  SensorDataAnalizeForSleep(Adresult_S,Adresult[0]);  //modify by jack. open sleep..
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
*/
