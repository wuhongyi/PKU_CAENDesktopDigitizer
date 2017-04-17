// DT_PSD.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 11月 26 10:24:24 2016 (+0800)
// Last-Updated: 一 4月 17 15:33:01 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 82
// URL: http://wuhongyi.cn 

#include "DT_PSD.hh"
#include "Analysis.hh"

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


  for (int ch = 0; ch < Nch; ++ch)
    {
      dpppsdParams.thr[ch] = 0;// Trigger Threshold
      dpppsdParams.nsbl[ch] = 2;    // used to specifiy the number of samples for the baseline averaging
      dpppsdParams.lgate[ch] = 32;    // Long Gate Width (N*4ns)
      dpppsdParams.sgate[ch] = 24;    // Short Gate Width (N*4ns)
      dpppsdParams.pgate[ch] = 8;     // Pre Gate Width (N*4ns)
      dpppsdParams.selft[ch] = 0;  // Self Trigger Mode: 0 -> Disabled 1 -> Enabled
      // Trigger configuration:
      // CAEN_DGTZ_DPP_TriggerConfig_Peak       -> trigger on peak. NOTE: Only for FW <= 13X.5
      // CAEN_DGTZ_DPP_TriggerConfig_Threshold  -> trigger on threshold 
      dpppsdParams.trgc[ch] = CAEN_DGTZ_DPP_TriggerConfig_Threshold;
      dpppsdParams.tvaw[ch] = 50;// Trigger Validation Acquisition Window
      dpppsdParams.csens[ch] = 0;// Charge sensibility: 0->40fc/LSB; 1->160fc/LSB; 2->640fc/LSB; 3->2,5pc/LSB
    }
  // Pile-Up rejection Mode
  //    CAEN_DGTZ_DPP_PSD_PUR_DetectOnly -> Only Detect Pile-Up
  //    CAEN_DGTZ_DPP_PSD_PUR_Enabled -> Reject Pile-Up
  // DPPParams[b].purh = CAEN_DGTZ_DPP_PSD_PUR_DetectOnly;
  dpppsdParams.purh = CAEN_DGTZ_DPP_PSD_PUR_DetectOnly; //NOTE: Ignored on 5730
  dpppsdParams.purgap = 100;  // Purity Gap   // Pile-up Rejection Gap  //NOTE: Ignored on 5730
  dpppsdParams.blthr = 3;     // Baseline Threshold  //This parameter is deprecated
  dpppsdParams.bltmo = 100;   // Baseline Timeout    //This parameter is deprecated
  dpppsdParams.trgho = 0;     // Trigger HoldOff



  
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


void DT_PSD::GetWaveform(bool monitor,int type)
{
  for(int ch = 0; ch < Nch; ch++)
    {
      if (!(par_enablemask & (1<<ch)))
	continue;

      for(unsigned int ev = 0; ev < NumEvents[ch]; ev++)
	{
	  Ne[ch]++;

	  HeaderPSD[0] = ch;
	  // Time Tag
	  HeaderPSD[1] = dpppsdevents[ch][ev].TimeTag;
	  // Energy
	  HeaderPSD[3] = dpppsdevents[ch][ev].ChargeLong;
	  HeaderPSD[2] = dpppsdevents[ch][ev].ChargeShort;
	  HeaderPSD[4] = dpppsdevents[ch][ev].Baseline;

	  HeaderPSD[6] = dpppsdevents[ch][ev].Format;
	  HeaderPSD[7] = dpppsdevents[ch][ev].Format2;
	  HeaderPSD[8] = dpppsdevents[ch][ev].Extras;
	  HeaderPSD[9] = dpppsdevents[ch][ev].Pur;
	  
	  CAEN_DGTZ_DecodeDPPWaveforms(handle, &dpppsdevents[ch][ev], dpppsdwaveforms);
	  // int size;
	  uint16_t *WaveLine;
	  // uint8_t *DigitalWaveLine;	  
	  HeaderPSD[5] = dpppsdwaveforms->Ns; // Number of samples
	  WaveLine = dpppsdwaveforms->Trace1; // First trace (for DPP-PSD it is ALWAYS the Input Signal)
	  // WaveLine = dpppsdwaveforms->Trace2; // Second Trace (if single trace mode, it is a sequence of zeroes)


	  // DigitalWaveLine = dpppsdwaveforms->DTrace1; // First Digital Trace (Gate Short)
	  // DigitalWaveLine = dpppsdwaveforms->DTrace2; // Second Digital Trace (Gate Long)
	  // DigitalWaveLine = dpppsdwaveforms->DTrace3; // Third Digital Trace (DIGITALPROBE1 set with CAEN_DGTZ_SetDPP_PSD_VirtualProbe)
	  // DigitalWaveLine = dpppsdwaveforms->DTrace4; // Fourth Digital Trace (DIGITALPROBE2 set with CAEN_DGTZ_SetDPP_PSD_VirtualProbe)

	  if(writedata)
	    {
	      if((buffid+HEADERPSD*4+HeaderPSD[5]*2) > BUFFLENGTH) SaveToFile();
	      memcpy(&buff[buffid],HeaderPSD,HEADERPSD*4);
	      memcpy(&buff[buffid+HEADERPSD*4],WaveLine,HeaderPSD[5]*2);
	      buffid = buffid+HEADERPSD*4+HeaderPSD[5]*2;
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
			  for (int point = 0; point < (int)(dpppsdwaveforms->Ns); ++point)
			    {
			      SingleWaveform->SetPoint(point,point,int(WaveLine[point]));
			    }
			  
			  flagupdatesinglewaveform = true;
			}
		    }
		  break;

		  
		case 1:
		  // Mutli
		  if(ch == MonitorChannel)
		    {
		      for (int point = 0; point < (int)(dpppsdwaveforms->Ns); ++point)
			{
			  MultiWaveform->SetPoint(CountPointMultiWaveform++,point,int(WaveLine[point]));
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
			  int SizeFFT = FFT(WaveLine, BufferFFT, (int)(dpppsdwaveforms->Ns), 0);
			  
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
			  for (int point = 0; point < (int)(dpppsdwaveforms->Ns); ++point)
			    {
			      BufferFFT[2*point+1] = 0;
			      BufferFFT[2*point] = double(WaveLine[point]);
			    }


			  unsigned int sizecomplexfft;
			  int power2 = 1;
			  while((1<<power2) < (int)(dpppsdwaveforms->Ns)) power2++;
			  if((1<<power2) == (int)(dpppsdwaveforms->Ns)) sizecomplexfft = (unsigned int)(1<<power2);
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
	    }// monitor


	  
	  
	}// loop on events
    }// loop on channels
}




// 
// DT_PSD.cc ends here
