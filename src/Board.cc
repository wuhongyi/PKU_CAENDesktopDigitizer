// Board.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 19:45:11 2016 (+0800)
// Last-Updated: 六 11月 26 13:17:27 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 8
// URL: http://wuhongyi.cn 

#include "Board.hh"

#include <cstdlib>
#include <cstdio>
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







  


  
}

Board::~Board()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......





// 
// Board.cc ends here
