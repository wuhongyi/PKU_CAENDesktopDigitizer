// MainFrame.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 18:54:13 2016 (+0800)
// Last-Updated: 六 3月 17 10:57:53 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 375
// URL: http://wuhongyi.cn 

#include "MainFrame.hh"
#include "DT_PHA.hh"
#include "DT_PSD.hh"
#include "DT_Standard.hh"

#include <fstream>
#include <iostream>

#include <unistd.h>
#include <sys/stat.h>//stat(const char *file_name,struct stat *buf)
#include <sys/time.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ClassImp(MainFrame)

MainFrame::MainFrame(const TGWindow * p)
: TGMainFrame(p)
{
  dig = NULL;
  board = NULL;
  startstop = false;
  writedata = false;

  OnlineCanvas = NULL;


  gStyle->SetOptStat(0);//不显示统计框


  dig = (Digitizer*) calloc(1,sizeof(Digitizer));
  dig->boardInfo = (CAEN_DGTZ_BoardInfo_t*) calloc(1,sizeof(CAEN_DGTZ_BoardInfo_t));

  dig->linkType = CAEN_DGTZ_USB;
  dig->linkNum = 0;
  dig->conetNode = 0;
  dig->baseAddress = 0;

  
  
  TGTab *TabPanel = new TGTab(this);
  this->AddFrame(TabPanel, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  Tab1 = TabPanel->AddTab("Init");
  MakeFoldPanelInit(Tab1);

  TGCompositeFrame *Tab2 = TabPanel->AddTab("Online");
  MakeFoldPanelOnline(Tab2);


  // What to clean up in dtor
  SetCleanup(kDeepCleanup);
  SetWindowName("PKU Digitizer");
  MapSubwindows();
  Resize(GetDefaultSize());
  MapWindow();


  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      statisticsgroup->HideFrame(channelstatisticsframe[i]);
    }

  Tab1->HideFrame(controlgroup);
  Tab1->HideFrame(statisticsgroup);
  // Print();

  OpenRunLog();
  WriteRunLog((char *)"\n\n##########OpenSoftware##########\ntime: ");
  WriteRunLog(GetTimeStringYmdHMS());
  CloseRunLog();
}

MainFrame::~MainFrame()
{
  free(dig);
  
  // Clean up all widgets, frames and layouthints that were used
  Cleanup();

  OpenRunLog();
  WriteRunLog((char *)"\ntime: ");
  WriteRunLog(GetTimeStringYmdHMS());
  WriteRunLog((char *)"\n##########CloseSoftware##########\n");
  CloseRunLog();
}

void MainFrame::CheckLicense(Digitizer* dig)
{
  uint32_t countdown1, countdown2;
  CAEN_DGTZ_ReadRegister(dig->boardHandle, LICENSE_REG, &countdown1);
  usleep(100000);
  CAEN_DGTZ_ReadRegister(dig->boardHandle, LICENSE_REG, &countdown2);
  if (countdown1 != countdown2 || countdown2 == 0)
    {
      int minleft = countdown2 * LICENSE_COUNTDOWN_MULT / 60000;
      printf("Warning: firmware is unlicensed. Remaining demo mode time: %d minutes\n", minleft);
    }
}


void MainFrame::CloseWindow()
{
  OpenRunLog();
  WriteRunLog((char *)"\ntime: ");
  WriteRunLog(GetTimeStringYmdHMS());
  WriteRunLog((char *)"\n##########CloseSoftware##########\n");
  CloseRunLog();
  
  gApplication->Terminate(0);
}

Bool_t MainFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  // WidgetMessageTypes.h

  // System predefined widget message types. Message types are constants  
  // that indicate which widget sent the message and by which widget      
  // function (sub-message). Make sure your own message types don't clash  
  // whith the ones defined in this file. ROOT reserves all message ids   
  // between 0 - 1000. User defined messages should be in the range       
  // 1001 - 10000. Sub-messages must always be in the range 1-255.        
  // To use MK_MSG() just cast your message id's to an EWidgetMessageType.

  // std::cout<<msg <<"  "<< parm1<<"  "<<parm2<<std::endl;
  // std::cout<<GET_MSG(msg)<<"  "<<GET_SUBMSG(msg)<<std::endl;

  switch(GET_MSG(msg))
    {
    case kC_COMMAND:
      switch (GET_SUBMSG(msg))
	{
	case kCM_MENU://TGMenuBar TGPopupMenu
	  // menu id,user data
	  // Selecting a menu item will generate the event
	  
	  break;

	  //================================================

	case kCM_MENUSELECT:
	  
	  break;

	  //================================================

	case kCM_BUTTON://TGTextButton TGPictureButton
	  // button id,user data
	  // Selecting a text or picture button will generate the event
	  switch(parm1)
	    {
	    case CONNECTBUTTON:
	      if(connectStyle->GetSelected() == 0)
		{
		  dig->linkType = CAEN_DGTZ_USB;
		  dig->linkNum = 0;
		  dig->conetNode = 0;
		  dig->baseAddress = 0;
		}
	      else
		{
		  dig->linkType = CAEN_DGTZ_PCI_OpticalLink;
		  dig->linkNum = 0;
		  dig->conetNode = 0;
		  dig->baseAddress = 0;
		}
	      if(initDigitizer())
		{
		  //error
		  StateMsg->SetText("Connect error ......");
		  BoardNameMsg->SetText("DTXXXX");
		}
	      else
		{
		  BoardNameMsg->SetText(board->GetName());
		  StateMsg->SetText("Boot success !");
		  connectButton->SetEnabled(0);
		  DeleteButton->SetEnabled(1);
		  ProgramButton->SetEnabled(1);
		}
	      break;

	    case DELETEBUTTON:
	      deleteDigitizer();
	      connectButton->SetEnabled(1);
	      DeleteButton->SetEnabled(0);
	      StateMsg->SetText("Please enter Connect");
	      BoardNameMsg->SetText("");
	      ProgramButton->SetEnabled(0);
	      for (int i = 0; i < MAX_CHANNEL; ++i)
		{
		  ChannelsCheckButton[i]->SetEnabled(0);
		}
	      break;
	      
	    }

	  
	  break;

	  //================================================

	case kCM_CHECKBUTTON://TGCheckButton
	  // button id,user data
	  // Selecting a check button will generate the event
	  
	  break;

	  //================================================

	case kCM_RADIOBUTTON://TGRadioButton
	  // button id,user data
	  // Selecting a radio button will generate the event
	  
	  break;

	  //================================================

	case kCM_LISTBOX://TGListBox
	  // listbox id,item id
	  // Selecting an item in the listbox will generate the event
	  
	  break;

	  //================================================

	case kCM_COMBOBOX://TGComboBox
	  // combobox id,item id
	  // Selecting an item in the combobox will generate the event
	  switch(parm1)
	    {
	    case DPPACQMODE:
	      if(int(board->GetDPPAcqMode()) != parm2)
		ProgramButton->SetEnabled(1);
	      break;
	    case DPPSAVEPARAM:
	      if(int(board->GetDPPSaveParam()) != parm2)
	      ProgramButton->SetEnabled(1);
	      break;
	      
	    default:
	      break;
	    }
	  
	  break;

	  //================================================

	case kCM_TAB://TGTab
	  // tab id,0
	  // Clicking on a tab will bring the associated composite frame to the   
	  // front and generate the event
	  
	  break;

	  //================================================
   
	default:
	  break;
	}

      break;
      
      //------------------------------------------------------------------------
      
    case kC_HSCROLL:
      switch (GET_SUBMSG(msg))
	{
	case kSB_LINEUP:
	  	  
	  break;

	  //================================================

	case kSB_LINEDOWN:
	  	  
	  break;

	  //================================================

	case kSB_PAGEUP:
	  	  
	  break;

	  //================================================

	case kSB_PAGEDOWN:
	  	  
	  break;

	  //================================================

	case kSB_SLIDERTRACK://TGHScrollBar
	  // position,0
	  // The TGHScrollBar will generate the event messages
	  
	  break;

	  //================================================

	case kSB_SLIDERPOS://TGHScrollBar
	  // position,0
	  // The TGHScrollBar will generate the event messages
	  
	  break;
	  
	  //================================================
   
	default:
	  break;  
	}
      
      break;

      //------------------------------------------------------------------------
      
    case kC_VSCROLL:
      switch (GET_SUBMSG(msg))
	{
	case kSB_LINEUP:
	  	  
	  break;

	  //================================================

	case kSB_LINEDOWN:
	  	  
	  break;

	  //================================================

	case kSB_PAGEUP:
	  	  
	  break;

	  //================================================

	case kSB_PAGEDOWN:
	  	  
	  break;

	  //================================================

	case kSB_SLIDERTRACK://TGVScrollBar
	  // position,0
	  // The TGVScrollBar will generate the event messages
	  
	  break;

	  //================================================

	case kSB_SLIDERPOS://TGVScrollBar
	  // position,0
	  // The TGVScrollBar will generate the event messages
	  
	  break;
	  
	  //================================================
   
	default:
	  break;  
	}
      
      break;

      //------------------------------------------------------------------------
      
    case kC_TEXTENTRY:
      switch (GET_SUBMSG(msg))
	{
	case kTE_TEXTCHANGED://TGTextEntry
	  // widget id,0
	  // Changing text in the text entry widget will generate the event
	  
	  break;
	  
	  //================================================
   
	case kTE_ENTER://TGTextEntry
	  // widget id,0
	  // Hitting the enter key will generate
	  
	  break;
	  
	  //================================================
   
	case kTE_TAB://TGTextEntry
	  // widget id,0
	  // Hitting the tab key will generate
	  
	  break;
	  
	  //================================================
   
	case kTE_KEY:
	  
	  break;

	  //================================================
   
	default:
	  break;  
	}
      
      break;

      //------------------------------------------------------------------------

    case kC_CONTAINER:
      switch (GET_SUBMSG(msg))
	{
	case kCT_ITEMCLICK://TGListView
	  // which button, location(y<<16|x)
	  // A list view can generate the events
	  
	  break;

	  //================================================
   
	case kCT_ITEMDBLCLICK://TGListView
	  // which button, location(y<<16|x)
	  // A list view can generate the events
	  
	  break;

	  //================================================
   
	case kCT_SELCHANGED://TGListView
	  // total items,selected items
	  // A list view can generate the events
	  
	  break;

	  //================================================
   
	case kCT_KEY:
	  
	  break;
	  
	  //================================================
   
	default:
	  break;  
	}
	  
      break;

      //------------------------------------------------------------------------

    case kC_HSLIDER:
      switch (GET_SUBMSG(msg))
	{
	  // Use the functions GetMinPosition(), GetMaxPosition() and             
	  // GetPosition() to retrieve the position of the slider.

	  // Use the function GetPointerPosition() to retrieve the position of    
	  // the pointer
	  
	case kSL_POS://TGDoubleHSlider TGTripleHSlider
	  // slider id,0
	  // Dragging the slider will generate the event
	  
	  break;

	  //================================================
   
	case kSL_TRACK:
	  	  
	  break;

	  //================================================
   
	case kSL_PRESS://TGDoubleHSlider TGTripleHSlider
	  // slider id,0
	  // Pressing the mouse will generate the event
	  
	  break;

	  //================================================
   
	case kSL_RELEASE://TGDoubleHSlider TGTripleHSlider
	  // slider id,0
	  // Releasing the mouse will generate the event
	  
	  break;

	  //================================================
   
	case kSL_POINTER://TGTripleHSlider
	  // slider id,0
	  // Moving the pointer will generate the event
	  
	  break;

	  //================================================
   
	default:
	  break;  
	}
	    
      break;

      //------------------------------------------------------------------------
      
    case kC_VSLIDER:
      switch (GET_SUBMSG(msg))
	{
	  // Use the functions GetMinPosition(), GetMaxPosition() and             
	  // GetPosition() to retrieve the position of the slider.

	  // Use the function GetPointerPosition() to retrieve the position of    
	  // the pointer
	  
	case kSL_POS://TGDoubleVSlider TGTripleHSlider
	  // slider id,0
	  // Dragging the slider will generate the event
	  
	  break;

	  //================================================
   
	case kSL_TRACK:
	  	  
	  break;

	  //================================================
   
	case kSL_PRESS://TGDoubleVSlider TGTripleHSlider
	  // slider id,0
	  // Pressing the mouse will generate the event
	  
	  break;

	  //================================================
   
	case kSL_RELEASE://TGDoubleVSlider TGTripleHSlider
	  // slider id,0
	  // Releasing the mouse will generate the event
	  
	  break;

	  //================================================
   
	case kSL_POINTER://TGTripleHSlider
	  // slider id,0
	  // Moving the pointer will generate the event
	  
	  break;

	  //================================================
   
	default:
	  break;  
	}

      break;

      //------------------------------------------------------------------------
      
    case kC_LISTTREE://
      switch (GET_SUBMSG(msg))
	{
	case kCT_ITEMCLICK://TGListTree
	  // which button, location(y<<16|x)
	  // A list tree can generate the events
	  
	  break;

	  //================================================
   
	case kCT_ITEMDBLCLICK://TGListTree
	  // which button, location(y<<16|x)
	  // A list tree can generate the events
	  
	  break;

	  //================================================
   
	case kCT_SELCHANGED:
	  
	  break;

	  //================================================
   
	case kCT_KEY:
	  
	  break;
	  
	  //================================================
   
	default:
	  break;  
	}            

      break;

      //------------------------------------------------------------------------
      
    case kC_TEXTVIEW:
      switch (GET_SUBMSG(msg))
	{
	case kTXT_ISMARKED:// TGView
	  // widget id, [true|false]  	  
	  break;

	  //================================================
   
	case kTXT_DATACHANGE:// TGView
	  // widget id, 0
	  
	  break;

	  //================================================
   
	case kTXT_CLICK2:// TGView
	  // widget id, position (y << 16) | x)
	  
	  break;

	  //================================================
   
	case kTXT_CLICK3:// TGView
	  // widget id, position (y << 16) | x)
	  
	  break;

	  //================================================
   
	case kTXT_F3:// TGView
	  // widget id, true
	  
	  break;

	  //================================================
   
	case kTXT_OPEN:
	  	  	  
	  break;

	  //================================================
   
	case kTXT_CLOSE:
	  	  	  
	  break;

	  //================================================
   
	case kTXT_SAVE:
   	  	  
	  break;

	  //================================================
   
	default:
	  break;  
	}
      
      break;

      //------------------------------------------------------------------------
      
    case kC_COLORSEL:
      switch (GET_SUBMSG(msg))
	{ 
	case kCOL_CLICK:// TGColorPalette TGColorPick
	  // widget id,0
	  // and the signal:  ColorSelected(Pixel_t color) 

	  
	  break;

	  //================================================
   
	case kCOL_SELCHANGED:// TGColorSelect
	  // widget id,pixel
	  // and the signal: ColorSelected(Pixel_t pixel)
	  
	  // The TGColorSelect widget is like a checkbutton but instead of the    
	  // check mark there is color area with a little down arrow. When        
	  // clicked on the arrow the TGColorPopup pops up. 

	  
	  break;

	  //================================================
   
	default:
	  break;  
	}
      
      break;

      //------------------------------------------------------------------------
      
    case kC_PATTERNSEL:
      switch (GET_SUBMSG(msg))
	{             
	case kPAT_CLICK:
	  	  
	  break;

	  //================================================
   
	case kPAT_SELCHANGED:
	  	  
	  break;

	  //================================================
   
	default:
	  break;  
	}
      
      break;

      //------------------------------------------------------------------------
      
    case kC_MARKERSEL:
      switch (GET_SUBMSG(msg))
	{             
	case kMAR_CLICK:
	  	  	  
	  break;

	  //================================================
   
	case kMAR_SELCHANGED:
	  	  	  
	  break;

	  //================================================
   
	default:
	  break;  
	}            

      break;

      //------------------------------------------------------------------------
      
    case kC_POPUP:
      switch (GET_SUBMSG(msg))
	{             
	case kPOP_HIDE:
	  	  	  	  
	  break;

	  //================================================
   
	default:
	  break;  
	}            

      break;

      //------------------------------------------------------------------------
      
    case kC_DOCK:
      switch (GET_SUBMSG(msg))
	{             
	case kDOCK_DOCK:
	  	  	  	  	  
	  break;

	  //================================================
   
	case kDOCK_UNDOCK:
	  	  	  	  	  
	  break;

	  //================================================
   
	case kDOCK_SHOW:
	  	  	  	  	  
	  break;

	  //================================================
   
	case kDOCK_HIDE:
	  	  	  	  	  
	  break;

	  //================================================
   
	default:
	  break;  
	}      

      break;

      //------------------------------------------------------------------------
      
    case kC_MDI:
      switch (GET_SUBMSG(msg))
	{             
	case kMDI_CURRENT:
	  	  	  	  	  	  
	  break;

	  //================================================
   
	case kMDI_CREATE:
	  	  	  	  	  	  
	  break;

	  //================================================
   
	case kMDI_CLOSE:
	  	  	  	  	  	  
	  break;

	  //================================================
   
	case kMDI_RESTORE:
	  	  	  	  	  	  
	  break;

	  //================================================
   
	case kMDI_MOVE:
	  	  	  	  	  	  
	  break;

	  //================================================
   
	case kMDI_SIZE:
	  	  	  	  	  	  
	  break;

	  //================================================
   
	case kMDI_MINIMIZE:
	  	  	  	  	  	  
	  break;

	  //================================================
   
	case kMDI_MAXIMIZE:
	  	  	  	  	  	  
	  break;

	  //================================================
   
	case kMDI_HELP:
	  	  	  	  	  	  
	  break;

	  //================================================
   
	case kMDI_MENU:
	  	  	  	  	  	  
	  break;

	  //================================================
   
	default:
	  break;  
	}

      break;

      //------------------------------------------------------------------------

    default:
      break;
    }


  return kTRUE;
}

void MainFrame::MakeFoldPanelInit(TGCompositeFrame *TabPanel)
{
  TGGroupFrame *initboaedgroup = new TGGroupFrame(TabPanel,"Connect");
  TabPanel->AddFrame(initboaedgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));

  TGHorizontalFrame *initboaed = new TGHorizontalFrame(initboaedgroup);
  initboaedgroup->AddFrame(initboaed, new TGLayoutHints(kLHintsExpandX | kLHintsTop,0/*left*/,0/*right*/,0/*top*/,0/*bottom*/));

  connectStyle = new TGComboBox(initboaed,-1);
  initboaed->AddFrame(connectStyle, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  connectStyle->AddEntry("USB",0);
  connectStyle->AddEntry("Optical",1);
  connectStyle->Select(0);
  connectStyle->Resize(100,20);
  // connectStyle->Associate(this);

  connectButton = new TGTextButton(initboaed,"Connect",CONNECTBUTTON);
  initboaed->AddFrame(connectButton, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 10,0, 0));
  connectButton->SetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1", false);
  connectButton->SetTextColor(0x0000ff, false);
  connectButton->Associate(this);
  connectButton->SetEnabled(1);
  
  DeleteButton = new TGTextButton(initboaed,"DeleteConnect",DELETEBUTTON);
  initboaed->AddFrame(DeleteButton, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 10,0, 0));
  DeleteButton->SetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1", false);
  DeleteButton->SetTextColor(0xff0000, false);
  DeleteButton->Associate(this);
  DeleteButton->SetEnabled(0);


  BoardNameMsg = new TGTextEntry(initboaed,
				 new TGTextBuffer(14), -1,
				 BoardNameMsg->GetDefaultGC()(),
				 BoardNameMsg->GetDefaultFontStruct(),
				 kRaisedFrame | kDoubleBorder,
				 GetWhitePixel());
  initboaed->AddFrame(BoardNameMsg, new TGLayoutHints(kLHintsLeft | kLHintsLeft, 10, 0, 0, 0));
  BoardNameMsg->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
  BoardNameMsg->SetTextColor(0x00FF00, false);
  BoardNameMsg->SetText("");
  // BoardNameMsg->Resize(100, 12);
  BoardNameMsg->SetEnabled(kFALSE);
  BoardNameMsg->SetFrameDrawn(kTRUE);//kFALSE
  
  
  StateMsg = new TGTextEntry(initboaed,
			     new TGTextBuffer(100), -1,
			     StateMsg->GetDefaultGC()(),
			     StateMsg->GetDefaultFontStruct(),
			     kRaisedFrame | kDoubleBorder,
			     GetWhitePixel());
  initboaed->AddFrame(StateMsg, new TGLayoutHints(kLHintsExpandX | kLHintsLeft, 10, 0, 0, 0));
  StateMsg->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
  StateMsg->SetTextColor(0xd15fee, false);
  StateMsg->SetText("Please enter Connect");
  // StateMsg->Resize(100, 12);
  StateMsg->SetEnabled(kFALSE);
  StateMsg->SetFrameDrawn(kFALSE);


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  
  TGGroupFrame *filesetgroup = new TGGroupFrame(TabPanel,"File");
  TabPanel->AddFrame(filesetgroup,new TGLayoutHints(kLHintsExpandX | kLHintsTop));

  TGHorizontalFrame *fileset = new TGHorizontalFrame(filesetgroup);
  filesetgroup->AddFrame(fileset,new TGLayoutHints(kLHintsExpandX | kLHintsTop));

  TGLabel *filepathlabel = new TGLabel(fileset,"File Path: ");
  fileset->AddFrame(filepathlabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 3, 5, 0));
  filepathtext = new TGTextEntry(fileset,new TGTextBuffer(100));
  fileset->AddFrame(filepathtext,new TGLayoutHints(kLHintsExpandX | kLHintsTop, 10,3,4,0));

  
  TGLabel *filenamelabel = new TGLabel(fileset,"File Name: ");
  fileset->AddFrame(filenamelabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10,3,5,0));
  filenametext = new TGTextEntry(fileset, new TGTextBuffer(20));
  fileset->AddFrame(filenametext,new TGLayoutHints(kLHintsLeft | kLHintsTop,10,3,4,0));

  TGLabel *filerunlabel = new TGLabel(fileset,"Run Num: ");
  fileset->AddFrame(filerunlabel,new TGLayoutHints(kLHintsLeft| kLHintsTop,10,3,5,0));

  filerunnum = new TGNumberEntry(fileset,0,5,RUNNUMBER,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);
  fileset->AddFrame(filerunnum,new TGLayoutHints(kLHintsLeft | kLHintsTop,0,3,4,0));
  // filerunnum->SetButtonToNum(0);
  // filerunnum->Associate(this);

  CompleteButton = new TGTextButton(fileset,"Complete");
  fileset->AddFrame(CompleteButton,new TGLayoutHints(kLHintsLeft | kLHintsTop,0,3,4,0));
  CompleteButton->Connect("Pressed()","MainFrame",this,"SetDataFileName()");

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  controlgroup = new TGGroupFrame(TabPanel,"Control");
  TabPanel->AddFrame(controlgroup,new TGLayoutHints(kLHintsExpandX | kLHintsTop));

  TGHorizontalFrame *channelgrouphframe = new TGHorizontalFrame(controlgroup);
  controlgroup->AddFrame(channelgrouphframe,new TGLayoutHints(kLHintsLeft | kLHintsTop,10,4,3,3));

  TGLabel *enabledchannellabel = new TGLabel(channelgrouphframe,"Enable Ch: ");
  channelgrouphframe->AddFrame(enabledchannellabel,new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));
  enabledchannellabel->SetTextColor(0xB22222);
  
  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      sprintf(tmp,"%02d",i);
      ChannelsCheckButton[i] = new TGCheckButton(channelgrouphframe,tmp);
      channelgrouphframe->AddFrame(ChannelsCheckButton[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,4,3,3));
      ChannelsCheckButton[i]->SetTextColor(0xA020F0);
      ChannelsCheckButton[i]->SetState(kButtonUp);
      ChannelsCheckButton[i]->Connect("Toggled(Bool_t)","MainFrame",this,"MarkChennelOrRecordLengthChange()");
      ChannelsCheckButton[i]->SetEnabled(0);
    }

  DPPAcqMode = new TGComboBox(channelgrouphframe,DPPACQMODE);
  channelgrouphframe->AddFrame(DPPAcqMode, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  DPPAcqMode->Associate(this);
  DPPAcqMode->AddEntry("Oscilloscope",0);
  DPPAcqMode->AddEntry("List",1);
  DPPAcqMode->AddEntry("Mixed",2);
  DPPAcqMode->Select(2);
  DPPAcqMode->Resize(100,20);

  DPPSaveParam = new TGComboBox(channelgrouphframe,DPPSAVEPARAM);
  channelgrouphframe->AddFrame(DPPSaveParam, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  DPPSaveParam->Associate(this);
  DPPSaveParam->AddEntry("EnergyOnly",0);
  DPPSaveParam->AddEntry("TimeOnly",1);
  DPPSaveParam->AddEntry("EnergyAndTime",2);
  DPPSaveParam->AddEntry("None",3);
  DPPSaveParam->AddEntry("ChargeAndTime",4);
  DPPSaveParam->Select(2);
  DPPSaveParam->Resize(100,20);
  
  TGLabel *recordlengthlabel = new TGLabel(channelgrouphframe,"Record Length: ");
  channelgrouphframe->AddFrame(recordlengthlabel,new TGLayoutHints(kLHintsLeft | kLHintsTop,10,3,3,0));

  RecordLength = new TGNumberEntry(channelgrouphframe,1024,5,RECORDLENGTH,TGNumberFormat::kNESInteger,TGNumberFormat::kNEAPositive);//TGNumberFormat::kNEAPositive TGNumberFormat::kNEANonNegative
  channelgrouphframe->AddFrame(RecordLength,new TGLayoutHints(kLHintsLeft | kLHintsTop,0,3,3,0));
  RecordLength->GetNumberEntry()->Connect("TextChanged(char*)","MainFrame",this,"MarkChennelOrRecordLengthChange()");
  

  ProgramButton = new TGTextButton(channelgrouphframe,"&ProgramDigitizer");
  channelgrouphframe->AddFrame(ProgramButton,new TGLayoutHints(kLHintsLeft | kLHintsTop,20,3,3,0));
  ProgramButton->Connect("Pressed()","MainFrame",this,"ProgramDigitizer()");
  ProgramButton->SetEnabled(0);

  AllocateButton = new TGTextButton(channelgrouphframe,"&AllocateMemory");
  channelgrouphframe->AddFrame(AllocateButton,new TGLayoutHints(kLHintsLeft | kLHintsTop,10,3,3,0));
  AllocateButton->Connect("Pressed()","MainFrame",this,"AllocateMemory()");
  AllocateButton->SetEnabled(0);

  
  // --------
  
  TGHorizontalFrame *cgrouphframe = new TGHorizontalFrame(controlgroup);
  controlgroup->AddFrame(cgrouphframe,new TGLayoutHints(kLHintsLeft | kLHintsTop,10,4,3,3));

  AdjustParButton = new TGTextButton(cgrouphframe,"&AdjustParFrame");
  cgrouphframe->AddFrame(AdjustParButton,new TGLayoutHints(kLHintsLeft | kLHintsTop,10,10,0,0));
  AdjustParButton->Connect("Pressed()","MainFrame",this,"AdjustParameters()");
  AdjustParButton->SetEnabled(0);
  AdjustParButton->SetToolTipText("Click will pop up the parameter adjustment interface.");


  PrintRegistersButton = new TGTextButton(cgrouphframe,"&PrintRegisters");
  cgrouphframe->AddFrame(PrintRegistersButton,new TGLayoutHints(kLHintsLeft | kLHintsTop,0,100,0,0));
  PrintRegistersButton->Connect("Pressed()","MainFrame",this,"PrintRegisters()");
  PrintRegistersButton->SetEnabled(0);
  
  
  StartStopButton = new TGTextButton(cgrouphframe,"&Start/Stop");
  cgrouphframe->AddFrame(StartStopButton,new TGLayoutHints(kLHintsLeft | kLHintsTop));
  StartStopButton->Connect("Pressed()","MainFrame",this,"StartStopRun()");
  StartStopButton->SetEnabled(0);

  OnlineCheckButton = new TGCheckButton(cgrouphframe,"&Online data");
  cgrouphframe->AddFrame(OnlineCheckButton,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,4,3,3));
  OnlineCheckButton->SetTextColor(0x000080);
  OnlineCheckButton->SetState(kButtonDown);
  OnlineCheckButton->Connect("Clicked()","MainFrame",this,"SetOnlineData()");


  WtiteDataButton = new TGTextButton(cgrouphframe,"&WriteData");
  cgrouphframe->AddFrame(WtiteDataButton,new TGLayoutHints(kLHintsLeft | kLHintsTop));
  WtiteDataButton->Connect("Pressed()","MainFrame",this,"SetWriteData()");
  WtiteDataButton->SetEnabled(0);



  
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  statisticsgroup = new TGGroupFrame(TabPanel,"Statistics");
  TabPanel->AddFrame(statisticsgroup,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

  TGHorizontalFrame *readoutframe = new TGHorizontalFrame(statisticsgroup);
  statisticsgroup->AddFrame(readoutframe, new TGLayoutHints(kLHintsExpandX | kLHintsTop,0/*left*/,0/*right*/,0/*top*/,0/*bottom*/));

  TGLabel *readoutdatalabel = new TGLabel(readoutframe,"Data Rate: ");
  readoutframe->AddFrame(readoutdatalabel,new TGLayoutHints(kLHintsLeft | kLHintsTop,10,3,3,0));
  
  StatisticsDataMsg = new TGTextEntry(readoutframe,
			     new TGTextBuffer(14), -1,
			     StatisticsDataMsg->GetDefaultGC()(),
			     StatisticsDataMsg->GetDefaultFontStruct(),
			     kRaisedFrame | kDoubleBorder,
			     GetWhitePixel());
  readoutframe->AddFrame(StatisticsDataMsg, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 0, 0, 0));
  StatisticsDataMsg->SetAlignment(kTextCenterX);
  StatisticsDataMsg->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
  StatisticsDataMsg->SetTextColor(0xFF0000, false);
  StatisticsDataMsg->SetText("");
  StatisticsDataMsg->SetEnabled(kFALSE);
  StatisticsDataMsg->SetFrameDrawn(kTRUE);

  TGLabel *readoutdataratelabel = new TGLabel(readoutframe,"MB/s ");
  readoutframe->AddFrame(readoutdataratelabel,new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));


  TGLabel *readoutfilesizemblabel = new TGLabel(readoutframe,"MB ");
  readoutframe->AddFrame(readoutfilesizemblabel,new TGLayoutHints(kLHintsRight | kLHintsTop,5,3,3,0));

  StatisticsFileSizeMsg = new TGTextEntry(readoutframe,
				      new TGTextBuffer(14), -1,
				      StatisticsDataMsg->GetDefaultGC()(),
				      StatisticsDataMsg->GetDefaultFontStruct(),
				      kRaisedFrame | kDoubleBorder,
				      GetWhitePixel());
  readoutframe->AddFrame(StatisticsFileSizeMsg, new TGLayoutHints(kLHintsTop | kLHintsRight, 10, 0, 0, 0));
  StatisticsFileSizeMsg->SetAlignment(kTextCenterX);
  StatisticsFileSizeMsg->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
  StatisticsFileSizeMsg->SetTextColor(0xFF0000, false);
  StatisticsFileSizeMsg->SetText("");
  StatisticsFileSizeMsg->SetEnabled(kFALSE);
  StatisticsFileSizeMsg->SetFrameDrawn(kTRUE);

  TGLabel *readoutfilesizelabel = new TGLabel(readoutframe,"File Size: ");
  readoutframe->AddFrame(readoutfilesizelabel,new TGLayoutHints(kLHintsRight | kLHintsTop,10,3,3,0));

  
  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      channelstatisticsframe[i] = new TGHorizontalFrame(statisticsgroup);
      statisticsgroup->AddFrame(channelstatisticsframe[i],new TGLayoutHints(kLHintsExpandX | kLHintsTop));

      sprintf(tmp,"Chan   %02d: ",i);
      channelstatisticslabel[i] = new TGLabel(channelstatisticsframe[i],tmp);
      channelstatisticsframe[i]->AddFrame(channelstatisticslabel[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,10,3,3,0));

      ChannelStatisticsDataMsg[i] = new TGTextEntry(channelstatisticsframe[i],
					  new TGTextBuffer(14), -1,
					  ChannelStatisticsDataMsg[i]->GetDefaultGC()(),
					  ChannelStatisticsDataMsg[i]->GetDefaultFontStruct(),
					  kRaisedFrame | kDoubleBorder,
					  GetWhitePixel());
      channelstatisticsframe[i]->AddFrame(ChannelStatisticsDataMsg[i], new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 0, 0, 0));
      ChannelStatisticsDataMsg[i]->SetAlignment(kTextCenterX);
      ChannelStatisticsDataMsg[i]->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
      ChannelStatisticsDataMsg[i]->SetTextColor(0x0000AA, false);
      ChannelStatisticsDataMsg[i]->SetText("");
      ChannelStatisticsDataMsg[i]->SetEnabled(kFALSE);
      ChannelStatisticsDataMsg[i]->SetFrameDrawn(kTRUE);

      channelstatisticsratelabel[i] = new TGLabel(channelstatisticsframe[i],"Hz/s ");
      channelstatisticsframe[i]->AddFrame(channelstatisticsratelabel[i],new TGLayoutHints(kLHintsLeft | kLHintsTop,5,3,3,0));
    }

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  std::ifstream in("../parset/Run.config");
  if(!in.is_open()) return;
  in.getline(tmp,200);
  filepathtext->Insert(tmp);
  in.getline(tmp,200);
  filenametext->Insert(tmp);
  in.close();

  std::ifstream inrunnumber("../parset/RunNumber");
  if(!inrunnumber.is_open()) return;
  inrunnumber.getline(tmp,200);
  filerunnum->SetText(tmp);
  inrunnumber.close();
  
}

void MainFrame::SetWriteData()
{
  if(writedata)
    {
      // writting ,do stop
      WtiteDataButton->SetText((char*)"&StartWrite");
      writedata = false;
      board->SetWriteData(false);
      // 这里应该需要sleep一下
      board->CloseFile();

      OpenRunLog();
      WriteRunLog((char *)"\n=====WriteStop   ");
      WriteRunLog(GetTimeStringYmdHMS());
      CloseRunLog();
      
      runnum++;
      filerunnum->SetIntNumber(runnum);
      std::ofstream out("../parset/RunNumber");
      out<<runnum<<std::endl;
      out.close();

      const char *path=filepathtext->GetText();
      const char *filen=filenametext->GetText();
      sprintf(Filename,"%s%s_R%04d.bin",path,filen,runnum);
      
      StartStopButton->SetEnabled(1);
    }
  else
    {
      // stop, do writting
      WtiteDataButton->SetText((char*)"&StopWrite");
      writedata = true;

      const char *path=filepathtext->GetText();
      const char *filen=filenametext->GetText();
      runnum = (int)filerunnum->GetIntNumber();
      sprintf(Filename,"%s%s_R%04d.bin",path,filen,runnum);

      OpenRunLog();
      WriteRunLog((char *)"\n=====WriteStart   ");
      WriteRunLog(GetTimeStringYmdHMS());
      sprintf(tmp,"\nFileNumber: %04d",runnum);
      WriteRunLog(tmp);
      CloseRunLog();
      
      board->OpenFile(Filename);
      board->SetWriteData(true);
      StartStopButton->SetEnabled(0);
    }
}

void MainFrame::SetOnlineData()
{
  if(!OnlineCheckButton->IsOn())
    {
      StatisticsDataMsg->SetText("");
      for (int i = 0; i < board->GetChannels(); ++i)
	{
	  if(ChannelsCheckButton[i]->IsOn())
	    {
	      ChannelStatisticsDataMsg[i]->SetText("");
	    }
	}
    }
}

void MainFrame::SetDataFileName()
{
  // const char *path=filepathtext->GetText();
  // const char *filen=filenametext->GetText();
  // runnum = (int)filerunnum->GetIntNumber();

  if(IsDirectoryExists(filepathtext->GetText()))
    {
      std::ofstream out("../parset/Run.config");
      out<<filepathtext->GetText()<<std::endl;
      out<<filenametext->GetText()<<std::endl;
      out.close();

      Tab1->ShowFrame(controlgroup);
      Tab1->ShowFrame(statisticsgroup);
    }
  else
    {
      std::cout<<"The output file directory does not exist"<<std::endl;
    }
}

void MainFrame::PrintRegisters()
{
  int MajorNumber;
  sscanf(dig->boardInfo->AMC_FirmwareRel, "%d", &MajorNumber);
  
  switch(dig->boardInfo->Model)
    {
    case CAEN_DGTZ_DT5724:
      if((strcmp(dig->boardInfo->ModelName, "DT5724") == 0) || (strcmp(dig->boardInfo->ModelName, "DT5724A") == 0))
	{
	  // if(MajorNumber == STANDARD_FW_CODE) ;
	}
      break;
      
    case CAEN_DGTZ_DT5730:
      if(strcmp(dig->boardInfo->ModelName, "DT5730") == 0)
	{
	  if(MajorNumber == V1730_DPP_PSD_CODE) Read_DGTZ_Register_725_730_DPP_PSD_Revision03(board->GetHandle(),board->GetChannels());
	  
	  // uint32_t Data;
	  // CAEN_DGTZ_ReadRegister(board->GetHandle(), 0x1080, &Data);
	  // Data = SetBit_32(6,Data);
	  // CAEN_DGTZ_WriteRegister(board->GetHandle(), 0x1080, Data);

	  // CAEN_DGTZ_ReadRegister(board->GetHandle(), 0x103C, &Data);
	  // Data = ClrBit_32(0,Data);
	  // Data = ClrBit_32(1,Data);
	  // Data = ClrBit_32(2,Data);
	  // Data = SetBit_32(0,Data);
	  // // Data = ClrBit_32(8,Data);
	  // // Data = ClrBit_32(9,Data);
	  // // Data = SetBit_32(8,Data);
	  // CAEN_DGTZ_WriteRegister(board->GetHandle(), 0x103C, Data);
	  
	  // CAEN_DGTZ_ReadRegister(board->GetHandle(), 0x8000, &Data);
	  // Data = SetBit_32(17,Data);
	  // Data = SetBit_32(12,Data);
	  // CAEN_DGTZ_WriteRegister(board->GetHandle(), 0x8000, Data);
	  // // CAEN_DGTZ_WriteRegister(board->GetHandle(), 0x8004, 0x1000);
	  // // CAEN_DGTZ_WriteRegister(board->GetHandle(), 0x8004, 0x20000);
	  // if(MajorNumber == V1730_DPP_PSD_CODE) Read_DGTZ_Register_725_730_DPP_PSD_Revision03(board->GetHandle(),board->GetChannels());

	  
	  if(MajorNumber == STANDARD_FW_CODE) Read_DGTZ_Register_725_730_STD_Revision00(board->GetHandle(),board->GetChannels());
	  if(MajorNumber == V1730_DPP_PHA_CODE)  Read_DGTZ_Register_725_730_DPP_PHA_Revision00(board->GetHandle(),board->GetChannels());
	}
      break;

    case CAEN_DGTZ_DT5742:
      if((strcmp(dig->boardInfo->ModelName, "DT5742") == 0) || (strcmp(dig->boardInfo->ModelName, "DT5742B") == 0))
	{
	  // if(MajorNumber == STANDARD_FW_CODE) ;
	}
      break;
      
    default:
      break;
    }

}


void MainFrame::StartStopRun()
{
  int ret = 0;
  if(startstop)
    {
      // runing,to stop
      StartStopButton->SetText((char*)"&Start");
      startstop = false;
      DeleteButton->SetEnabled(1);
      SendASoftwareTriggerButton->SetEnabled(0);
      WtiteDataButton->SetEnabled(0);
      StatisticsDataMsg->SetText("");
      for (int i = 0; i < board->GetChannels(); ++i)
	{
	  if(ChannelsCheckButton[i]->IsOn())
	    {
	      ChannelStatisticsDataMsg[i]->SetText("");
	    }
	}
    }
  else
    {
      // stop, do runing
      StartStopButton->SetText((char*)"&Stop");
      startstop = true;
      DeleteButton->SetEnabled(0);
      SendASoftwareTriggerButton->SetEnabled(1);
      WtiteDataButton->SetEnabled(1);
      board->SetStatisticsClear();
      PrevRateTime = GetTime();
      
      // Clear unclean data (without this changing RecordLength at RunTime Freezes the digitizer)
      ret = CAEN_DGTZ_ClearData(board->GetHandle());
      if(ret) printf("Error: CAEN_DGTZ_ClearData\n ");
      ret = CAEN_DGTZ_SWStartAcquisition(board->GetHandle());
      if(ret) printf("Error: CAEN_DGTZ_SWStartAcquisition\n ");
      
      RunReadData();
    }
  
}

void MainFrame::RunReadData()
{
  int ret = 0;
  std::cout<<"read loop ..."<<std::endl;

  while(startstop)
    {
      // Read data
      // Save data
      // Send Online data
      /* Read data from the board */
      if(board->ReadLoop())
	{
	  if(!board->GetEvent())
	    {
	      board->GetWaveform(MonitorCheckButton->IsOn(),MonitorTypeBox->GetSelected());
	    }	  
	}
      else
	{
	  // 温度过高
	  // uint32_t lstatus;
	  // ret = CAEN_DGTZ_ReadRegister(handle, CAEN_DGTZ_ACQ_STATUS_ADD, &lstatus);
	  // if (lstatus & (0x1 << 19)) {
	  //   ErrCode = ERR_OVERTEMP;
	  
	}

      CurrentTime = GetTime();
      ElapsedTime = CurrentTime - PrevRateTime;
      if (ElapsedTime > 1000)
	{
	  sprintf(tmp,"%0.2f",GetFileSizeMB(Filename));
	  StatisticsFileSizeMsg->SetText(tmp);
	  
	  if(OnlineCheckButton->IsOn())
	    {
	      sprintf(tmp,"%0.2f",(float)board->GetStatisticsNb()/((float)ElapsedTime*1048.576f));
	      StatisticsDataMsg->SetText(tmp);
	      Ne = board->GetStatisticsNe();
	      for (int i = 0; i < board->GetChannels(); ++i)
		{
		  if(ChannelsCheckButton[i]->IsOn())
		    {
		      // printf("Ch %d  Count %0.2f\n",i,*(Ne+i)*1000.0f/(float)ElapsedTime);
		      sprintf(tmp,"%0.0f",*(Ne+i)*1000.0f/(float)ElapsedTime);
		      ChannelStatisticsDataMsg[i]->SetText(tmp);
		    }
	      
		}
	  
	      board->SetStatisticsClear();
	    }

	  if(MonitorCheckButton->IsOn())
	    {

	      switch(MonitorTypeBox->GetSelected())
		{
		case 0://Single
		  OnlineCanvas->cd();
		  // board->GetSingleWaveform()->Print();
		  board->GetSingleWaveform()->SetLineColor(kRed);
		  board->GetSingleWaveform()->Draw("AL");//DrawCopy("hist");
		  OnlineCanvas->SetLogy(0);
		  OnlineCanvas->Modified();
		  OnlineCanvas->Update();
		  break;

		case 1://Multi
		  OnlineCanvas->cd();
		  board->GetMultiWaveform()->Draw("AP");
		  OnlineCanvas->SetLogy(0);
		  OnlineCanvas->Modified();
		  OnlineCanvas->Update();
		  break;

		case 2:
		  OnlineCanvas->cd();
		  // board->GetEnergySpectrum()->SetLineColor(kRed);
		  board->GetEnergySpectrum()->Draw("AP");
		  OnlineCanvas->SetLogy(0);
		  OnlineCanvas->Modified();
		  OnlineCanvas->Update();
		  break;

		case 3:
		  OnlineCanvas->cd();
		  board->GetSingleFFTCAEN()->SetLineColor(kRed);
		  board->GetSingleFFTCAEN()->Draw("AL");
		  board->GetSingleFFTCAEN()->GetXaxis()->SetTitle("MHz");
		  board->GetSingleFFTCAEN()->GetYaxis()->SetTitle("dB");
		  OnlineCanvas->SetLogy(0);
		  OnlineCanvas->Modified();
		  OnlineCanvas->Update();
		  break;

		case 4:
		  OnlineCanvas->cd();
		  board->GetSingleFFTXIA()->SetLineColor(kRed);
		  board->GetSingleFFTXIA()->Draw("AL");
		  // board->GetSingleFFTXIA()->GetXaxis()->SetTitle("MHz");
		  // board->GetSingleFFTXIA()->GetYaxis()->SetTitle("dB");
		  OnlineCanvas->SetLogy(1);
		  OnlineCanvas->Modified();
		  OnlineCanvas->Update();
		  break;

		  
		default:
		  break;
		}
	      
	      
	      board->SetUpdateSingleWaveform();
	      board->SetUpdateSingleFFT();
	    }
	  
	  PrevRateTime = CurrentTime;
	}
      
      gSystem->ProcessEvents();
    }

  ret = CAEN_DGTZ_SWStopAcquisition(board->GetHandle());
  if(ret) printf("Error: CAEN_DGTZ_SWStopAcquisition\n ");
  
  std::cout<<"done !!!"<<std::endl;
  
  // if write data,close file


  std::cout<<"finish !!!"<<std::endl;
}

bool MainFrame::IsDirectoryExists(const char *path)
{
  struct stat fileStat;
  if ((stat(path, &fileStat) == 0) && S_ISDIR(fileStat.st_mode))
    return true;
  else
    return false;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int MainFrame::initDigitizer()
{
  int ret; 
  int MajorNumber;
  
  CAEN_DGTZ_SWStopAcquisition(0);
  CAEN_DGTZ_CloseDigitizer(0);
  
  ret = CAEN_DGTZ_OpenDigitizer(dig->linkType, dig->linkNum, dig->conetNode, dig->baseAddress, &dig->boardHandle);
  if (ret)
    {
      StateMsg->SetText("Can't open digitizer");
      return ret;
    }
  // ret = CAEN_DGTZ_SWStopAcquisition(dig->boardHandle);
  
  ret = CAEN_DGTZ_GetInfo(dig->boardHandle, dig->boardInfo);
  if (ret)
    {
      StateMsg->SetText("Get Board Info Error ...");
      return ret;
    }
  printf("Connected to CAEN Digitizer Model %s\n", dig->boardInfo->ModelName);
  printf("ROC FPGA Release is %s\n", dig->boardInfo->ROC_FirmwareRel);
  printf("AMC FPGA Release is %s\n", dig->boardInfo->AMC_FirmwareRel);
  printf("Model is %d\n", dig->boardInfo->Model);
  printf("Channels is %d\n", dig->boardInfo->Channels);
  printf("FormFactor is %d\n", dig->boardInfo->FormFactor);
  printf("FamilyCode is %d\n", dig->boardInfo->FamilyCode);
  printf("SerialNumber is %d\n", dig->boardInfo->SerialNumber);
  printf("PCB_Revision is %d\n", dig->boardInfo->PCB_Revision);
  printf("ADC_NBits is %d\n", dig->boardInfo->ADC_NBits);
  printf("CommHandle is %d\n", dig->boardInfo->CommHandle);
  printf("VMEHandle is %d\n", dig->boardInfo->VMEHandle);
  printf("License is %s\n", dig->boardInfo->License);


  sscanf(dig->boardInfo->AMC_FirmwareRel, "%d", &MajorNumber);
  if (MajorNumber >= 128)
    {
      printf("This digitizer has a DPP firmware\n");
      CheckLicense(dig);
    }

  
  // Get Number of Channels, Number of bits, Number of Groups of the board
  if(MajorNumber == STANDARD_FW_CODE)
    {
      if(GetMoreBoardInfo(dig->boardHandle, dig->boardInfo))
	{
	  // TODO
	}

    }

  
  // CAEN_DGTZ_Reset(dig->boardHandle);
  ret |= CAEN_DGTZ_Reset(dig->boardHandle);
  if (ret != 0)
    {
      // printf("Error: Unable to reset digitizer.\nPlease reset digitizer manually then restart the program\n");
      StateMsg->SetText("Please reset digitizer manually then restart the program");
      return -ret;
    }


  switch(dig->boardInfo->Model)
    {
    case CAEN_DGTZ_DT5720:
      if(strcmp(dig->boardInfo->ModelName, "DT5720") == 0)
	{
	  if(MajorNumber == V1720_DPP_PSD_CODE) board = new DT_PSD(dig,(char*)"DT5720_PSD");
	  for (int i = 0; i < 4; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	  if(MajorNumber == STANDARD_FW_CODE) board = new DT_Standard(dig,(char*)"DT5720_STD");
	  for (int i = 0; i < 4; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	}

      if(strcmp(dig->boardInfo->ModelName, "DT5720B") == 0)
	{
	  if(MajorNumber == V1720_DPP_PSD_CODE) board = new DT_PSD(dig,(char*)"DT5720B_PSD");
	  for (int i = 0; i < 4; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	  if(MajorNumber == STANDARD_FW_CODE) board = new DT_Standard(dig,(char*)"DT5720B_STD");
	  for (int i = 0; i < 4; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	}      
      break;

    case CAEN_DGTZ_V1724:
      if(strcmp(dig->boardInfo->ModelName, "V1724") == 0)
	{
	  if(MajorNumber == STANDARD_FW_CODE) board = new DT_Standard(dig,(char*)"V1724_STD");
	  for (int i = 0; i < 8; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	  if(MajorNumber == V1724_DPP_PHA_CODE) board = new DT_PHA(dig,(char*)"V1724_PHA");
	  for (int i = 0; i < 8; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	}
      break;

      
    case CAEN_DGTZ_DT5724:
      if(strcmp(dig->boardInfo->ModelName, "DT5724") == 0)
	{
	  if(MajorNumber == STANDARD_FW_CODE) board = new DT_Standard(dig,(char*)"DT5724_STD");
	  for (int i = 0; i < 4; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	}
      
      if(strcmp(dig->boardInfo->ModelName, "DT5724A") == 0)
	{
	  if(MajorNumber == STANDARD_FW_CODE) board = new DT_Standard(dig,(char*)"DT5724A_STD");
	  for (int i = 0; i < 2; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	}
      break;

    case CAEN_DGTZ_V1730:
      if(strcmp(dig->boardInfo->ModelName, "V1730") == 0)
	{
	  if(MajorNumber == V1730_DPP_PSD_CODE) board = new DT_PSD(dig,(char*)"V1730_PSD");
	  for (int i = 0; i < 16; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	  if(MajorNumber == STANDARD_FW_CODE) board = new DT_Standard(dig,(char*)"V1730_STD");
	  for (int i = 0; i < 16; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	  if(MajorNumber == V1730_DPP_PHA_CODE) board = new DT_PHA(dig,(char*)"V1730_PHA");
	  for (int i = 0; i < 16; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	}
      break;
      
    case CAEN_DGTZ_DT5730:
      if(strcmp(dig->boardInfo->ModelName, "DT5730") == 0)
	{
	  if(MajorNumber == V1730_DPP_PSD_CODE) board = new DT_PSD(dig,(char*)"DT5730_PSD");
	  for (int i = 0; i < 8; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	  if(MajorNumber == STANDARD_FW_CODE) board = new DT_Standard(dig,(char*)"DT5730_STD");
	  for (int i = 0; i < 8; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	  if(MajorNumber == V1730_DPP_PHA_CODE) board = new DT_PHA(dig,(char*)"DT5730_PHA");
	  for (int i = 0; i < 8; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	}
      break;

    case CAEN_DGTZ_DT5742:
      if(strcmp(dig->boardInfo->ModelName, "DT5742") == 0)
	{
	  if(MajorNumber == STANDARD_FW_CODE) board = new DT_Standard(dig,(char*)"DT5742_STD");
	  for (int i = 0; i < 16; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	}
      if(strcmp(dig->boardInfo->ModelName, "DT5742B") == 0)
	{
	  if(MajorNumber == STANDARD_FW_CODE) board = new DT_Standard(dig,(char*)"DT5742B_STD");
	  for (int i = 0; i < 16; ++i) ChannelsCheckButton[i]->SetEnabled(1);
	}
      break;
      
      
    default:
      // printf("not support this board now ...");
      StateMsg->SetText("Not support this board now ...");
      break;
    }

  if(board == NULL)
    {
      // printf("not support this board now ...");
      StateMsg->SetText("Not support this board now ...");
      return 100;
    }

  PrintRegistersButton->SetEnabled(1);
  ProgramButton->SetEnabled(1);
  AdjustParButton->SetEnabled(1);
  return ret;
}

void MainFrame::deleteDigitizer()
{
  if(board != NULL)
    {
      delete board;
      board = NULL;
    }
      
  CAEN_DGTZ_SWStopAcquisition(0);
  CAEN_DGTZ_CloseDigitizer(0);  
  AdjustParButton->SetEnabled(0);
  PrintRegistersButton->SetEnabled(0);
  StartStopButton->SetEnabled(0);
  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      statisticsgroup->HideFrame(channelstatisticsframe[i]);
    }
  StatisticsDataMsg->SetText("");
}

int MainFrame::GetMoreBoardInfo(int handle, CAEN_DGTZ_BoardInfo_t *BoardInfo)
{
  int ret = 0;
  CAEN_DGTZ_DRS4Frequency_t freq;

  switch(BoardInfo->FamilyCode)
    {
	      

    default:
      break;
    }
	  
  return ret;
}

void MainFrame::MarkChennelOrRecordLengthChange()
{
  if(!connectButton->IsEnabled()) ProgramButton->SetEnabled(1);
  AllocateButton->SetEnabled(0);
}

void MainFrame::ProgramDigitizer()
{
  int ret = 0;
  int MajorNumber;
  unsigned int enablemask = 0;
  int recordlength = 0;

  MonitorChannelBox->RemoveAll();
  
  if(!(MajorNumber == STANDARD_FW_CODE))
    {
      board->SetDPPAcquisitionMode( CAEN_DGTZ_DPP_AcqMode_t(DPPAcqMode->GetSelected()), CAEN_DGTZ_DPP_SaveParam_t(DPPSaveParam->GetSelected()));
    }
  
  board->SetAcquisitionMode(CAEN_DGTZ_SW_CONTROLLED);
  // std::cout<<"RecLen: "<<RecordLength->GetIntNumber()<<std::endl;
  board->SetRecordLength(RecordLength->GetIntNumber());

  board->SetIOLevel(CAEN_DGTZ_IOLevel_TTL);
  board->SetExtTriggerInputMode(CAEN_DGTZ_TRGMODE_ACQ_ONLY);// CAEN_DGTZ_TRGMODE_ACQ_ONLY CAEN_DGTZ_TRGMODE_DISABLED
  for (int i = 0; i < MAX_CHANNEL; ++i)
    {
      statisticsgroup->HideFrame(channelstatisticsframe[i]);
      if(ChannelsCheckButton[i]->IsEnabled() && ChannelsCheckButton[i]->IsOn())
	{
	  enablemask = SetBit_32((unsigned short)i,enablemask);
	  statisticsgroup->ShowFrame(channelstatisticsframe[i]);

	  sprintf(tmp,"Ch %02d",i);
	  MonitorChannelBox->AddEntry(tmp, i);
	}
    }

  board->SetChannelEnableMask(enablemask);
  board->SetChannelSelfTrigger(CAEN_DGTZ_TRGMODE_ACQ_ONLY);


  if(!(MajorNumber == STANDARD_FW_CODE))
    {
      board->SetDPPEventAggregation(0,0);
    }
  
  board->SetRunSynchronizationMode(CAEN_DGTZ_RUN_SYNC_Disabled);
  
  if(board->ProgramDigitizer())
    {
      // error
      StateMsg->SetText("Program Digitizer Error ...");
    }
  else
    {
      StateMsg->SetText("Program Digitizer OK ...");
    }

  board->GetRecordLength(&recordlength);
  RecordLength->SetIntNumber(recordlength);
  
  sscanf(dig->boardInfo->AMC_FirmwareRel, "%d", &MajorNumber);

  // Read again the board infos, just in case some of them were changed by the programming
  // (like, for example, the TSample and the number of channels if DES mode is changed)
  if(MajorNumber == STANDARD_FW_CODE)
    {
      ret = CAEN_DGTZ_GetInfo(dig->boardHandle, dig->boardInfo);
      if (ret)
	{
	  StateMsg->SetText("Get Board Info Second Error ...");
	}
      printf("===================================================\n");
      printf("Connected to CAEN Digitizer Model %s\n", dig->boardInfo->ModelName);
      printf("ROC FPGA Release is %s\n", dig->boardInfo->ROC_FirmwareRel);
      printf("AMC FPGA Release is %s\n", dig->boardInfo->AMC_FirmwareRel);
      printf("Model is %d\n", dig->boardInfo->Model);
      printf("Channels is %d\n", dig->boardInfo->Channels);
      printf("FormFactor is %d\n", dig->boardInfo->FormFactor);
      printf("FamilyCode is %d\n", dig->boardInfo->FamilyCode);
      printf("SerialNumber is %d\n", dig->boardInfo->SerialNumber);
      printf("PCB_Revision is %d\n", dig->boardInfo->PCB_Revision);
      printf("ADC_NBits is %d\n", dig->boardInfo->ADC_NBits);
      printf("CommHandle is %d\n", dig->boardInfo->CommHandle);
      printf("VMEHandle is %d\n", dig->boardInfo->VMEHandle);
      printf("License is %s\n", dig->boardInfo->License);

      if(GetMoreBoardInfo(dig->boardHandle, dig->boardInfo))
	{

	}

      
    }

  // const int allow_trigger_overlap_bit = 1<<1;
  // printf("wu === 0x8004  %d \n",CAEN_DGTZ_WriteRegister(dig->boardHandle, CAEN_DGTZ_BROAD_CH_CONFIGBIT_SET_ADD, allow_trigger_overlap_bit));
  
  OnlineCanvas->cd();
  OnlineCanvas->Clear();
  OnlineCanvas->Modified();
  OnlineCanvas->Update();
  board->InitMonitorGraph();
  ProgramButton->SetEnabled(0);
  AllocateButton->SetEnabled(1);

}


void MainFrame::AllocateMemory()
{
  if(board->AllocateMemory())
    {
      // error
      // printf("Allocate Memory error\n");
      StateMsg->SetText("Allocate Memory Error ...");
    }
  else
    {
      StateMsg->SetText("Allocate Memory OK ...");
    }

  AllocateButton->SetEnabled(0);
  StartStopButton->SetEnabled(1);
}

void MainFrame::AdjustParameters()
{
  new ParSetTable(fClient->GetRoot(), this,board);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MainFrame::MakeFoldPanelOnline(TGCompositeFrame *TabPanel)
{
  TGHorizontalFrame *monitorframe = new TGHorizontalFrame(TabPanel);
  TabPanel->AddFrame(monitorframe, new TGLayoutHints(kLHintsExpandX | kLHintsTop,0/*left*/,0/*right*/,0/*top*/,0/*bottom*/));
  
  MonitorCheckButton = new TGCheckButton(monitorframe,"&Monitor");
  monitorframe->AddFrame(MonitorCheckButton,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,4,5,3));
  MonitorCheckButton->SetTextColor(0xEE0000);
  MonitorCheckButton->SetState(kButtonDown);
  MonitorCheckButton->SetState(kButtonUp);
  // MonitorCheckButton->Connect("Clicked()","MainFrame",this,"");


  TGLabel *monitorchannellabel = new TGLabel(monitorframe,"Channel: ");
  monitorframe->AddFrame(monitorchannellabel,new TGLayoutHints(kLHintsLeft | kLHintsTop,10,3,5,0));
  
  MonitorChannelBox = new TGComboBox(monitorframe);
  monitorframe->AddFrame(MonitorChannelBox,new TGLayoutHints(kLHintsLeft | kLHintsTop,2,3,3,3));
  MonitorChannelBox->Resize(60, 20);
  MonitorChannelBox->Connect("Selected(Int_t)","MainFrame",this,"MonitorChannelSelect(int)");
  

  TGLabel *monitortypelabel = new TGLabel(monitorframe,"Type: ");
  monitorframe->AddFrame(monitortypelabel,new TGLayoutHints(kLHintsLeft | kLHintsTop,10,3,5,0));
  
  MonitorTypeBox = new TGComboBox(monitorframe);
  monitorframe->AddFrame(MonitorTypeBox,new TGLayoutHints(kLHintsLeft | kLHintsTop,2,3,3,3));
  MonitorTypeBox->Resize(100, 20);
  MonitorTypeBox->AddEntry("SingleWave",0);
  MonitorTypeBox->AddEntry("MultiWave",1);
  MonitorTypeBox->AddEntry("Energy",2);
  MonitorTypeBox->AddEntry("FFT(CAEN)",3);
  MonitorTypeBox->AddEntry("complexFFT(XIA)",4);
  MonitorTypeBox->Select(0);


  MonitorClearButton = new TGTextButton(monitorframe,"&Clear");
  monitorframe->AddFrame(MonitorClearButton,new TGLayoutHints(kLHintsRight | kLHintsTop,10,10,5,0));
  MonitorClearButton->Connect("Pressed()","MainFrame",this,"MonitorClear()");
  MonitorClearButton->SetEnabled(1);

  
  SendASoftwareTriggerButton = new TGTextButton(monitorframe,"&SendATrigger");
  monitorframe->AddFrame(SendASoftwareTriggerButton,new TGLayoutHints(kLHintsRight | kLHintsTop,10,10,5,0));
  SendASoftwareTriggerButton->Connect("Pressed()","MainFrame",this,"SendASoftwareTrigger()");
  SendASoftwareTriggerButton->SetEnabled(0);

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGCompositeFrame *CanvasFrame = new TGCompositeFrame(TabPanel, 60, 60, kHorizontalFrame);
  TabPanel->AddFrame(CanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));
  
  TRootEmbeddedCanvas *onlinecanvas = new TRootEmbeddedCanvas("OnlineCanvas", CanvasFrame, 100, 100);
  CanvasFrame->AddFrame(onlinecanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));
  
  OnlineCanvas = onlinecanvas->GetCanvas();
  OnlineCanvas->SetBorderMode(0); //no red frame
  
}

void MainFrame::SendASoftwareTrigger()
{
  int ret = 0;
  ret = CAEN_DGTZ_SendSWtrigger(board->GetHandle()); // Send a software trigger
  if(ret) std::cout<<"Error:  CAEN_DGTZ_SendSWtrigger"<<std::endl;
}

void MainFrame::MonitorClear()
{
  OnlineCanvas->Clear();
  board->ClearMonitorGraph();
  OnlineCanvas->Modified();
  OnlineCanvas->Update();
}

void MainFrame::MonitorChannelSelect(int id)
{
  printf("Select: %d\n",id);

  if(board->GetMonitorChannel() != id)
    {
      board->ClearMonitorGraph();

      board->SetMonitorChannel(id);
      board->SetUpdateSingleWaveform();
      board->SetUpdateSingleFFT();
    }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// 
// MainFrame.cc ends here
