// offline.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 12月  8 19:25:34 2016 (+0800)
// Last-Updated: 四 12月  8 20:21:38 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1
// URL: http://wuhongyi.cn 

#ifndef _OFFLINE_H_
#define _OFFLINE_H_

#define EVENTLENGTH 65535

class offline
{
public:
  offline();
  virtual ~offline();

  void SetPulsePolarity(bool pp) {PulsePolarity = pp;}//设置脉冲极性
  
  void SetADCMSPS(int adc);// MHz
  void SetPreampTau(double tau);//us

  void SetFastFilterPar(int fl,int fg,int thre);//ns-建议100  ns-建议100  units
  void SetSlowFilterPar(int sl,int sg);//ns ns

  void SetCalculateBaselinePoint(int n) {CalculateBaselinePoint = n;}

  void SetEventData(int size,short *data);
  void SetEventData(int size,int *data);


  int GetEnergy();// <0 error  >0 energy

private:
  void DataPrimaryProcess();//将数据转为基线为0的正脉冲
  
private:
  bool PulsePolarity;
  int Module_ADCMSPS;
  double PreampTau;

  int Threshold;
  int FastLen, FastGap;
  int SlowLen, SlowGap;

  int CalculateBaselinePoint;
  int baseline;
  double deltaT;
  unsigned int bsum0;
  double b1, c0, c1, c2;
  unsigned int offset, x, y;

  double fastfilter;
  double slowfilter;
  
  unsigned int esum0[32768], esum1[32768], esum2[32768];
  unsigned int fsum0[32768], fsum1[32768];
  
  int Size;
  int Data[EVENTLENGTH];
};

#endif /* _OFFLINE_H_ */
// 
// offline.hh ends here
