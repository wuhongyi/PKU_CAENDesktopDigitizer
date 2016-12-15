// offline.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 12月  8 19:25:47 2016 (+0800)
// Last-Updated: 三 12月 14 10:45:14 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 69
// URL: http://wuhongyi.cn 

#include "offline.hh"

#include "TSpline.h"
#include <cmath>
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

offline::offline()
{


}

offline::~offline()
{


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void offline::SetADCMSPS(int adc)
{
  Module_ADCMSPS = adc;
  if((Module_ADCMSPS != 100) && (Module_ADCMSPS != 250) && (Module_ADCMSPS != 500)) std::cout<<"ADCMSPS != 100,250,500"<<std::endl;
  deltaT = 1.0/((double)Module_ADCMSPS);
}

void offline::SetPreampTau(double tau)
{
  PreampTau = tau;
  b1 = exp(-1.0 * deltaT / PreampTau);
}

void offline::SetFastFilterPar(double fl,double fg,int thre)
{
  FastFilterRange = 0;

  // Calculate fast length
  if(Module_ADCMSPS == 100)
    {
      FL = (unsigned int)ROUND(fl * (double)Module_ADCMSPS / std::pow(2.0, (double)FastFilterRange));
      FG = (unsigned int)ROUND(fg * (double)Module_ADCMSPS / std::pow(2.0, (double)FastFilterRange));
    }
  else
    {
      if(Module_ADCMSPS == 250)
	{
	  FL = (unsigned int)ROUND(fl * (double)(Module_ADCMSPS / 2) / std::pow(2.0, (double)FastFilterRange));
	  FG = (unsigned int)ROUND(fg * (double)(Module_ADCMSPS / 2) / std::pow(2.0, (double)FastFilterRange));
	}
      else
	{
	if(Module_ADCMSPS == 500)
	  {
	    FL = (unsigned int)ROUND(fl * (double)(Module_ADCMSPS / 5) / std::pow(2.0, (double)FastFilterRange));
	    FG = (unsigned int)ROUND(fg * (double)(Module_ADCMSPS / 5) / std::pow(2.0, (double)FastFilterRange));
	  }
	}
    }
			
  if( (FL + FG) > FASTFILTER_MAX_LEN )
    {
      FG = FASTFILTER_MAX_LEN - FL;
    }
  if( FL < MIN_FASTLENGTH_LEN )
    {
      FL = MIN_FASTLENGTH_LEN;
      if( (FL + FG) > FASTFILTER_MAX_LEN )
	{
	  FG = FASTFILTER_MAX_LEN - MIN_FASTLENGTH_LEN;
	}
    }
  

  if(Module_ADCMSPS == 100)
    FastThresh = (int)(thre * (double)FL);
  else if(Module_ADCMSPS == 250)
    FastThresh = (int)(thre * (double)FL * 2.0);
  else if(Module_ADCMSPS == 500)
    FastThresh = (int)(thre * (double)FL * 5.0);

  // Check FastThresh limit
  if( FastThresh >= FAST_THRESHOLD_MAX )
    {
      FastThresh = (int)(((double)FAST_THRESHOLD_MAX / (double)FL - 0.5) * (double)FL);	// in ADC counts
    }
}

void offline::SetSlowFilterPar(double sl,double sg,int slowrange)
{
  SlowFilterRange = slowrange;
  
  if(Module_ADCMSPS == 100)
    {
      SL = (int)ROUND(sl * (double)Module_ADCMSPS / std::pow(2.0, (double)SlowFilterRange));
      SG = (int)ROUND(sg * (double)Module_ADCMSPS / std::pow(2.0, (double)SlowFilterRange));
    }
  else
    {
      if(Module_ADCMSPS == 250)
	{
	  SL = (int)ROUND(sl * (double)(Module_ADCMSPS / 2) / std::pow(2.0, (double)SlowFilterRange));
	  SG = (int)ROUND(sg * (double)(Module_ADCMSPS / 2) / std::pow(2.0, (double)SlowFilterRange));
	}
      else
	{
	  if(Module_ADCMSPS == 500)
	    {
	      SL = (int)ROUND(sl * (double)(Module_ADCMSPS / 5) / std::pow(2.0, (double)SlowFilterRange));
	      SG = (int)ROUND(sg * (double)(Module_ADCMSPS / 5) / std::pow(2.0, (double)SlowFilterRange));
	    }
	}
    }

  
  if( (SL + SG) > SLOWFILTER_MAX_LEN)
    {
      SG = SLOWFILTER_MAX_LEN - SL;
    }
  if( SG < MIN_SLOWGAP_LEN)
    {
      SG = MIN_SLOWGAP_LEN;
      if((SL + SG) > SLOWFILTER_MAX_LEN)
	{
	  SL = SLOWFILTER_MAX_LEN - MIN_SLOWGAP_LEN;
	}
    }
  if( SL < MIN_SLOWLENGTH_LEN)
    {
      SL = MIN_SLOWLENGTH_LEN;
      if((SL + SG) > SLOWFILTER_MAX_LEN)
	{
	  SG = SLOWFILTER_MAX_LEN - MIN_SLOWLENGTH_LEN;
	}
    }
}

void offline::PrintFilterPar()
{

  if(Module_ADCMSPS == 100)
    {
      std::cout<<"TRIGGER_RISETIME:"<<(double)FL * std::pow(2.0, (double)FastFilterRange) / (double)Module_ADCMSPS<<"  TRIGGER_FLATTOP:"<<(double)FG * std::pow(2.0, (double)FastFilterRange) / (double)Module_ADCMSPS<<"  TRIGGER_THRESHOLD:"<<(double)FastThresh / (double)FL<<std::endl;
      std::cout<<"ENERGY_RISETIME:"<<(double)SL * std::pow(2.0, (double)SlowFilterRange) / (double)Module_ADCMSPS<<"  ENERGY_FLATTOP:"<<(double)SG * std::pow(2.0, (double)SlowFilterRange) / (double)Module_ADCMSPS<<std::endl;
    }
  else
    {
      if(Module_ADCMSPS == 250)
	{
	  std::cout<<"TRIGGER_RISETIME:"<<(double)FL * std::pow(2.0, (double)FastFilterRange) / (double)(Module_ADCMSPS / 2)<<"  TRIGGER_FLATTOP:"<<(double)FG * std::pow(2.0, (double)FastFilterRange) / (double)(Module_ADCMSPS / 2)<<"  TRIGGER_THRESHOLD:"<<(double)FastThresh / ((double)FL * 2.0)<<std::endl;
	  std::cout<<"ENERGY_RISETIME:"<<(double)SL * std::pow(2.0, (double)SlowFilterRange) / (double)(Module_ADCMSPS / 2)<<"  ENERGY_FLATTOP:"<<(double)SG * std::pow(2.0, (double)SlowFilterRange) / (double)(Module_ADCMSPS / 2)<<std::endl;
	}
      else
	{
	  if(Module_ADCMSPS == 500)
	    {
	      std::cout<<"TRIGGER_RISETIME:"<<(double)FL * std::pow(2.0, (double)FastFilterRange) / (double)(Module_ADCMSPS / 5)<<"  TRIGGER_FLATTOP:"<<(double)FG * std::pow(2.0, (double)FastFilterRange) / (double)(Module_ADCMSPS / 5)<<"  TRIGGER_THRESHOLD:"<<(double)FastThresh / ((double)FL * 5.0)<<std::endl;
	      std::cout<<"ENERGY_RISETIME:"<<(double)SL * std::pow(2.0, (double)SlowFilterRange) / (double)(Module_ADCMSPS / 5)<<"  ENERGY_FLATTOP:"<<(double)SG * std::pow(2.0, (double)SlowFilterRange) / (double)(Module_ADCMSPS / 5)<<std::endl;
	    }
	}
    }

}

void offline::SetEventData(unsigned short size,unsigned short *data)
{
  Size = size;
  for (int i = 0; i < Size; ++i)
    {
      Data[i] = data[i];
    }
  DataPrimaryProcess();
}

void offline::SetEventData(int size,short *data)
{
  Size = size;
  for (int i = 0; i < Size; ++i)
    {
      Data[i] = data[i];
    }
  DataPrimaryProcess();
}

void offline::SetEventData(int size,int *data)
{
  Size = size;
  for (int i = 0; i < Size; ++i)
    {
      Data[i] = data[i];
    }
  DataPrimaryProcess();
}

void offline::DataPrimaryProcess()
{
  bsum0 = 0;
  for (int i = 0; i < CalculateBaselinePoint; ++i)
    {
      bsum0 += Data[i];
    }
  baseline = bsum0/CalculateBaselinePoint;

  for (int i = 0; i < Size; ++i)
    {
      if(PulsePolarity)
	{
	  Data[i] -= baseline;
	}
      else
	{
	  Data[i] = baseline - Data[i];
	}
    }
}

void offline::GetWaveData(int *data)
{
for (int i = 0; i < Size; ++i)
  {
    data[i] = Data[i];
  }
}

void offline::GetFastFilter(int *data)
{
  FastLen = FL * (unsigned int)std::pow(2.0, (double)FastFilterRange);
  FastGap = FG * (unsigned int)std::pow(2.0, (double)FastFilterRange);
  // std::cout<<"FastLen:"<<FastLen<<"  FastGap:"<<FastGap<<std::endl;
  
  offset = 2*FastLen + FastGap - 1;
  for(x = offset; x < Size; x++)
    {
      fsum0 = 0;
      for(y = (x-offset); y < (x-offset+FastLen); y++)
	{
	  fsum0 += Data[y];
	}
      fsum1 = 0;
      for(y = (x-offset+FastLen+FastGap); y < (x-offset+2*FastLen+FastGap); y++)
	{
	  fsum1 += Data[y];
	}
      // std::cout<<x<<"  "<<fsum0<<"  "<<fsum1<<std::endl;
      data[x] = ((double)fsum1 - (double)fsum0)/(double)FastLen;
    }
  for(x = 0; x < offset; x++)
    {
      data[x] = data[offset];
    }
}

void offline::GetSlowFilter(int *data)
{
  SlowLen = SL * (unsigned int)std::pow(2.0, (double)SlowFilterRange);
  SlowGap = SG * (unsigned int)std::pow(2.0, (double)SlowFilterRange);
  
  c0 = -(1.0 - b1) * std::pow(b1, (double)SlowLen) * 4.0 / (1.0 - std::pow(b1, (double)SlowLen));
  c1 = (1.0 - b1) * 4.0;
  c2 = (1.0 - b1) * 4.0 / (1.0 - std::pow(b1, (double)SlowLen));  

  offset = 2*SlowLen + SlowGap - 1;
  for(x = offset; x < Size; x++)
    {
      esum0 = 0;
      for(y = (x-offset); y < (x-offset+SlowLen); y++)
	{
	  if(y < 0)
	    {
	      // std::cout<<"y<0  " <<SlowGap<<"  "<<SlowLen<<"  "<<y<<std::endl;
	      // return -1;
	      esum0 += 0;
	    }
	  else
	    {
	      esum0 += Data[y];
	    }
	}
      esum1 = 0;
      for(y = (x-offset+SlowLen); y < (x-offset+SlowLen+SlowGap); y++)
	{
	  if(y < 0)
	    {
	      std::cout<<"error: y<0"<<std::endl;
	    }
	  esum1 += Data[y];
	}
      esum2 = 0;
      for(y = (x-offset+SlowLen+SlowGap); y < (x-offset+2*SlowLen+SlowGap); y++)
	{
	  if(y < 0)
	    {
	      std::cout<<"error: y<0"<<std::endl;
	    }
	  esum2 += Data[y];
	}
      data[x] = c0*(double)esum0+c1*(double)esum1+c2*(double)esum2;
    }

    for(x = 0; x < offset; x++)
    {
      data[x] = data[offset];
    }
}

double offline::GetRiseTime()
{
  int temp = -1;
  for (int i = 0; i < Size; ++i)
    {
      if(Data[i] > temp) temp = Data[i];
    }

  double percent10,percent90;
  percent10 = temp*0.1;
  percent90 = temp*0.9;

  int min = -1;
  int max = -1;

  for (int i = 0; i < Size; ++i)
    {
      if(Data[i] > percent10)
	{
	  min = i-1;
	  break;
	}
    }

  for (int i = 0; i < Size; ++i)
    {
      if(Data[i] > percent90)
	{
	  max = i;
	  break;
	}
    }

  if(max< 0 || min < 0) return -1;

  // std::cout<<"--: min:"<<min<<"  max:"<<max<<std::endl;
  // std::cout<<"--: min:"<<percent10<<"  max:"<<percent90<<std::endl;
  // TSpline3 *spline3 = new TSpline3();//必须单调才能使用？？？
  // for (int i = min; i <= max; ++i)
  //   {
  //     spline3->SetPoint(i-min,Data[i],i);
  //     std::cout<<i-min<<" "<<Data[i]<<" "<<i<<std::endl;
  //   }

  // std::cout<<"=90 :"<<spline3->Eval(percent90)<<std::endl;
  // return (spline3->Eval(percent90)-spline3->Eval(percent10))*(1000.0/Module_ADCMSPS);
  return -1;
}

int offline::GetEnergy()
{
  if(Module_ADCMSPS == 100)
    Threshold = (double)FastThresh / (double)FL;
  else if(Module_ADCMSPS == 250)
    Threshold = (double)FastThresh / ((double)FL * 2.0);
  else if(Module_ADCMSPS == 500)
    Threshold = (double)FastThresh / ((double)FL * 5.0);
	
  FastLen = FL * (unsigned int)std::pow(2.0, (double)FastFilterRange);
  FastGap = FG * (unsigned int)std::pow(2.0, (double)FastFilterRange);
  SlowLen = SL * (unsigned int)std::pow(2.0, (double)SlowFilterRange);
  SlowGap = SG * (unsigned int)std::pow(2.0, (double)SlowFilterRange);


  c0 = -(1.0 - b1) * std::pow(b1, (double)SlowLen) * 4.0 / (1.0 - std::pow(b1, (double)SlowLen));
  c1 = (1.0 - b1) * 4.0;
  c2 = (1.0 - b1) * 4.0 / (1.0 - std::pow(b1, (double)SlowLen));

  offset = 2*FastLen + FastGap - 1;
  for(x = offset; x < Size; x++)
    {
      fsum0 = 0;
      for(y = (x-offset); y < (x-offset+FastLen); y++)
	{
	  fsum0 += Data[y];
	}
      fsum1 = 0;
      for(y = (x-offset+FastLen+FastGap); y < (x-offset+2*FastLen+FastGap); y++)
	{
	  fsum1 += Data[y];
	}
      fastfilter = ((double)fsum1 - (double)fsum0)/(double)FastLen;

      if(fastfilter >= Threshold) break;
      if(x == Size-1)
	{
	  // std::cout<<"Under Threshold ..."<<std::endl;
	  return -1;
	}
    }

  // std::cout<<"Threshold:"<<Threshold<<"  x:"<<x<<std::endl;
  x = x+SlowLen+SlowGap/2;
  // x = x+SlowLen+SlowGap-2;
  offset = 2*SlowLen + SlowGap - 1;

  esum0 = 0;
  for(y = (x-offset); y < (x-offset+SlowLen); y++)
    {
      if(y < 0)
	{
	  // std::cout<<"y<0  " <<SlowGap<<"  "<<SlowLen<<"  "<<y<<std::endl;
	  esum0 += 0;
	}
      else
	{
	  esum0 += Data[y];
	}
    }
  esum1 = 0;
  for(y = (x-offset+SlowLen); y < (x-offset+SlowLen+SlowGap); y++)
    {
      if(y < 0)
	{
	  std::cout<<"y<0"<<std::endl;
	  return -1;
	}
      esum1 += Data[y];
    }
  esum2 = 0;
  for(y = (x-offset+SlowLen+SlowGap); y < (x-offset+2*SlowLen+SlowGap); y++)
    {
      if(y < 0)
	{
	  std::cout<<"y<0"<<std::endl;
	  return -1;
	}
      esum2 += Data[y];
    }
  return c0*(double)esum0+c1*(double)esum1+c2*(double)esum2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



// 
// offline.cc ends here
