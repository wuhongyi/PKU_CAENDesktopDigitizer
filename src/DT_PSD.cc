// DT_PSD.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 11月 26 10:24:24 2016 (+0800)
// Last-Updated: 二 11月 29 21:12:27 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 21
// URL: http://wuhongyi.cn 

#include "DT_PSD.hh"

#include <cstdlib>
#include <cstdio>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DT_PSD::DT_PSD(Digitizer* dig,const char *name)
  : Board(dig,name)
{

}

DT_PSD::~DT_PSD()
{
  if(readoutBuffer != NULL)
    {
      CAEN_DGTZ_FreeReadoutBuffer(&readoutBuffer);
      readoutBuffer = NULL;
    }

  CAEN_DGTZ_FreeDPPEvents(handle, (void **)dpppsdevents);
  CAEN_DGTZ_FreeDPPWaveforms(handle,dpppsdwaveforms);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int DT_PSD::ProgramDigitizer()
{
  printf("Program DPP-PSD\n");
  int ret = 0;
  
  ret |= CAEN_DGTZ_WriteRegister(handle, 0x811C, 0xC003C);// serve per mandare SW trg individuali e per abilitare il ts reset
  ret |= CAEN_DGTZ_WriteRegister(handle, 0x8000, 0x01000110);// Channel Control Reg (indiv trg, seq readout)
  ret |= CAEN_DGTZ_WriteRegister(handle, 0x8004, 0x00000004);// Enable indiv trgin

  
  // CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope CAEN_DGTZ_DPP_ACQ_MODE_List CAEN_DGTZ_DPP_ACQ_MODE_Mixed
  // CAEN_DGTZ_DPP_SAVE_PARAM_EnergyOnly Only energy (DPP-PHA) or charge (DPP-PSD/DPP-CI v2) is returned 
  // CAEN_DGTZ_DPP_SAVE_PARAM_TimeOnly   Only time is returned 
  // CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime  Both energy/charge and time are returned
  // CAEN_DGTZ_DPP_SAVE_PARAM_ChargeAndTime   eprecated On DPP-PSD and DPP-CI use CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime
  // CAEN_DGTZ_DPP_SAVE_PARAM_None   No histogram data is returned
  ret |= CAEN_DGTZ_SetDPPAcquisitionMode(handle, CAEN_DGTZ_DPP_ACQ_MODE_Mixed, CAEN_DGTZ_DPP_SAVE_PARAM_ChargeAndTime);
  
  // CAEN_DGTZ_SW_CONTROLLED  CAEN_DGTZ_S_IN_CONTROLLED  CAEN_DGTZ_FIRST_TRG_CONTROLLED
  ret |= CAEN_DGTZ_SetAcquisitionMode(handle, CAEN_DGTZ_SW_CONTROLLED);
  
  uint32_t RecordLength;
  RecordLength = 5000;
  ret |= CAEN_DGTZ_SetRecordLength(handle, RecordLength);


  // CAEN_DGTZ_IOLevel_NIM / CAEN_DGTZ_IOLevel_TTL
  ret |= CAEN_DGTZ_SetIOLevel(handle, CAEN_DGTZ_IOLevel_TTL);


  // CAEN_DGTZ_TRGMODE_DISABLED  CAEN_DGTZ_TRGMODE_EXTOUT_ONLY  CAEN_DGTZ_TRGMODE_ACQ_ONLY  CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT
  ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle, CAEN_DGTZ_TRGMODE_ACQ_ONLY);

  ret |= CAEN_DGTZ_SetChannelEnableMask(handle, 0xff);
  
  ret |= CAEN_DGTZ_SetDPPEventAggregation(handle, 0, 0);


  // CAEN_DGTZ_RUN_SYNC_Disabled  CAEN_DGTZ_RUN_SYNC_TrgOutTrgInDaisyChain  CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain  CAEN_DGTZ_RUN_SYNC_SinFanout  CAEN_DGTZ_RUN_SYNC_GpioGpioDaisyChain
  ret |= CAEN_DGTZ_SetRunSynchronizationMode(handle, CAEN_DGTZ_RUN_SYNC_Disabled);

  
  ret |= CAEN_DGTZ_SetDPPParameters(handle, 0/*channel*/, &dpppsdParams);



  // if(ret) printf("-- 1 --");

  if(ret) printf("Warning: errors found while programming the digitizer. ");
  return ret;
}

int DT_PSD::AllocateMemory()
{
  int ret = 0;

  if(readoutBuffer != NULL)
    {
      CAEN_DGTZ_FreeReadoutBuffer(&readoutBuffer);
      readoutBuffer = NULL;
      CAEN_DGTZ_FreeDPPEvents(handle, (void **)dpppsdevents);
      CAEN_DGTZ_FreeDPPWaveforms(handle,dpppsdwaveforms);
    }

 
  ret = CAEN_DGTZ_MallocReadoutBuffer(handle, &readoutBuffer, &bufferSize); // WARNING: This malloc must be done after the digitizer programming 
  ret |= CAEN_DGTZ_MallocDPPEvents(handle, (void **)dpppsdevents, &size); 
  // Allocate memory for the waveforms 
  ret |= CAEN_DGTZ_MallocDPPWaveforms(handle, (void **)&dpppsdwaveforms, &size); 



  
  return ret;
}


// 
// DT_PSD.cc ends here
