// PrintRegisters.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 12月 12 10:47:24 2016 (+0800)
// Last-Updated: 二 12月 20 12:52:06 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 17
// URL: http://wuhongyi.cn 

#include "PrintRegisters.hh"
#include "Global.hh"

#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>

#include <cstdio>
#include <cstdlib>
#include<cstring>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Read_DGTZ_Register(int handle,int MaxNChInBoard)
{
  int ret = 0;
  uint32_t Register_Data;
  char bstr[64];
  bzero(bstr, sizeof(bstr));

  printf("\n\n==========================================================\n\n");
  
  // CONFIG
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8000, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIG (0x8000,rw): %s\n",bstr);

  // CONFIG(BitSet)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8004, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIG (BitSet) (0x8004,rw): %s\n",bstr);

  // CONFIG(BitClear)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8008, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIG (BitClear) (0x8008,rw): %s\n",bstr);

  // BUFF_ORG
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x800C, &Register_Data);
  // printf("BUFF_ORG (0x800C): %u\n",Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("BUFF_ORG (0x800C,rw): %s\n",bstr);

  // CUST_SIZE
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8020, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CUST_SIZE (0x8020,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1020 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("CUST_SIZE (0x1%d20,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // ACQUISITION_CONTROL
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8100, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("ACQUISITION_CONTROL (0x8100,rw): %s\n",bstr);

  // ACQUISITION_STATUS
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8104, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("ACQUISITION_STATUS (0x8104,r): %s\n",bstr);

  // SOFTWARE_TRIGGER
  // ret |= CAEN_DGTZ_ReadRegister(handle, 0x8108, &Register_Data);
  // Decimal2Binary(Register_Data,bstr);
  // printf("SOFTWARE_TRIGGER (0x8108,w): %s\n",bstr);   

  // GLOBAL_TRIGGER_MASK
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x810C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("GLOBAL_TRIGGER_MASK (0x810C,rw): %s\n",bstr);

  // TRIGGER_OUT_MASK
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8110, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("TRIGGER_OUT_MASK (0x8110,rw): %s\n",bstr);

  // Reserved
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8114, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Reserved (0x8114): %s\n",bstr);
      
  // LVDS_DATA
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8118, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("LVDS_DATA (0x8118,rw): %s\n",bstr);

  // FRONT_PANEL_CONTROL
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x811C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("FRONT_PANEL_CONTROL (0x811C,rw): %s\n",bstr);

  // CHANNEL_ENABLE_MASK
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8120, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CHANNEL_ENABLE_MASK (0x8120,rw): %s\n",bstr);

  // FIRMWARE_REVISION
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8124, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("FIRMWARE_REVISION (0x8124,r): %s\n",bstr);

  // Reserved
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x812C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Reserved (0x812C): %s\n",bstr);

  // Reserved
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8130, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Reserved (0x8130): %s\n",bstr);

  // Reserved
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8134, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Reserved (0x8134): %s\n",bstr);
      
  // MONITOR_DAC_CONTROL
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8138, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("MONITOR_DAC_CONTROL (0x8138,rw): %s\n",bstr);

  // SW_CLOCK_SYNC
  // ret |= CAEN_DGTZ_ReadRegister(handle, 0x813C, &Register_Data);
  // Decimal2Binary(Register_Data,bstr);
  // printf("SW_CLOCK_SYNC (0x813C,w): %s\n",bstr);
      
  // BOARD_INFO
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8140, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("BOARD_INFO (0x8140,r): %s\n",bstr);

  // MINITOR_DAC_MODE
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8144, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("MINITOR_DAC_MODE (0x8144,rw): %s\n",bstr);

  // Reserved
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8148, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Reserved (0x8148): %s\n",bstr);
      
  // EVENT_SIZE
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x814C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("EVENT_SIZE (0x814C,rw): %s\n",bstr);

  // Reserved
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8150, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Reserved (0x8150): %s\n",bstr);

  // Reserved
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8154, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Reserved (0x8154): %s\n",bstr);
      
  // TIME_BOMB_DOWNCOUNTER
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8158, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("TIME_BOMB_DOWNCOUNTER (0x8158,r): %s\n",bstr);

  // Reserved
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x815C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Reserved (0x815C): %s\n",bstr);

  // Reserved
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8164, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Reserved (0x8164): %s\n",bstr);

  // Reserved
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x816C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Reserved (0x816C): %s\n",bstr);
           
  // RUN_START_STOP_DELAY
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8170, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("RUN_START_STOP_DELAY (0x8170,rw): %s\n",bstr);

  // Reserved
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8174, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Reserved (0x8174): %s\n",bstr);
      
  // VETO
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x817C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("VETO (0x817C,rw): %s\n",bstr);

  printf("Reserved (0x8180-0x819C)\n");
  printf("Reserved (0x8200-0x83FC)\n");
      
  // READOUT_CONTROL
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF00, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("READOUT_CONTROL (0xEF00,rw): %s\n",bstr);

  // READOUT_STATUS
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF04, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("READOUT_STATUS (0xEF04,rw): %s\n",bstr);

  // VME_GEO_ADDRESS
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF08, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("VME_GEO_ADDRESS (0xEF08,rw): %s\n",bstr);

  // VME_MULITCAST_CONTROL
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF0C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("VME_MULITCAST_CONTROL (0xEF0C,rw): %s\n",bstr);

  // VME_ADDRESS_RELOCATION
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF10, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("VME_ADDRESS_RELOCATION (0xEF10,rw): %s\n",bstr);

  // VME_INTERRUPT_ID
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF14, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("VME_INTERRUPT_ID (0xEF14,rw): %s\n",bstr);

  // READOUT_INTERRUPT_CONDITION(EVENT NUMBER)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF18, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("READOUT_INTERRUPT_CONDITION(EVENT NUMBER) (0xEF18,rw): %s\n",bstr);

  // READOUT_BLT_AGGREGATE_NUMBER
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF1C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("READOUT_BLT_AGGREGATE_NUMBER (0xEF1C,rw): %s\n",bstr);

  // DUMMY
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF20, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("DUMMY (0xEF20,rw): %s\n",bstr);

  // SOFTWARE_RESET
  // ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF24, &Register_Data);
  // Decimal2Binary(Register_Data,bstr);
  // printf("SOFTWARE_RESET (0xEF24,w): %s\n",bstr);

  // SOFTWARE_CLEAR
  // ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF28, &Register_Data);
  // Decimal2Binary(Register_Data,bstr);
  // printf("SOFTWARE_CLEAR (0xEF28,w): %s\n",bstr);
      
  // BOARD_CONFIGURATION_RELOAD
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF34, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("BOARD_CONFIGURATION_RELOAD (0xEF34,rw): %s\n",bstr);

  // CONFIGURATION_ROM_CHECKSUM
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF000, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_CHECKSUM (0xF000,r): %s\n",bstr);

  // CONFIGURATION_ROM_CHECKSUM_LENGTH2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF004, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_CHECKSUM_LENGTH2 (0xF004,r): %s\n",bstr);

  // CONFIGURATION_ROM_CHECKSUM_LENGTH1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF008, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_CHECKSUM_LENGTH1 (0xF008,r): %s\n",bstr);

  // CONFIGURATION_ROM_CHECKSUM_LENGTH0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF00C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_CHECKSUM_LENGTH0 (0xF00C,r): %s\n",bstr);

  // CONFIGURATION_ROM_CONSTANT2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF010, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_CONSTANT2 (0xF010,r): %s\n",bstr);

  // CONFIGURATION_ROM_CONSTANT1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF014, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_CONSTANT1 (0xF014,r): %s\n",bstr);

  // CONFIGURATION_ROM_CONSTANT0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF018, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_CONSTANT0 (0xF018,r): %s\n",bstr);

  // CONFIGURATION_ROM_C_CODE
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF01C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_C_CODE (0xF01C,r): %s\n",bstr);

  // CONFIGURATION_ROM_R_CODE
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF020, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_R_CODE (0xF020,r): %s\n",bstr);

  // CONFIGURATION_ROM_OUI2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF024, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_OUI2 (0xF024,r): %s\n",bstr);

  // CONFIGURATION_ROM_OUI1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF028, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_OUI1 (0xF028,r): %s\n",bstr);

  // CONFIGURATION_ROM_OUI0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF02C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_OUI0 (0xF02C,r): %s\n",bstr);

  // CONFIGURATION_ROM_VERSION
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF030, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_VERSION (0xF030,r): %s\n",bstr);

  // CONFIGURATION_ROM_FORM_FACTOR
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF034, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_FORM_FACTOR (0xF034,r): %s\n",bstr);

  // CONFIGURATION_ROM_BOARD_ID1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF038, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_BOARD_ID1 (0xF038,r): %s\n",bstr);

  // CONFIGURATION_ROM_BOARD_ID0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF03C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_BOARD_ID0 (0xF03C,r): %s\n",bstr);

  // CONFIGURATION_ROM_REVIS3
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF040, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_REVIS3 (0xF040,r): %s\n",bstr);

  // CONFIGURATION_ROM_REVIS2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF044, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_REVIS2 (0xF044,r): %s\n",bstr);

  // CONFIGURATION_ROM_REVIS1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF048, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_REVIS1 (0xF048,r): %s\n",bstr);

  // CONFIGURATION_ROM_PCB_RESION
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF04C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_PCB_RESION (0xF04C,r): %s\n",bstr);

  // CONFIGURATION_ROM_SERNUM1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF080, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_SERNUM1 (0xF080,r): %s\n",bstr);

  // CONFIGURATION_ROM_SERNUM0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF084, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_SERNUM0 (0xF084,r): %s\n",bstr);

  // CONFIGURATION_ROM_VCXO_TYPE
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF088, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("CONFIGURATION_ROM_VCXO_TYPE (0xF088,r): %s\n",bstr);

  // PRE_TRG
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8038, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("PRE_TRG (0x8038,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1038 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("PRE_TRG (0x1%d38,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // NEV_AGGREGATE
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8034, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("NEV_AGGREGATE (0x8034,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1034 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("NEV_AGGREGATE (0x1%d34,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP_TRG_FILTER_SMOOTHING(a)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8054, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("DPP_TRG_FILTER_SMOOTHING(a) (0x8054,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1054 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP_TRG_FILTER_SMOOTHING(a) (0x1%d54,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP_INPUT_RISE_TIME(Tirt)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8058, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("DPP_INPUT_RISE_TIME(Tirt) (0x8058,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1058 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP_INPUT_RISE_TIME(Tirt) (0x1%d58,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP_TRAPEZOID_RISE_TIME(Trise)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x805C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("DPP_TRAPEZOID_RISE_TIME(Trise) (0x805C,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x105C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP_TRAPEZOID_RISE_TIME(Trise) (0x1%d5C,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP_TRAPEZOID_FLAT_TOP(Tflattop)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8060, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("DPP_TRAPEZOID_FLAT_TOP(Tflattop) (0x8060,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1060 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP_TRAPEZOID_FLAT_TOP(Tflattop) (0x1%d60,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP_PEAKINGTIME(Tpk)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8064, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("DPP_PEAKINGTIME(Tpk) (0x8064,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1064 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP_PEAKINGTIME(Tpk) (0x1%d64,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP_DECAY_TIME(M)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8068, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("DPP_DECAY_TIME(M) (0x8068,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1068 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP_DECAY_TIME(M) (0x1%d68,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP_TRG_THRESHOLD(thr)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x806C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("DPP_TRG_THRESHOLD(thr) (0x806C,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x106C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP_TRG_THRESHOLD(thr) (0x1%d6C,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP_RTD_WINDOW_WIDTH(Trtdw)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8070, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("DPP_RTD_WINDOW_WIDTH(Trtdw) (0x8070,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1070 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP_RTD_WINDOW_WIDTH(Trtdw) (0x1%d70,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP_RTRG_HOLD_OFF(Ttrgho)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8074, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("DPP_RTRG_HOLD_OFF(Ttrgho) (0x8074,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1074 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP_RTRG_HOLD_OFF(Ttrgho) (0x1%d74,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP_PEAK_HOLD_OFF_EXTENSION(Tpkhoe)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8078, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("DPP_PEAK_HOLD_OFF_EXTENSION(Tpkhoe) (0x8078,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1078 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP_PEAK_HOLD_OFF_EXTENSION(Tpkhoe) (0x1%d78,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP_BASELINE_HOLD_OFF_EXTENSION(Tblhoe)
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x807C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("DPP_BASELINE_HOLD_OFF_EXTENSION(Tblhoe) (0x807C,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x107C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP_BASELINE_HOLD_OFF_EXTENSION(Tblhoe) (0x1%d7C,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP_MISC
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8080, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("DPP_MISC (0x8080,rw) (all channels): %s\n",bstr);
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1080 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP_MISC (0x1%d80,rw) (channel %d): %s\n",ch,ch,bstr);
    }

  // if(ret)
  //   {
  //     printf("Error:  Read_DGTZ_Register !!!\n");
  //   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Read_DGTZ_Register_725_730_DPP_PSD_Revision03(int handle,int MaxNChInBoard)
{
  int ret = 0;
  uint32_t Register_Data;
  char bstr[64];
  bzero(bstr, sizeof(bstr));

  printf("\n\n==========================================================\n\n");

  // Record Length
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1020 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Record Length (0x1%d20) (channel %d): %s\n",ch,ch,bstr);
    }

  // Input Dynamic Range
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1028 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Input Dynamic Range (0x1%d28) (channel %d): %s\n",ch,ch,bstr);
    }

  // Number of Events per Aggregate
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1034 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Number of Events per Aggregate (0x1%d34) (channel %d): %s\n",ch,ch,bstr);
    }

  // Pre Trigger
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1038 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Pre Trigger (0x1%d38) (channel %d): %s\n",ch,ch,bstr);
    }

  // CFD settings
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x103C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("CFD settings (0x1%d3C) (channel %d): %s\n",ch,ch,bstr);
    }

  // Charge Zero Suppression Threshold
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1044 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Charge Zero Suppression Threshold (0x1%d44) (channel %d): %s\n",ch,ch,bstr);
    }

  // Short Gate Width
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1054 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Short Gate Width (0x1%d54) (channel %d): %s\n",ch,ch,bstr);
    }

  // Long Gate Width
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1058 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Long Gate Width (0x1%d58) (channel %d): %s\n",ch,ch,bstr);
    }

  // Gate Offset
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x105C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Gate Offset (0x1%d5C) (channel %d): %s\n",ch,ch,bstr);
    }

  // Trigger Threshold
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1060 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Trigger Threshold (0x1%d60) (channel %d): %s\n",ch,ch,bstr);
    }

  // Fixed Baseline
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1064 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Fixed Baseline (0x1%d64) (channel %d): %s\n",ch,ch,bstr);
    }

  // Shaped Trigger Width
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1070 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Shaped Trigger Width (0x1%d70) (channel %d): %s\n",ch,ch,bstr);
    }  
    
  // Trigger Hold-Off Width
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1074 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Trigger Hold-Off Width (0x1%d74) (channel %d): %s\n",ch,ch,bstr);
    }  
      
  // Threshold for the PSD cut
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1078 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Threshold for the PSD cut (0x1%d78) (channel %d): %s\n",ch,ch,bstr);
    }    

  // PUR-GAP Threshold
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x107C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("PUR-GAP Threshold (0x1%d7C) (channel %d): %s\n",ch,ch,bstr);
    }    

  // DPP Algorithm Control
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1080 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP Algorithm Control (0x1%d80) (channel %d): %s\n",ch,ch,bstr);
    }

  // DPP Algorithm Control 2
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1084 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP Algorithm Control 2 (0x1%d84) (channel %d): %s\n",ch,ch,bstr);
    }      
  
  // Channel n Status
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1088 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Channel n Status (0x1%d88) (channel %d): %s\n",ch,ch,bstr);
    }    

  // AMC Firmware Revision
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x108C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("AMC Firmware Revision (0x1%d8C) (channel %d): %s\n",ch,ch,bstr);
    }

  // DC Offset
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1098 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DC Offset (0x1%d98) (channel %d): %s\n",ch,ch,bstr);
    }     

  // Channel n ADC Temperature
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x10A8 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Channel n ADC Temperature (0x1%dA8) (channel %d): %s\n",ch,ch,bstr);
    }    

  // Veto Width
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x10D4 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Veto Width (0x1%dD4) (channel %d): %s\n",ch,ch,bstr);
    }

  // Baseline Freeze Time
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x10D8 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Baseline Freeze Time (0x1%dD8) (channel %d): %s\n",ch,ch,bstr);
    }

  // Board Configuration
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8000, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board Configuration (0x8000): %s\n",bstr);


  // Aggregate Organization
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x800C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Aggregate Organization (0x800C): %s\n",bstr);

  // Acquisition Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8100, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Acquisition Control (0x8100): %s\n",bstr);  

  // Acquisition Status
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8104, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Acquisition Status (0x8104): %s\n",bstr);

  // Global Trigger Mask
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x810C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Global Trigger Mask (0x810C): %s\n",bstr);

  // Front Panel TRG-OUT (GPO) Enable Mask
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8110, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Front Panel TRG-OUT (GPO) Enable Mask (0x8110): %s\n",bstr);

  // LVDS I/O Data
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8118, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("LVDS I/O Data (0x8118): %s\n",bstr);  

  // Front Panel I/O Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x811C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Front Panel I/O Control (0x811C): %s\n",bstr);

  // Channel Enable Mask
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8120, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Channel Enable Mask (0x8120): %s\n",bstr);  

  // ROC FPGA Firmware Revision
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8124, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("ROC FPGA Firmware Revision (0x8124): %s\n",bstr);

  // Set Monitor DAC
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8138, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Set Monitor DAC (0x8138): %s\n",bstr);  

  // Board Info
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8140, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board Info (0x8140): %s\n",bstr);

  // Monitor DAC Mode
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8144, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Monitor DAC Mode (0x8144): %s\n",bstr);

  // Event Size
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x814C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Event Size (0x814C): %s\n",bstr);

  // Time Bomb Downcounter
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8158, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Time Bomb Downcounter (0x8158): %s\n",bstr); 

  // Fan Speed Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8168, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Fan Speed Control (0x8168): %s\n",bstr);

  // Run/Start/Stop Delay
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8170, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Run/Start/Stop Delay (0x8170): %s\n",bstr);

  // Board Failure Status
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8178, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board Failure Status (0x8178): %s\n",bstr);

  // Disable External Trigger
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x817C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Disable External Trigger (0x817C): %s\n",bstr);  

  // Trigger Validation Mask
  for (int ch = 0; ch < MaxNChInBoard/2; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, 0x8180+4*ch, &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Trigger Validation Mask (0x8180+(4n)) (couple index %d): %s\n",ch,bstr);
    }    
  
  // Front Panel LVDS I/O New Features
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x81A0, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Front Panel LVDS I/O New Features (0x81A0): %s\n",bstr);

  // Readout Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF00, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Readout Control (0xEF00): %s\n",bstr);

  // Readout Status
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF04, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Readout Status (0xEF04): %s\n",bstr);

  // Board ID
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF08, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board ID (0xEF08): %s\n",bstr);

  // MCST Base Address and Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF0C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("MCST Base Address and Control (0xEF0C): %s\n",bstr);

  // Relocation Address
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF10, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Relocation Address (0xEF10): %s\n",bstr); 

  // Interrupt Status/ID
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF14, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Interrupt Status/ID (0xEF14): %s\n",bstr); 
  
  // Interrupt Event Number
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF18, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Interrupt Event Number (0xEF18): %s\n",bstr); 
  
  // Aggregate Number per BLT
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF1C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Aggregate Number per BLT (0xEF1C): %s\n",bstr); 
    
  // Scratch
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF20, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Scratch (0xEF20): %s\n",bstr); 

  // Configuration ROM Checksum
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF000, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum (0xF000): %s\n",bstr); 
  
  // Configuration ROM Checksum Length BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF004, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum Length BYTE 2 (0xF004): %s\n",bstr); 

  // Configuration ROM Checksum Length BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF008, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum Length BYTE 1 (0xF008): %s\n",bstr); 

  // Configuration ROM Checksum Length BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF00C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum Length BYTE 0 (0xF00C): %s\n",bstr); 
  
  // Configuration ROM Constant BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF010, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Constant BYTE 2 (0xF010): %s\n",bstr); 

  // Configuration ROM Constant BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF014, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Constant BYTE 1 (0xF014): %s\n",bstr); 

  // Configuration ROM Constant BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF018, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Constant BYTE 0 (0xF018): %s\n",bstr); 
  
  // Configuration ROM R Code
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF020, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM R Code (0xF020): %s\n",bstr); 

  // Configuration ROM IEEE OUI BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF024, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM IEEE OUI BYTE 2 (0xF024): %s\n",bstr); 

  // Configuration ROM IEEE OUI BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF028, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM IEEE OUI BYTE 1 (0xF028): %s\n",bstr); 

  // Configuration ROM IEEE OUI BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF02C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM IEEE OUI BYTE 0 (0xF02C): %s\n",bstr); 
  
  // Configuration ROM Board Version
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF030, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Version (0xF030): %s\n",bstr); 

  // Configuration ROM Board Form Factor
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF034, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Form Factor (0xF034): %s\n",bstr); 

  // Configuration ROM Board ID BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF038, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board ID BYTE 1 (0xF038): %s\n",bstr); 

  // Configuration ROM Board ID BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF03C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board ID BYTE 0 (0xF03C): %s\n",bstr);   
  
  // Configuration ROM PCB Revision BYTE 3
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF040, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 3 (0xF040): %s\n",bstr); 

  // Configuration ROM PCB Revision BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF044, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 2 (0xF044): %s\n",bstr); 

  // Configuration ROM PCB Revision BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF048, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 1 (0xF048): %s\n",bstr); 

  // Configuration ROM PCB Revision BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF04C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 0 (0xF04C): %s\n",bstr);

  // Configuration ROM FLASH Type
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF050, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM FLASH Type (0xF050): %s\n",bstr);

  // Configuration ROM Board Serial Number BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF080, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Serial Number BYTE 1 (0xF080): %s\n",bstr);

  // Configuration ROM Board Serial Number BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF084, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Serial Number BYTE 0 (0xF084): %s\n",bstr);

  // Configuration ROM VCXO Type
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF088, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM VCXO Type (0xF088): %s\n",bstr);
  

  
  if(ret)
    {
      printf("Error:  Read_DGTZ_Register_725_730_DPP_PSD_Revision03 !!!\n");
    }  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// return  0=success; -1=error
void Read_DGTZ_Register_725_730_STD_Revision00(int handle,int MaxNChInBoard)
{
  int ret = 0;
  uint32_t Register_Data;
  char bstr[64];
  bzero(bstr, sizeof(bstr));

  printf("\n\n==========================================================\n\n");

  // Dummy32
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1024 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Dummy32 (0x1%d24) (channel %d): %s\n",ch,ch,bstr);
    }

  // Channel n Input Dynamic Range
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1028 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Channel n Input Dynamic Range (0x1%d28) (channel %d): %s\n",ch,ch,bstr);
    }

  // Channel n Pulse Width
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1070 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf(" (0x1%d70) (channel %d): %s\n",ch,ch,bstr);
    }

  // Channel n Trigger Threshold
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1080 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Channel n Trigger Threshold (0x1%d80) (channel %d): %s\n",ch,ch,bstr);
    }
  
  // Couple n Self-Trigger Logic
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1084 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Couple n Self-Trigger Logic (0x1%d84) (channel %d): %s\n",ch,ch,bstr);
    }

  // Channel n Status
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1088 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Channel n Status (0x1%d88) (channel %d): %s\n",ch,ch,bstr);
    }

  // AMC Firmware Revision
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x108C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("AMC Firmware Revision (0x1%d8C) (channel %d): %s\n",ch,ch,bstr);
    }

  // Channel n DC Offset
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1098 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Channel n DC Offset (0x1%d98) (channel %d): %s\n",ch,ch,bstr);
    }  

  // Channel n ADC Temperature
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x10A8 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Channel n ADC Temperature (0x1%dA8) (channel %d): %s\n",ch,ch,bstr);
    }

  // Board Configuration
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8000, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board Configuration (0x8000): %s\n",bstr);

  // Buffer Organization
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x800C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Buffer Organization (0x800C): %s\n",bstr);

  // Custom Size
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8020, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Custom Size (0x8020): %s\n",bstr);

  // Channel ADC Calibration
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x809C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Channel ADC Calibration (0x809C): %s\n",bstr);

  // Acquisition Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8100, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Acquisition Control (0x8100): %s\n",bstr);

  // Acquisition Status
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8104, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Acquisition Status (0x8104): %s\n",bstr);
  
  // Software Trigger
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8108, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Software Trigger (0x8108): %s\n",bstr);  

  // Global Trigger Mask
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x810C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Global Trigger Mask (0x810C): %s\n",bstr);

  // Front Panel TRG-OUT (GPO) Enable Mask
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8110, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Front Panel TRG-OUT (GPO) Enable Mask (0x8110): %s\n",bstr);  
  
  // Post Trigger
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8114, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Post Trigger (0x8114): %s\n",bstr);

  // LVDS I/O Data
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8118, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("LVDS I/O Data (0x8118): %s\n",bstr);  

  // Front Panel I/O Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x811C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Front Panel I/O Control (0x811C): %s\n",bstr);

  // Channel Enable Mask
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8120, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Channel Enable Mask (0x8120): %s\n",bstr);  

  // ROC FPGA Firmware Revision
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8124, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("ROC FPGA Firmware Revision (0x8124): %s\n",bstr);
  
  // Event Stored
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x812C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Event Stored (0x812C): %s\n",bstr);

  // Set Monitor DAC
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8138, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Set Monitor DAC (0x8138): %s\n",bstr);  

  // Software Clock Sync
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x813C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Software Clock Sync (0x813C): %s\n",bstr);

  // Board Info
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8140, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board Info (0x8140): %s\n",bstr);

  // Monitor DAC Mode
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8144, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Monitor DAC Mode (0x8144): %s\n",bstr);

  // Event Size
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x814C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Event Size (0x814C): %s\n",bstr);

  // Fan Speed Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8168, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Fan Speed Control (0x8168): %s\n",bstr);  
  
  // Memory Buffer Almost Full Level
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x816C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Memory Buffer Almost Full Level (0x816C): %s\n",bstr);  

  // Run/Start/Stop Delay
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8170, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Run/Start/Stop Delay (0x8170): %s\n",bstr);

  // Board Failure Status
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8178, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board Failure Status (0x8178): %s\n",bstr);
  
  // Front Panel LVDS I/O New Features
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x81A0, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Front Panel LVDS I/O New Features (0x81A0): %s\n",bstr);  

  // Channels Shutdown
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x81C0, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Channels Shutdown (0x81C0): %s\n",bstr);  

  // Readout Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF00, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Readout Control (0xEF00): %s\n",bstr);

  // Readout Status
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF04, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Readout Status (0xEF04): %s\n",bstr);

  // Board ID
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF08, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board ID (0xEF08): %s\n",bstr);

  // MCST Base Address and Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF0C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("MCST Base Address and Control (0xEF0C): %s\n",bstr);

  // Relocation Address
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF10, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Relocation Address (0xEF10): %s\n",bstr); 

  // Interrupt Status/ID
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF14, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Interrupt Status/ID (0xEF14): %s\n",bstr);

  // Interrupt Event Number
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF18, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Interrupt Event Number (0xEF18): %s\n",bstr); 
  
  // Aggregate Number per BLT
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF1C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Aggregate Number per BLT (0xEF1C): %s\n",bstr); 
    
  // Scratch
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF20, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Scratch (0xEF20): %s\n",bstr);   

  // Software Reset
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF24, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Software Reset (0xEF24): %s\n",bstr);

  // Software Clear
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF28, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Software Clear (0xEF28): %s\n",bstr);  

  // Configuration Reload
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF34, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration Reload (0xEF34): %s\n",bstr);  

  // Configuration ROM Checksum
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF000, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum (0xF000): %s\n",bstr); 
  
  // Configuration ROM Checksum Length BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF004, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum Length BYTE 2 (0xF004): %s\n",bstr); 

  // Configuration ROM Checksum Length BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF008, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum Length BYTE 1 (0xF008): %s\n",bstr); 

  // Configuration ROM Checksum Length BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF00C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum Length BYTE 0 (0xF00C): %s\n",bstr); 
  
  // Configuration ROM Constant BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF010, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Constant BYTE 2 (0xF010): %s\n",bstr); 

  // Configuration ROM Constant BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF014, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Constant BYTE 1 (0xF014): %s\n",bstr); 

  // Configuration ROM Constant BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF018, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Constant BYTE 0 (0xF018): %s\n",bstr); 
  
  // Configuration ROM C Code
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF01C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM C Code (0xF01C): %s\n",bstr);  

  // Configuration ROM R Code
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF020, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM R Code (0xF020): %s\n",bstr); 

  // Configuration ROM IEEE OUI BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF024, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM IEEE OUI BYTE 2 (0xF024): %s\n",bstr); 

  // Configuration ROM IEEE OUI BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF028, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM IEEE OUI BYTE 1 (0xF028): %s\n",bstr); 

  // Configuration ROM IEEE OUI BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF02C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM IEEE OUI BYTE 0 (0xF02C): %s\n",bstr); 
  
  // Configuration ROM Board Version
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF030, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Version (0xF030): %s\n",bstr); 

  // Configuration ROM Board Form Factor
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF034, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Form Factor (0xF034): %s\n",bstr); 

  // Configuration ROM Board ID BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF038, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board ID BYTE 1 (0xF038): %s\n",bstr); 

  // Configuration ROM Board ID BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF03C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board ID BYTE 0 (0xF03C): %s\n",bstr);

  // Configuration ROM PCB Revision BYTE 3
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF040, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 3 (0xF040): %s\n",bstr); 

  // Configuration ROM PCB Revision BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF044, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 2 (0xF044): %s\n",bstr); 

  // Configuration ROM PCB Revision BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF048, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 1 (0xF048): %s\n",bstr); 

  // Configuration ROM PCB Revision BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF04C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 0 (0xF04C): %s\n",bstr);

  // Configuration ROM FLASH Type
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF050, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM FLASH Type (0xF050): %s\n",bstr);

  // Configuration ROM Board Serial Number BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF080, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Serial Number BYTE 1 (0xF080): %s\n",bstr);

  // Configuration ROM Board Serial Number BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF084, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Serial Number BYTE 0 (0xF084): %s\n",bstr);

  // Configuration ROM VCXO Type
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF088, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM VCXO Type (0xF088): %s\n",bstr);
  
  if(ret)
    {
      printf("Error:  Read_DGTZ_Register_725_730_STD_Revision00 !!!\n");
    }   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Read_DGTZ_Register_725_730_DPP_PHA_Revision00(int handle,int MaxNChInBoard)
{
  int ret = 0;
  uint32_t Register_Data;
  char bstr[64];
  bzero(bstr, sizeof(bstr));

  printf("\n\n==========================================================\n\n");

  // Record Length
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1020 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Record Length (0x1%d20) (channel %d): %s\n",ch,ch,bstr);
    }

  // Input Dynamic Range
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1028 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Input Dynamic Range (0x1%d28) (channel %d): %s\n",ch,ch,bstr);
    }

  // Number of Events per Aggregate
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1034 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Number of Events per Aggregate (0x1%d34) (channel %d): %s\n",ch,ch,bstr);
    }

  // Pre Trigger
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1038 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Pre Trigger (0x1%d38) (channel %d): %s\n",ch,ch,bstr);
    }

  // Data Flush
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x103C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Data Flush (0x1%d3C) (channel %d): %s\n",ch,ch,bstr);
    }
  
  // Channel n Stop Acquisition
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1040 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Channel n Stop Acquisition (0x1%d40) (channel %d): %s\n",ch,ch,bstr);
    }

  // RC-CR2 Smoothing Factor
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1054 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("RC-CR2 Smoothing Factor (0x1%d54) (channel %d): %s\n",ch,ch,bstr);
    }
  
  // Input Rise Time
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1058 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Input Rise Time (0x1%d58) (channel %d): %s\n",ch,ch,bstr);
    }

  // Trapezoid Rise Time
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x105C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Trapezoid Rise Time (0x1%d5C) (channel %d): %s\n",ch,ch,bstr);
    }

  // Trapezoid Flat Top
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1060 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Trapezoid Flat Top (0x1%d60) (channel %d): %s\n",ch,ch,bstr);
    }

  // Peaking Time
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1064 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Peaking Time (0x1%d64) (channel %d): %s\n",ch,ch,bstr);
    }

  // Decay Time
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1068 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Decay Time (0x1%d68) (channel %d): %s\n",ch,ch,bstr);
    }
  
  // Trigger Threshold
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x106C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Trigger Threshold (0x1%d6C) (channel %d): %s\n",ch,ch,bstr);
    }

  // Rise Time Validation Window
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1070 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Rise Time Validation Window (0x1%d70) (channel %d): %s\n",ch,ch,bstr);
    } 

  // Trigger Hold-Off
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1074 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Trigger Hold-Off (0x1%d74) (channel %d): %s\n",ch,ch,bstr);
    }  

  // Peak Hold-Off
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1078 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Peak Hold-Off (0x1%d78) (channel %d): %s\n",ch,ch,bstr);
    } 
  
  // Baseline Hold-Off
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x107C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Baseline Hold-Off (0x1%d7C) (channel %d): %s\n",ch,ch,bstr);
    }    

  // DPP Algorithm Control
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1080 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP Algorithm Control (0x1%d80) (channel %d): %s\n",ch,ch,bstr);
    }

  // Shaped Trigger Width
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1084 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Shaped Trigger Width (0x1%d84) (channel %d): %s\n",ch,ch,bstr);
    } 

  // Channel n Status
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1088 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Channel n Status (0x1%d88) (channel %d): %s\n",ch,ch,bstr);
    }    

  // AMC Firmware Revision
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x108C | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("AMC Firmware Revision (0x1%d8C) (channel %d): %s\n",ch,ch,bstr);
    }

  // DC Offset
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x1098 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DC Offset (0x1%d98) (channel %d): %s\n",ch,ch,bstr);
    }     

  // DPP Algorithm Control 2
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x10A0 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("DPP Algorithm Control 2 (0x1%dA0) (channel %d): %s\n",ch,ch,bstr);
    }     

  // Channel n ADC Temperature
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x10A8 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Channel n ADC Temperature (0x1%dA8) (channel %d): %s\n",ch,ch,bstr);
    } 

  // Individual Software Trigger
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x10C0 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Individual Software Trigger (0x1%dC0) (channel %d): %s\n",ch,ch,bstr);
    } 

  // Fine Gain
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x10C4 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Fine Gain (0x1%dC4) (channel %d): %s\n",ch,ch,bstr);
    } 

  // Veto Width
  for (int ch = 0; ch < MaxNChInBoard; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, (0x10D4 | (ch<<8)), &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Veto Width (0x1%dD4) (channel %d): %s\n",ch,ch,bstr);
    }

  // Board Configuration
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8000, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board Configuration (0x8000): %s\n",bstr);

  // Aggregate Organization
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x800C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Aggregate Organization (0x800C): %s\n",bstr);

  // Channel ADC Calibration
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x809C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Channel ADC Calibration (0x809C): %s\n",bstr);
  
  // Channels Shutdown
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x80BC, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Channels Shutdown (0x80BC): %s\n",bstr);

  // Acquisition Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8100, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Acquisition Control (0x8100): %s\n",bstr);  
  
  // Acquisition Status
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8104, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Acquisition Status (0x8104): %s\n",bstr);

  // Software Trigger
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8108, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Software Trigger (0x8108): %s\n",bstr);

  // Global Trigger Mask
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x810C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Global Trigger Mask (0x810C): %s\n",bstr);

  // Front Panel TRG-OUT (GPO) Enable Mask
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8110, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Front Panel TRG-OUT (GPO) Enable Mask (0x8110): %s\n",bstr);

  // LVDS I/O Data
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8118, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("LVDS I/O Data (0x8118): %s\n",bstr);

  // Front Panel I/O Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x811C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Front Panel I/O Control (0x811C): %s\n",bstr);

  // Channel Enable Mask
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8120, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Channel Enable Mask (0x8120): %s\n",bstr);  

  // ROC FPGA Firmware Revision
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8124, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("ROC FPGA Firmware Revision (0x8124): %s\n",bstr);

  // Set Monitor DAC
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8138, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Set Monitor DAC (0x8138): %s\n",bstr);  

  // Software Clock Sync
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x813C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Software Clock Sync (0x813C): %s\n",bstr);  

  // Board Info
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8140, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board Info (0x8140): %s\n",bstr);

  // Monitor DAC Mode
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8144, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Monitor DAC Mode (0x8144): %s\n",bstr);

  // Event Size
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x814C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Event Size (0x814C): %s\n",bstr);

  // Time Bomb Downcounter
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8158, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Time Bomb Downcounter (0x8158): %s\n",bstr); 

  // Fan Speed Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8168, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Fan Speed Control (0x8168): %s\n",bstr);

  // Run/Start/Stop Delay
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8170, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Run/Start/Stop Delay (0x8170): %s\n",bstr);

  // Board Failure Status
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x8178, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board Failure Status (0x8178): %s\n",bstr);

  // Disable External Trigger
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x817C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Disable External Trigger (0x817C): %s\n",bstr);    
  
  // Trigger Validation Mask
  for (int ch = 0; ch < MaxNChInBoard/2; ++ch)
    {
      ret |= CAEN_DGTZ_ReadRegister(handle, 0x8180+4*ch, &Register_Data);
      Decimal2Binary(Register_Data,bstr);
      printf("Trigger Validation Mask (0x8180+(4n)) (couple index %d): %s\n",ch,bstr);
    }    
  
  // Front Panel LVDS I/O New Features
  ret |= CAEN_DGTZ_ReadRegister(handle, 0x81A0, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Front Panel LVDS I/O New Features (0x81A0): %s\n",bstr);

  // Readout Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF00, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Readout Control (0xEF00): %s\n",bstr);

  // Readout Status
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF04, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Readout Status (0xEF04): %s\n",bstr);

  // Board ID
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF08, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Board ID (0xEF08): %s\n",bstr);

  // MCST Base Address and Control
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF0C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("MCST Base Address and Control (0xEF0C): %s\n",bstr);

  // Relocation Address
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF10, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Relocation Address (0xEF10): %s\n",bstr); 

  // Interrupt Status/ID
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF14, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Interrupt Status/ID (0xEF14): %s\n",bstr);

  // Interrupt Event Number
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF18, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Interrupt Event Number (0xEF18): %s\n",bstr); 

  // Aggregate Number per BLT
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF1C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Aggregate Number per BLT (0xEF1C): %s\n",bstr); 
    
  // Scratch
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF20, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Scratch (0xEF20): %s\n",bstr);

  // Software Reset
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF24, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Software Reset (0xEF24): %s\n",bstr);

  // Software Clear
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF28, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Software Clear (0xEF28): %s\n",bstr);

  // Configuration Reload
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xEF34, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration Reload (0xEF34): %s\n",bstr);

  // Configuration ROM Checksum
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF000, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum (0xF000): %s\n",bstr); 
  
  // Configuration ROM Checksum Length BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF004, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum Length BYTE 2 (0xF004): %s\n",bstr); 

  // Configuration ROM Checksum Length BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF008, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum Length BYTE 1 (0xF008): %s\n",bstr); 

  // Configuration ROM Checksum Length BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF00C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Checksum Length BYTE 0 (0xF00C): %s\n",bstr); 
  
  // Configuration ROM Constant BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF010, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Constant BYTE 2 (0xF010): %s\n",bstr); 

  // Configuration ROM Constant BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF014, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Constant BYTE 1 (0xF014): %s\n",bstr); 

  // Configuration ROM Constant BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF018, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Constant BYTE 0 (0xF018): %s\n",bstr); 
  
  // Configuration ROM C Code
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF01C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM C Code (0xF01C): %s\n",bstr); 

  // Configuration ROM R Code
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF020, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM R Code (0xF020): %s\n",bstr); 

  // Configuration ROM IEEE OUI BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF024, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM IEEE OUI BYTE 2 (0xF024): %s\n",bstr); 

  // Configuration ROM IEEE OUI BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF028, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM IEEE OUI BYTE 1 (0xF028): %s\n",bstr); 

  // Configuration ROM IEEE OUI BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF02C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM IEEE OUI BYTE 0 (0xF02C): %s\n",bstr); 
  
  // Configuration ROM Board Version
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF030, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Version (0xF030): %s\n",bstr); 

  // Configuration ROM Board Form Factor
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF034, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Form Factor (0xF034): %s\n",bstr);  
  
  // Configuration ROM Board ID BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF038, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board ID BYTE 1 (0xF038): %s\n",bstr); 

  // Configuration ROM Board ID BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF03C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board ID BYTE 0 (0xF03C): %s\n",bstr);   
  
  // Configuration ROM PCB Revision BYTE 3
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF040, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 3 (0xF040): %s\n",bstr); 

  // Configuration ROM PCB Revision BYTE 2
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF044, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 2 (0xF044): %s\n",bstr); 

  // Configuration ROM PCB Revision BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF048, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 1 (0xF048): %s\n",bstr); 

  // Configuration ROM PCB Revision BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF04C, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM PCB Revision BYTE 0 (0xF04C): %s\n",bstr);

  // Configuration ROM FLASH Type
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF050, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM FLASH Type (0xF050): %s\n",bstr);

  // Configuration ROM Board Serial Number BYTE 1
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF080, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Serial Number BYTE 1 (0xF080): %s\n",bstr);

  // Configuration ROM Board Serial Number BYTE 0
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF084, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM Board Serial Number BYTE 0 (0xF084): %s\n",bstr);

  // Configuration ROM VCXO Type
  ret |= CAEN_DGTZ_ReadRegister(handle, 0xF088, &Register_Data);
  Decimal2Binary(Register_Data,bstr);
  printf("Configuration ROM VCXO Type (0xF088): %s\n",bstr);
  
  if(ret)
    {
      printf("Error:  Read_DGTZ_Register_725_730_DPP_PHA_Revision00 !!!\n");
    }   

}




// 
// PrintRegisters.cc ends here
