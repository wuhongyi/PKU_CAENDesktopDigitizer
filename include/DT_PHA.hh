// DT_PHA.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 12月  3 09:57:02 2016 (+0800)
// Last-Updated: 六 12月  3 17:45:30 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 2
// URL: http://wuhongyi.cn 

#ifndef _DT_PHA_H_
#define _DT_PHA_H_

#include "Global.hh"

#include "Board.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DT_PHA : public Board
{
public:
  DT_PHA(Digitizer* dig,const char *name);
  virtual ~DT_PHA();

  virtual int ProgramDigitizer();
  virtual int AllocateMemory();
  virtual int GetEvent();
  virtual int GetWaveform();
  
};

#endif /* _DT_PHA_H_ */
// 
// DT_PHA.hh ends here
