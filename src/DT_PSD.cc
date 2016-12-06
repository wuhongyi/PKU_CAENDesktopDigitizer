// DT_PSD.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 11月 26 10:24:24 2016 (+0800)
// Last-Updated: 二 12月  6 13:48:30 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 60
// URL: http://wuhongyi.cn 

#include "DT_PSD.hh"

#include <cstdlib>
#include <cstdio>
#include <iostream>
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
      CAEN_DGTZ_FreeDPPEvents(handle, (void **)dpppsdevents);
      CAEN_DGTZ_FreeDPPWaveforms(handle,dpppsdwaveforms);
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int DT_PSD::ProgramDigitizer()
{
  printf("Program DPP-PSD\n");
  int ret = 0;
  
  // ret |= CAEN_DGTZ_WriteRegister(handle, 0x811C, 0xC003C);// serve per mandare SW trg individuali e per abilitare il ts reset
  // ret |= CAEN_DGTZ_WriteRegister(handle, 0x8000, 0x01000010);// Channel Control Reg (indiv trg, seq readout)
  // ret |= CAEN_DGTZ_WriteRegister(handle, 0x8004, 0x00000004);// Enable indiv trgin

  
  ret |= CAEN_DGTZ_SetDPPAcquisitionMode(handle, par_dppacqmode, par_dppsaveparam);
  
  ret |= CAEN_DGTZ_SetAcquisitionMode(handle, par_acqmode);
  
  ret |= CAEN_DGTZ_SetRecordLength(handle, par_recordlength);
  ret |= CAEN_DGTZ_GetRecordLength(handle, &par_recordlength,0);//总是拿ch=0
  
  ret |= CAEN_DGTZ_SetIOLevel(handle, par_iolevel);

  ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle, par_exttriggerinputmode);

  ret |= CAEN_DGTZ_SetChannelEnableMask(handle, par_enablemask);
  
  ret |= CAEN_DGTZ_SetDPPEventAggregation(handle, par_dppeventaggregationthreshold, par_dppeventaggregationmaxsize);

  ret |= CAEN_DGTZ_SetRunSynchronizationMode(handle, par_runsyncmode);

  
  // if (FamilyCode == CAEN_DGTZ_XX720_FAMILY_CODE ||
  //     FamilyCode == CAEN_DGTZ_XX790_FAMILY_CODE ||
  //     FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE ||
  //     FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE)
  //   ret |= CAEN_DGTZ_SetDPPTriggerMode(handle, CAEN_DGTZ_DPP_TriggerMode_Normal);


  
  // if ((FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) || (FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE))
  //   {
  //     // x740 x742

  //   }
  // else
  //   {
  //     if (FamilyCode != CAEN_DGTZ_XX730_FAMILY_CODE && FamilyCode != CAEN_DGTZ_XX725_FAMILY_CODE)
  // 	{
		
  // 	  ret |= CAEN_DGTZ_SetChannelSelfTrigger(handle, par_channelselftrigger, par_enablemask);
  // 	}

  //     if (FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE || FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE)
  // 	{
  // 	  // x725 x730
  // 	  ret |= CAEN_DGTZ_SetChannelSelfTrigger(handle, par_channelselftrigger, par_enablemask);
  // 	}
	      
  //   }

  // if(ret) printf("-- 1 --");

  if(ret) printf("Warning: errors found while programming the digitizer. ");
  return ret;
}

int DT_PSD::AllocateMemory()
{
  int ret = 0;

  if(readoutBuffer != NULL)
    {
      ret = CAEN_DGTZ_FreeReadoutBuffer(&readoutBuffer);
      readoutBuffer = NULL;
      ret = CAEN_DGTZ_FreeDPPEvents(handle, (void **)dpppsdevents);
      ret = CAEN_DGTZ_FreeDPPWaveforms(handle,dpppsdwaveforms);
    }

 
  ret = CAEN_DGTZ_MallocReadoutBuffer(handle, &readoutBuffer, &bufferSize); // WARNING: This malloc must be done after the digitizer programming
  // std::cout<<"BufferSize: "<<bufferSize<<"  "<<sizeof(readoutBuffer)<<std::endl;
  ret |= CAEN_DGTZ_MallocDPPEvents(handle, (void **)dpppsdevents, &size); 
  // std::cout<<"EventSize: "<<size<<"  "<<sizeof(dpppsdevents)<<std::endl;
  ret |= CAEN_DGTZ_MallocDPPWaveforms(handle, (void **)&dpppsdwaveforms, &size);// Allocate memory for the waveforms 
  // std::cout<<"Waveforms: "<<size<<"  "<<sizeof(dpppsdwaveforms)<<std::endl;
  
  if(ret) printf("Warning: errors malloc space the digitizer. ");
  
  return ret;
}


int DT_PSD::GetEvent()
{
  return CAEN_DGTZ_GetDPPEvents(handle, readoutBuffer, bufferSize, (void**)dpppsdevents, NumEvents);
   
}


int DT_PSD::GetWaveform(bool monitor,int type)
{
  for(int ch = 0; ch < Nch; ch++)
    {
      if (!(par_enablemask & (1<<ch)))
	continue;

      for(int ev = 0; ev < NumEvents[ch]; ev++)
	{
	  Ne[ch]++;

	  // Time Tag
	  // dpppsdevents[ch][ev].TimeTag;

	  // Energy
	  // dpppsdevents[ch][ev].ChargeLong;
	  // dpppsdevents[ch][ev].ChargeShort;

	  CAEN_DGTZ_DecodeDPPWaveforms(handle, &dpppsdevents[ch][ev], dpppsdwaveforms);
	  // int size;
	  uint16_t *WaveLine;
	  // uint8_t *DigitalWaveLine;	  
	  // size = (int)(dpppsdwaveforms->Ns); // Number of samples
	  WaveLine = dpppsdwaveforms->Trace1; // First trace (for DPP-PSD it is ALWAYS the Input Signal)
	  // WaveLine = dpppsdwaveforms->Trace2; // Second Trace (if single trace mode, it is a sequence of zeroes)


	  // DigitalWaveLine = dpppsdwaveforms->DTrace1; // First Digital Trace (Gate Short)
	  // DigitalWaveLine = dpppsdwaveforms->DTrace2; // Second Digital Trace (Gate Long)
	  // DigitalWaveLine = dpppsdwaveforms->DTrace3; // Third Digital Trace (DIGITALPROBE1 set with CAEN_DGTZ_SetDPP_PSD_VirtualProbe)
	  // DigitalWaveLine = dpppsdwaveforms->DTrace4; // Fourth Digital Trace (DIGITALPROBE2 set with CAEN_DGTZ_SetDPP_PSD_VirtualProbe)

	  if(monitor)
	    {
	      if(type == 0)
		{
		  // Single
		  if(ch == MonitorChannel)
		    {
		      if(!flagupdatesinglewaveform)
			{
			  for (int point = 0; point < (int)(dpppsdwaveforms->Ns); ++point)
			    {
			      SingleWaveform->SetPoint(point,point,int(WaveLine[point]));
			    }
			  
			  flagupdatesinglewaveform = true;
			}
		    }
		}
	      else
		{
		  if(type == 1)
		    {
		      // Mutli
		      if(ch == MonitorChannel)
			{
			  for (int point = 0; point < (int)(dpppsdwaveforms->Ns); ++point)
			    {
			      MultiWaveform->Fill(point,int(WaveLine[point]));
			    }
			}
		      

		    }// type =1
		  else
		    {


		    } // type >1
		}// type > 0
	    }// monitor


	  
	  
	}// loop on events
    }// loop on channels
}




// 
// DT_PSD.cc ends here











