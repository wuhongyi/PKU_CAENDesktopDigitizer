// PrintRegisters.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 12月 12 10:47:24 2016 (+0800)
// Last-Updated: 一 12月 12 10:52:16 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
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


// 
// PrintRegisters.cc ends here
