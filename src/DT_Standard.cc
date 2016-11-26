// DT_Standard.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 11月 26 10:28:50 2016 (+0800)
// Last-Updated: 六 11月 26 19:44:46 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 9
// URL: http://wuhongyi.cn 

#include "DT_Standard.hh"

#include <cstdlib>
#include <cstdio>
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

  uint32_t RecordLength;
  RecordLength = 2000;

  uint32_t PostTrigger;
  PostTrigger = 200;
  

  ret |= CAEN_DGTZ_SetRecordLength(handle, RecordLength);
  ret |= CAEN_DGTZ_GetRecordLength(handle, &RecordLength);

  ret |= CAEN_DGTZ_SetPostTriggerSize(handle, PostTrigger);
  ret |= CAEN_DGTZ_GetPostTriggerSize(handle, &PostTrigger);

  // CAEN_DGTZ_IOLevel_NIM / CAEN_DGTZ_IOLevel_TTL
  ret |= CAEN_DGTZ_SetIOLevel(handle, CAEN_DGTZ_IOLevel_TTL);




  return 0;
}

int DT_Standard::AllocateMemory()
{
  int ret = 0;

  // Allocate memory for the event data and readout buffer
  if(NBits == 8)
    ret = CAEN_DGTZ_AllocateEvent(handle, (void**)&Event8);
  else
    {
      if (FamilyCode != CAEN_DGTZ_XX742_FAMILY_CODE)
	{
	  ret = CAEN_DGTZ_AllocateEvent(handle, (void**)&Event16);
	}
      else
	{
	  ret = CAEN_DGTZ_AllocateEvent(handle, (void**)&Event742);
	}
    }

  ret |= CAEN_DGTZ_MallocReadoutBuffer(handle, &readoutBuffer,&bufferSize); // WARNING: This malloc must be done after the digitizer programming 


  
  return ret;
}


// 
// DT_Standard.cc ends here
