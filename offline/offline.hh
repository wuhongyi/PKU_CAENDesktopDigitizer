// offline.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 12月  8 19:25:34 2016 (+0800)
// Last-Updated: 日 12月 11 20:19:09 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 21
// URL: http://wuhongyi.cn 

#ifndef _OFFLINE_H_
#define _OFFLINE_H_

#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define EVENTLENGTH 65535

#define FASTFILTER_MAX_LEN 127
#define FAST_THRESHOLD_MAX 65535
#define MIN_FASTLENGTH_LEN 2
#define SLOWFILTER_MAX_LEN 127
#define MIN_SLOWLENGTH_LEN 2
#define MIN_SLOWGAP_LEN 3

#define FASTFILTERRANGE_MAX 0
#define FASTFILTERRANGE_MIN 0
#define SLOWFILTERRANGE_MAX 6
#define SLOWFILTERRANGE_MIN 1

#define ROUND(x)    ((x) < 0.0 ? ceil((x) - 0.5) : floor((x) + 0.5))

class offline
{
public:
  offline();
  virtual ~offline();

  void SetPulsePolarity(bool pp) {PulsePolarity = pp;}//设置脉冲极性
  
  void SetADCMSPS(int adc);// MHz
  void SetPreampTau(double tau);//us

  void SetFastFilterPar(double fl,double fg,int thre);//us-建议0.1  us-建议0.1  units
  void SetSlowFilterPar(double sl,double sg,int slowrange = 2);//us us
  void PrintFilterPar();
  
  void SetCalculateBaselinePoint(int n) {CalculateBaselinePoint = n;}

  void SetEventData(int size,short *data);
  void SetEventData(int size,int *data);

  void GetWaveData(int *data);
  void GetFastFilter(int *data);
  void GetSlowFilter(int *data);
  int GetEnergy();// <0 error  >0 energy

private:
  void DataPrimaryProcess();//将数据转为基线为0的正脉冲
  
private:
  bool PulsePolarity;
  int Module_ADCMSPS;
  double PreampTau;

  int FastFilterRange;
  int FL,FG;
  int FastThresh;
  int SlowFilterRange;
  int SL,SG;
  
  int Threshold;
  int FastLen, FastGap;
  int SlowLen, SlowGap;

  int CalculateBaselinePoint;
  int baseline;
  double deltaT;
  int bsum0;
  double b1, c0, c1, c2;
  int offset, x, y;

  double fastfilter;
  double slowfilter;
  
  int esum0, esum1, esum2;
  int fsum0, fsum1;
  
  int Size;
  int Data[EVENTLENGTH];
};

#endif /* _OFFLINE_H_ */
// 
// offline.hh ends here
