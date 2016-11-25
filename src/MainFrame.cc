// MainFrame.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 18:54:13 2016 (+0800)
// Last-Updated: 五 11月 25 21:48:15 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 10
// URL: http://wuhongyi.cn 

#include "MainFrame.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ClassImp(MainFrame)

MainFrame::MainFrame(const TGWindow * p)
{
  dig = (Digitizer*) calloc(1,sizeof(Digitizer));
  dig->boardInfo = (CAEN_DGTZ_BoardInfo_t*) calloc(1,sizeof(CAEN_DGTZ_BoardInfo_t));

  dig->linkType = CAEN_DGTZ_USB;
  dig->linkNum = 0;
  dig->conetNode = 0;
  dig->baseAddress = 0;

  initDigitizer();
  
  
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

  free(dig);
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int MainFrame::initDigitizer()
{
  int ret; 
  int majorNumber;

  ret = CAEN_DGTZ_OpenDigitizer(dig->linkType, dig->linkNum, dig->conetNode, dig->baseAddress, &dig->boardHandle);
  if (ret)
    {
      printf("Can't open digitizer\n");
      return ret;
    }
  
  ret = CAEN_DGTZ_GetInfo(dig->boardHandle, dig->boardInfo);
  if (ret) return ret;

  printf("Connected to CAEN Digitizer Model %s\n", dig->boardInfo->ModelName);
  printf("ROC FPGA Release is %s\n", dig->boardInfo->ROC_FirmwareRel);
  printf("AMC FPGA Release is %s\n", dig->boardInfo->AMC_FirmwareRel);

  printf("Model is %d\n", dig->boardInfo->Model);
  printf("Channels is %d\n", dig->boardInfo->Channels);
  printf("FormFactor is %d\n", dig->boardInfo->FormFactor);
  printf("FamilyCode is %d\n", dig->boardInfo->FamilyCode);
  printf("SerialNumber is %d\n", dig->boardInfo->SerialNumber);
  printf("PCB_Revision is %d\n", dig->boardInfo->PCB_Revision);
  printf("ADC_NBits is %d\n", dig->boardInfo->ADC_NBits);
  printf("CommHandle is %d\n", dig->boardInfo->CommHandle);
  printf("VMEHandle is %d\n", dig->boardInfo->VMEHandle);
  printf("License is %s\n", dig->boardInfo->License);

  
}

void MainFrame::deleteDigitizer()
{


  
}


// 
// MainFrame.cc ends here
