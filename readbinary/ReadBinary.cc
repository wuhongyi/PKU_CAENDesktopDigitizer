// ReadBinary.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 7月 29 18:50:48 2017 (+0800)
// Last-Updated: 六 7月 29 19:31:38 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1
// URL: http://wuhongyi.cn 

#include "ReadBinary.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ReadBinary::ReadBinary(TString rawfilepath,TString fn,int runnumber)
{
  benchmark = new TBenchmark;

  
}

ReadBinary::~ReadBinary()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool ReadBinary::IsFileExists(const char *name)
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


void ReadBinary::Process()
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



  size_t n = read(fd,&header,headerlength);
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

    benchmark->Start("readbinary");//计时开始
  



    benchmark->Show("readbinary");//计时结束并输出时间
}


// 
// ReadBinary.cc ends here
