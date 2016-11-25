// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 18:49:01 2016 (+0800)
// Last-Updated: 五 11月 25 20:05:46 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#include "MainFrame.hh"

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
