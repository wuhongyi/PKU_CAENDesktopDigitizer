// Global.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 11月 30 09:30:24 2016 (+0800)
// Last-Updated: 三 11月 30 09:30:58 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1
// URL: http://wuhongyi.cn 

#include "Global.hh"
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

// 
// Global.cc ends here
