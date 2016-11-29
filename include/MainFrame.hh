// MainFrame.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 18:53:51 2016 (+0800)
// Last-Updated: 二 11月 29 19:54:41 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 22
// URL: http://wuhongyi.cn 

#ifndef _MAINFRAME_H_
#define _MAINFRAME_H_

#include "Global.hh"

#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>


#include "RVersion.h"//版本判断
#include "TApplication.h"
#include "TArrow.h"
#include "TAxis.h"
#include "TBenchmark.h"
#include "TBranch.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TColor.h"
#include "TCutG.h"
#include "TDatime.h"
#include "TError.h"
#include "TF1.h"
#include "TF2.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TFormula.h"
#include "TGaxis.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TList.h"
#include "TLorentzVector.h"
#include "TMarker.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TMatrixDEigen.h"
#include "TMultiGraph.h"
#include "TNtuple.h"
#include "TObject.h"
#include "TPad.h"
#include "TPaveLabel.h"
#include "TPaveStats.h"
#include "TPaveText.h"
#include "TRandom.h"
#include "TRandom1.h"
#include "TRandom2.h"
#include "TRandom3.h"
#include "TRint.h"
#include "TROOT.h"
#include "TSlider.h"
#include "TSpectrum.h"
#include "TSpectrum2.h"
#include "TStopwatch.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTimer.h"
#include "TTimeStamp.h"
#include "TTree.h"
#include "TVector3.h"
#include "TVectorD.h"

#include "TG3DLine.h"
#include "TGButton.h"
#include "TGButtonGroup.h"
#include "TGCanvas.h"
#include "TGClient.h"
#include "TGColorSelect.h"
#include "TGComboBox.h"
#include "TGDoubleSlider.h"
#include "TGFileDialog.h"
#include "TGFontDialog.h"
#include "TGFrame.h"
#include "TGHtml.h"
#include "TGIcon.h"
#include "TGImageMap.h"
#include "TGLabel.h"
#include "TGLayout.h"
#include "TGListBox.h"
#include "TGListTree.h"
#include "TGMenu.h"
#include "TGMsgBox.h"
#include "TGNumberEntry.h"
#include "TGPicture.h"
#include "TGResourcePool.h"
#include "TGSimpleTable.h"
#include "TGSlider.h"
#include "TGSpeedo.h"
#include "TGSplitter.h"
#include "TGTab.h"
#include "TGTextEdit.h"
#include "TGTextEntry.h"
#include "TGTextView.h"
#include "TGTextViewStream.h"
#include "TGTreeTable.h"
#include "TGTripleSlider.h"
#include "TGWindow.h"
#include "TRootEmbeddedCanvas.h"
#include "TRootHelpDialog.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Board;

class MainFrame : public TGMainFrame
{
public:
  MainFrame(const TGWindow * p);
  virtual ~MainFrame();

  void SetDataFileName();
  void StartStopRun();
  void SetOnlineData();
  void SetWriteData();
  void MarkChennelOrRecordLengthChange();
  void ProgramDigitizer();
  void AllocateMemory();

  
  
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

private:
  void CloseWindow(); //close main window
  bool IsDirectoryExists(const char *path);

  
  void MakeFoldPanelInit(TGCompositeFrame *TabPanel);
  void MakeFoldPanelPar(TGCompositeFrame *TabPanel);


  int initDigitizer();
  void deleteDigitizer();


  int GetMoreBoardInfo(int handle, CAEN_DGTZ_BoardInfo_t *BoardInfo);

private:
  TGComboBox *connectStyle;
  TGTextButton *connectButton;
  TGTextButton *DeleteButton;
  TGTextEntry *BoardNameMsg;
  TGTextEntry *StateMsg;

  
  TGTextEntry *filepathtext;
  TGTextEntry *filenametext;
  TGNumberEntry *filerunnum;
  TGTextButton *CompleteButton;


  TGCheckButton *ChannelsCheckButton[16];
  TGNumberEntry *RecordLength;
  TGTextButton *ProgramButton;
  TGTextButton *AllocateButton;
  
  TGTextButton *StartStopButton;
  TGCheckButton *OnlineCheckButton;//->IsOn()
  TGTextButton *WtiteDataButton;

  
private:
  Digitizer* dig;
  Board *board;


  char Filename[1024];
  int runnum;
  char tmp[256];

  ClassDef(MainFrame,0)
};

#endif /* _MAINFRAME_H_ */
// 
// MainFrame.hh ends here
