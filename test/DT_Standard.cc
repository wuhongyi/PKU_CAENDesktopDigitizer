// DT_Standard.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 11月 26 10:28:50 2016 (+0800)
// Last-Updated: 一 4月 17 15:33:01 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 58
// URL: http://wuhongyi.cn 

#include "DT_Standard.hh"
#include "Analysis.hh"

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


void DT_Standard::GetWaveform(bool monitor,int type)
{
  for (int i = 0; i < (int)numEvents; ++i)
    {
      // Get one event from the readout buffer
      CAEN_DGTZ_GetEventInfo(handle, readoutBuffer, bufferSize, i, &EventInfo, &EventPtr);
      // printf("EventSize: %d   BoardId: %d   Pattern: %d   ChannelMask: %d   EventCounter: %d   TriggerTimeTag: %d\n",EventInfo.EventSize,EventInfo.BoardId,EventInfo.Pattern,EventInfo.ChannelMask,EventInfo.EventCounter,EventInfo.TriggerTimeTag);

      // decode the event 
      if (NBits == 8)
	{
	  CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event8);
	  for (int ch = 0; ch < Nch; ++ch)
	    {
	      if (!(EventInfo.ChannelMask & (1<<ch))) continue;
	      Ne[ch]++;


	      if(monitor)
		{
		  if(type == 0)
		    {
		      // Single
		      if(ch == MonitorChannel)
			{
			  if(!flagupdatesinglewaveform)
			    {
			      for (int point = 0; point < (int)(Event8->ChSize[ch]); ++point)
				{
				  SingleWaveform->SetPoint(point,point,int(Event8->DataChannel[ch][point]));
				}
			      flagupdatesinglewaveform = true;
			    }
			}
		    }//type=0
		  else
		    {
		      if(type == 1)
			{
			  // Mutli
			  if(ch == MonitorChannel)
			    {
			      for (int point = 0; point < (int)(Event8->ChSize[ch]); ++point)
				{
				  MultiWaveform->SetPoint(CountPointMultiWaveform++,point,int(Event8->DataChannel[ch][point]));
				}
			    }
		      

			}// type=1
		      else
			{


			}
		    }// type>0
		}//monitor

	    }// channel
	}//NBits == 8
      else
	{
	  if (FamilyCode != CAEN_DGTZ_XX742_FAMILY_CODE)
	    {
	      CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event16);
	      for (int ch = 0; ch < Nch; ++ch)
		{
		  if (!(EventInfo.ChannelMask & (1<<ch))) continue;
		  Ne[ch]++;

		  HeaderSTD[0] = ch;
		  HeaderSTD[1] = EventInfo.TriggerTimeTag;
		  HeaderSTD[2] = EventInfo.EventCounter;
		  HeaderSTD[3] = EventInfo.Pattern;
		  HeaderSTD[4] = Event16->ChSize[ch];
		
		  if(writedata)
		    {
		      if((buffid+HEADERSTD*4+HeaderSTD[4]*2) > BUFFLENGTH) SaveToFile();
		      memcpy(&buff[buffid],HeaderSTD,HEADERSTD*4);
		      memcpy(&buff[buffid+HEADERSTD*4],Event16->DataChannel[ch],HeaderSTD[4]*2);
		      buffid = buffid+HEADERSTD*4+HeaderSTD[4]*2;
		    }


		  if(monitor)
		    {
		      switch(type)
			{
			case 0:
			  // Single
			  if(ch == MonitorChannel)
			    {
			      if(!flagupdatesinglewaveform)
				{
				  for (int point = 0; point < (int)(Event16->ChSize[ch]); ++point)
				    {
				      SingleWaveform->SetPoint(point,point,int(Event16->DataChannel[ch][point]));
				    }
				  flagupdatesinglewaveform = true;
				}
			    }
			  break;

			case 1:
			  // Mutli
			  if(ch == MonitorChannel)
			    {
			      for (int point = 0; point < (int)(Event16->ChSize[ch]); ++point)
				{
				  MultiWaveform->SetPoint(CountPointMultiWaveform++,point,int(Event16->DataChannel[ch][point]));
				}
			    }
			  break;

			case 2:
			  // Energy
			
			  break;

			case 3:
			  // SingleFFT(CAEN)
			  if(ch == MonitorChannel)
			    {
			      if(!flagupdatesinglefft)
				{
				  int SizeFFT = FFT(Event16->DataChannel[ch], BufferFFT, (int)(Event16->ChSize[ch]), 0);
			  
				  for (int point = 0; point < SizeFFT; ++point)
				    {
				      SingleFFTCAEN->SetPoint(point,point,BufferFFT[point]);
				    }
			  
				  flagupdatesinglefft = true;
				}
			    }
			  break;

			case 4:
			  // SingleFFT(XIA)
			  if(ch == MonitorChannel)
			    {
			      if(!flagupdatesinglefft)
				{
				  for (int point = 0; point < (int)(Event16->ChSize[ch]); ++point)
				    {
				      BufferFFT[2*point+1] = 0;
				      BufferFFT[2*point] = double(Event16->DataChannel[ch][point]);
				    }

				  unsigned int sizecomplexfft;
				  int power2 = 1;
				  while((1<<power2) < (int)(Event16->ChSize[ch])) power2++;
				  if((1<<power2) == (int)(Event16->ChSize[ch])) sizecomplexfft = (unsigned int)(1<<power2);
				  else sizecomplexfft = (unsigned int)(1<<(power2-1));
				  Pixie16complexFFT(BufferFFT, sizecomplexfft);
				  
				  for (int point = 0; point < (int)(sizecomplexfft)/2; ++point)
				    {
				      SingleFFTXIA->SetPoint(point,double(point),std::sqrt(BufferFFT[2*point]*BufferFFT[2*point]+BufferFFT[2*point+1]*BufferFFT[2*point+1]));
				    }

				  flagupdatesinglefft = true;
				}
			    }
			  break;

			default:
			  break;
			}


		    }//monitor
		
		
		
		}
	    
	    }
	  else
	    {
	      CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event742);
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
	    } // x742
	}// NBits > 8

    }// numEvents

}




// 
// DT_Standard.cc ends here
