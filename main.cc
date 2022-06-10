// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 18:49:01 2016 (+0800)
// Last-Updated: 五 6月 10 08:55:41 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
// URL: http://wuhongyi.cn 

#include "MainFrame.hh"

#include "RVersion.h"//版本判断
#include "TApplication.h"
#include "TBenchmark.h"
#include "TError.h"
#include "TList.h"
#include "TObject.h"
#include "TRint.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{

  TRint *theApp = new TRint("Rint", &argc, argv);

  MainFrame mainWindow(gClient->GetRoot());

  theApp->Run();
  
  return 0;
}

// 
// main.cc ends here
