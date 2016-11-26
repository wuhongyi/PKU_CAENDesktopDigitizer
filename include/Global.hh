// Global.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 21:05:40 2016 (+0800)
// Last-Updated: 六 11月 26 16:30:59 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef struct {
  CAEN_DGTZ_BoardInfo_t* boardInfo;
  
  CAEN_DGTZ_ConnectionType linkType;
  int linkNum;
  int conetNode;
  int baseAddress;
  int boardHandle;
  
} Digitizer;


enum ButtonIDs
  {
    CONNECTBUTTON,
    DELETEBUTTON,
    COMPLETEBUTTON,
    STARTSTOPBUTTON,
    WRITEDATABUTTON,
    ONLINECHECKBUTTON

  };

enum NumberEntryIDs
  {
    RUNNUMBER
  };


#endif /* _GLOBAL_H_ */

// 
// Global.hh ends here
