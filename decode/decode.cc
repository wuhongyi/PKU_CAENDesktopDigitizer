// decode.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 12月  6 19:25:21 2016 (+0800)
// Last-Updated: 二 12月  6 19:46:46 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#include "r2root.hh"

#include "TString.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  if(argc != 3)
    {
      std::cout<<"error(argc != 3)      ./"<<argv[0]<<" [FileName]  [RuNnumber]"<<std::endl;
      std::cout<<"example:  ./decode  data  11"<<std::endl;
      return 0;
    }

  TString rawfilepath = "../data/";
  TString rootfilepath = "../data/";

  TString filename(argv[1]);
  TString RunNumber(argv[2]);
  int  runnumber = RunNumber.Atoi();

  r2root *r2r=new r2root(rawfilepath,rootfilepath,filename,runnumber);
  r2r->Process();
  delete r2r;
  
  return 0;
}

// 
// decode.cc ends here
