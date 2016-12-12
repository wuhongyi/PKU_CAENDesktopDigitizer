// ParSetTable.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 11月 30 13:02:11 2016 (+0800)
// Last-Updated: 一 12月 12 10:20:05 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 25
// URL: http://wuhongyi.cn 

#ifndef _PARSETTABLE_H_
#define _PARSETTABLE_H_

#include "Global.hh"
#include "Board.hh"

#include "TG3DLine.h"
#include "TGButton.h"
#include "TGButtonGroup.h"
#include "TGCanvas.h"
#include "TGClient.h"
#include "TGColorSelect.h"
#include "TGComboBox.h"
#include "TGDoubleSlider.h"
#include "TGFileDialog.h"
#include "TGFontDialog.h"
#include "TGFrame.h"
#include "TGHtml.h"
#include "TGIcon.h"
#include "TGImageMap.h"
#include "TGLabel.h"
#include "TGLayout.h"
#include "TGListBox.h"
#include "TGListTree.h"
#include "TGMenu.h"
#include "TGMsgBox.h"
#include "TGNumberEntry.h"
#include "TGPicture.h"
#include "TGResourcePool.h"
#include "TGSimpleTable.h"
#include "TGSlider.h"
#include "TGSpeedo.h"
#include "TGSplitter.h"
#include "TGTab.h"
#include "TGTextEdit.h"
#include "TGTextEntry.h"
#include "TGTextView.h"
#include "TGTextViewStream.h"
#include "TGTreeTable.h"
#include "TGTripleSlider.h"
#include "TGWindow.h"
#include "TRootEmbeddedCanvas.h"
#include "TRootHelpDialog.h"

#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ParSetTable : public TGTransientFrame
{
public:
  ParSetTable(const TGWindow *p = 0, const TGWindow *main = 0, Board *b = 0);
  virtual ~ParSetTable();

  void CopyParameter();
  void LoadParameter();
  void LoadSTDParameter();
  void LoadPSDParameter();
  void ApplyParameter();
  void ApplySTDParameter();
  void ApplyPSDParameter();
  void CancelParameterFrame();

private:
  unsigned int channelmask;
  Board *board;
  
private:
  TGHorizontalFrame *BaseAllParRows;
  TGHorizontalFrame *BaseParSTDRaws;
  TGHorizontalFrame *BaseParRows[MAX_CHANNEL];
  TGHorizontalFrame *BaseParPHAPSDRows[MAX_CHANNEL];
  
  // ParSet
  TGLabel *allchannellabel;
  TGLabel *posttriggersizelabel;
  TGNumberEntry *PostTriggerSize;
  TGLabel *enabledchannellabel[MAX_CHANNEL];
  TGLabel *selftriggerlabel[MAX_CHANNEL];
  TGComboBox *SelfTriggerBox[MAX_CHANNEL];
  TGLabel *pulsepolaritylabel[MAX_CHANNEL];
  TGComboBox *PulsePolarityBox[MAX_CHANNEL];
  TGLabel *dcoffsetlabel[MAX_CHANNEL];
  TGNumberEntry *DCOffset[MAX_CHANNEL];
  TGLabel *pretriggersizelabel[MAX_CHANNEL];
  TGNumberEntry *PreTriggerSize[MAX_CHANNEL];
  TGLabel *thresholdlabel[MAX_CHANNEL];
  TGNumberEntry *Threshold[MAX_CHANNEL];


  TGHorizontalFrame *OperationRows;
  TGComboBox *CopyChannelSelect;
  TGTextButton *ParSetTableCopyButton;
  TGTextButton *ParSetTableLoadButton;
  TGTextButton *ParSetTableApplyButton;
  TGTextButton *ParSetTableCancelButton;
  
  char tmp[256];
};

#endif /* _PARSETTABLE_H_ */
// 
// ParSetTable.hh ends here
