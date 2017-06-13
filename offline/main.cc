// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 12月  8 19:21:20 2016 (+0800)
// Last-Updated: 二 6月 13 15:11:03 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 235
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
  //以下两个选一个： 手动填写root文件所在路径或者直接使用当前文件所在路径
  // gSystem->Setenv("Dir","/home/wuhongyi");//手动填写路径
  // gSystem->Setenv("Dir",dir);//当前文件路径
  int AddFileNumber = wuReadData::ReadValue<int>("AddFileNumber","ReadData.txt");
  // std::cout<<"AddFileNumber: "<<AddFileNumber<<std::endl;
  //=======================================================
  std::string InputFile;
  for (int i = 0; i < AddFileNumber; ++i)
    {
      char InputFileName[1024];
      sprintf(InputFileName,"InputFileName_%d",i);
      InputFile = wuReadData::ReadValue<std::string>(InputFileName,"ReadData.txt");
      fChain->Add(InputFile.c_str());
    }
  // fChain->Print();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  Short_t         ch;
  UShort_t        size;
  UInt_t          timestamp;
  UShort_t        dt[65535];   //[size]
  UShort_t        data[65535];   //[size]

  // Int_t           ch;
  // Int_t           size;
  // UInt_t          timestamp;
  // Int_t           dt[65535];   //[size]
  // Int_t           data[65535];   //[size]
  
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
  std::cout<<"TotalEntry:  "<<TotalEntry<<std::endl;
  
  int SelectChannel = wuReadData::ReadValue<int>("SelectChannel","ReadData.txt");
  int VotoChannel = wuReadData::ReadValue<int>("VotoChannel","ReadData.txt");
  int VotoTime = wuReadData::ReadValue<int>("VotoTime","ReadData.txt");
  offline *off = new offline();

  std::string OutputFileName = wuReadData::ReadValue<std::string>("OutputFileName","ReadData.txt");
  
  bool PulsePolarity;
  if(wuReadData::ReadValue<int>("PulsePolarity","ReadData.txt") == 1)
    PulsePolarity = true;
  else
    PulsePolarity = false;
  int ADCMSPS = wuReadData::ReadValue<int>("ADCMSPS","ReadData.txt");

  int CalculateVertexPoint = wuReadData::ReadValue<int>("CalculateVertexPoint","ReadData.txt");
  int CalculateBaselinePoint = wuReadData::ReadValue<int>("CalculateBaselinePoint","ReadData.txt");
  int CalculateRiseTimeType = wuReadData::ReadValue<int>("CalculateRiseTimeType","ReadData.txt");
  double FastFilterFL = wuReadData::ReadValue<double>("FastFilterFL","ReadData.txt");
  double FastFilterFG = wuReadData::ReadValue<double>("FastFilterFG","ReadData.txt");
  int FastFilterThre = wuReadData::ReadValue<int>("FastFilterThre","ReadData.txt");
  double SlowFilterSL = wuReadData::ReadValue<double>("SlowFilterSL","ReadData.txt");
  double SlowFilterSG = wuReadData::ReadValue<double>("SlowFilterSG","ReadData.txt");
  int SlowFilterRange = wuReadData::ReadValue<int>("SlowFilterRange","ReadData.txt");
  double PreampTau = wuReadData::ReadValue<double>("PreampTau","ReadData.txt");
  
  off->SetPulsePolarity(PulsePolarity);
  off->SetADCMSPS(ADCMSPS);
  off->SetCalculateVertexPoint(CalculateVertexPoint);
  off->SetCalculateBaselinePoint(CalculateBaselinePoint);
  off->SetCalculateRiseTimeType(CalculateRiseTimeType);//设置上升时间计算方法
  
  off->SetFastFilterPar(FastFilterFL,FastFilterFG,FastFilterThre);//100
  off->SetSlowFilterPar(SlowFilterSL,SlowFilterSG,SlowFilterRange);
  off->SetPreampTau(PreampTau);

  off->PrintFilterPar();
  
  TCanvas *c1 = new TCanvas("c1","",1200,800);
  // gStyle->SetOptStat(0);//不显示统计框
  c1->ToggleEventStatus();//底端信息栏
  // c1->ToggleEditor();
  // c1->ToggleToolBar();
  // c1->Clear("");
  // c1->Divide(/*col*/,/*raw*/);
  // c1->SetGridx();//SetGridy();
  // c1->SetLogx();//SetLogy(); SetLogz();
  // c1->SetName("");

  TH1I *energy = new TH1I("energy","",65536,0,65536);//2048,0,32768
  energy->GetXaxis()->SetTitle("Energy[ch]");
  TH1I *time = new TH1I("time","",2000,0,2000);
  time->GetXaxis()->SetTitle("RiseTime[ns]");
  TH2I *energytime = new TH2I("energytime","",2000,0,2000,2048,0,32768);//2048,0,32768
  energytime->GetXaxis()->SetTitle("RiseTime[ns]");
  energytime->GetYaxis()->SetTitle("Energy[ch]");

  TH2I *deltaEE = new TH2I("DeltaE-E","",2048,0,32768,2048,0,32768);//2048,0,32768,2048,0,32728
  deltaEE->GetXaxis()->SetTitle("E[ch]");
  deltaEE->GetYaxis()->SetTitle("DeltaE[ch]");

  TH1I *energyQ = new TH1I("energyQ","",163840,0,655360);//2048,0,32768
  energyQ->GetXaxis()->SetTitle("Energy[ch]");

  // TH2D *energypsd = new TH2D("energypsd","",5000,0,10000,1000,0,1);
  TH2D *energypsd = new TH2D("energypsd","",5000,0,200000,5000,0,200000);
  
  TGraph *filter = new TGraph();

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  if(argc == 1)
    {
      std::cout<<"Calculate Energy:"<<std::endl;
      int CountUnderThreshold = 0;
      int CountChannelEntey = 0;
      double tempenergy,tempenergyvote;
      
      for (Long64_t entry = 0; entry < TotalEntry; ++entry)
	{//循环处理从这里开始
	  fChain->GetEvent(entry);//这个是重点，拿到TChain中第entry行数据
	  if(entry % 10000 == 0)
	    {
	      std::cout<<"Process Event: "<<entry<<std::endl;
	      std::cout<<"UnderThreshold: "<<CountUnderThreshold<<" / "<<CountChannelEntey<<std::endl;
	      CountUnderThreshold = 0;
	      CountChannelEntey = 0;
	    }
	    
	  if(ch != SelectChannel) continue;
	  CountChannelEntey++;
	  off->SetEventData(size, data);

	  // vote
	  Long64_t timevote1 = timestamp;
	  Long64_t timevote2;
	  bool voteflag = false;
	  if(VotoChannel > -1)
	    {
	      for (Long64_t tempvote = entry+1; tempvote < TotalEntry; ++tempvote)
		{
		  fChain->GetEvent(tempvote);
		  if(ch == SelectChannel) break;
		  if(ch == VotoChannel)
		    {
		      timevote2 = timestamp;
		      // if(timevote1 > timevote2) timevote2 = timevote2+((Long64_t)1<<32);
		      if(TMath::Abs(timevote2-timevote1) <= VotoTime)
			{
			  voteflag = true;
			  break;
			}
		    }
		}
	    }
	  if(voteflag)
	    {
	      // tempenergy = off->GetEnergy();
	      tempenergy = off->GetWaveHigh();
	      off->SetEventData(size, data);
	      tempenergyvote = off->GetEnergy();
	      // tempenergyvote = off->GetWaveHigh();
	      deltaEE->Fill(tempenergyvote,tempenergy);
	      continue;
	    }
	  
	  // if(data[0]<15700 || data[0]>15800) continue;
	  
	  tempenergy = off->GetEnergy();
	  // tempenergy = off->GetWaveHigh();
	  
	  if(tempenergy < 0)
	    {
	      CountUnderThreshold++;
	    }
	  else
	    {
	      energy->Fill(tempenergy);
	      
	      double risetime = off->GetRiseTime();
	      // if(tempenergy > 1680 && tempenergy < 1720)
		{
		  time->Fill(risetime);
		  energytime->Fill(risetime,tempenergy);
		}
	    }
	  
	}//循环处理到这里结束
      std::cout<<std::endl;
      gBenchmark->Show("tree");//计时结束并输出时间

      c1->Divide(2,2);
      c1->cd(1);
      energy->Draw();
      c1->cd(2);
      time->Draw();
      c1->cd(3);
      energytime->Draw("colz");
      c1->cd(4);
      deltaEE->Draw("colz");
      c1->Update();

      TFile *outputrootfile = new TFile(OutputFileName.c_str(),"RECREATE");//"RECREATE" "READ"
      if(!outputrootfile->IsOpen())
	{
	  std::cout<<"Can't open root file"<<std::endl;
	}
 
      // outputrootfile->ls("");
      // TObject->Write();
      // TH1D *h = (TH1D*)outputrootfile->Get("name");
      outputrootfile->cd();
      c1->Write();
      energy->Write();
      time->Write();
      energytime->Write();
      deltaEE->Write();
      outputrootfile->Close();
    }
  
  if(argc == 4)
    {
      int fastfilter[65535];
      int slowfilter[65535];
      int wavedata[65535];
      int firstorderdifferential[65535];
      
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
	      fChain->GetEvent(entry);

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
	      fChain->GetEvent(entry);

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
	      fChain->GetEvent(entry);

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

      if(argv[1][0] == 'I' || argv[1][0] == 'i')
	{
	  std::cout<<"Calculate First Order Differential:"<<std::endl;
	  for (Long64_t entry = NMIN; entry <= NMAX; ++entry)
	    {//循环处理从这里开始
	      if(entry > TotalEntry) break;
	      fChain->GetEvent(entry);

	      if(ch != SelectChannel) continue;
	      off->SetEventData(size, data);
	      off->GetFirstOrderDifferential(firstorderdifferential);
	      for (int i = 0; i < size; ++i)
		{
		  filter->SetPoint(CountPoint++,i,firstorderdifferential[i]);
		}
	    }//循环处理到这里结束

	  c1->cd();
	  filter->Draw("AP");
	  c1->Update();
	}

      
      gBenchmark->Show("tree");//计时结束并输出时间
    }


  if(argc == 2)
    {
      int wavedata[65535];
      int CountPoint = 0;
      
      if(argv[1][0] == '1')
	{
      
	  for (Long64_t entry = 0; entry < TotalEntry; ++entry)
	    {//循环处理从这里开始
	      fChain->GetEvent(entry);

	      if(ch != SelectChannel) continue;
	      off->SetEventData(size, data);

	      double tempenergy = off->GetEnergy();
	      double risetime = off->GetRiseTime();

	      if(tempenergy > 22200)
		{
		  if(/*(risetime>129&&risetime<131) ||*/ (risetime>144&&risetime<146))
		    {
		      off->GetWaveData(wavedata);
		      for (int i = 0; i < size; ++i)
			{
			  filter->SetPoint(CountPoint++,i,wavedata[i]);
			}
		    }
		}
	      
	    }//循环处理到这里结束

	  c1->cd();
	  filter->Draw("AP");
	  c1->Update();
	}


      if(argv[1][0] == '2')
	{
      
	  for (Long64_t entry = 0; entry < TotalEntry; ++entry)
	    {//循环处理从这里开始
	      fChain->GetEvent(entry);

	      if(ch != SelectChannel) continue;
	      off->SetEventData(size, data);

	      double tempenergy = off->GetEnergy();
	      // double tempenergy = off->GetWaveHigh();
	      double risetime = off->GetRiseTime();
	      if(tempenergy > 20900 && tempenergy < 21100 && risetime > 1100 && risetime < 1200)
		{
		  off->GetWaveData(wavedata);
		  for (int i = 0; i < size; ++i)
		    {
		      filter->SetPoint(CountPoint++,i,wavedata[i]);
		    }
		}
	      
	    }//循环处理到这里结束

	  c1->cd();
	  filter->Draw("AP");
	  c1->Update();
	}

      if(argv[1][0] == 'E' || argv[1][0] == 'e')
	{
	  for (Long64_t entry = 0; entry < TotalEntry; ++entry)
	    {//循环处理从这里开始
	      fChain->GetEvent(entry);

	      if(ch != SelectChannel) continue;
	      off->SetEventData(size, data);

	      int tempenergy = off->GetQEnergy(20,120);
	      if(tempenergy > 0) energyQ->Fill(tempenergy);
	      
	    }//循环处理到这里结束

	  c1->cd();
	  energyQ->Draw();
	  c1->Update();
	}

      if(argv[1][0] == 'P' || argv[1][0] == 'p')
	{
	  for (Long64_t entry = 0; entry < TotalEntry; ++entry)
	    {//循环处理从这里开始
	      fChain->GetEvent(entry);

	      if(ch != SelectChannel) continue;
	      off->SetEventData(size, data);

	      // int tempshort = off->GetQEnergy(0,40);
	      // int tempenergy = off->GetQEnergy(20,120);
	      // if(tempshort> 0 && tempenergy > 0) energypsd->Fill(tempenergy*0.0283828+3.16749,double(tempshort)/double(tempenergy));
	      // if(tempshort> 0 && tempenergy > 0) energypsd->Fill(tempenergy*0.0426472-3.81777,double(tempshort)/double(tempenergy));

	      int tempshort = off->GetQEnergy(0,40);
	      int tempenergy = off->GetQEnergy(-40,80);
	      if(tempshort> 0 && tempenergy > 0) energypsd->Fill(tempenergy,tempshort);

	      
	      // 0.0283828  3.16749
	      // 0.0426472  -3.81777
	    }//循环处理到这里结束

	  c1->cd();
	  energypsd->GetXaxis()->SetTitle("keV");
	  energypsd->GetYaxis()->SetTitle("short/long");
	  energypsd->Draw("colz");
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
