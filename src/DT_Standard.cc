// DT_Standard.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 11月 26 10:28:50 2016 (+0800)
// Last-Updated: 一 12月  5 12:50:56 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 26
// URL: http://wuhongyi.cn 

#include "DT_Standard.hh"

#include <cstdlib>
#include <cstdio>
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DT_Standard::DT_Standard(Digitizer* dig,const char *name)
  : Board(dig,name)
{

}

DT_Standard::~DT_Standard()
{
  if(Event8 != NULL)
    {
      CAEN_DGTZ_FreeEvent(handle, (void**)&Event8);
      Event8 = NULL;
    }
  
  if(Event16 != NULL)
    {
      CAEN_DGTZ_FreeEvent(handle, (void**)&Event16);
      Event16 = NULL;
    }
  
  if(readoutBuffer != NULL)
    {
      CAEN_DGTZ_FreeReadoutBuffer(&readoutBuffer);
      readoutBuffer = NULL;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int DT_Standard::ProgramDigitizer()
{
  printf("Program Standard\n");
  int ret = 0;

  ret |= CAEN_DGTZ_SetAcquisitionMode(handle, par_acqmode);
  
  ret |= CAEN_DGTZ_SetRecordLength(handle, par_recordlength);
  ret |= CAEN_DGTZ_GetRecordLength(handle, &par_recordlength);


  // if(BoardInfo.FamilyCode != CAEN_DGTZ_XX742_FAMILY_CODE) {
  //   uint32_t pt;
  //   ret |= CAEN_DGTZ_GetPostTriggerSize(handle, &pt);
  //   WDcfg.PostTrigger = pt;
  // }
  

  ret |= CAEN_DGTZ_SetIOLevel(handle, par_iolevel);

  ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle, par_exttriggerinputmode);

  ret |= CAEN_DGTZ_SetChannelEnableMask(handle, par_enablemask);

  ret |= CAEN_DGTZ_SetRunSynchronizationMode(handle, par_runsyncmode);

  uint32_t numAggr = 0;
  CAEN_DGTZ_GetMaxNumAggregatesBLT(handle,&numAggr);
  std::cout<<"Gets the max number of aggregates of each block transfer: "<<numAggr<<std::endl;
  // CAEN_DGTZ_SetMaxNumAggregatesBLT(int handle, uint32_t numAggr);

  
  // // custom setting for X742 boards
  // if (BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) {
  //   ret |= CAEN_DGTZ_SetFastTriggerDigitizing(handle,WDcfg.FastTriggerEnabled);
  //   ret |= CAEN_DGTZ_SetFastTriggerMode(handle,WDcfg.FastTriggerMode);
  // }
  // if ((BoardInfo.FamilyCode == CAEN_DGTZ_XX751_FAMILY_CODE) || (BoardInfo.FamilyCode == CAEN_DGTZ_XX731_FAMILY_CODE)) {
  //   ret |= CAEN_DGTZ_SetDESMode(handle, WDcfg.DesMode);
  // }

  // if (BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) {
  //   ret |= CAEN_DGTZ_SetDecimationFactor(handle, WDcfg.DecimationFactor);
  // }

  ret |= CAEN_DGTZ_SetMaxNumEventsBLT(handle, 1023);
  
  if(ret) printf("Warning: errors found while programming the digitizer. ");
  return ret; 
}

int DT_Standard::AllocateMemory()
{
  int ret = 0;
  
  if(readoutBuffer != NULL)
    {
      ret |= CAEN_DGTZ_FreeReadoutBuffer(&readoutBuffer);
      readoutBuffer = NULL;
      if(NBits == 8)
	ret |= CAEN_DGTZ_FreeEvent(handle, (void**)&Event8);
      else
	{
	  if(FamilyCode != CAEN_DGTZ_XX742_FAMILY_CODE)
	    {
	      ret |= CAEN_DGTZ_FreeEvent(handle, (void**)&Event16);
	    }
	  else
	    {
	      ret |= CAEN_DGTZ_FreeEvent(handle, (void**)&Event742);
	    }
	}
    }
  
  // Allocate memory for the event data and readout buffer
  if(NBits == 8)
    ret |= CAEN_DGTZ_AllocateEvent(handle, (void**)&Event8);
  else
    {
      if (FamilyCode != CAEN_DGTZ_XX742_FAMILY_CODE)
	{
	  ret |= CAEN_DGTZ_AllocateEvent(handle, (void**)&Event16);
	}
      else
	{
	  ret |= CAEN_DGTZ_AllocateEvent(handle, (void**)&Event742);
	}
    }

  ret |= CAEN_DGTZ_MallocReadoutBuffer(handle, &readoutBuffer,&bufferSize); // WARNING: This malloc must be done after the digitizer programming 

  if(ret) printf("Warning: errors malloc space the digitizer. ");
  return ret;
}


int DT_Standard::GetEvent()
{
  int ret = 0;
  ret = CAEN_DGTZ_GetNumEvents(handle,readoutBuffer,bufferSize,&numEvents);
  // printf("NumEvent: %d\n",numEvents);

  return ret;
}


int DT_Standard::GetWaveform()
{
  int ret = 0;
  for (int i = 0; i < (int)numEvents; ++i)
  {
    // Get one event from the readout buffer
    ret = CAEN_DGTZ_GetEventInfo(handle, readoutBuffer, bufferSize, i, &EventInfo, &EventPtr);
    // printf("EventSize: %d   BoardId: %d   Pattern: %d   ChannelMask: %d   EventCounter: %d   TriggerTimeTag: %d\n",EventInfo.EventSize,EventInfo.BoardId,EventInfo.Pattern,EventInfo.ChannelMask,EventInfo.EventCounter,EventInfo.TriggerTimeTag);

    
    // decode the event 
    if (NBits == 8)
      {
	ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event8);
	for (int ch = 0; ch < Nch; ++ch)
	  {
	    if (!(EventInfo.ChannelMask & (1<<ch))) continue;
	    Ne[ch]++;
	    
	  }
      }
    else
      {
	if (FamilyCode != CAEN_DGTZ_XX742_FAMILY_CODE)
	  {
	    ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event16);
	    for (int ch = 0; ch < Nch; ++ch)
	      {
		if (!(EventInfo.ChannelMask & (1<<ch))) continue;
		Ne[ch]++;
	    
	      }
	    
	  }
	else
	  {
	    ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event742);
	    // if (WDcfg.useCorrections != -1)
	    //   { // if manual corrections
	    // 	uint32_t gr;
	    // 	for (gr = 0; gr < MaxGroupNumber; gr++)
	    // 	  {
	    // 	    if ( ((par_enablemask >> gr) & 0x1) == 0)
	    // 	      continue;
	    // 	    ApplyDataCorrection( &(X742Tables[gr]), WDcfg.DRS4Frequency, WDcfg.useCorrections, &(Event742->DataGroup[gr]));
	    // 	  }
	    //   }
	  }
      }

  }

}

// 
// DT_Standard.cc ends here
