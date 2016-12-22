// offline.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 12月  8 19:25:34 2016 (+0800)
// Last-Updated: 四 12月 22 20:31:14 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 31
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

class TF1;
class TGraph;

class offline
{
public:
  offline();
  virtual ~offline();

  // 设置脉冲极性
  void SetPulsePolarity(bool pp) {PulsePolarity = pp;}//设置脉冲极性
  // 设置采样频率
  void SetADCMSPS(int adc);// MHz
  // 设置衰减时间常数
  void SetPreampTau(double tau);//us
  // 设置参与计算最大值的点个数（最大值点之后n个点取平均）
  void SetCalculateVertexPoint(int n) {CalculateVertexPoint = n;} 

  // 设置 fast filter 参数
  void SetFastFilterPar(double fl,double fg,int thre);//us-建议0.1  us-建议0.1  units
  // 设置 energy filter 参数
  void SetSlowFilterPar(double sl,double sg,int slowrange = 2);//us us

  // 输出所有 filter 参数
  void PrintFilterPar();

  // 设置参与计算基线的点个数（从第一个采样点开始）
  void SetCalculateBaselinePoint(int n) {CalculateBaselinePoint = n;}

  // 设置计算上升时间的算法（0-线性插值，1-上升区间拟合，2-降低上升区间采样频率处理）
  void SetCalculateRiseTimeType(int type = 0) {CalculateRiseTimeType = type;}
  
  // 设置该事件的波形
  void SetEventData(unsigned short size,unsigned short *data);
  void SetEventData(int size,short *data);
  void SetEventData(int size,int *data);

  // 获取该事件的波形（处理为基线为0的正脉冲）
  void GetWaveData(int *data);
  void GetFastFilter(int *data);
  void GetSlowFilter(int *data);

  // 获取该事件的上升事件
  double GetRiseTime();
  // 货物该事件的能量
  int GetEnergy();// <0 error  >0 energy

private:
  void DataPrimaryProcess();//将数据转为基线为0的正脉冲

  TF1 *tf1fitpol3;
  TGraph *tgraphfitpol3;
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

  int CalculateRiseTimeType;
  int CalculateVertexPoint;
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
