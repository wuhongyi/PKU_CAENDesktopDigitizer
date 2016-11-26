// Board.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 19:44:44 2016 (+0800)
// Last-Updated: 六 11月 26 21:10:01 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 24
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
  virtual int AllocateMemory() = 0;
  
private:

  
private:

protected:
  CAEN_DGTZ_DPP_PHA_Params_t dppphaParams;
  CAEN_DGTZ_DPP_PSD_Params_t dpppsdParams;

  CAEN_DGTZ_EventInfo_t       EventInfo;
  
  CAEN_DGTZ_UINT16_EVENT_t    *Event16; // generic event struct with 16 bit data (10, 12, 14 and 16 bit digitizers 
  CAEN_DGTZ_UINT8_EVENT_t     *Event8; // generic event struct with 8 bit data (only for 8 bit digitizers) 
  CAEN_DGTZ_X742_EVENT_t       *Event742;  // custom event struct with 8 bit data (only for 8 bit digitizers) 
  CAEN_DGTZ_DRS4Correction_t X742Tables[MAX_X742_GROUP_SIZE];
  int ReloadCfgStatus;
  CAEN_DGTZ_DPP_PHA_Event_t *dppphaevents[MAX_DPP_PHA_CHANNEL_SIZE];
  CAEN_DGTZ_DPP_PSD_Event_t *dpppsdevents[MAX_DPP_PSD_CHANNEL_SIZE];

  CAEN_DGTZ_DPP_PHA_Waveforms_t *dppphawaveforms;
  CAEN_DGTZ_DPP_PSD_Waveforms_t *dpppsdwaveforms;

  char* readoutBuffer;
  uint32_t bufferSize;
  uint32_t size;  
  
protected:
  char Name[128];
  int handle;
  int NBits;
  int Nch;
  double Ts;
  int FamilyCode;

  bool BoardSupportsCalibration;
  bool BoardSupportsTemperatureRead;

};

#endif /* _BOARD_H_ */
// 
// Board.hh ends here










