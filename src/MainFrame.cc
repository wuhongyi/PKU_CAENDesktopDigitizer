// MainFrame.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 18:54:13 2016 (+0800)
// Last-Updated: 五 11月 25 20:07:10 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#include "MainFrame.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ClassImp(MainFrame)

MainFrame::MainFrame(const TGWindow * p)
{

  TGTab *TabPanel = new TGTab(this);
  this->AddFrame(TabPanel, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  TGCompositeFrame *Tab1 = TabPanel->AddTab("Init");
  MakeFoldPanelInit(Tab1);

  TGCompositeFrame *Tab2 = TabPanel->AddTab("Par");
  MakeFoldPanelPar(Tab2);


  
  SetWindowName("PKU Digitizer");
  MapSubwindows();
  MapWindow();
}

MainFrame::~MainFrame()
{


}

Bool_t MainFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{



  return kTRUE;
}

void MainFrame::MakeFoldPanelInit(TGCompositeFrame *TabPanel)
{


}

void MainFrame::MakeFoldPanelPar(TGCompositeFrame *TabPanel)
{

}



// 
// MainFrame.cc ends here
