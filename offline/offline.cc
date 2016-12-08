// offline.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 12月  8 19:25:47 2016 (+0800)
// Last-Updated: 四 12月  8 20:39:04 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
// URL: http://wuhongyi.cn 

#include "offline.hh"

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
  deltaT = 1.0/((double)Module_ADCMSPS);
}

void offline::SetPreampTau(double tau)
{
  PreampTau = tau;
  b1 = exp(-1.0 * deltaT / PreampTau);
}

void offline::SetFastFilterPar(int fl,int fg,int thre)
{
  Threshold = thre;
  FastLen = fl*Module_ADCMSPS/1000;
  FastGap = fg*Module_ADCMSPS/1000;
}

void offline::SetSlowFilterPar(int sl,int sg)
{
  SlowLen = sl*Module_ADCMSPS/1000;
  SlowGap = sg*Module_ADCMSPS/1000;

  c0 = -(1.0 - b1) * std::pow(b1, (double)SlowLen) * 4.0 / (1.0 - std::pow(b1, (double)SlowLen));
  c1 = (1.0 - b1) * 4.0;
  c2 = (1.0 - b1) * 4.0 / (1.0 - pow(b1, (double)SlowLen));
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

int offline::GetEnergy()
{
  offset = 2*FastLen + FastGap - 1;
  for(x = offset; x < Size; x++)
    {
      fsum0[x] = 0;
      for(y = (x-offset); y < (x-offset+FastLen); y++)
	{
	  fsum0[x] += Data[y];
	}
      fsum1[x] = 0;
      for(y = (x-offset+FastLen+FastGap); y < (x-offset+2*FastLen+FastGap); y++)
	{
	  fsum1[x] += Data[y];
	}
      fastfilter = ((double)fsum1[x] - (double)fsum0[x])/(double)FastLen;

      if(fastfilter > Threshold) break;
      if(x == Size-1)
	{
	  std::cout<<"Under Threshold ..."<<std::endl;
	  return -1;
	}
    }
  
  x = x+SlowLen+SlowGap/2;
  offset = 2*SlowLen + SlowGap - 1;

  esum0[x] = 0;
  for(y = (x-offset); y < (x-offset+SlowLen); y++)
    {
      esum0[x] += Data[y];
    }
  esum1[x] = 0;
  for(y = (x-offset+SlowLen); y < (x-offset+SlowLen+SlowGap); y++)
    {
      esum1[x] += Data[y];
    }
  esum2[x] = 0;
  for(y = (x-offset+SlowLen+SlowGap); y < (x-offset+2*SlowLen+SlowGap); y++)
    {
      esum2[x] += Data[y];
    }
  return c0*(double)esum0[x] +c1*(double)esum1[x]+c2*(double)esum2[x];
}

// 
// offline.cc ends here
