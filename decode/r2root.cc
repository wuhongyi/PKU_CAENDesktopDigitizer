// r2root.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 12月  6 19:34:10 2016 (+0800)
// Last-Updated: 二 12月  6 20:53:55 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 22
// URL: http://wuhongyi.cn 

#include "r2root.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <string.h>
#include <stdio.h>

#include <iostream>
#include <climits>
#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

r2root::r2root(TString rawfilepath,TString rootfilepath,TString fn,int runnumber)
{
  benchmark = new TBenchmark;
  nevt = 0;
  flag = -1;

  sprintf(filename,"%s%s_R%04d.bin",rawfilepath.Data(),fn.Data(),runnumber);
  sprintf(rootfilename,"%s%s_R%04d.root",rootfilepath.Data(),fn.Data(),runnumber);
}

r2root::~r2root()
{

}

bool r2root::IsFileExists(const char *name)
{
  if((access(name,F_OK))!=-1)  
    {  
      return true;
    }  
  else  
    {  
      return false;
    } 
}


void r2root::Process()
{
  if(!IsFileExists(filename))
    {
      std::cout<<"Can't find raw data file -- "<<filename<<std::endl;
      return;
    }

  if((fd = open(filename,O_RDONLY))<0)
    {
      std::cout<<"Can't open file -- "<<filename<<std::endl;
      return;
    }

  size_t n = read(fd,&header,16);
  if(n <= 0) return;
  for (int i = 0; i < 16; ++i)
    {
      if(header[i] == '_')
	{
	  if(header[i+1] == 'S' && header[i+2] == 'T' && header[i+3] == 'D')
	    {
	      flag = 0;
	      std::cout<<"DATA "<<header<<"   Type: STD"<<std::endl;
	    }
	  if(header[i+1] == 'P' && header[i+2] == 'H' && header[i+3] == 'A')
	    {
	      flag = 1;
	      std::cout<<"DATA "<<header<<"   Type: PHA"<<std::endl;
	    }
	  if(header[i+1] == 'P' && header[i+2] == 'S' && header[i+3] == 'D')
	    {
	      flag = 2;
	      std::cout<<"DATA "<<header<<"   Type: PSD"<<std::endl;
	    }
	  break;
	}
    }
    if(flag < 0) return;
  
  benchmark->Start("r2root");//计时开始
	       
  file = new TFile(rootfilename,"RECREATE");
  t = new TTree("tree","PKU Desktop Digitizer Data");

  switch(flag)
    {
    case 0:
      t->Branch("ch",&ch,"ch/I");
      t->Branch("size",&size,"size/I");
      t->Branch("timestamp",&timestamp,"timestamp/i");
      t->Branch("eventcounter",&eventcounter,"eventcounter/i");
      t->Branch("pattern",&pattern,"pattern/i");
      t->Branch("dt",&dt,"dt[size]/I");
      t->Branch("data",&data,"data[size]/I");
      t->Branch("nevt",&nevt,"nevt/I");
      break;

    case 1:
      t->Branch("ch",&ch,"ch/I");
      t->Branch("size",&size,"size/I");
      t->Branch("timestamp",&timestamp,"timestamp/i");
      t->Branch("energy",&energy,"energy/I");
      t->Branch("dt",&dt,"dt[size]/I");
      t->Branch("data",&data,"data[size]/I");
      t->Branch("nevt",&nevt,"nevt/I"); 
      break;

    case 2:
      t->Branch("ch",&ch,"ch/I");
      t->Branch("size",&size,"size/I");
      t->Branch("timestamp",&timestamp,"timestamp/i");
      t->Branch("chargeshort",&chargeshort,"chargeshort/I");
      t->Branch("chargelong",&chargelong,"chargelong/I");
      t->Branch("baseline",&baseline,"baseline/I");
      t->Branch("dt",&dt,"dt[size]/I");
      t->Branch("data",&data,"data[size]/I");
      t->Branch("nevt",&nevt,"nevt/I");       
      break;
    }
  
  while(ReadEvent())
    {
      t->Fill();
      nevt++;
      if(nevt%10000 == 0) std::cout<<"nevt: "<<nevt<<std::endl;
    }



  
  file->cd();
  t->Write();
  file->Close();

  benchmark->Show("r2root");//计时结束并输出时间
}

bool r2root::ReadEvent()
{
  size_t n;
  switch(flag)
    {
    case 0:
      n = read(fd,&HeaderSTD,4*HEADERSTD);
      if(n <= 0) return false;
      ch = HeaderSTD[0];
      size = HeaderSTD[4];
      timestamp = HeaderSTD[1];
      eventcounter = HeaderSTD[2];
      pattern = HeaderSTD[3];
      break;

    case 1:
      n = read(fd,&HeaderPHA,4*HEADERPHA);
      if(n <= 0) return false;
      ch = HeaderPHA[0];
      size = HeaderPHA[3];
      timestamp = HeaderPHA[1];
      energy = HeaderPHA[2];
      break;

    case 2:
      n = read(fd,&HeaderPSD,4*HEADERPSD);
      if(n <= 0) return false;
      ch = HeaderPSD[0];
      size = HeaderPSD[5];
      timestamp = HeaderPSD[1];
      chargeshort = HeaderPSD[2];
      chargelong = HeaderPSD[3];
      baseline = HeaderPSD[4];
      break;
    }

  for (int i = 0; i < size; ++i)
    {
      n = read(fd,&rawdata,2);
      if(n <= 0) return false;
      dt[i] = i;
      data[i] = rawdata;
    }
  
  return true;
}


// 
// r2root.cc ends here
