// Board.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 19:44:44 2016 (+0800)
// Last-Updated: 二 9月  7 21:25:31 2021 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 81
// URL: http://wuhongyi.cn 

#ifndef _BOARD_H_
#define _BOARD_H_

#include "Global.hh"

#include "TGraph.h"
#include "TH1.h"
// #include "TH2.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define BUFFLENGTH  5400000
#define HEADERSTD 5
#define HEADERPSD 10
#define HEADERPHA 8

class Board
{
public:
  Board(Digitizer* dig,const char *name);
  virtual ~Board();

  char *GetName() {return Name;}
  int GetHandle() {return handle;}
  int GetChannels() {return Nch;}
  int GetFamilyCode() {return FamilyCode;}
  int GetNBits() {return NBits;}

  bool ReadLoop();

  void SetStatisticsClear();
  int GetStatisticsNb() {return Nb;}
  int *GetStatisticsNe() {return Ne;}


  CAEN_DGTZ_DPP_AcqMode_t GetDPPAcqMode() {return par_dppacqmode;}
  CAEN_DGTZ_DPP_SaveParam_t GetDPPSaveParam() {return par_dppsaveparam;}
  
  void SetDPPAcquisitionMode(CAEN_DGTZ_DPP_AcqMode_t mode,CAEN_DGTZ_DPP_SaveParam_t par) {par_dppacqmode = mode;par_dppsaveparam = par;}
  void SetDPPEventAggregation(int threshold, int maxsize) {par_dppeventaggregationthreshold = threshold;par_dppeventaggregationmaxsize = maxsize;}

  
  void SetAcquisitionMode(CAEN_DGTZ_AcqMode_t mode) {par_acqmode = mode;}
  void SetRecordLength(int l) {par_recordlength = l;}
  void GetRecordLength(int *l) {*l = par_recordlength;}
  void SetIOLevel(CAEN_DGTZ_IOLevel_t io) {par_iolevel = io;}
  void SetExtTriggerInputMode(CAEN_DGTZ_TriggerMode_t mode) {par_exttriggerinputmode = mode;}
  void SetChannelEnableMask(uint32_t mask) {par_enablemask = mask;}
  void SetRunSynchronizationMode(CAEN_DGTZ_RunSyncMode_t mode) {par_runsyncmode = mode;}
  void SetChannelSelfTrigger(CAEN_DGTZ_TriggerMode_t mode) {par_channelselftrigger = mode;}
  
public:  
  // return  0 = Success; negative numbers are error codes
  virtual int ProgramDigitizer() = 0;
  virtual int AllocateMemory() = 0;
  virtual int GetEvent() = 0;
  virtual void GetWaveform(bool monitor = false,int type = 0) = 0;
  
public:

  void InitMonitorGraph();
  void ClearMonitorGraph();
  TGraph *GetSingleWaveform() {return SingleWaveform;}
  TGraph *GetMultiWaveform() {return MultiWaveform;}
  TH1I *GetEnergySpectrum() {return EnergySpectrum;}
  TGraph *GetSingleFFTCAEN() {return SingleFFTCAEN;}
  TGraph *GetSingleFFTXIA() {return SingleFFTXIA;}
  
  int GetMonitorChannel() {return MonitorChannel;}
  void SetMonitorChannel(int ch) {MonitorChannel = ch;}
  void SetUpdateSingleWaveform() {flagupdatesinglewaveform = false;}
  void SetUpdateSingleFFT() {flagupdatesinglefft = false;}
  
  void SetWriteData(bool flag) {writedata = flag;} 
  bool OpenFile(const char *filename);
  bool CloseFile();
  void SaveToFile();

  CAEN_DGTZ_DPP_PHA_Params_t dppphaParams;
  CAEN_DGTZ_DPP_PSD_Params_t dpppsdParams;
  
protected:
  TGraph *SingleWaveform;
  TGraph *MultiWaveform;
  TH1I *EnergySpectrum;
  TGraph *SingleFFTCAEN;
  TGraph *SingleFFTXIA;
  
  int CountPointMultiWaveform;
  bool flagupdatesinglewaveform;// 0-需要刷新  1-已刷新
  bool flagupdatesinglefft;// 0-需要刷新  1-已刷新
  int MonitorChannel;
  bool writedata;

  FILE *FileSave;
  unsigned char buff[BUFFLENGTH];
  int buffid;

  unsigned int HeaderSTD[HEADERSTD];// 0-ch 1-TimeTag 2-EventCounter 3-Pattern 4-size
  unsigned int HeaderPSD[HEADERPSD];// 0-ch 1-TimeTag 2-chargeshort 3-chargelong 4-baseline 5-size 6-Format 7-Format2 8-Extras 9-Pur
  unsigned int HeaderPHA[HEADERPHA];// 0-ch 1-TimeTagHigh 2-TimeTagLow 3-Energy 4-size 5-Format 6-Extras 7-Extras2
  
protected:
  CAEN_DGTZ_DPP_AcqMode_t par_dppacqmode;// CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope CAEN_DGTZ_DPP_ACQ_MODE_List CAEN_DGTZ_DPP_ACQ_MODE_Mixed
  
  CAEN_DGTZ_DPP_SaveParam_t par_dppsaveparam;
  // CAEN_DGTZ_DPP_SAVE_PARAM_EnergyOnly Only energy (DPP-PHA) or charge (DPP-PSD/DPP-CI v2) is returned 
  // CAEN_DGTZ_DPP_SAVE_PARAM_TimeOnly   Only time is returned 
  // CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime  Both energy/charge and time are returned
  // CAEN_DGTZ_DPP_SAVE_PARAM_ChargeAndTime   eprecated On DPP-PSD and DPP-CI use CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime
  // CAEN_DGTZ_DPP_SAVE_PARAM_None   No histogram data is returned

  int par_dppeventaggregationthreshold;
  int par_dppeventaggregationmaxsize;
  

  CAEN_DGTZ_RunSyncMode_t par_runsyncmode;// CAEN_DGTZ_RUN_SYNC_Disabled  CAEN_DGTZ_RUN_SYNC_TrgOutTrgInDaisyChain  CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain  CAEN_DGTZ_RUN_SYNC_SinFanout  CAEN_DGTZ_RUN_SYNC_GpioGpioDaisyChain
  CAEN_DGTZ_TriggerMode_t par_exttriggerinputmode;// CAEN_DGTZ_TRGMODE_DISABLED  CAEN_DGTZ_TRGMODE_EXTOUT_ONLY  CAEN_DGTZ_TRGMODE_ACQ_ONLY  CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT
  CAEN_DGTZ_IOLevel_t par_iolevel;// CAEN_DGTZ_IOLevel_NIM  CAEN_DGTZ_IOLevel_TTL
  CAEN_DGTZ_AcqMode_t par_acqmode;// CAEN_DGTZ_SW_CONTROLLED  CAEN_DGTZ_S_IN_CONTROLLED  CAEN_DGTZ_FIRST_TRG_CONTROLLED
  CAEN_DGTZ_TriggerMode_t par_channelselftrigger;// CAEN_DGTZ_TRGMODE_DISABLED  CAEN_DGTZ_TRGMODE_EXTOUT_ONLY  CAEN_DGTZ_TRGMODE_ACQ_ONLY  CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT

  uint32_t par_enablemask;
  uint32_t par_recordlength;
  
protected:
  CAEN_DGTZ_EventInfo_t       EventInfo;
  CAEN_DGTZ_UINT16_EVENT_t    *Event16; // generic event struct with 16 bit data (10, 12, 14 and 16 bit digitizers 
  CAEN_DGTZ_UINT8_EVENT_t     *Event8; // generic event struct with 8 bit data (only for 8 bit digitizers) 
  CAEN_DGTZ_X742_EVENT_t       *Event742;  // custom event struct with 8 bit data (only for 8 bit digitizers) 
  CAEN_DGTZ_DRS4Correction_t X742Tables[MAX_X742_GROUP_SIZE];
  uint32_t MaxGroupNumber;
  int ReloadCfgStatus;
  CAEN_DGTZ_DPP_PHA_Event_t *dppphaevents[MAX_DPP_PHA_CHANNEL_SIZE];
  CAEN_DGTZ_DPP_PSD_Event_t *dpppsdevents[MAX_DPP_PSD_CHANNEL_SIZE];
  uint32_t NumEvents[MAX_CHANNEL];// PHA PSD
  uint32_t numEvents;// STD
  // MAX_DPP_CHANNEL_SIZE
  
  CAEN_DGTZ_DPP_PHA_Waveforms_t *dppphawaveforms;
  CAEN_DGTZ_DPP_PSD_Waveforms_t *dpppsdwaveforms;

  char* readoutBuffer;
  uint32_t bufferSize;
  char *EventPtr;
  uint32_t size;  

protected:
  // online
  int Nb;
  int Ne[MAX_CHANNEL];
  double BufferFFT[65536];

  
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
