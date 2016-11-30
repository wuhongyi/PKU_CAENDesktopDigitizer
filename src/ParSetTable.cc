// ParSetTable.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 11月 30 13:02:22 2016 (+0800)
// Last-Updated: 三 11月 30 21:25:55 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 104
// URL: http://wuhongyi.cn 

#include "ParSetTable.hh"

#include <cstdlib>
#include <cstdio>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ParSetTable::ParSetTable(const TGWindow *p,const TGWindow *main, Board *b)
  : TGTransientFrame(p,main)
{
  SetCleanup(kDeepCleanup);
  
  board = b;
  channelmask = 0;
  CAEN_DGTZ_GetChannelEnableMask(board->GetHandle(),&channelmask);
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

      PulsePolarityBox[i] = new TGComboBox(BaseParRows[i]);
      BaseParRows[i]->AddFrame(PulsePolarityBox[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));
      PulsePolarityBox[i]->AddEntry("Positive", 0);
      PulsePolarityBox[i]->AddEntry("Negative", 1);
      PulsePolarityBox[i]->Resize(80, 20);

      sprintf(tmp,"DC Offset:");
      dcoffsetlabel[i] = new TGLabel(BaseParRows[i],tmp); 
      BaseParRows[i]->AddFrame(dcoffsetlabel[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));
      DCOffset[i] = new TGNumberEntry(BaseParRows[i],0,2,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,-50,50);
      BaseParRows[i]->AddFrame(DCOffset[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));

      sprintf(tmp,"Threshold:");
      thresholdlabel[i] = new TGLabel(BaseParRows[i],tmp); 
      BaseParRows[i]->AddFrame(thresholdlabel[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));
      Threshold[i] = new TGNumberEntry(BaseParRows[i],0,5,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,16383);
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
	  BaseAllParRows->HideFrame(BaseParSTDRaws);
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
  CAEN_DGTZ_PulsePolarity_t pulsepolarity;// = CAEN_DGTZ_PulsePolarityPositive;
  unsigned int dcoffset = 0;
  unsigned int threshold = 0;
  unsigned int pretrigger = 0;
  unsigned int posttrigger = 0;
  int ret = 0;
  
  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      if(i < board->GetChannels() && TstBit_32(i,channelmask))
	{
	  // Pulse Polarity
	  ret = CAEN_DGTZ_GetChannelPulsePolarity(board->GetHandle(),i,&pulsepolarity);
	  std::cout<<"load PS:"<<pulsepolarity<<std::endl;
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelPulsePolarity"<<std::endl;
	  // PulsePolarityBox[i]->Select(int(pulsepolarity));
	  if(pulsepolarity == CAEN_DGTZ_PulsePolarityPositive)
	    PulsePolarityBox[i]->Select(0);
	  else
	    PulsePolarityBox[i]->Select(1);

	  
	  // DC Offset
	  ret = CAEN_DGTZ_GetChannelDCOffset(board->GetHandle(),i,&dcoffset);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelDCOffset"<<std::endl;
	  if(100.0*int(dcoffset)/0xFFFF-50 > 0)
	    DCOffset[i]->SetIntNumber(100.0*int(dcoffset)/0xFFFF-50+0.5);
	  else
	    DCOffset[i]->SetIntNumber(100.0*int(dcoffset)/0xFFFF-50);

	  // Channel Trigger Threshold
	  ret = CAEN_DGTZ_GetChannelTriggerThreshold(board->GetHandle(),i,&threshold);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelTriggerThreshold"<<std::endl;
	  Threshold[i]->SetIntNumber(int(threshold));
	  
	  if(strstr(board->GetName(),(char*)"STD") == NULL)
	    {
	      // 不存在
	      // DPP
	      ret = CAEN_DGTZ_GetDPPPreTriggerSize(board->GetHandle(),i,&pretrigger);
	      if(ret) std::cout<<"Error: CAEN_DGTZ_GetDPPPreTriggerSize"<<std::endl;
	      PreTriggerSize[i]->SetIntNumber(int(pretrigger));
	    }
	  else
	    {
	      // 存在
	      // STD
	      // TODO
	      ret = CAEN_DGTZ_GetPostTriggerSize(board->GetHandle(),&posttrigger);
	      if(ret) std::cout<<"Error: CAEN_DGTZ_GetPostTriggerSize"<<std::endl;
	      PostTriggerSize->SetIntNumber(int(posttrigger));
	      
	    }
	  
	}
    }
}

void ParSetTable::ApplyParameter()
{
  CAEN_DGTZ_PulsePolarity_t pulsepolarity;// = CAEN_DGTZ_PulsePolarityPositive;
  unsigned int dcoffset = 0;
  unsigned int threshold = 0;
  unsigned int pretrigger = 0;
  unsigned int posttrigger = 0;
  int ret = 0;
  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      if(i < board->GetChannels() && TstBit_32(i,channelmask))
	{
	  // Pulse Polarity
	  std::cout<<"apply PS:"<<PulsePolarityBox[i]->GetSelected()<<std::endl;
	  if(PulsePolarityBox[i]->GetSelected() == 0)
	    ret = CAEN_DGTZ_SetChannelPulsePolarity(board->GetHandle(),i,CAEN_DGTZ_PulsePolarityPositive);
	  else
	    ret = CAEN_DGTZ_SetChannelPulsePolarity(board->GetHandle(),i,CAEN_DGTZ_PulsePolarityNegative);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelPulsePolarity"<<std::endl;
	  ret = CAEN_DGTZ_GetChannelPulsePolarity(board->GetHandle(),i,&pulsepolarity);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelPulsePolarity"<<std::endl;
	  if(pulsepolarity == CAEN_DGTZ_PulsePolarityPositive)
	    PulsePolarityBox[i]->Select(0);
	  else
	    PulsePolarityBox[i]->Select(1);
	  std::cout<<"apply PS:"<<pulsepolarity<<std::endl;

	  // DC Offset
	  ret = CAEN_DGTZ_SetChannelDCOffset(board->GetHandle(),i,(unsigned int)((DCOffset[i]->GetIntNumber()+50)*0xFFFF/100.0));
	  if(ret) std::cout<<"Error: CAEN_DGTZ_SetChannelDCOffset"<<std::endl;
	  ret = CAEN_DGTZ_GetChannelDCOffset(board->GetHandle(),i,&dcoffset);
	  if(ret) std::cout<<"Error: CAEN_DGTZ_GetChannelDCOffset"<<std::endl;
	  if(100.0*int(dcoffset)/0xFFFF-50 > 0)
	    DCOffset[i]->SetIntNumber(100.0*int(dcoffset)/0xFFFF-50+0.5);
	  else
	    DCOffset[i]->SetIntNumber(100.0*int(dcoffset)/0xFFFF-50);

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
	      ret = CAEN_DGTZ_SetPostTriggerSize(board->GetHandle(),posttrigger);
	      if(ret) std::cout<<"Error: CAEN_DGTZ_SetPostTriggerSize"<<std::endl;
	      ret = CAEN_DGTZ_GetPostTriggerSize(board->GetHandle(),&posttrigger);
	      if(ret) std::cout<<"Error: CAEN_DGTZ_GetPostTriggerSize"<<std::endl;
	      PostTriggerSize->SetIntNumber(int(posttrigger));
	    }

	  
	}
    }
}

void ParSetTable::CancelParameterFrame()
{
  DeleteWindow();
}



// 
// ParSetTable.cc ends here
