; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "JLogServer.h"
LastPage=0

ClassCount=6
Class1=CJLogServerApp
Class2=CJLogServerDoc
Class3=CJLogServerView
Class4=CMainFrame

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Class6=CCfgListen
Resource3=IDD_CFG_LISTEN

[CLS:CJLogServerApp]
Type=0
HeaderFile=JLogServer.h
ImplementationFile=JLogServer.cpp
Filter=N
LastObject=CJLogServerApp

[CLS:CJLogServerDoc]
Type=0
HeaderFile=JLogServerDoc.h
ImplementationFile=JLogServerDoc.cpp
Filter=N
LastObject=CJLogServerDoc

[CLS:CJLogServerView]
Type=0
HeaderFile=JLogServerView.h
ImplementationFile=JLogServerView.cpp
Filter=C


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_SERVER9
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=JLogServer.cpp
ImplementationFile=JLogServer.cpp
Filter=D
LastObject=ID_APP_ABOUT

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_CFG_LISTEN
Command8=ID_CLEAN_LOG
Command9=ID_SERVER1
Command10=ID_SERVER2
Command11=ID_SERVER3
Command12=ID_SERVER4
Command13=ID_SERVER5
Command14=ID_SERVER6
Command15=ID_SERVER7
Command16=ID_SERVER8
Command17=ID_SERVER9
Command18=ID_SERVER10
Command19=ID_VIEW_STATUS_BAR
Command20=ID_APP_ABOUT
CommandCount=20

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_NEXT_PANE
Command5=ID_PREV_PANE
CommandCount=5

[DLG:IDD_CFG_LISTEN]
Type=1
Class=CCfgListen
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC1,static,1342308352
Control4=IDC_STATIC2,static,1342308352
Control5=IDC_EDIT_ADDRESS,edit,1350631552
Control6=IDC_EDIT_PORT,edit,1350631552

[CLS:CCfgListen]
Type=0
HeaderFile=CfgListen.h
ImplementationFile=CfgListen.cpp
BaseClass=CDialog
Filter=D
LastObject=CCfgListen
VirtualFilter=dWC

