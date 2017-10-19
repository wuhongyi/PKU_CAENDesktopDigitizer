// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 7月 29 18:50:09 2017 (+0800)
// Last-Updated: 六 7月 29 19:31:38 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1
// URL: http://wuhongyi.cn 

#include "ReadBinary.hh"

#include "TString.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  if((argc != 4)
    {
      std::cout<<argv[0]<<" [FileName]  [RuNnumber]  [par]"<<std::endl;
      std::cout<<"par:  E  e  "<<std::endl;
      std::cout<<"example:  ./read  data  11  e"<<std::endl;
      return 0;
    }

  TString rawfilepath = "/home/wuhongyi/PKU_CAENDesktopDigitizer/data/";// 必须以 '/' 结尾
  TString rootfilepath = "/home/wuhongyi/PKU_CAENDesktopDigitizer/data/";// 必须以 '/' 结尾

  TString filename(argv[1]);
  TString RunNumber(argv[2]);
  int  runnumber = RunNumber.Atoi();



  
  return 0;
}

// 
// main.cc ends here
