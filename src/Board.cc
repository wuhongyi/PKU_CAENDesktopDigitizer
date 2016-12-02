// Board.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 19:45:11 2016 (+0800)
// Last-Updated: 四 12月  1 13:36:18 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 15
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
  ReloadCfgStatus = 0x7FFFFFFF;// Init to the bigger positive number
  
}

Board::~Board()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool Board::ReadLoop()
{
  int ret = 0;
  ret = CAEN_DGTZ_ReadData(handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, readoutBuffer, &bufferSize);
  if(ret) printf("Error: CAEN_DGTZ_ReadData. ");
  std::cout<<"BufferSize: "<<bufferSize<<std::endl;
  return bufferSize;
}



// 
// Board.cc ends here
