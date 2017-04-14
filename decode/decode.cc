// decode.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 12月  6 19:25:21 2016 (+0800)
// Last-Updated: 五 4月 14 22:39:32 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 12
// URL: http://wuhongyi.cn 

#include "r2root.hh"

#include "TString.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  // std::cout<<"argc: "<<argc<<std::endl;
  if((argc-3)*(argc-5) != 0)
    {
      std::cout<<"(argc == 3)      "<<argv[0]<<" [FileName]  [RuNnumber]"<<std::endl;
      std::cout<<"example:  ./decode  data  11"<<std::endl;
      std::cout<<"(argc == 5)      "<<argv[0]<<" [FileName]  [RuNnumber]  [StartEvent]  [StopEvent]"<<std::endl;
      std::cout<<"example:  ./decode  data  11  10000 200000"<<std::endl;
      return 0;
    }

  TString rawfilepath = "/home/wuhongyi/PKU_CAENDesktopDigitizer20170227/data/";// 必须以 '/' 结尾
  TString rootfilepath = "/home/wuhongyi/PKU_CAENDesktopDigitizer20170227/data/";// 必须以 '/' 结尾

  TString filename(argv[1]);
  TString RunNumber(argv[2]);
  int  runnumber = RunNumber.Atoi();

  int StartEvent,StopEvent;
  if(argc == 5)
    {
      TString StartNumber(argv[3]);
      TString StopNumber(argv[4]);
      StartEvent = StartNumber.Atoi();
      StopEvent = StopNumber.Atoi();
    }
  r2root *r2r=new r2root(rawfilepath,rootfilepath,filename,runnumber,argc,StartEvent,StopEvent);
  r2r->Process();
  delete r2r;
  
  return 0;
}

// 
// decode.cc ends here
