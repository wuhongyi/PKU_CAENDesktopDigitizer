// r2root.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 12月  6 19:33:48 2016 (+0800)
// Last-Updated: 六 4月 15 15:39:35 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 16
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
#define HEADERPSD 10
#define HEADERPHA 8

class r2root
{
public:
  r2root(TString rawfilepath,TString rootfilepath,TString fn,int runnumber,int argc = -1,int start = -1,int stop = -1);
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
  unsigned int HeaderPSD[HEADERPSD];// 0-ch 1-TimeTag 2-chargeshort 3-chargelong 4-baseline 5-size 6-Format 7-Format2 8-Extras 9-Pur
  unsigned int HeaderPHA[HEADERPHA];// 0-ch 1-TimeTagHigh 2-TimeTagLow 3-Energy 4-size 5-Format 6-Extras 7-Extras2
  unsigned short rawdata;

  Int_t ARGC;
  Int_t StartEvent;
  Int_t StopEvent;
  Int_t nevt;
  TFile *file;
  TTree *t;
  TBenchmark *benchmark;
  char filename[1024];
  char rootfilename[1024];
private:
  Short_t ch;
  UShort_t size;
  UInt_t timestamp;
  ULong64_t TimeStamp;
  UInt_t eventcounter;
  UInt_t pattern;
  Short_t chargeshort;
  Short_t chargelong;
  Short_t baseline;
  UShort_t energy;

  UInt_t format;
  UInt_t format2;
  UShort_t extrasUS;
  UInt_t extrasUI;
  UInt_t extras2;
  Short_t pur;
  UShort_t data[MAXTRACEN];
  UShort_t dt[MAXTRACEN];
};

#endif /* _R2ROOT_H_ */
// 
// r2root.hh ends here
