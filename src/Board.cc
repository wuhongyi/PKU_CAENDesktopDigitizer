// Board.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 19:45:11 2016 (+0800)
// Last-Updated: 二 12月  6 13:03:01 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 29
// URL: http://wuhongyi.cn 

#include "Board.hh"

#include <cstdlib>
#include <cstdio>
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Board::Board(Digitizer* dig,const char *name)
{
  sprintf(Name,"%s",name);
  printf("-- Board Name is %s \n",Name);
  
  handle = dig->boardHandle;
  
  BoardSupportsCalibration = dig->boardInfo->FamilyCode == CAEN_DGTZ_XX751_FAMILY_CODE || dig->boardInfo->FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE || dig->boardInfo->FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE;
  BoardSupportsTemperatureRead = dig->boardInfo->FamilyCode == CAEN_DGTZ_XX751_FAMILY_CODE || dig->boardInfo->FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE || dig->boardInfo->FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE;
  
  Nch = dig->boardInfo->Channels;
  NBits = dig->boardInfo->ADC_NBits;
  FamilyCode = dig->boardInfo->FamilyCode;
  
  switch(dig->boardInfo->FamilyCode)
    {
    case CAEN_DGTZ_XX724_FAMILY_CODE:
      Ts = 10.0;
      break;

    case CAEN_DGTZ_XX730_FAMILY_CODE:
      Ts = 2.0;
      break;

    default:
      break;
    }


  Event16 = NULL;
  Event8 = NULL;
  Event742 = NULL;
  readoutBuffer = NULL;
  EventPtr = NULL;
  ReloadCfgStatus = 0x7FFFFFFF;// Init to the bigger positive number

  SingleWaveform = NULL;
  MultiWaveform = NULL;
  flagupdatesinglewaveform = false;
  MonitorChannel = -1;
}

Board::~Board()
{
  if(SingleWaveform != NULL)
    {
      delete SingleWaveform;
      SingleWaveform = NULL;
    }
  if(MultiWaveform != NULL)
    {
      delete MultiWaveform;
      MultiWaveform = NULL;
    }
}

void Board::SetStatisticsClear()
{
  Nb = 0;
  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      Ne[i] = 0;
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool Board::ReadLoop()
{
  int ret = 0;
  ret = CAEN_DGTZ_ReadData(handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, readoutBuffer, &bufferSize);
  if(ret) printf("Error: CAEN_DGTZ_ReadData. ");
  // std::cout<<"BufferSize: "<<bufferSize<<std::endl;
  Nb += bufferSize;
  return bufferSize;
}


void Board::InitMonitorGraph()
{
  if(SingleWaveform != NULL)
    {
      delete SingleWaveform;
      SingleWaveform = NULL;
    }
  if(MultiWaveform != NULL)
    {
      delete MultiWaveform;
      MultiWaveform = NULL;
    }

  SingleWaveform = new TGraph();
  MultiWaveform = new TH2I("MultiWaveform","",par_recordlength,0,par_recordlength,4096,0,1<<NBits);
}

void Board::ClearMonitorGraph()
{
  MultiWaveform->Reset("ICES");
  if(SingleWaveform != NULL)
    {
      delete SingleWaveform;
      SingleWaveform = NULL;
    }
  SingleWaveform = new TGraph();
}


// 
// Board.cc ends here










