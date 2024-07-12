; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CURLTesterDlg
LastTemplate=CHeaderCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "URLTester.h"

ClassCount=11
Class1=CURLTesterApp
Class2=CURLTesterDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDR_MENU_MAIN
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CIPEdit
Class5=CDNSEdit
Class6=CURLListCtrl
Class7=CURLComboBoxEx
Class8=CURLEdit
Class9=CPortEdit
Class10=CMyEdit
Class11=CURLHeaderCtrl
Resource4=IDD_URLTESTER_DIALOG
Resource5=IDS_TRAY_ICON

[CLS:CURLTesterApp]
Type=0
HeaderFile=URLTester.h
ImplementationFile=URLTester.cpp
Filter=N

[CLS:CURLTesterDlg]
Type=0
HeaderFile=URLTesterDlg.h
ImplementationFile=URLTesterDlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_COMBO_HTTP_VERSION

[CLS:CAboutDlg]
Type=0
HeaderFile=URLTesterDlg.h
ImplementationFile=URLTesterDlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177294
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_EDIT_EMAIL,edit,1342244864
Control6=IDC_EDIT_BLOG,edit,1342244992

[DLG:IDD_URLTESTER_DIALOG]
Type=1
Class=CURLTesterDlg
ControlCount=46
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC_RESULT,button,1342177287
Control4=IDC_LIST_RESULT,SysListView32,1350631425
Control5=IDC_STATIC_REQUEST,button,1342177287
Control6=IDC_EDIT_LOG_REQUEST,edit,1353777412
Control7=IDC_EDIT_DNS,edit,1350631552
Control8=IDC_BUTTON_ADD_DNS,button,1342242816
Control9=IDC_EDIT_IP,edit,1350631552
Control10=IDC_BUTTON_ADD_IP,button,1342242816
Control11=IDC_BUTTON_START_BATCH,button,1342242816
Control12=IDC_BUTTON_STOP,button,1476460544
Control13=IDC_STATIC_PARAMETER,button,1342177287
Control14=IDC_STATIC_PORT,static,1342308352
Control15=IDC_COMBO_PORT,combobox,1344339970
Control16=IDC_STATIC_TIMEOUT,static,1342308352
Control17=IDC_COMBO_TIMEOUT,combobox,1344339971
Control18=IDC_BUTTON_EXIT,button,1073807360
Control19=IDC_BUTTON_DNS_SELECT_ALL,button,1342242816
Control20=IDC_BUTTON_DNS_UNSELECT,button,1342242816
Control21=IDC_BUTTON_DNS_DELETE,button,1342242816
Control22=IDC_BUTTON_IP_SELECT_ALL,button,1342242816
Control23=IDC_BUTTON_UNSELECT,button,1342242816
Control24=IDC_BUTTON_IP_DELETE,button,1342242816
Control25=IDC_LIST_DNS,SysListView32,1350631433
Control26=IDC_LIST_IP,SysListView32,1350631433
Control27=IDC_STATIC_FILE,button,1342193671
Control28=IDC_COMBO_FILE,combobox,1344340034
Control29=IDC_BUTTON_START_QUICK,button,1342242817
Control30=IDC_STATIC_METHOD,static,1342308352
Control31=IDC_COMBO_METHOD,combobox,1344339971
Control32=IDC_STATIC_RESPONSE,button,1342177287
Control33=IDC_EDIT_LOG_RESPONSE,edit,1353777412
Control34=IDC_BUTTON_RESULT_DELETE,button,1342242816
Control35=IDC_BUTTON_RESULT_SELECT_ALL,button,1342242816
Control36=IDC_BUTTON_RESULT_UNSELECT,button,1342242816
Control37=IDC_BUTTON_RESULT_EXPORT,button,1342242816
Control38=IDC_BUTTON_RESULT_CLEAR,button,1342242816
Control39=IDC_STATIC_HEAD,button,1342259207
Control40=IDC_COMBO_REQUEST_HEAD,combobox,1344340034
Control41=IDC_PROGRESS,msctls_progress32,1082130432
Control42=IDC_STATIC,static,1342308352
Control43=IDC_STATIC,static,1342308352
Control44=IDC_BUTTON_PAUSE,button,1476460544
Control45=IDC_STATIC_HTTP_VERSION,static,1342308352
Control46=IDC_COMBO_HTTP_VERSION,combobox,1344340227

[MNU:IDR_MENU_MAIN]
Type=1
Class=CURLTesterDlg
Command1=IDM_IMPORT_DNS
Command2=IDM_IMPORT_IP
Command3=IDM_EXIT
Command4=IDM_HELP_MANUAL
Command5=IDM_HELP_ABOUT
CommandCount=5

[CLS:CIPEdit]
Type=0
HeaderFile=IPEdit.h
ImplementationFile=IPEdit.cpp
BaseClass=CEdit
Filter=W
VirtualFilter=WC
LastObject=CIPEdit

[CLS:CDNSEdit]
Type=0
HeaderFile=DNSEdit.h
ImplementationFile=DNSEdit.cpp
BaseClass=CEdit
Filter=W
VirtualFilter=WC
LastObject=CDNSEdit

[CLS:CURLListCtrl]
Type=0
HeaderFile=URLListCtrl.h
ImplementationFile=URLListCtrl.cpp
BaseClass=CListCtrl
Filter=W
LastObject=CURLListCtrl
VirtualFilter=FWC

[CLS:CURLComboBoxEx]
Type=0
HeaderFile=URLComboBoxEx.h
ImplementationFile=URLComboBoxEx.cpp
BaseClass=CComboBoxEx
Filter=W
VirtualFilter=cWC
LastObject=CURLComboBoxEx

[CLS:CURLEdit]
Type=0
HeaderFile=URLEdit.h
ImplementationFile=URLEdit.cpp
BaseClass=CEdit
Filter=W
LastObject=CURLEdit

[CLS:CPortEdit]
Type=0
HeaderFile=PortEdit.h
ImplementationFile=PortEdit.cpp
BaseClass=CEdit
Filter=W
VirtualFilter=WC
LastObject=CPortEdit

[CLS:CMyEdit]
Type=0
HeaderFile=MyEdit.h
ImplementationFile=MyEdit.cpp
BaseClass=CEdit
Filter=W
VirtualFilter=WC
LastObject=CMyEdit

[CLS:CURLHeaderCtrl]
Type=0
HeaderFile=URLHeaderCtrl.h
ImplementationFile=URLHeaderCtrl.cpp
BaseClass=CHeaderCtrl
Filter=W
VirtualFilter=JWC
LastObject=CURLHeaderCtrl

[MNU:IDS_TRAY_ICON]
Type=1
Class=CAboutDlg
Command1=IDM_TRAY_SHOWorHIDE
Command2=IDM_TRAY_EXIT
CommandCount=2

