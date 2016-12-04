// Global.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 11月 30 09:30:24 2016 (+0800)
// Last-Updated: 日 12月  4 12:24:52 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#include "Global.hh"
#include <cstring> 
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





// 
// Global.cc ends here
