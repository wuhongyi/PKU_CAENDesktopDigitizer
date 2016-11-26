// Board.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 19:44:44 2016 (+0800)
// Last-Updated: 六 11月 26 15:14:36 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 17
// URL: http://wuhongyi.cn 

#ifndef _BOARD_H_
#define _BOARD_H_

#include "Global.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Board
{
public:
  Board(Digitizer* dig,const char *name);
  virtual ~Board();

  char *GetName() {return Name;}
  
  // return  0 = Success; negative numbers are error codes
  virtual int ProgramDigitizer() = 0;

  
private:

  
private:
  CAEN_DGTZ_UINT16_EVENT_t    *Event16=NULL; /* generic event struct with 16 bit data (10, 12, 14 and 16 bit digitizers */

  CAEN_DGTZ_UINT8_EVENT_t     *Event8=NULL; /* generic event struct with 8 bit data (only for 8 bit digitizers) */ 
  CAEN_DGTZ_X742_EVENT_t       *Event742=NULL;  /* custom event struct with 8 bit data (only for 8 bit digitizers) */
  CAEN_DGTZ_DRS4Correction_t X742Tables[MAX_X742_GROUP_SIZE];


private:
  char Name[128];
  int handle;
  int NBits;
  int Nch;
  double Ts;


  bool BoardSupportsCalibration;
  bool BoardSupportsTemperatureRead;
  
};

#endif /* _BOARD_H_ */
// 
// Board.hh ends here










