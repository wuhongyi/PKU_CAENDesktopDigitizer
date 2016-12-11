// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 12月  8 19:21:20 2016 (+0800)
// Last-Updated: 日 12月 11 21:10:22 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 51
// URL: http://wuhongyi.cn 

#include "wuReadData.hh"
#include "offline.hh"

#include <iostream>

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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{

  // Create an interactive ROOT application
  TRint *theApp = new TRint("Rint", &argc, argv);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  //初始参数
  TString TreeName="tree";//这里为要处理的文件中 tree name
  // create first the chain with all the files
  TChain *fChain=new TChain(TreeName);
  // std::cout << "Creating the chain" << std::endl;
  fChain->SetCacheSize(20*1024*1024);
  TString dir = gSystem->DirName(__FILE__);//获取当前文件main.cc所在路径 
  dir.ReplaceAll("/./","/");
  // std::cout<<dir<<std::endl;
  //=======================================================
  //以下两个选一个： 手动填写root文件所在路径 或 者直接使用当前文件所在路径
  // gSystem->Setenv("Dir","/home/wuhongyi");//手动填写路径
  std::string InputFile = wuReadData::ReadValue<std::string>("InputFileName","ReadData.txt");
  gSystem->Setenv("Dir",dir);//当前文件路径
  //=======================================================
  //将要处理的文件放在这里，支持tree名相同的多个结构相同的文件。特别适合用于Geant4多线程模拟的输出文件处理。
  string InputFileName = "$Dir"+InputFile;
  fChain->Add(InputFileName.c_str());
  // chain->Print();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  Int_t           ch;
  Int_t           size;
  UInt_t          timestamp;
  Int_t           dt[65535];   //[size]
  Int_t           data[65535];   //[size]

  TBranch        *b_ch;   //!
  TBranch        *b_size;   //!
  TBranch        *b_timestamp;   //!
  TBranch        *b_dt;   //!
  TBranch        *b_data;   //!
   
  fChain->SetBranchAddress("ch", &ch, &b_ch);
  fChain->SetBranchAddress("size", &size, &b_size);
  fChain->SetBranchAddress("timestamp", &timestamp, &b_timestamp);
  fChain->SetBranchAddress("dt", dt, &b_dt);
  fChain->SetBranchAddress("data", data, &b_data);
   
  std::cout <<std::endl<< "=== Running Hongyi Wu Analysis ===" << std::endl;

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  gBenchmark->Start("tree");//计时开始
  // std::string OutputFileName = wuReadData::ReadValue<string>("OutputFileName","ReadData.txt");
  Long64_t TotalEntry = fChain->GetEntries();//拿到TChain中总entry行数

  int SelectChannel = 4;
  offline *off = new offline();
  off->SetPulsePolarity(false);
  off->SetADCMSPS(500);
  off->SetPreampTau(700);
  off->SetFastFilterPar(0.1,0.1,200);
  off->SetSlowFilterPar(1.2,0.5);
  off->SetCalculateBaselinePoint(400);

  TCanvas *c1 = new TCanvas("c1","",600,400);
  // gStyle->SetOptStat(0);//不显示统计框
  // c1->ToggleEventStatus();//底端信息栏
  // c1->ToggleEditor();
  // c1->ToggleToolBar();
  // c1->Clear("");
  // c1->Divide(/*col*/,/*raw*/);
  // c1->SetGridx();//SetGridy();
  // c1->SetLogx();//SetLogy(); SetLogz();
  // c1->SetName("");

  TH1D *energy = new TH1D("energy","",8192,0,8192);
  TGraph *filter = new TGraph();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  if(argc == 1)
    {
      std::cout<<"Calculate Energy:"<<std::endl;
      
      for (Long64_t entry = 0; entry < TotalEntry; ++entry)
	{//循环处理从这里开始
	  fChain->GetEvent(entry);//这个是重点，拿到TChain中第entry行数据
	  if(entry % 1000 == 0) std::cout<<"Process Event: "<<entry<<std::endl;

	  if(ch != SelectChannel) continue;
	  off->SetEventData(size, data);

	  energy->Fill(off->GetEnergy());
	}//循环处理到这里结束
      std::cout<<std::endl;
      gBenchmark->Show("tree");//计时结束并输出时间

      c1->cd();
      energy->Draw();
      c1->Update();
    }
  
  if(argc == 4)
    {
      int fastfilter[65535];
      int slowfilter[65535];
      int wavedata[65535];

      TString argv2(argv[2]);
      TString argv3(argv[3]);
      int NMIN = argv2.Atoi();
      int NMAX = argv3.Atoi();

      int CountPoint = 0;

      if(argv[1][0] == 'W' || argv[1][0] == 'w')
	{
	  std::cout<<"Draw Wave Data:"<<std::endl;
	  for (Long64_t entry = NMIN; entry <= NMAX; ++entry)
	    {//循环处理从这里开始
	      if(entry > TotalEntry) break;
	      fChain->GetEvent(entry);//这个是重点，拿到TChain中第entry行数据
	      if(entry % 1000 == 0) std::cout<<"Process Event: "<<entry<<std::endl;

	      if(ch != SelectChannel) continue;
	      off->SetEventData(size, data);
	      off->GetWaveData(wavedata);
	      for (int i = 0; i < size; ++i)
		{
		  filter->SetPoint(CountPoint++,i,wavedata[i]);
		}
	    }//循环处理到这里结束

	  c1->cd();
	  filter->Draw("AP");
	  c1->Update();
	}

      
      if(argv[1][0] == 'F' || argv[1][0] == 'f')
	{
	  std::cout<<"Calculate Fast Filter:"<<std::endl;
	  for (Long64_t entry = NMIN; entry <= NMAX; ++entry)
	    {//循环处理从这里开始
	      if(entry > TotalEntry) break;
	      fChain->GetEvent(entry);//这个是重点，拿到TChain中第entry行数据
	      if(entry % 1000 == 0) std::cout<<"Process Event: "<<entry<<std::endl;

	      if(ch != SelectChannel) continue;
	      off->SetEventData(size, data);
	      off->GetFastFilter(fastfilter);
	      for (int i = 0; i < size; ++i)
		{
		  filter->SetPoint(CountPoint++,i,fastfilter[i]);
		}
	    }//循环处理到这里结束

	  c1->cd();
	  filter->Draw("AP");
	  c1->Update();
	}

      if(argv[1][0] == 'S' || argv[1][0] == 's')
	{
	  std::cout<<"Calculate Slow Filter:"<<std::endl;
	  for (Long64_t entry = NMIN; entry <= NMAX; ++entry)
	    {//循环处理从这里开始
	      if(entry > TotalEntry) break;
	      fChain->GetEvent(entry);//这个是重点，拿到TChain中第entry行数据
	      if(entry % 1000 == 0) std::cout<<"Process Event: "<<entry<<std::endl;

	      if(ch != SelectChannel) continue;
	      off->SetEventData(size, data);
	      off->GetSlowFilter(slowfilter);
	      for (int i = 0; i < size; ++i)
		{
		  filter->SetPoint(CountPoint++,i,slowfilter[i]);
		}
	    }//循环处理到这里结束

	  c1->cd();
	  filter->Draw("AP");
	  c1->Update();
	}

      gBenchmark->Show("tree");//计时结束并输出时间
    }
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // and enter the event loop...
  theApp->Run();
  delete theApp;
  
  return 0;
}

// 
// main.cc ends here
