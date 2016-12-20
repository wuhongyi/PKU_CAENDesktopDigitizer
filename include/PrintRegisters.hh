// PrintRegisters.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 12月 12 10:46:59 2016 (+0800)
// Last-Updated: 二 12月 20 12:15:34 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 5
// URL: http://wuhongyi.cn 

#ifndef _PRINTREGISTERS_H_
#define _PRINTREGISTERS_H_

// Read DGTZ Registers 
// return  0=success; -1=error
void Read_DGTZ_Register(int handle,int MaxNChInBoard);

// return  0=success; -1=error
void Read_DGTZ_Register_725_730_DPP_PHA_Revision00(int handle,int MaxNChInBoard);

// return  0=success; -1=error
void Read_DGTZ_Register_725_730_DPP_PSD_Revision03(int handle,int MaxNChInBoard);

// return  0=success; -1=error
void Read_DGTZ_Register_725_730_STD_Revision00(int handle,int MaxNChInBoard);

#endif /* _PRINTREGISTERS_H_ */
// 
// PrintRegisters.hh ends here
