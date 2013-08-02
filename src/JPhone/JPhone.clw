; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCfgDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "JPhone.h"

ClassCount=5
Class1=CJPhoneApp
Class2=CJPhoneDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_CONTACT_DIALOG
Class4=CCfgDlg
Resource4=IDD_CFG_DIALOG
Resource5=IDD_JPHONE_DIALOG
Class5=CContactDlg
Resource6=IDR_MENU_POP

[CLS:CJPhoneApp]
Type=0
HeaderFile=JPhone.h
ImplementationFile=JPhone.cpp
Filter=N
LastObject=CJPhoneApp

[CLS:CJPhoneDlg]
Type=0
HeaderFile=JPhoneDlg.h
ImplementationFile=JPhoneDlg.cpp
Filter=D
LastObject=ID_POP_CFG
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=JPhoneDlg.h
ImplementationFile=JPhoneDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_JPHONE_DIALOG]
Type=1
Class=CJPhoneDlg
ControlCount=15
Control1=IDC_STATIC_DISPLAY,static,1342308865
Control2=IDC_BUTTON_OFFHOOK,button,1342242816
Control3=IDC_BUTTON_ONHOOK,button,1342242816
Control4=IDC_BUTTON_1,button,1342242816
Control5=IDC_BUTTON_2,button,1342242816
Control6=IDC_BUTTON_3,button,1342242816
Control7=IDC_BUTTON_4,button,1342242816
Control8=IDC_BUTTON_5,button,1342242816
Control9=IDC_BUTTON_6,button,1342242816
Control10=IDC_BUTTON_7,button,1342242816
Control11=IDC_BUTTON_8,button,1342242816
Control12=IDC_BUTTON_9,button,1342242816
Control13=IDC_BUTTON_STAR,button,1342242816
Control14=IDC_BUTTON_0,button,1342242816
Control15=IDC_BUTTON_POUND,button,1342242816

[DLG:IDD_CFG_DIALOG]
Type=1
Class=CCfgDlg
ControlCount=21
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC1,static,1342308352
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_STATIC2,static,1342308352
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_STATIC3,static,1342308352
Control7=IDC_EDIT3,edit,1350631552
Control8=IDC_STATIC4,static,1342308352
Control9=IDC_EDIT4,edit,1350631552
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC5,static,1342308352
Control12=IDC_EDIT5,edit,1350631552
Control13=IDC_STATIC6,static,1342308352
Control14=IDC_EDIT6,edit,1350631552
Control15=IDC_CHECK1,button,1342242819
Control16=IDC_CHECK2,button,1342242819
Control17=IDC_STATIC,button,1342177287
Control18=IDC_RADIO3,button,1342308361
Control19=IDC_RADIO4,button,1342177289
Control20=IDOK,button,1342242817
Control21=IDCANCEL,button,1342242816

[CLS:CCfgDlg]
Type=0
HeaderFile=CfgDlg.h
ImplementationFile=CfgDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCfgDlg

[MNU:IDR_MENU_POP]
Type=1
Class=?
Command1=ID_POP_CFG
Command2=ID_POP_CONTACT
Command3=ID_POP_DUMP_MEMORY
Command4=ID_POP_ABOUT
Command5=ID_POP_EXIT
CommandCount=5

[DLG:IDD_CONTACT_DIALOG]
Type=1
Class=CContactDlg
ControlCount=1
Control1=IDC_TREE_CONTACT,SysTreeView32,1350631431

[CLS:CContactDlg]
Type=0
HeaderFile=ContactDlg.h
ImplementationFile=ContactDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CContactDlg
VirtualFilter=dWC

