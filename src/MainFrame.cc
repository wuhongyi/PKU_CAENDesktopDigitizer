// MainFrame.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 25 18:54:13 2016 (+0800)
// Last-Updated: 日 11月 27 10:29:48 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 89
// URL: http://wuhongyi.cn 

#include "MainFrame.hh"
#include "DT_PSD.hh"
#include "DT_Standard.hh"

#include <fstream>
#include <iostream>

#include <unistd.h>
#include <sys/stat.h>//stat(const char *file_name,struct stat *buf)
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ClassImp(MainFrame)

MainFrame::MainFrame(const TGWindow * p)
: TGMainFrame(p)
{
  dig = NULL;
  board = NULL;

  
  
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

  TGCompositeFrame *Tab2 = TabPanel->AddTab("Par");
  MakeFoldPanelPar(Tab2);


  // What to clean up in dtor
  SetCleanup(kDeepCleanup);
  SetWindowName("PKU Digitizer");
  MapSubwindows();
  Resize(GetDefaultSize());
  MapWindow();

  // Print();
}

MainFrame::~MainFrame()
{
  free(dig);
  
  // Clean up all widgets, frames and layouthints that were used
  Cleanup();
  
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
		  // StateMsg->SetText("Connect error ......");
		  BoardNameMsg->SetText("");
		}
	      else
		{
		  BoardNameMsg->SetText(board->GetName());
		  StateMsg->SetText("Boot success !");
		  connectButton->SetEnabled(0);
		  DeleteButton->SetEnabled(1);
		}
	      break;

	    case DELETEBUTTON:
	      deleteDigitizer();
	      connectButton->SetEnabled(1);
	      DeleteButton->SetEnabled(0);
	      StateMsg->SetText("Please enter Connect");
	      BoardNameMsg->SetText("");
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

  

  TGGroupFrame *controlgroup = new TGGroupFrame(TabPanel,"Control");
  TabPanel->AddFrame(controlgroup,new TGLayoutHints(kLHintsExpandX | kLHintsTop));

  TGHorizontalFrame *cgrouphframe = new TGHorizontalFrame(controlgroup);
  controlgroup->AddFrame(cgrouphframe,new TGLayoutHints(kLHintsLeft | kLHintsTop,10,4,3,3));

  StartStopButton = new TGTextButton(cgrouphframe,"&Start/Stop");
  cgrouphframe->AddFrame(StartStopButton,new TGLayoutHints(kLHintsLeft | kLHintsTop));
  StartStopButton->Connect("Pressed()","MainFrame",this,"StartStopRun()");
  StartStopButton->SetEnabled(0);

  OnlineCheckButton = new TGCheckButton(cgrouphframe,"&Online data");
  cgrouphframe->AddFrame( OnlineCheckButton,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,4,3,3));
  OnlineCheckButton->SetTextColor(0x000080);
  OnlineCheckButton->SetState(kButtonDown);
  OnlineCheckButton->Connect("Clicked()","MainFrame",this,"SetOnlineData()");


  WtiteDataButton = new TGTextButton(cgrouphframe,"&WriteData");
  cgrouphframe->AddFrame(WtiteDataButton,new TGLayoutHints(kLHintsLeft | kLHintsTop));
  WtiteDataButton->Connect("Pressed()","MainFrame",this,"SetWriteData()");
  WtiteDataButton->SetEnabled(0);






  

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

void MainFrame::StartStopRun()
{

  
}


bool MainFrame::IsDirectoryExists(const char *path)
{
  struct stat fileStat;
  if ((stat(path, &fileStat) == 0) && S_ISDIR(fileStat.st_mode))
    return true;
  else
    return false;
}

void MainFrame::MakeFoldPanelPar(TGCompositeFrame *TabPanel)
{


  
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
      // printf("Can't open digitizer\n");
      StateMsg->SetText("Can't open digitizer");
      return ret;
    }
  
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
  if (MajorNumber >= 128) {
    printf("This digitizer has a DPP firmware\n");
  }

  // Get Number of Channels, Number of bits, Number of Groups of the board
  if(MajorNumber == STANDARD_FW_CODE)
    {
      if(GetMoreBoardInfo(dig->boardHandle, dig->boardInfo))
	{

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
	}
      
      if(strcmp(dig->boardInfo->ModelName, "DT5724A") == 0)
	{
	  if(MajorNumber == STANDARD_FW_CODE) board = new DT_Standard(dig,(char*)"DT5724A_STD");
	}
      break;
      
    case CAEN_DGTZ_DT5730:
      if(strcmp(dig->boardInfo->ModelName, "DT5730") == 0)
	{
	  if(MajorNumber == V1730_DPP_PSD_CODE) board = new DT_PSD(dig,(char*)"DT5730_PSD");
	}
      break;

    case CAEN_DGTZ_DT5742:
      if(strcmp(dig->boardInfo->ModelName, "DT5742") == 0)
	{
	  if(MajorNumber == STANDARD_FW_CODE) board = new DT_Standard(dig,(char*)"DT5742_STD");
	}
      if(strcmp(dig->boardInfo->ModelName, "DT5742B") == 0)
	{
	  if(MajorNumber == STANDARD_FW_CODE) board = new DT_Standard(dig,(char*)"DT5742B_STD");
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


  if(board->ProgramDigitizer())
    {
      // error
      // printf("Program Digitizer error\n");
      StateMsg->SetText("Program Digitizer Error ...");
      return 100;
    }

  // Read again the board infos, just in case some of them were changed by the programming
  // (like, for example, the TSample and the number of channels if DES mode is changed)
  if(MajorNumber == STANDARD_FW_CODE)
    {
      ret = CAEN_DGTZ_GetInfo(dig->boardHandle, dig->boardInfo);
      if (ret)
	{
	  StateMsg->SetText("Get Board Info Second Error ...");
	  return ret;
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
  
  if(board->AllocateMemory())
    {
      // error
      // printf("Allocate Memory error\n");
      StateMsg->SetText("Allocate Memory Error ...");
      return 100;
    }
  

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

// 
// MainFrame.cc ends here










