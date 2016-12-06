// DT_PSD.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 11月 26 10:23:23 2016 (+0800)
// Last-Updated: 二 12月  6 10:58:44 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

#ifndef _DT_PSD_H_
#define _DT_PSD_H_

#include "Global.hh"

#include "Board.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DT_PSD : public Board
{
public:
  DT_PSD(Digitizer* dig,const char *name);
  virtual ~DT_PSD();


  virtual int ProgramDigitizer();
  virtual int AllocateMemory();
  virtual int GetEvent();
  virtual int GetWaveform(bool monitor = false,int type = 0);
  
private:
  




};

#endif /* _DT_PSD_H_ */

// 
// DT_PSD.hh ends here
