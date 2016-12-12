// Global.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 11月 30 09:30:24 2016 (+0800)
// Last-Updated: 一 12月 12 20:11:48 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 14
// URL: http://wuhongyi.cn 

#include "Global.hh"
#include <cstring>
#include <ctime>
#include <sys/stat.h>//stat(const char *file_name,struct stat *buf)
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

static FILE *PKU_DGTZ_LogFilePointer = NULL;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Set a bit in a 32-bit unsigned integer.
unsigned int SetBit_32(unsigned short bit,unsigned int value)//将value第bit位设为1
{
  return(value | (unsigned int)(std::pow(2.0, (double)bit)));
}

// Clear a bit in a 32-bit unsigned integer.
unsigned int ClrBit_32(unsigned short bit,unsigned int value)//将value第bit位设为0
{
  value = SetBit_32(bit, value);
  return(value ^ (unsigned int)(std::pow(2.0, (double)bit)));
}

// Test a bit in a 32-bit unsigned integer.
unsigned int TstBit_32(unsigned short bit,unsigned int value )//获得value第bit位数值
{
  return(((value & (unsigned int)(std::pow(2.0, (double)bit))) >> bit));
}


int Decimal2Binary(uint32_t d, char *bstr)
{
  // if (d < 0) return -1;
  int mod = 0;
  char tmpstr[64];
  bzero(tmpstr, sizeof(tmpstr));
  bzero(bstr, sizeof(bstr));
 
  unsigned int i = 0;
  if(d==0)
    {
      tmpstr[0]='0';
      i++;
    }
  while (d > 0)
    {
      mod = d % 2;
      d /= 2;
      tmpstr[i] = mod + '0';
      i++;
    }
 
  //copy
  unsigned int len = strlen(tmpstr);
  for (i = 0; i < len; i++)
    {
      bstr[i] = tmpstr[len - i - 1];
    }
  return (int)len;
}


long GetTime()
{
  long time_ms;
  struct timeval t1;
  struct timezone tz;
  gettimeofday(&t1, &tz);
  time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
  return time_ms;
}

double GetFileSizeMB(const char *name)
{
  struct stat statbuff;  
  if(stat(name, &statbuff) < 0){  
    printf("errno：%d\n",errno);
    printf("ERR  ：%s\n",strerror(errno));
    return 0;  
  }else{  
    return statbuff.st_size/(1024.0*1024.0);
  }
}


std::string GetTimeStringYmdHMS()
{
  time_t lt = time(NULL);
  tm* current = localtime(&lt);
  char str[100];
  strftime(str,100,"%Y%m%d%H%M%S",current);
  return std::string(str);
}


void OpenRunLog()
{
  PKU_DGTZ_LogFilePointer = fopen("../run.log","aw");
  if(PKU_DGTZ_LogFilePointer == NULL)
    printf("open file ../run.log fail \n");
}

void WriteRunLog(char *log)
{
  fprintf(PKU_DGTZ_LogFilePointer,"%s",log);
}

void WriteRunLog(std::string log)
{
  fprintf(PKU_DGTZ_LogFilePointer,"%s",log.c_str());
}

void CloseRunLog()
{
  if(PKU_DGTZ_LogFilePointer != NULL)
    fclose(PKU_DGTZ_LogFilePointer);
  PKU_DGTZ_LogFilePointer = NULL; 
}

// 
// Global.cc ends here
