// ParSetTable.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 11月 30 13:02:22 2016 (+0800)
// Last-Updated: 二 12月  6 19:22:09 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 187
// URL: http://wuhongyi.cn 

#include "ParSetTable.hh"

#include <cstdlib>
#include <cstdio>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ParSetTable::ParSetTable(const TGWindow *p,const TGWindow *main, Board *b)
  : TGTransientFrame(p,main)
{
  SetCleanup(kDeepCleanup);

  int ret = 0;
  board = b;
  channelmask = 0;
  ret = CAEN_DGTZ_GetChannelEnableMask(board->GetHandle(),&channelmask);
  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelEnableMask"<<std::endl;
  // std::cout<<"@@ ChMask: "<<channelmask<<std::endl;

  
  TGGroupFrame *basepargroup = new TGGroupFrame(this,"Base");
  this->AddFrame(basepargroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));


  BaseAllParRows = new TGHorizontalFrame(basepargroup);
  basepargroup->AddFrame(BaseAllParRows, new TGLayoutHints(kLHintsExpandX | kLHintsTop,0/*left*/,0/*right*/,0/*top*/,20/*bottom*/));
  
  sprintf(tmp,"Ch All: ");
  allchannellabel = new TGLabel(BaseAllParRows,tmp); 
  BaseAllParRows->AddFrame(allchannellabel,new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));
  allchannellabel->SetTextColor(0xFF0000);

  BaseParSTDRaws = new TGHorizontalFrame(BaseAllParRows);
  BaseAllParRows->AddFrame(BaseParSTDRaws, new TGLayoutHints(kLHintsExpandX | kLHintsTop,0/*left*/,0/*right*/,0/*top*/,0/*bottom*/));

  sprintf(tmp,"PostTrigger[%%]:");
  posttriggersizelabel = new TGLabel(BaseParSTDRaws,tmp); 
  BaseParSTDRaws->AddFrame(posttriggersizelabel,new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));
  PostTriggerSize = new TGNumberEntry(BaseParSTDRaws,0,3,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,100);
  BaseParSTDRaws->AddFrame(PostTriggerSize,new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));
  

  // TGHorizontal3DLine *ln0 = new TGHorizontal3DLine(basepargroup, 10, 3);
  // basepargroup->AddFrame(ln0, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 5, 5));
  
  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      BaseParRows[i] = new TGHorizontalFrame(basepargroup);
      basepargroup->AddFrame(BaseParRows[i], new TGLayoutHints(kLHintsExpandX | kLHintsTop,0/*left*/,0/*right*/,0/*top*/,0/*bottom*/));

      sprintf(tmp,"Ch %02d: ",i);
      enabledchannellabel[i] = new TGLabel(BaseParRows[i],tmp); 
      BaseParRows[i]->AddFrame(enabledchannellabel[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));
      enabledchannellabel[i]->SetTextColor(0x00AAAA);


      sprintf(tmp,"Trigger:");
      selftriggerlabel[i] = new TGLabel(BaseParRows[i],tmp); 
      BaseParRows[i]->AddFrame(selftriggerlabel[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,0,3,0));
      SelfTriggerBox[i] = new TGComboBox(BaseParRows[i]);
      BaseParRows[i]->AddFrame(SelfTriggerBox[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,1,3,3,0));
      if(strstr(board->GetName(),(char*)"STD") != NULL)
	{
	  SelfTriggerBox[i]->AddEntry("DISABLED", 0);
	  SelfTriggerBox[i]->AddEntry("ACQ_ONLY", 1);
	  SelfTriggerBox[i]->AddEntry("EXTOUT_ONLY", 2);
	  SelfTriggerBox[i]->AddEntry("ACQ & EXTOUT", 3);
	}
      if(strstr(board->GetName(),(char*)"PSD") != NULL)
	{
	  SelfTriggerBox[i]->AddEntry("DISABLED", 0);
	  SelfTriggerBox[i]->AddEntry("ENABLED", 1);
	}
      
      SelfTriggerBox[i]->Resize(80, 20);
      SelfTriggerBox[i]->Select(0);

      sprintf(tmp,"Polarity:");
      pulsepolaritylabel[i] = new TGLabel(BaseParRows[i],tmp); 
      BaseParRows[i]->AddFrame(pulsepolaritylabel[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,0,3,0));  
      PulsePolarityBox[i] = new TGComboBox(BaseParRows[i]);
      BaseParRows[i]->AddFrame(PulsePolarityBox[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,1,3,3,0));
      PulsePolarityBox[i]->AddEntry("Positive", 0);
      PulsePolarityBox[i]->AddEntry("Negative", 1);
      PulsePolarityBox[i]->Resize(80, 20);
      PulsePolarityBox[i]->Select(0);
      
      sprintf(tmp,"DC Offset:");
      dcoffsetlabel[i] = new TGLabel(BaseParRows[i],tmp); 
      BaseParRows[i]->AddFrame(dcoffsetlabel[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));
      DCOffset[i] = new TGNumberEntry(BaseParRows[i],0,5,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,-50,50);//-50,50
      BaseParRows[i]->AddFrame(DCOffset[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));

      sprintf(tmp,"Threshold:");
      thresholdlabel[i] = new TGLabel(BaseParRows[i],tmp); 
      BaseParRows[i]->AddFrame(thresholdlabel[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));
      Threshold[i] = new TGNumberEntry(BaseParRows[i],0,5,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,(1<<board->GetNBits())-1);
      BaseParRows[i]->AddFrame(Threshold[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));


      
      BaseParPHAPSDRows[i] = new TGHorizontalFrame(BaseParRows[i]);
      BaseParRows[i]->AddFrame(BaseParPHAPSDRows[i], new TGLayoutHints(kLHintsExpandX | kLHintsTop,0/*left*/,0/*right*/,0/*top*/,0/*bottom*/));
      
      sprintf(tmp,"PreTrigger:");
      pretriggersizelabel[i] = new TGLabel(BaseParPHAPSDRows[i],tmp); 
      BaseParPHAPSDRows[i]->AddFrame(pretriggersizelabel[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));
      PreTriggerSize[i] = new TGNumberEntry(BaseParPHAPSDRows[i],0,5,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,10000);
      BaseParPHAPSDRows[i]->AddFrame(PreTriggerSize[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));

    }




  TGHorizontal3DLine *ln1 = new TGHorizontal3DLine(this, 50, 3);
  this->AddFrame(ln1, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));


  OperationRows = new TGHorizontalFrame(this);
  this->AddFrame(OperationRows,new TGLayoutHints(kLHintsLeft | kLHintsTop));

  TGLabel *copychannellabel = new TGLabel(OperationRows, "Select channel #");
  OperationRows->AddFrame(copychannellabel, new TGLayoutHints(kLHintsCenterX, 5, 2, 3, 0));
  CopyChannelSelect = new TGComboBox(OperationRows,-1);
  OperationRows->AddFrame(CopyChannelSelect, new TGLayoutHints(kLHintsCenterX, 0, 5, 3, 0));
  CopyChannelSelect->Resize(40,20);


  ParSetTableCopyButton = new TGTextButton(OperationRows,"&Copy",-1);
  OperationRows->AddFrame(ParSetTableCopyButton,new TGLayoutHints(kLHintsCenterX,5,30,3,0));
  // ParSetTableCopyButton->Associate(this);
  ParSetTableCopyButton->Connect("Clicked()", "ParSetTable", this, "CopyParameter()");
  
  ParSetTableLoadButton = new TGTextButton(OperationRows,"&Load",-1);
  OperationRows->AddFrame(ParSetTableLoadButton,new TGLayoutHints(kLHintsCenterX,50,3,3,0));
  // ParSetTableLoadButton->Associate(this);
  ParSetTableLoadButton->Connect("Clicked()", "ParSetTable", this, "LoadParameter()");
  
  ParSetTableApplyButton = new TGTextButton(OperationRows,"&Apply",-1);
  OperationRows->AddFrame(ParSetTableApplyButton,new TGLayoutHints(kLHintsCenterX,20,3,3,0));
  // ParSetTableApplyButton->Associate(this);
  // ParSetTableApplyButton->ChangeBackground(0x0000ff);
  ParSetTableApplyButton->Connect("Clicked()", "ParSetTable", this, "ApplyParameter()");
  
  ParSetTableCancelButton = new TGTextButton(OperationRows,"&Cancel",-1);
  OperationRows->AddFrame(ParSetTableCancelButton,new TGLayoutHints(kLHintsCenterX,20,3,3,0));
  // ParSetTableCancelButton->Associate(this);
  ParSetTableCancelButton->ChangeBackground(0x0000FF);
  ParSetTableCancelButton->Connect("Clicked()", "ParSetTable", this, "CancelParameterFrame()");
  ParSetTableCancelButton->SetToolTipText("Click to close this windows.");
  
  MapSubwindows();
  Resize();// resize to default size
  CenterOnParent();
  SetWindowName("Parameter Setting");
  MapWindow();


  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      if(strstr(board->GetName(),(char*)"PHA") == NULL &&  strstr(board->GetName(),(char*)"PSD") == NULL)
	{
	  BaseParRows[i]->HideFrame(BaseParPHAPSDRows[i]);
	  BaseAllParRows->ShowFrame(BaseParSTDRaws);
	}
      else
	{
	  // PHA PSD
	  BaseParRows[i]->ShowFrame(BaseParPHAPSDRows[i]);
	  BaseAllParRows->HideFrame(BaseParSTDRaws);
	}
      
      basepargroup->HideFrame(BaseParRows[i]);
      if(i < board->GetChannels() && TstBit_32(i,channelmask))
	{
	  basepargroup->ShowFrame(BaseParRows[i]);
	  // std::cout<<"@@ "<<i<<std::endl;
	  sprintf(tmp,"%02d",i);
	  CopyChannelSelect->AddEntry(tmp,i);
	}
    }


  LoadParameter();
}

ParSetTable::~ParSetTable()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ParSetTable::CopyParameter()
{
  // PulsePolarityBox[i]->GetSelected();
}

void ParSetTable::LoadParameter()
{
  CAEN_DGTZ_PulsePolarity_t loadpulsepolarity;// = CAEN_DGTZ_PulsePolarityPositive;
  CAEN_DGTZ_TriggerPolarity_t triggerpolarity;
  CAEN_DGTZ_TriggerMode_t selftrigger;
  unsigned int dcoffset = 0;
  unsigned int threshold = 0;
  unsigned int pretrigger = 0;
  unsigned int posttrigger = 0;
  int ret = 0;

  // PSD PHA Load 没用
  if(!(strstr(board->GetName(),(char*)"STD") != NULL)) return;
  
  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      if(i < board->GetChannels() && TstBit_32(i,channelmask))
	{
	  // SelfTrigger
	  // In case of Digital Pulse Processing (DPP) firmware, this function can also be used when it is required to manage the global trigger and/or propagate it out on TRG-OUT connector.
	  ret = CAEN_DGTZ_GetChannelSelfTrigger(board->GetHandle(),i,&selftrigger);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelSelfTrigger"<<std::endl;
	  if(selftrigger == CAEN_DGTZ_TRGMODE_DISABLED) SelfTriggerBox[i]->Select(0);
	  if(selftrigger == CAEN_DGTZ_TRGMODE_ACQ_ONLY) SelfTriggerBox[i]->Select(1);
	  if(selftrigger == CAEN_DGTZ_TRGMODE_EXTOUT_ONLY) SelfTriggerBox[i]->Select(2);
	  if(selftrigger == CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT) SelfTriggerBox[i]->Select(3);
	  
	  // DC Offset
	  ret = CAEN_DGTZ_GetChannelDCOffset(board->GetHandle(),i,&dcoffset);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelDCOffset"<<std::endl;
	  if(100.0*int(dcoffset)/0xFFFF-50 > 0)
	    DCOffset[i]->SetIntNumber(100.0*int(dcoffset)/0xFFFF-50+0.5);
	  else
	    DCOffset[i]->SetIntNumber(100.0*int(dcoffset)/0xFFFF-50);
	  // DCOffset[i]->SetIntNumber(int(dcoffset));

	  
	  // Channel Trigger Threshold
	  ret = CAEN_DGTZ_GetChannelTriggerThreshold(board->GetHandle(),i,&threshold);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelTriggerThreshold"<<std::endl;
	  Threshold[i]->SetIntNumber(int(threshold));

	  
	  if(strstr(board->GetName(),(char*)"STD") == NULL)
	    {
	      // 不存在
	      // DPP
	      
	      // Pulse Polarity
	      ret = CAEN_DGTZ_GetChannelPulsePolarity(board->GetHandle(),i,&loadpulsepolarity);
	      std::cout<<"load PS:"<<loadpulsepolarity<<std::endl;
	      if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelPulsePolarity"<<std::endl;
	      // PulsePolarityBox[i]->Select(int(pulsepolarity));
	      if(loadpulsepolarity == CAEN_DGTZ_PulsePolarityPositive)
		PulsePolarityBox[i]->Select(0);
	      else
		PulsePolarityBox[i]->Select(1);

	      
	      ret = CAEN_DGTZ_GetDPPPreTriggerSize(board->GetHandle(),i,&pretrigger);
	      if(ret) std::cout<<"Error: CAEN_DGTZ_GetDPPPreTriggerSize"<<std::endl;
	      PreTriggerSize[i]->SetIntNumber(int(pretrigger));
	    }
	  else
	    {
	      // 存在
	      // STD
	      ret = CAEN_DGTZ_GetTriggerPolarity(board->GetHandle(),i,&triggerpolarity);
	      std::cout<<"load PS:"<<triggerpolarity<<std::endl;
	      if(ret) std::cout<<"Error: CAEN_DGTZ_GetTriggerPolarity"<<std::endl;
	      // PulsePolarityBox[i]->Select(int(pulsepolarity));
	      if(triggerpolarity == CAEN_DGTZ_TriggerOnRisingEdge)
		PulsePolarityBox[i]->Select(0);
	      else
		PulsePolarityBox[i]->Select(1);

	      
	      ret = CAEN_DGTZ_GetPostTriggerSize(board->GetHandle(),&posttrigger);
	      if(ret) std::cout<<"Error: CAEN_DGTZ_GetPostTriggerSize"<<std::endl;
	      std::cout<<"load posttrigger:"<<posttrigger<<std::endl;
	      PostTriggerSize->SetIntNumber(int(posttrigger));
	      
	    }
	  
	}
    }
}

void ParSetTable::ApplyParameter()
{
  if(strstr(board->GetName(),(char*)"STD") != NULL)
    {
      ApplySTDParameter();
    }
  else
    {
      if(strstr(board->GetName(),(char*)"PSD") != NULL)
	{
	  ApplyPSDParameter();
	}
      else
	{
	  printf("Error:  ParSetTable::ApplyParameter\n");
	}
  
      }
  return;
  
  CAEN_DGTZ_PulsePolarity_t pulsepolarity;// = CAEN_DGTZ_PulsePolarityPositive;
  CAEN_DGTZ_TriggerPolarity_t triggerpolarity;
  CAEN_DGTZ_TriggerMode_t selftrigger;
  unsigned int dcoffset = 0;
  unsigned int threshold = 0;
  unsigned int pretrigger = 0;
  unsigned int posttrigger = 0;
  int ret = 0;

  unsigned int testdata = 0;

  ret = CAEN_DGTZ_ReadRegister(board->GetHandle(), 0x8000, &testdata);
  std::cout<<"apply RR1:"<<TstBit_32(6,testdata)<<std::endl;
  ret = CAEN_DGTZ_ReadRegister(board->GetHandle(), 0x810C, &testdata);
  std::cout<<"apply 810C:  [0]"<<TstBit_32(0,testdata)<<"  [1]"<<TstBit_32(1,testdata)<<"  [2]"<<TstBit_32(2,testdata)<<"  [3]"<<TstBit_32(3,testdata)<<std::endl;
  std::cout<<"apply 810C:  [20]"<<TstBit_32(20,testdata)<<"  [21]"<<TstBit_32(21,testdata)<<"  [22]"<<TstBit_32(22,testdata)<<"  [23]"<<TstBit_32(23,testdata)<<std::endl;
	  std::cout<<"apply 810C:  [24]"<<TstBit_32(24,testdata)<<"  [25]"<<TstBit_32(25,testdata)<<"  [26]"<<TstBit_32(26,testdata)<<std::endl;
  
  // testdata = SetBit_32(0,testdata);
  // testdata = ClrBit_32(1,testdata);
  // testdata = ClrBit_32(2,testdata);
  // ret = CAEN_DGTZ_WriteRegister(board->GetHandle(), 0x8084, testdata);
  
  // ret = CAEN_DGTZ_ReadRegister(board->GetHandle(), 0x810C, &testdata);
  // std::cout<<"GlobalTriggerMask:  [0]"<<TstBit_32(0,testdata)<<"  [1]"<<TstBit_32(1,testdata)<<"  [2]"<<TstBit_32(2,testdata)<<"  [3]"<<TstBit_32(3,testdata)<<std::endl;
  
  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      if(i < board->GetChannels() && TstBit_32(i,channelmask))
	{
	  ret = CAEN_DGTZ_ReadRegister(board->GetHandle(), 0x1084+(i<<8), &testdata);
	  std::cout<<"apply 1n84:  [0]"<<TstBit_32(0,testdata)<<"  [1]"<<TstBit_32(1,testdata)<<"  [2]"<<TstBit_32(2,testdata)<<std::endl;
	  
	  testdata = SetBit_32(0,testdata);
	  testdata = ClrBit_32(1,testdata);
	  testdata = ClrBit_32(2,testdata);
	  ret = CAEN_DGTZ_WriteRegister(board->GetHandle(), 0x1084+(i<<8), testdata);
	  ret = CAEN_DGTZ_ReadRegister(board->GetHandle(), 0x1084+(i<<8), &testdata);
	  std::cout<<"apply 1n84:  [0]"<<TstBit_32(0,testdata)<<"  [1]"<<TstBit_32(1,testdata)<<"  [2]"<<TstBit_32(2,testdata)<<std::endl;

	  
	  // testdata = SetBit_32(6,testdata);
	  // ret = CAEN_DGTZ_WriteRegister(board->GetHandle(), 0x8000, testdata);
	  // ret = CAEN_DGTZ_ReadRegister(board->GetHandle(), 0x8000, &testdata);
	  // std::cout<<"apply RR2:"<<TstBit_32(6,testdata)<<std::endl;
	  // std::cout<<"apply PS:"<<PulsePolarityBox[i]->GetSelected()<<std::endl;
	  
	  if (board->GetFamilyCode() != CAEN_DGTZ_XX730_FAMILY_CODE && board->GetFamilyCode() != CAEN_DGTZ_XX725_FAMILY_CODE)
	    {
	      ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_ACQ_ONLY, (1<<i));

	      if(SelfTriggerBox[i]->GetSelected() == 0)
		{
		  ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_DISABLED, (1<<i));
		  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
		}
	      if(SelfTriggerBox[i]->GetSelected() == 1)
		{
		  ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_ACQ_ONLY, (1<<i));
		  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
		}
	      if(SelfTriggerBox[i]->GetSelected() == 2)
		{
		  ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_EXTOUT_ONLY, (1<<i));
		  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
		}
	      if(SelfTriggerBox[i]->GetSelected() == 3)
		{
		  ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT, (1<<i));
		  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
		}     
	    }
	  else
	    {
	      // x730 x725
	      if(strstr(board->GetName(),(char*)"STD") == NULL)
		{
		  // 不存在
		  // PSD PHA

		  
		}
	      else
		{
		  // 存在
		  // STD
		  if(i%2 == 0)
		    {
		      if (channelmask & (0x3 << i))
			{
			  int mode = SelfTriggerBox[i]->GetSelected();
			  uint32_t pair_chmask = 0;

			  // Build mode and relevant channelmask. The behaviour is that,
			  // if the triggermode of one channel of the pair is DISABLED,
			  // this channel doesn't take part to the trigger generation.
			  // Otherwise, if both are different from DISABLED, the one of
			  // the even channel is used.
			  if (SelfTriggerBox[i]->GetSelected() != 0)
			    {
			      if (SelfTriggerBox[i+1]->GetSelected() == 0)
				pair_chmask = (0x1 << i);
			      else
				pair_chmask = (0x3 << i);
			    }
			  else
			    {
			      mode = SelfTriggerBox[i+1]->GetSelected();
			      pair_chmask = (0x2 << i);
			    }

			  pair_chmask &= channelmask;
			  if(mode == 0)
			    {
			      ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_DISABLED, pair_chmask);
			      if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
			    }
			  if(mode == 1)
			    {
			      ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_ACQ_ONLY, pair_chmask);
			      if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
			    }
			  if(mode == 2)
			    {
			      ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_EXTOUT_ONLY, pair_chmask);
			      if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
			    }
			  if(mode == 3)
			    {
			      ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT, pair_chmask);
			      if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
			    }

		      
			}
		    }

		}
	      
	    }

	  ret = CAEN_DGTZ_GetChannelSelfTrigger(board->GetHandle(),i,&selftrigger);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelSelfTrigger"<<std::endl;
	  if(selftrigger == CAEN_DGTZ_TRGMODE_DISABLED) SelfTriggerBox[i]->Select(0);
	  if(selftrigger == CAEN_DGTZ_TRGMODE_ACQ_ONLY) SelfTriggerBox[i]->Select(1);
	  if(selftrigger == CAEN_DGTZ_TRGMODE_EXTOUT_ONLY) SelfTriggerBox[i]->Select(2);
	  if(selftrigger == CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT) SelfTriggerBox[i]->Select(3);
	  
	  // DC Offset
	  ret = CAEN_DGTZ_SetChannelDCOffset(board->GetHandle(),i,(unsigned int)((DCOffset[i]->GetIntNumber()+50)*0xFFFF/100.0));
	  // ret = CAEN_DGTZ_SetChannelDCOffset(board->GetHandle(),i,(unsigned int)DCOffset[i]->GetIntNumber());
	  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelDCOffset"<<std::endl;
	  ret = CAEN_DGTZ_GetChannelDCOffset(board->GetHandle(),i,&dcoffset);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelDCOffset"<<std::endl;
	  if(100.0*int(dcoffset)/0xFFFF-50 > 0)
	    DCOffset[i]->SetIntNumber(100.0*int(dcoffset)/0xFFFF-50+0.5);
	  else
	    DCOffset[i]->SetIntNumber(100.0*int(dcoffset)/0xFFFF-50);
	  // DCOffset[i]->SetIntNumber(int(dcoffset));
					     
	  // Channel Trigger Threshold
	  ret = CAEN_DGTZ_SetChannelTriggerThreshold(board->GetHandle(),i,(unsigned int)Threshold[i]->GetIntNumber());
	  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelTriggerThreshold"<<std::endl;
	  // std::cout<<"Th: "<<(unsigned int)Threshold[i]->GetIntNumber()<<std::endl;
	  ret = CAEN_DGTZ_GetChannelTriggerThreshold(board->GetHandle(),i,&threshold);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelTriggerThreshold"<<std::endl;
	  Threshold[i]->SetIntNumber(int(threshold));
	  
	  if(strstr(board->GetName(),(char*)"STD") == NULL)
	    {
	      // 不存在
	      // DPP
	      if(PulsePolarityBox[i]->GetSelected() == 0)
		{
		  std::cout<<"apply   CAEN_DGTZ_PulsePolarityPositive"<<std::endl;
		  ret = CAEN_DGTZ_SetChannelPulsePolarity(board->GetHandle(),i,CAEN_DGTZ_PulsePolarityPositive);
		}
	      else
		{
		  std::cout<<"apply   CAEN_DGTZ_PulsePolarityNegative"<<std::endl;
		  ret = CAEN_DGTZ_SetChannelPulsePolarity(board->GetHandle(),i,CAEN_DGTZ_PulsePolarityNegative);
		}
	      if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelPulsePolarity"<<std::endl;
	      ret = CAEN_DGTZ_GetChannelPulsePolarity(board->GetHandle(),i,&pulsepolarity);
	      if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelPulsePolarity"<<std::endl;
	      if(pulsepolarity == CAEN_DGTZ_PulsePolarityPositive)
		PulsePolarityBox[i]->Select(0);
	      else
		PulsePolarityBox[i]->Select(1);
	      std::cout<<"apply PS:"<<pulsepolarity<<std::endl;

	      
	      ret = CAEN_DGTZ_SetDPPPreTriggerSize(board->GetHandle(),i,(unsigned int)PreTriggerSize[i]->GetIntNumber());
	      if(ret) std::cout<<"Error: CAEN_DGTZ_SetDPPPreTriggerSize"<<std::endl;
	      // std::cout<<"Pr: "<<(unsigned int)(unsigned int)PreTriggerSize[i]->GetIntNumber()<<std::endl;
	      ret = CAEN_DGTZ_GetDPPPreTriggerSize(board->GetHandle(),i,&pretrigger);
	      if(ret) std::cout<<"Error: CAEN_DGTZ_GetDPPPreTriggerSize"<<std::endl;
	      PreTriggerSize[i]->SetIntNumber(int(pretrigger));
	    }
	  else
	    {
	      // 存在
	      // STD
	      if(PulsePolarityBox[i]->GetSelected() == 0)
	        {
	          std::cout<<"apply   CAEN_DGTZ_TriggerOnRisingEdge"<<std::endl;
	          ret = CAEN_DGTZ_SetTriggerPolarity(board->GetHandle(),i,CAEN_DGTZ_TriggerOnRisingEdge);
	        }
	      else
	        {
	          std::cout<<"apply   CAEN_DGTZ_TriggerOnFallingEdge"<<std::endl;
	          ret = CAEN_DGTZ_SetTriggerPolarity(board->GetHandle(),i,CAEN_DGTZ_TriggerOnFallingEdge);
	        }
	      if(ret) std::cout<<"Error: CAEN_DGTZ_SetTriggerPolarity"<<std::endl;
	      ret = CAEN_DGTZ_GetTriggerPolarity(board->GetHandle(),i,&triggerpolarity);
	      if(ret) std::cout<<"Error: CAEN_DGTZ_SetTriggerPolarity"<<std::endl;
	      if(triggerpolarity == CAEN_DGTZ_TriggerOnRisingEdge)
	        PulsePolarityBox[i]->Select(0);
	      else
	        PulsePolarityBox[i]->Select(1);
	      std::cout<<"apply PS:"<<triggerpolarity<<std::endl;

	      
	      ret = CAEN_DGTZ_SetPostTriggerSize(board->GetHandle(),(unsigned int)PostTriggerSize->GetIntNumber());
	      if(ret) std::cout<<"Error: CAEN_DGTZ_SetPostTriggerSize"<<std::endl;
	      ret = CAEN_DGTZ_GetPostTriggerSize(board->GetHandle(),&posttrigger);
	      if(ret) std::cout<<"Error: CAEN_DGTZ_GetPostTriggerSize"<<std::endl;
	      PostTriggerSize->SetIntNumber(int(posttrigger));
	    }

	  
	}
    }
}

void ParSetTable::ApplySTDParameter()
{
  CAEN_DGTZ_TriggerPolarity_t triggerpolarity;
  CAEN_DGTZ_TriggerMode_t selftrigger;
  unsigned int dcoffset = 0;
  unsigned int threshold = 0;
  unsigned int posttrigger = 0;
  int ret = 0;
  
  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      if(i < board->GetChannels() && TstBit_32(i,channelmask))
	{
	  if (board->GetFamilyCode() != CAEN_DGTZ_XX730_FAMILY_CODE && board->GetFamilyCode() != CAEN_DGTZ_XX725_FAMILY_CODE)
	    {
	      ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_ACQ_ONLY, (1<<i));

	      if(SelfTriggerBox[i]->GetSelected() == 0)
		{
		  ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_DISABLED, (1<<i));
		  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
		}
	      if(SelfTriggerBox[i]->GetSelected() == 1)
		{
		  ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_ACQ_ONLY, (1<<i));
		  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
		}
	      if(SelfTriggerBox[i]->GetSelected() == 2)
		{
		  ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_EXTOUT_ONLY, (1<<i));
		  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
		}
	      if(SelfTriggerBox[i]->GetSelected() == 3)
		{
		  ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT, (1<<i));
		  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
		}     
	    }
	  else
	    {
	      // x730 x725
	      if(i%2 == 0)
		{
		  if (channelmask & (0x3 << i))
		    {
		      int mode = SelfTriggerBox[i]->GetSelected();
		      uint32_t pair_chmask = 0;

		      // Build mode and relevant channelmask. The behaviour is that,
		      // if the triggermode of one channel of the pair is DISABLED,
		      // this channel doesn't take part to the trigger generation.
		      // Otherwise, if both are different from DISABLED, the one of
		      // the even channel is used.
		      if (SelfTriggerBox[i]->GetSelected() != 0)
			{
			  if (SelfTriggerBox[i+1]->GetSelected() == 0)
			    pair_chmask = (0x1 << i);
			  else
			    pair_chmask = (0x3 << i);
			}
		      else
			{
			  mode = SelfTriggerBox[i+1]->GetSelected();
			  pair_chmask = (0x2 << i);
			}

		      pair_chmask &= channelmask;
		      if(mode == 0)
			{
			  ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_DISABLED, pair_chmask);
			  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
			}
		      if(mode == 1)
			{
			  ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_ACQ_ONLY, pair_chmask);
			  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
			}
		      if(mode == 2)
			{
			  ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_EXTOUT_ONLY, pair_chmask);
			  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
			}
		      if(mode == 3)
			{
			  ret = CAEN_DGTZ_SetChannelSelfTrigger(board->GetHandle(), CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT, pair_chmask);
			  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelSelfTrigger"<<std::endl;
			}

		    }
		}


	    }// x730 x725

	  ret = CAEN_DGTZ_GetChannelSelfTrigger(board->GetHandle(),i,&selftrigger);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelSelfTrigger"<<std::endl;
	  if(selftrigger == CAEN_DGTZ_TRGMODE_DISABLED) SelfTriggerBox[i]->Select(0);
	  if(selftrigger == CAEN_DGTZ_TRGMODE_ACQ_ONLY) SelfTriggerBox[i]->Select(1);
	  if(selftrigger == CAEN_DGTZ_TRGMODE_EXTOUT_ONLY) SelfTriggerBox[i]->Select(2);
	  if(selftrigger == CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT) SelfTriggerBox[i]->Select(3);
	  // DC Offset
	  ret = CAEN_DGTZ_SetChannelDCOffset(board->GetHandle(),i,(unsigned int)((DCOffset[i]->GetIntNumber()+50)*0xFFFF/100.0));
	  // ret = CAEN_DGTZ_SetChannelDCOffset(board->GetHandle(),i,(unsigned int)DCOffset[i]->GetIntNumber());
	  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelDCOffset"<<std::endl;
	  ret = CAEN_DGTZ_GetChannelDCOffset(board->GetHandle(),i,&dcoffset);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelDCOffset"<<std::endl;
	  if(100.0*int(dcoffset)/0xFFFF-50 > 0)
	    DCOffset[i]->SetIntNumber(100.0*int(dcoffset)/0xFFFF-50+0.5);
	  else
	    DCOffset[i]->SetIntNumber(100.0*int(dcoffset)/0xFFFF-50);
	  // DCOffset[i]->SetIntNumber(int(dcoffset));
					     
	  // Channel Trigger Threshold
	  ret = CAEN_DGTZ_SetChannelTriggerThreshold(board->GetHandle(),i,(unsigned int)Threshold[i]->GetIntNumber());
	  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelTriggerThreshold"<<std::endl;
	  // std::cout<<"Th: "<<(unsigned int)Threshold[i]->GetIntNumber()<<std::endl;
	  ret = CAEN_DGTZ_GetChannelTriggerThreshold(board->GetHandle(),i,&threshold);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelTriggerThreshold"<<std::endl;
	  Threshold[i]->SetIntNumber(int(threshold));


	  // TriggerPolarity
	  if(PulsePolarityBox[i]->GetSelected() == 0)
	    {
	      std::cout<<"apply   CAEN_DGTZ_TriggerOnRisingEdge"<<std::endl;
	      ret = CAEN_DGTZ_SetTriggerPolarity(board->GetHandle(),i,CAEN_DGTZ_TriggerOnRisingEdge);
	    }
	  else
	    {
	      std::cout<<"apply   CAEN_DGTZ_TriggerOnFallingEdge"<<std::endl;
	      ret = CAEN_DGTZ_SetTriggerPolarity(board->GetHandle(),i,CAEN_DGTZ_TriggerOnFallingEdge);
	    }
	  if(ret) std::cout<<"Error: CAEN_DGTZ_SetTriggerPolarity"<<std::endl;
	  ret = CAEN_DGTZ_GetTriggerPolarity(board->GetHandle(),i,&triggerpolarity);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_SetTriggerPolarity"<<std::endl;
	  if(triggerpolarity == CAEN_DGTZ_TriggerOnRisingEdge)
	    PulsePolarityBox[i]->Select(0);
	  else
	    PulsePolarityBox[i]->Select(1);
	  std::cout<<"apply PS:"<<triggerpolarity<<std::endl;

	      
	  ret = CAEN_DGTZ_SetPostTriggerSize(board->GetHandle(),(unsigned int)PostTriggerSize->GetIntNumber());
	  if(ret) std::cout<<"Error: CAEN_DGTZ_SetPostTriggerSize"<<std::endl;
	  ret = CAEN_DGTZ_GetPostTriggerSize(board->GetHandle(),&posttrigger);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetPostTriggerSize"<<std::endl;
	  PostTriggerSize->SetIntNumber(int(posttrigger));

	  
	}//channelmask

    }//MAX_CHANNEL

}

void ParSetTable::ApplyPSDParameter()
{
  CAEN_DGTZ_DPP_PSD_Params_t dpppsdParams;
  CAEN_DGTZ_PulsePolarity_t pulsepolarity;
  unsigned int dcoffset = 0;
  unsigned int pretrigger = 0;
  int ret = 0;

  for (int ch = 0; ch < board->GetChannels(); ++ch)
    {
      dpppsdParams.thr[ch] = 0;// Trigger Threshold
      dpppsdParams.nsbl[ch] = 2;    // used to specifiy the number of samples for the baseline averaging
      dpppsdParams.lgate[ch] = 32;    // Long Gate Width (N*4ns)
      dpppsdParams.sgate[ch] = 24;    // Short Gate Width (N*4ns)
      dpppsdParams.pgate[ch] = 8;     // Pre Gate Width (N*4ns)
      dpppsdParams.selft[ch] = 0;  // Self Trigger Mode: 0 -> Disabled 1 -> Enabled
      // Trigger configuration:
      // CAEN_DGTZ_DPP_TriggerConfig_Peak       -> trigger on peak. NOTE: Only for FW <= 13X.5
      // CAEN_DGTZ_DPP_TriggerConfig_Threshold  -> trigger on threshold 
      dpppsdParams.trgc[ch] = CAEN_DGTZ_DPP_TriggerConfig_Threshold;
      dpppsdParams.tvaw[ch] = 50;// Trigger Validation Acquisition Window
      dpppsdParams.csens[ch] = 0;// Charge sensibility: 0->40fc/LSB; 1->160fc/LSB; 2->640fc/LSB; 3->2,5pc/LSB
    }
  // Pile-Up rejection Mode
  //    CAEN_DGTZ_DPP_PSD_PUR_DetectOnly -> Only Detect Pile-Up
  //    CAEN_DGTZ_DPP_PSD_PUR_Enabled -> Reject Pile-Up
  // DPPParams[b].purh = CAEN_DGTZ_DPP_PSD_PUR_DetectOnly;
  dpppsdParams.purh = CAEN_DGTZ_DPP_PSD_PUR_DetectOnly; //NOTE: Ignored on 5730
  dpppsdParams.purgap = 100;  // Purity Gap   // Pile-up Rejection Gap  //NOTE: Ignored on 5730
  dpppsdParams.blthr = 3;     // Baseline Threshold  //This parameter is deprecated
  dpppsdParams.bltmo = 100;   // Baseline Timeout    //This parameter is deprecated
  dpppsdParams.trgho = 0;     // Trigger HoldOff


  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      if(i < board->GetChannels() && TstBit_32(i,channelmask))
	{
	  // Channel Trigger Threshold
	  dpppsdParams.thr[i] = Threshold[i]->GetIntNumber();
	  
	  // Trigger Enabled
	  dpppsdParams.selft[i] = SelfTriggerBox[i]->GetSelected();


	  // DC Offset
	  ret = CAEN_DGTZ_SetChannelDCOffset(board->GetHandle(),i,(unsigned int)((DCOffset[i]->GetIntNumber()+50)*0xFFFF/100.0));
	  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelDCOffset"<<std::endl;
	  ret = CAEN_DGTZ_GetChannelDCOffset(board->GetHandle(),i,&dcoffset);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelDCOffset"<<std::endl;
	  if(100.0*int(dcoffset)/0xFFFF-50 > 0)
	    DCOffset[i]->SetIntNumber(100.0*int(dcoffset)/0xFFFF-50+0.5);
	  else
	    DCOffset[i]->SetIntNumber(100.0*int(dcoffset)/0xFFFF-50);

	  // PulsePolarity
	  if(PulsePolarityBox[i]->GetSelected() == 0)
	    {
	      std::cout<<"apply   CAEN_DGTZ_PulsePolarityPositive"<<std::endl;
	      ret = CAEN_DGTZ_SetChannelPulsePolarity(board->GetHandle(),i,CAEN_DGTZ_PulsePolarityPositive);
	    }
	  else
	    {
	      std::cout<<"apply   CAEN_DGTZ_PulsePolarityNegative"<<std::endl;
	      ret = CAEN_DGTZ_SetChannelPulsePolarity(board->GetHandle(),i,CAEN_DGTZ_PulsePolarityNegative);
	    }
	  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelPulsePolarity"<<std::endl;
	  ret = CAEN_DGTZ_GetChannelPulsePolarity(board->GetHandle(),i,&pulsepolarity);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelPulsePolarity"<<std::endl;
	  if(pulsepolarity == CAEN_DGTZ_PulsePolarityPositive)
	    PulsePolarityBox[i]->Select(0);
	  else
	    PulsePolarityBox[i]->Select(1);

	  // PreTriggerSize
	  ret = CAEN_DGTZ_SetDPPPreTriggerSize(board->GetHandle(),i,(unsigned int)PreTriggerSize[i]->GetIntNumber());
	  if(ret) std::cout<<"Error: CAEN_DGTZ_SetDPPPreTriggerSize"<<std::endl;
	  ret = CAEN_DGTZ_GetDPPPreTriggerSize(board->GetHandle(),i,&pretrigger);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetDPPPreTriggerSize"<<std::endl;
	  PreTriggerSize[i]->SetIntNumber(int(pretrigger));

	  
	}//channelmask

    }//MAX_CHANNEL

  ret = CAEN_DGTZ_SetDPPParameters(board->GetHandle(), channelmask, &dpppsdParams);
  if(ret) std::cout<<"Error: CAEN_DGTZ_SetDPPParameters"<<std::endl;
  
}
  



void ParSetTable::CancelParameterFrame()
{
  DeleteWindow();
}



// 
// ParSetTable.cc ends here
