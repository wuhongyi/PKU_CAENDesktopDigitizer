// r2root.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 12月  6 19:33:48 2016 (+0800)
// Last-Updated: 二 12月  6 20:52:34 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 9
// URL: http://wuhongyi.cn 

#ifndef _R2ROOT_H_
#define _R2ROOT_H_

#include "TString.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TBenchmark.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#define MAXTRACEN 65535
#define HEADERSTD 5
#define HEADERPSD 6
#define HEADERPHA 4

class r2root
{
public:
  r2root(TString rawfilepath,TString rootfilepath,TString fn,int runnumber);
  virtual ~r2root();

  void Process();

private:
  bool IsFileExists(const char *name);//判断文件是否存在
  bool ReadEvent();

  
private:
  int fd; // File descripter
  int flag;// 0-STD 1-PHA 2-PSD 3-x742
  char header[16];
  unsigned int HeaderSTD[HEADERSTD];// 0-ch 1-TimeTag 2-EventCounter 3-Pattern 4-size
  unsigned int HeaderPSD[HEADERPSD];// 0-ch 1-TimeTag 2-chargeshort 3-chargelong 4-baseline 5-size
  unsigned int HeaderPHA[HEADERPHA];// 0-ch 1-TimeTag 2-Energy 3-size
  unsigned short rawdata;

  
  Int_t nevt;
  TFile *file;
  TTree *t;
  TBenchmark *benchmark;
  char filename[1024];
  char rootfilename[1024];
private:
  int ch;
  int size;
  UInt_t timestamp;
  UInt_t eventcounter;
  UInt_t pattern;
  int chargeshort;
  int chargelong;
  int baseline;
  int energy;
  int data[MAXTRACEN];
  int dt[MAXTRACEN];
};

#endif /* _R2ROOT_H_ */
// 
// r2root.hh ends here
