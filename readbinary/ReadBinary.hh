// ReadBinary.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 7月 29 18:50:38 2017 (+0800)
// Last-Updated: 六 7月 29 19:31:38 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1
// URL: http://wuhongyi.cn 

#ifndef _READBINARY_H_
#define _READBINARY_H_

#include "TString.h"
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

class ReadBinary
{
public:
  ReadBinary(TString rawfilepath,TString fn,int runnumber);
  virtual ~ReadBinary();

  void Process();

private:
  bool IsFileExists(const char *name);//判断文件是否存在

  
private:
  int fd; // File descripter
  int flag;// 0-STD 1-PHA 2-PSD 3-x742
  char header[16];
  int headerlength;//header length


  TBenchmark *benchmark;
};

#endif /* _READBINARY_H_ */
// 
// ReadBinary.hh ends here
