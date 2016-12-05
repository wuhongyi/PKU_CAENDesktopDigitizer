// MainFrame.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 18:54:13 2016 (+0800)
// Last-Updated: 一 12月  5 21:34:28 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 233
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

  SingleWaveform = NULL;
  MultiWaveform = NULL;
  OnlineCanvas = NULL;
    
  dig = (Digitizer*) calloc(1,sizeof(Digitizer));
  dig->boardInfo = (CAEN_DGTZ_BoardInfo_t*) calloc(1,sizeof(CAEN_DGTZ_BoardInfo_t));

  dig->linkType = CAEN_DGTZ_USB;
  dig->linkNum = 0;
  dig->conetNode = 0;
  dig->baseAddress = 0;

  
  
  TGTab *TabPanel = new TGTab(this);
  this->AddFrame(TabPanel, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  TGCompositeFrame *Tab1 = TabPanel->AddTab("Init");
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
  
  // Print();
}

MainFrame::~MainFrame()
{
  free(dig);
  
  // Clean up all widgets, frames and layouthints that were used
  Cleanup();
  
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
      char text[100];
      printf("Warning: firmware is unlicensed. Remaining demo mode time: %d minutes\n", minleft);
    }
}


void MainFrame::CloseWindow()
{
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

  std::cout<<msg <<"  "<< parm1<<"  "<<parm2<<std::endl;
  std::cout<<GET_MSG(msg)<<"  "<<GET_SUBMSG(msg)<<std::endl;

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
  
  TGGroupFrame *controlgroup = new TGGroupFrame(TabPanel,"Control");
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
  TabPanel->AddFrame(statisticsgroup,new TGLayoutHints(kLHintsExpandX | kLHintsTop));

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
  const char *path=filepathtext->GetText();
  const char *filen=filenametext->GetText();
  runnum=(int)filerunnum->GetIntNumber();

  sprintf(Filename,"%s%s_R%04d.bin",path,filen,runnum);

  if(IsDirectoryExists(filepathtext->GetText()))
    {
      std::ofstream out("../parset/Run.config");
      out<<filepathtext->GetText()<<std::endl;
      out<<filenametext->GetText()<<std::endl;
      out.close();

      // StartStopButton->SetEnabled(1);
      // StartStopButton->SetText("Start");
    }
  else
    {
      std::cout<<"The output file directory does not exist"<<std::endl;
    }
}

void MainFrame::PrintRegisters()
{
  Read_DGTZ_Register(board->GetHandle(),board->GetChannels());
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
	      board->GetWaveform();

	      
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


  
  ret |= CAEN_DGTZ_Reset(dig->boardHandle);
  if (ret != 0)
    {
      // printf("Error: Unable to reset digitizer.\nPlease reset digitizer manually then restart the program\n");
      StateMsg->SetText("Please reset digitizer manually then restart the program");
      return -ret;
    }


  
  switch(dig->boardInfo->Model)
    {
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
  MonitorChannelBox->AddEntry("Select",-1);
  
  if(!(MajorNumber == STANDARD_FW_CODE))
    {
      board->SetDPPAcquisitionMode(CAEN_DGTZ_DPP_ACQ_MODE_Mixed, CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime);
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
  

  if(SingleWaveform != NULL)
    {
      delete SingleWaveform;
      SingleWaveform = NULL;
    }
  if(MultiWaveform != NULL)
    {
      delete MultiWaveform;
      MultiWaveform = NULL;
    }

  SingleWaveform = new TGraph();
  MultiWaveform = new TH2I("MultiWaveform","",recordlength,0,recordlength,4096,0,1<<board->GetNBits());
  
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
  monitorframe->AddFrame(MonitorCheckButton,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,4,3,3));
  MonitorCheckButton->SetTextColor(0xEE0000);
  MonitorCheckButton->SetState(kButtonDown);
  MonitorCheckButton->SetState(kButtonUp);
  // MonitorCheckButton->Connect("Clicked()","MainFrame",this,"");

  MonitorChannelBox = new TGComboBox(monitorframe);
  monitorframe->AddFrame(MonitorChannelBox,new TGLayoutHints(kLHintsLeft | kLHintsTop,10,3,3,03));
  MonitorChannelBox->Resize(60, 20);
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGCompositeFrame *CanvasFrame = new TGCompositeFrame(TabPanel, 60, 60, kHorizontalFrame);
  TabPanel->AddFrame(CanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));
  
  TRootEmbeddedCanvas *onlinecanvas = new TRootEmbeddedCanvas("OnlineCanvas", CanvasFrame, 100, 100);
  CanvasFrame->AddFrame(onlinecanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));
  
  OnlineCanvas = onlinecanvas->GetCanvas();
  OnlineCanvas->SetBorderMode(0); //no red frame

  

  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MainFrame::Read_DGTZ_Register(int handle,int MaxNChInBoard)
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
// MainFrame.cc ends here
