// URLTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include <curl/curl.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "URLTester.h"
#include "URLTesterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LINE_LENGTH_MAX   1024 // The max length of a line text
#define RESPONSE_SIZE_MAX 8192 // The max size of text showed in response memo
#define FILE_LENGTH_MAX   1024
#define HEAD_LENGTH_MAX   8192
#define LIST_COUNT_MAX    10000
#define RESULT_COUNT_MAX  1000000

#define GET_TEXT(Ctrl, str) \
    Ctrl.GetWindowText(str); \
    str.TrimLeft(); \
    str.TrimRight()

// 设置ListCtrl控件的属性
// CheckBox，行选，无格
inline void SET_LISTCTRL_STYLE(CURLListCtrl& m_lv)
{
	LONG lStyle;
	lStyle = GetWindowLong(m_lv.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(m_lv.m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = 0;
	/*dwStyle = ::GetWindowLong(m_lv.GetSafeHwnd(), GWL_STYLE);*/
	dwStyle = LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT;
	dwStyle &= ~LVS_EX_GRIDLINES;
	m_lv.SetExtendedStyle(dwStyle);
	/*ListView_SetExtendedListViewStyleEx(m_lv.GetSafeHwnd(), dwStyle, dwStyle); */
}

typedef bool (*TCheckFunc)(const char*);
inline bool ADD_LISTCTRL_ITEM_BY_TEXT(const char* str,CURLListCtrl& ListCtrl,TCheckFunc CheckFunc,const char* ErrMsg)
{
    CString str1(str);
    if (str1.IsEmpty()) {
		if ((ErrMsg != NULL) && (ErrMsg[0] != 0))
			AfxMessageBox(ErrMsg);
		return false;
    }

    if (CheckFunc((LPCTSTR)str1)) {
		bool bExist = false;
		for (int i=0; i<ListCtrl.GetItemCount(); ++i) {
		CString str2 = ListCtrl.GetItemText(i, 1);
		str2.TrimLeft();
		str2.TrimRight();
		if (str2 == str1) {
			if ((ErrMsg != NULL) && (ErrMsg[0] != 0)) {
				CString strErrMsg;
				strErrMsg.Format("“%s”已经存在，不能重复添加。", (LPCTSTR)str1);
				AfxMessageBox(strErrMsg);
			}
			bExist = true;
			break;
		}
		}
		if (!bExist) {
			if (ListCtrl.GetItemCount() > LIST_COUNT_MAX) {
				AfxMessageBox("数据太多，不能再插入。");
				return false;
			}
			int nRow = ListCtrl.InsertItem(ListCtrl.GetItemCount(), _T("0"));
			ListCtrl.SetItemText(nRow, 1, (LPCTSTR)str1);
		}
    }
    else {
		if ((ErrMsg != NULL) && (ErrMsg[0] != 0))
			AfxMessageBox(ErrMsg);
    }
	return true;
}

inline  bool ADD_LISTCTRL_ITEM_BY_EDITCTRL(CEdit& EditCtrl,CURLListCtrl& ListCtrl,TCheckFunc CheckFunc,const char* ErrMsg)
{
    CString str;
    GET_TEXT(EditCtrl, str);
    return ADD_LISTCTRL_ITEM_BY_TEXT(str,ListCtrl,CheckFunc,ErrMsg);
}

inline void ADD_PROCESS_LOG(CEdit& EditCtrl, const CString& LogStr)
{
    CString str;
	EditCtrl.GetWindowText(str);
	if (EditCtrl.GetWindowTextLength() > 1024000)
	str = str.Mid(str.GetLength()/10);
    str += LogStr;
    str += "\r\n";
    EditCtrl.SetWindowText(str);
}

static ClxDialog::DLGCTLINFO dcMenuGroup[] =
{
	{IDC_STATIC_RESULT,      ClxDialog::ELASTICX, 100},
	{IDC_LIST_RESULT,        ClxDialog::ELASTICX, 100},
	{IDC_STATIC_FILE,        ClxDialog::ELASTICX, 100},
	{IDC_STATIC_HEAD,        ClxDialog::ELASTICX, 100},
	{IDC_COMBO_FILE,         ClxDialog::ELASTICX, 100},
	{IDC_COMBO_REQUEST_HEAD, ClxDialog::ELASTICX, 100},
	{IDC_STATIC_RESPONSE,    ClxDialog::ELASTICX, 100},
	{IDC_EDIT_LOG_RESPONSE,  ClxDialog::ELASTICX, 100},

	{IDC_STATIC_RESPONSE,    ClxDialog::ELASTICY, 100},
	{IDC_EDIT_LOG_RESPONSE,  ClxDialog::ELASTICY, 100},
	{IDC_STATIC_REQUEST,     ClxDialog::ELASTICY, 100},
	{IDC_EDIT_LOG_REQUEST,   ClxDialog::ELASTICY, 100},	
	{IDC_STATIC_PARAMETER,   ClxDialog::ELASTICY, 100},

	{IDC_STATIC_PARAMETER,   ClxDialog::MOVEX,    100},
	{IDC_STATIC_PORT,        ClxDialog::MOVEX,    100},
	{IDC_STATIC_TIMEOUT,     ClxDialog::MOVEX,    100},
	{IDC_STATIC_METHOD,      ClxDialog::MOVEX,    100},
	{IDC_COMBO_PORT,         ClxDialog::MOVEX,    100},
	{IDC_COMBO_TIMEOUT,      ClxDialog::MOVEX,    100},
	{IDC_COMBO_METHOD,       ClxDialog::MOVEX,    100},
    {IDC_STATIC_HTTP_VERSION,ClxDialog::MOVEX,    100},
    {IDC_COMBO_HTTP_VERSION, ClxDialog::MOVEX,    100},
    

	{IDC_BUTTON_START_BATCH, ClxDialog::MOVEX,    100},
	{IDC_BUTTON_START_QUICK, ClxDialog::MOVEX,    100},
	{IDC_BUTTON_STOP,        ClxDialog::MOVEX,    100},
	{IDC_BUTTON_PAUSE,       ClxDialog::MOVEX,    100},
	{IDC_BUTTON_EXIT,        ClxDialog::MOVEX,    100},

	{IDC_STATUSBAR_MAIN,     ClxDialog::MOVEY,    100},
	{IDC_STATUSBAR_MAIN,     ClxDialog::ELASTICX, 100},
	{IDC_PROGRESS,           ClxDialog::MOVEY,    100},
	{IDC_PROGRESS,           ClxDialog::ELASTICX, 100}	
};

bool IsPort(CString& str)
{
	if (str.IsEmpty()) return false;
	if ('0' == str[0]) return false;

	for (int i=0; i<str.GetLength(); ++i) { // 65535
		if (i > 4) return false;
		if ((str[i] < '0') || (str[i] > '9'))
			return false;
	}

	return (atoi(str) > 65535) ? false : true;
}
CString DWORDToString(DWORD dw)
{
    char buf[30] = {0};
    _snprintf(buf, sizeof(buf)-1, "%lu", dw);
    return buf;
}

bool IsValidIP(const char* str)
{
    int section = 0;    
    int nDotCount = 0;
    char data[4] = {0};
    char* datap = data;
    const char* strp = str;
    while (true) {
        if (('.' == *strp) || ('\0' == *strp)) {
            if (('\0' == *strp) && (nDotCount != 3)) return false;
            
            ++nDotCount;
            if (nDotCount > 4) return false;

            *datap = 0;
            section = atoi(data);
            if ((section < 0) || (section > 255)) return false;            
            if ('\0' == *strp) break;
            
            ++strp;
            datap = data;
        }
        else 
        {
            *datap++ = *strp++;
            if (datap-data > 3) return false;
        }
    }

    return true;
}
bool IsValidDNS(const char* str)
{
    return true;
}

CString GetLastErrorString()
{
    CString strMsg;
    LPVOID lpMsgBuf = NULL;
    ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
                 |FORMAT_MESSAGE_FROM_SYSTEM
                 |FORMAT_MESSAGE_IGNORE_INSERTS,
                 NULL,
                 ::GetLastError(),
                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                 (LPTSTR)&lpMsgBuf,
                 0,
                 NULL);
    
    strMsg = (LPCSTR)lpMsgBuf;
    ::LocalFree(lpMsgBuf);
    return strMsg;
}

void Trim(char* str)
{
    char* strp = str;
    size_t len1 = strlen(str);    
    char* str2 = new char[len1+1];

    while (true)
    {
        if ((' ' == *strp)
         || ('\t' == *strp)
         || ('\r' == *strp)
         || ('n' == *strp)
         ) {
            ++strp;
        }
        else {
            break;
        }
    }

    strcpy(str2, str);
    for (int i=len1; i>0; --i) {
        if (((' ' == str2[i-1])
         || ('\t' == str2[i-1])
         || ('\r' == str2[i-1])
         || ('\n' == str2[i-1])
         ))
        {
            ;
        }
        else
        {
            str2[i] = 0;
            break;
        }
    }
    
    strcpy(str, str2);
    delete []str2;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CEdit	m_edBlog;
	CEdit	m_edEmail;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_EDIT_BLOG, m_edBlog);
	DDX_Control(pDX, IDC_EDIT_EMAIL, m_edEmail);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strEmail = "eyjian@qq.com";
	m_edEmail.SetWindowText(strEmail);
	
    CString strBlog = "博客：http://aquester.cublog.cn";
    m_edBlog.SetWindowText(strBlog);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CURLTesterDlg dialog

CURLTesterDlg::CURLTesterDlg(CWnd* pParent /*=NULL*/)
	: ClxDialog(CURLTesterDlg::IDD, pParent), m_trayIcon(IDS_TRAY_ICON)
{
	//{{AFX_DATA_INIT(CURLTesterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_bQuickTest = true;
    m_dwThreadId = 0;
    m_hWorkThread = INVALID_HANDLE_VALUE;
    m_pURLSocket = NULL;

	m_szHomeDir[0] = 0;
    m_szConfFilename[0] = 0;
	memset(&m_StartParam, 0, sizeof(m_StartParam));
    
    m_cHttpVersion = '0';
}
CURLTesterDlg::~CURLTesterDlg()
{
    delete m_pURLSocket;
}

void CURLTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CURLTesterDlg)
	DDX_Control(pDX, IDC_BUTTON_PAUSE, m_buPause);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_BUTTON_UNSELECT_IP, m_buUnselectIP);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_buStop);
	DDX_Control(pDX, IDC_BUTTON_START_QUICK, m_buStartQuick);
	DDX_Control(pDX, IDC_BUTTON_START_BATCH, m_buStartBatch);
	DDX_Control(pDX, IDC_BUTTON_IP_SELECT_ALL, m_buSELECTALLIP);
	DDX_Control(pDX, IDC_BUTTON_IP_DELETE, m_buDelIP);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_buExit);
	DDX_Control(pDX, IDC_BUTTON_DNS_UNSELECT, m_buUnselectDNS);
	DDX_Control(pDX, IDC_BUTTON_DNS_SELECT_ALL, m_buSELECTALLDNS);
	DDX_Control(pDX, IDC_BUTTON_DNS_DELETE, m_buDelDNS);
	DDX_Control(pDX, IDC_BUTTON_ADD_IP, m_buAddIP);
	DDX_Control(pDX, IDC_BUTTON_ADD_DNS, m_buAddDNS);
	DDX_Control(pDX, IDC_EDIT_LOG_REQUEST, m_edRequest);
	DDX_Control(pDX, IDC_EDIT_LOG_RESPONSE, m_edResponse);
	DDX_Control(pDX, IDC_COMBO_TIMEOUT, m_cbTimeout);
	DDX_Control(pDX, IDC_COMBO_PORT, m_cbPort);
	DDX_Control(pDX, IDC_COMBO_METHOD, m_cbMethod);
	DDX_Control(pDX, IDC_COMBO_REQUEST_HEAD, m_cbHead);
	DDX_Control(pDX, IDC_COMBO_FILE, m_cbFile);
	DDX_Control(pDX, IDC_EDIT_IP, m_edIP);
	DDX_Control(pDX, IDC_EDIT_DNS, m_edDNS);
	DDX_Control(pDX, IDC_LIST_IP, m_lvIP);
	DDX_Control(pDX, IDC_LIST_DNS, m_lvDNS);
	DDX_Control(pDX, IDC_LIST_RESULT, m_lvResult);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CURLTesterDlg, CDialog)
	//{{AFX_MSG_MAP(CURLTesterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_ADD_DNS, OnButtonAddDns)
	ON_BN_CLICKED(IDC_BUTTON_DNS_SELECT_ALL, OnButtonDnsSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_DNS_UNSELECT, OnButtonDnsUnselect)
	ON_BN_CLICKED(IDC_BUTTON_DNS_DELETE, OnButtonDnsDelete)
	ON_BN_CLICKED(IDC_BUTTON_ADD_IP, OnButtonAddIp)
	ON_BN_CLICKED(IDC_BUTTON_IP_SELECT_ALL, OnButtonIpSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_UNSELECT, OnButtonUnselect)
	ON_BN_CLICKED(IDC_BUTTON_IP_DELETE, OnButtonIpDelete)
	ON_BN_CLICKED(IDC_BUTTON_START_BATCH, OnButtonStartBatch)
	ON_BN_CLICKED(IDC_BUTTON_START_QUICK, OnButtonStartQuick)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_IMPORT_DNS, OnImportDns)
	ON_COMMAND(IDM_IMPORT_IP, OnImportIp)
	ON_COMMAND(IDM_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(IDM_HELP_MANUAL, OnHelpManual)
	ON_BN_CLICKED(IDC_BUTTON_RESULT_SELECT_ALL, OnButtonResultSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_RESULT_UNSELECT, OnButtonResultUnselect)
	ON_BN_CLICKED(IDC_BUTTON_RESULT_DELETE, OnButtonResultDelete)
	ON_BN_CLICKED(IDC_BUTTON_RESULT_EXPORT, OnButtonResultExport)
	ON_BN_CLICKED(IDC_BUTTON_RESULT_CLEAR, OnButtonResultClear)
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnButtonPause)
	ON_COMMAND(IDM_TRAY_SHOWorHIDE, OnTRAYSHOWorHIDE)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_COMMAND(IDM_TRAY_EXIT, OnTrayExit)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_MESSAGE(WM_AFTER_WORK, OnAfterWork)
	ON_MESSAGE(WM_LISTCTRL_ITEM_CLICK, OnListCtrlItemClick)
	ON_MESSAGE(WM_LISTCTRL_ITEM_CHANGED, OnListCtrlItemChanged)	
	ON_CBN_SELCHANGE(IDC_COMBO_HTTP_VERSION, OnSelchangeComboHttpVersion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CURLTesterDlg message handlers

void CURLTesterDlg::OnSize(UINT nType, int cx, int cy)
{
	ClxDialog::OnSize(nType, cx, cy);
	RedrawWindow();
}
void CURLTesterDlg::OnSizing(UINT nSide, LPRECT lpRect)
{
	ClxDialog::OnSizing(nSide, lpRect);
}

int CURLTesterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	int Widths[] = { 80,160,260,300,350,-1 };
	RECT rc = { m_MinRect.left,m_MinRect.bottom-30,m_MinRect.right,m_MinRect.bottom };
	m_StatusBar.Create(WS_VISIBLE, rc, this, IDC_STATUSBAR_MAIN);	
	m_StatusBar.ShowWindow(SW_SHOW);
	m_StatusBar.SetParts(6, Widths);
	return 0;
}

LRESULT CURLTesterDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	//ShowWindow(SW_SHOWNORMAL); //return 0L;
	return m_trayIcon.OnTrayNotification(wParam,lParam);
}

BOOL CURLTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_edIP.SetLimitText(15); // 127.127.127.127
	m_edDNS.SetLimitText(50);

	InitListViewOfIP();
	InitListViewOfDNS();
	InitListViewOfResult();	

    m_cbMethod.SetCurSel(0);
	m_cbTimeout.SetCurSel(4);    
	if (m_cbPort.GetCount() < 1)
		m_cbPort.SetWindowText("80");
	
	m_cbFile.LimitText(FILE_LENGTH_MAX);
    if (m_cbFile.GetCount() < 1)
        m_cbFile.SetWindowText("/");
    else
        m_cbFile.SetCurSel(0);

	m_cbHead.LimitText(HEAD_LENGTH_MAX);
	if (m_cbHead.GetCount() > 0)
		m_cbFile.SetCurSel(0);

    m_pURLSocket = new CURLSocket(this);

    DWORD dwConfFilenameLength = ::GetModuleFileName(AfxGetInstanceHandle(), m_szConfFilename, sizeof(m_szConfFilename)-2);
    if (dwConfFilenameLength > 0) {
		size_t len = strrchr(m_szConfFilename, '\\') - m_szConfFilename;
		strncpy(m_szHomeDir, m_szConfFilename, len);
		m_szHomeDir[len] = 0;
        strcpy(&m_szConfFilename[len], "\\URLTester.ini");
        ImportConfFromFile(false, true);
    }

	// FILE
	m_tipOfFile.Create(&m_cbFile, TTS_ALWAYSTIP);	
	m_tipOfFile.AddTool(&m_cbFile);
	m_tipOfFile.SetTipTextColor(RGB(0,0,255));
    m_tipOfFile.SetDelayTime(150);
	// HEAD
	m_tipOfHead.Create(&m_cbHead, TTS_ALWAYSTIP);	
	m_tipOfHead.AddTool(&m_cbHead);
	m_tipOfHead.SetTipTextColor(RGB(0,0,255));
    m_tipOfHead.SetDelayTime(150);

	GetWindowRect(&m_MinRect);
	ShowSizeIcon();
	SetControlProperty(dcMenuGroup, sizeof(dcMenuGroup)/sizeof(ClxDialog::DLGCTLINFO));
		
	UpdateStatusBar();		
	InitAutoComplete();

	m_hTrayIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_trayIcon.SetNotificationWnd(this, WM_ICON_NOTIFY);
	m_trayIcon.SetIcon(m_hTrayIcon, "隐藏URLTester");

	if (1 == m_StartParam.WindowSize) {
		ShowWindow(SW_SHOWMAXIMIZED);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CURLTesterDlg::UpdateStatusBar()
{
	//if (!IsWindowVisible()) return;

	CString str;
	str.Format("域名:%d,%d", m_lvDNS.GetCheckCount(), m_lvDNS.GetItemCount());
	m_StatusBar.SetText((LPCTSTR)str, 0, 0); // SBT_NOBORDERS, SBT_POPOUT
	str.Format("IP:%d,%d", m_lvIP.GetCheckCount(), m_lvIP.GetItemCount());
	m_StatusBar.SetText((LPCTSTR)str, 1, 0);
	str.Format("代码:%d,%d", m_lvResult.GetCheckCount(), m_lvResult.GetItemCount());
	m_StatusBar.SetText((LPCTSTR)str, 2, 0);
}

BOOL CURLTesterDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_tipOfFile.m_hWnd != NULL) m_tipOfFile.RelayEvent(pMsg);
	if (m_tipOfHead.m_hWnd != NULL) m_tipOfHead.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

void CURLTesterDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_tipOfFile.UpdateTipText("文件必须以“/”打头，如：/index.htm", &m_cbFile);
	m_tipOfHead.UpdateTipText("HTTP请求头各项之间以“;;”分隔，如：Referer: www.ibx.com;;Accept: */*", &m_cbHead);	

	CDialog::OnMouseMove(nFlags, point);
}

void CURLTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{				
		//(SC_MINIMIZE == nID) ? ModifyStyleEx(0, WS_EX_TOOLWINDOW) : ModifyStyleEx(WS_EX_TOOLWINDOW, 0);
		//if (SC_MINIMIZE == nID) ShowWindow(SW_HIDE);
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CURLTesterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CURLTesterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CURLTesterDlg::InitListViewOfIP()
{
	SET_LISTCTRL_STYLE(m_lvIP);
    m_lvIP.InsertColumn(0, "", LVCFMT_CENTER, 20);
    m_lvIP.InsertColumn(1, "IP地址", LVCFMT_LEFT, 122);

	m_lvIP.RefreshColumnInfo();
}
void CURLTesterDlg::InitListViewOfDNS()
{
	SET_LISTCTRL_STYLE(m_lvDNS);
    m_lvDNS.InsertColumn(0, "", LVCFMT_CENTER, 20);
    m_lvDNS.InsertColumn(1, "域名", LVCFMT_LEFT, 150);
	m_lvDNS.RefreshColumnInfo();
}
void CURLTesterDlg::InitListViewOfResult()
{
	SET_LISTCTRL_STYLE(m_lvResult);

	m_lvResult.InsertColumn(0, "", LVCFMT_CENTER, 20);
	m_lvResult.InsertColumn(1, "域名", LVCFMT_LEFT, 200);
	m_lvResult.InsertColumn(2, "IP地址", LVCFMT_LEFT, 150);
	m_lvResult.InsertColumn(3, "代码", LVCFMT_LEFT, 50);

    m_lvResult.InsertColumn(4, "域名时长", LVCFMT_LEFT, 80);
    m_lvResult.InsertColumn(5, "连接时长", LVCFMT_LEFT, 80);
    m_lvResult.InsertColumn(6, "发送时长", LVCFMT_LEFT, 80);
    m_lvResult.InsertColumn(7, "接收时长", LVCFMT_LEFT, 80);
    m_lvResult.InsertColumn(8, "发送字节", LVCFMT_LEFT, 80);
    m_lvResult.InsertColumn(9, "接收字节", LVCFMT_LEFT, 80);

	m_lvResult.RefreshColumnInfo();
}


void CURLTesterDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_trayIcon.SetIcon((HICON)NULL, NULL);
	StopThread();
	ExportConfToFile();
	CDialog::OnClose();
}
void CURLTesterDlg::OnDestroy() 
{	
	m_tipOfFile.DestroyWindow();
	m_tipOfHead.DestroyWindow();
	CDialog::OnDestroy();
		
	// TODO: Add your message handler code here
	CloseThread();
}

void CURLTesterDlg::OnWorking()
{
    m_buStop.EnableWindow(TRUE);
	m_buPause.EnableWindow(TRUE);
    m_buExit.EnableWindow(FALSE);
    m_buDelIP.EnableWindow(FALSE);
    m_buAddIP.EnableWindow(FALSE);
	m_buDelDNS.EnableWindow(FALSE);	
	m_buAddDNS.EnableWindow(FALSE);
    m_buUnselectIP.EnableWindow(FALSE);	
	m_buStartQuick.EnableWindow(FALSE);
	m_buStartBatch.EnableWindow(FALSE);
	m_buSELECTALLIP.EnableWindow(FALSE);	
	m_buUnselectDNS.EnableWindow(FALSE);
	m_buSELECTALLDNS.EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESULT_SELECT_ALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESULT_UNSELECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESULT_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESULT_CLEAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESULT_EXPORT)->EnableWindow(FALSE);
    m_cbFile.EnableWindow(FALSE);

    m_lvDNS.EnableWindow(FALSE);
    m_lvIP.EnableWindow(FALSE);
    m_lvResult.EnableWindow(FALSE);
    m_cbPort.EnableWindow(FALSE);
    m_cbTimeout.EnableWindow(FALSE);
    m_edRequest.SetReadOnly(TRUE);
    m_edResponse.SetReadOnly(TRUE); 
	
	m_ctrlProgress.BringWindowToTop();
	m_ctrlProgress.ShowWindow(SW_SHOW);
}
void CURLTesterDlg::OnAfterWork()
{
    m_buStop.EnableWindow(FALSE);
	m_buPause.EnableWindow(FALSE);
    m_buExit.EnableWindow(TRUE);
    m_buDelIP.EnableWindow(TRUE);
    m_buAddIP.EnableWindow(TRUE);
	m_buDelDNS.EnableWindow(TRUE);	
	m_buAddDNS.EnableWindow(TRUE);
    m_buUnselectIP.EnableWindow(TRUE);	
	m_buStartQuick.EnableWindow(TRUE);
	m_buStartBatch.EnableWindow(TRUE);
	m_buSELECTALLIP.EnableWindow(TRUE);	
	m_buUnselectDNS.EnableWindow(TRUE);
	m_buSELECTALLDNS.EnableWindow(TRUE); 
	GetDlgItem(IDC_BUTTON_RESULT_SELECT_ALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RESULT_UNSELECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RESULT_DELETE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RESULT_CLEAR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RESULT_EXPORT)->EnableWindow(TRUE);
    m_cbFile.EnableWindow(TRUE);
    
    m_lvDNS.EnableWindow(TRUE);
    m_lvIP.EnableWindow(TRUE);
    m_lvResult.EnableWindow(TRUE);
    m_cbPort.EnableWindow(TRUE);
    m_cbTimeout.EnableWindow(TRUE);
    m_edRequest.SetReadOnly(FALSE);
    m_edResponse.SetReadOnly(FALSE);

	m_ctrlProgress.ShowWindow(SW_HIDE);

	CString strTips;
	if (m_bQuickTest) {
		m_buStartQuick.SetFocus();
	}
	else {
		strTips.Format("共完成“%d”个URLs测试。", m_ctrlProgress.GetCurTaskCount());
		AfxMessageBox(strTips);
		m_buStartBatch.SetFocus();
	}
}

void CURLTesterDlg::OnListCtrlItemChanged(WPARAM wParam, LPARAM lParam)
{
	HD_NOTIFY *phdn = (HD_NOTIFY *)lParam;
	// iItem -> Row
	// iButton -> Col
	if (0 == phdn->iButton)
	{		
		CURLListCtrl* pURLListCtrl = (CURLListCtrl *)wParam;
		if (pURLListCtrl->GetItemCount() > 0)
			UpdateStatusBar();			
	}
}
void CURLTesterDlg::OnListCtrlItemClick(WPARAM wParam, LPARAM lParam)
{	
	HD_NOTIFY *phdn = (HD_NOTIFY *)lParam;
	CURLListCtrl* pURLListCtrl = (CURLListCtrl *)wParam;

	// iItem -> Row
	// iButton -> Col
	if (0 == phdn->iButton)
	{
		// 不能放在OnListCtrlItemChanged中，否则死循环了
		if (pURLListCtrl->GetCheck(phdn->iItem)) {
			pURLListCtrl->SetItemText(phdn->iItem, phdn->iButton, "0");
			//pURLListCtrl->SetItemData(phdn->iItem, 1);
		}			
		else {
			pURLListCtrl->SetItemText(phdn->iItem, phdn->iButton, "1");
			//pURLListCtrl->SetItemData(phdn->iItem, 0);
		}			
	}
	else if (1 == phdn->iButton)
	{		
		CString str = pURLListCtrl->GetItemText(phdn->iItem, phdn->iButton);

		if (pURLListCtrl == &m_lvIP)
			GetDlgItem(IDC_EDIT_IP)->SetWindowText(str);
		else if (pURLListCtrl == &m_lvDNS)
			GetDlgItem(IDC_EDIT_DNS)->SetWindowText(str);
	}
}

void CURLTesterDlg::OnButtonAddDns() 
{
	// TODO: Add your control notification handler code here
    ADD_LISTCTRL_ITEM_BY_EDITCTRL(m_edDNS, m_lvDNS, IsValidDNS, "请输入正确的域名，然后再点击添加。");
	UpdateStatusBar();
	EditInputEnd(&m_edDNS);
}

void CURLTesterDlg::OnButtonDnsSelectAll() 
{
	// TODO: Add your control notification handler code here
	m_lvDNS.CheckAll();
	UpdateStatusBar();
}

void CURLTesterDlg::OnButtonDnsUnselect() 
{
	// TODO: Add your control notification handler code here
	m_lvDNS.Uncheck();
	UpdateStatusBar();
}

void CURLTesterDlg::OnButtonDnsDelete() 
{
	// TODO: Add your control notification handler code here
	m_lvDNS.DeleteSelected();
	UpdateStatusBar();
}

void CURLTesterDlg::OnButtonAddIp() 
{
	// TODO: Add your control notification handler code here
	ADD_LISTCTRL_ITEM_BY_EDITCTRL(m_edIP, m_lvIP, IsValidIP, "请输入正确的IP地址，然后再点击添加。");
	UpdateStatusBar();
	EditInputEnd(&m_edIP);
}

void CURLTesterDlg::OnButtonIpSelectAll() 
{
	// TODO: Add your control notification handler code here
	m_lvIP.CheckAll();
	UpdateStatusBar();
}

void CURLTesterDlg::OnButtonUnselect() 
{
	// TODO: Add your control notification handler code here
	m_lvIP.Uncheck();
	UpdateStatusBar();
}

void CURLTesterDlg::OnButtonIpDelete() 
{
	// TODO: Add your control notification handler code here
	m_lvIP.DeleteSelected();
	UpdateStatusBar();
}

void CURLTesterDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	//OnAfterWork();	
	StopThread();
	SendMessage(WM_AFTER_WORK);
}

void CURLTesterDlg::OnButtonPause() 
{
	if (INVALID_HANDLE_VALUE == m_hWorkThread) return;
	// TODO: Add your control notification handler code here
	CString str;
	m_buPause.GetWindowText(str);
	if (str == "暂停测试(&P)") {
		::SuspendThread(m_hWorkThread);
		m_buPause.SetWindowText("继续测试(&C)");
	}
	else {
		::ResumeThread(m_hWorkThread);
		m_buPause.SetWindowText("暂停测试(&P)");
	}
}

void CURLTesterDlg::OnButtonStartBatch() 
{
	// TODO: Add your control notification handler code here
    if (!CanBatchCheck()) return;
	if (!CheckFile()) return;
    if (!CheckPort()) return;

	m_buPause.EnableWindow(TRUE);
    m_bQuickTest = false;
    StartThread();
}

void CURLTesterDlg::OnButtonStartQuick() 
{
	// TODO: Add your control notification handler code here
    if (!QuickCheck()) return;
    if (!CheckFile()) return;
	if (!CheckPort()) return;

    m_bQuickTest = true;    
	EditInputEnd(&m_edIP);
	EditInputEnd(&m_edDNS);
	EditInputEnd(&m_cbFile);
	EditInputEnd(&m_cbHead);
	m_buPause.EnableWindow(FALSE);

    GetDlgItem(IDC_EDIT_LOG_REQUEST)->SetWindowText("");
    GetDlgItem(IDC_EDIT_LOG_RESPONSE)->SetWindowText("");
    StartThread();
}

void CURLTesterDlg::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
    SendMessage(WM_CLOSE);
}

bool CURLTesterDlg::CheckFile()
{
    CString strFile;
    GET_TEXT(m_cbFile, strFile);    
    if (strFile.IsEmpty()) {
        AfxMessageBox("无效请求文件，请重新输入，如：/welcome.htm。");
        m_cbFile.SetFocus();
        return false;
    }
    if (strFile[0] != '/') {
        AfxMessageBox("无效请求文件，请重新输入，如：/welcome.htm。");
        m_cbFile.SetFocus();
        return false;
    }

    return true;
}
bool CURLTesterDlg::CheckPort()
{
	CString str;
	m_cbPort.GetWindowText(str);
	str.TrimLeft();
	str.TrimRight();

	if (!IsPort(str)) {
		AfxMessageBox("无效的端口号(0~65535)，请重新输入。");
		m_cbPort.SetFocus();
		return false;
	}
	return true;
}

bool CURLTesterDlg::QuickCheck()
{
    CString strIP;
    CString strDNS;

    m_edIP.GetWindowText(strIP);
    strIP.TrimLeft();
    strIP.TrimRight();
    m_edDNS.GetWindowText(strDNS);
    strDNS.TrimLeft();
    strDNS.TrimRight();
        
    if ( (strIP.IsEmpty()) && (strDNS.IsEmpty()) ) {
        AfxMessageBox("快速测试时，域名和IP地址不能同时为空。");
        m_edDNS.SetFocus();
        return false;
    }
    if (!strDNS.IsEmpty()) {
        if (!IsValidDNS(strDNS)) {
            AfxMessageBox("无效的域名，请重新输入。");
            m_edDNS.SetFocus();
            return false;
        }
    }
    if (!strIP.IsEmpty()) {
        if (!IsValidDNS(strIP)) {
            AfxMessageBox("无效的IP地址，请重新输入。");
            m_edIP.SetFocus();
            return false;
        }
    }

    return true;
}

bool CURLTesterDlg::CanBatchCheck()
{
    if ( (0 == m_lvDNS.GetCheckCount()) && (0 == m_lvIP.GetCheckCount()) ) {
        AfxMessageBox("不能执行批量测试，因为即没有选择域名，也没有选择IP地址。");
        if (m_lvDNS.GetItemCount())
            m_lvDNS.SetFocus();
        else
            m_lvIP.SetFocus();
        return false;
    }
    return true;
}

void CURLTesterDlg::KeepConf()
{
    CString str;
    m_cbPort.GetWindowText(str);
    AddPort((LPCTSTR)str);
    m_cbFile.GetWindowText(str);
    AddFile((LPCTSTR)str);
	m_cbHead.GetWindowText(str);
    AddHead((LPCTSTR)str);
}
void CURLTesterDlg::FilterHead(CString& strHead)
{
	for (int i=1; i<strHead.GetLength(); ++i) {
		if ((';' == strHead[i]) && (';' == strHead[i-1])) {
			strHead.SetAt(i-1, '\r');
			strHead.SetAt(i, '\n');
		}
	}
}
DWORD ThreadProc(void *param)
{
    CURLTesterDlg* pURLTesterDlg = (CURLTesterDlg *)param;
    return pURLTesterDlg->ThreadProc();
}
DWORD CURLTesterDlg::ThreadProc()
{    
    CString str;
    CString strIP;
    CString strDNS;
    CString strFile;
	CString strHead;
    CString strMethod;
    CString strRequest;    
    long lSecond = 0;
    int nIPLoopCount = 0; // IP Count
    int nDNSLoopCount = 0; // DNS Count
    DWORD dwResponseCode = 0;
    unsigned short usPort = 0;
    char szResponse[4096] = {0};	
	int nCurTaskCount = 0;
	int nTotalTaskCount = 0;
     
    if (m_bQuickTest) {
        nIPLoopCount = 1;
        nDNSLoopCount = 1;
		nTotalTaskCount = 1;
    }
    else {        
        nIPLoopCount = m_lvIP.GetCheckCount();
		nDNSLoopCount = m_lvDNS.GetCheckCount();

		nTotalTaskCount = ((0 == nIPLoopCount) ? 1 : nIPLoopCount) * ((0 == nDNSLoopCount) ? 1 : nDNSLoopCount);

		nIPLoopCount = (0 == nIPLoopCount) ? 1 : m_lvIP.GetItemCount();                
        nDNSLoopCount = (0 == nDNSLoopCount) ? 1 : m_lvDNS.GetItemCount();
    }

	m_ctrlProgress.SetRange(0, nTotalTaskCount);
	m_ctrlProgress.SetPos(0);

    // COMMON
    GET_TEXT(m_cbFile, strFile);
	GET_TEXT(m_cbHead, strHead);
    GET_TEXT(m_cbMethod, strMethod); 
	FilterHead(strHead);

    // Port & Timeout(s)
    GET_TEXT(m_cbPort, str);    
    usPort = atoi((LPCTSTR)str);
    GET_TEXT(m_cbTimeout, str);
    lSecond = atoi((LPCTSTR)str);
    
    // DNS LOOP
    for (int IdxOfDns=0; IdxOfDns<nDNSLoopCount; ++IdxOfDns)
    {
        strIP.Empty();
        strDNS.Empty();

        // Get DNS
        if (m_bQuickTest) {            
            GET_TEXT(m_edDNS, strDNS);
        }
        else if (m_lvDNS.GetCheckCount() > 0) {         
            if (m_lvDNS.GetCheck(IdxOfDns)) {            
                strDNS = m_lvDNS.GetItemText(IdxOfDns, 1);
                strDNS.TrimLeft();
                strDNS.TrimRight(); 
            }
            else {
                continue;
            }
        }

        // IP LOOP
        for (int IdxOfIP=0; IdxOfIP<nIPLoopCount; ++IdxOfIP)
        {       
            if (m_bQuickTest) {
                GET_TEXT(m_edIP, strIP);
            }
            else if (m_lvIP.GetCheckCount() > 0) {
                if (m_lvIP.GetCheck(IdxOfIP)) {            
                    strIP = m_lvIP.GetItemText(IdxOfIP, 1);
                    strIP.TrimLeft();
                    strIP.TrimRight(); 
                }
                else {
                    continue;
                }
            }

            if (strIP.IsEmpty() && strDNS.IsEmpty()) break;            
            if (strDNS.IsEmpty())
            {
                strDNS = strIP;
            }
            else if (strIP.IsEmpty()) {
                struct in_addr in;
                in.s_addr = m_pURLSocket->GetIPByHostname(strDNS);
                strIP = inet_ntoa(in);            
            }       

#define ADD_RESULT_LOG(dwResponseCode) \
			++nCurTaskCount; \
			m_ctrlProgress.SetPos(nCurTaskCount); \
			m_ctrlProgress.SetCurTaskCount(nCurTaskCount); \
			AddResultLog((LPCTSTR)strDNS, (LPCTSTR)strIP, dwResponseCode, \
                m_pURLSocket->GetDNSTime(),m_pURLSocket->GetConnectTime(),m_pURLSocket->GetSendTime(),m_pURLSocket->GetRecvTime(), \
                m_pURLSocket->GetTotalByteSent(), m_pURLSocket->GetTotalByteReived()) \

            if (!m_pURLSocket->TimedConnect((LPCTSTR)strDNS, (LPCTSTR)strIP, usPort, lSecond)) {
                m_pURLSocket->Close();
				ADD_RESULT_LOG(RESPONSE_CODE_CONNECT);
                continue;
            }

            KeepConf();
            strRequest.Format(
                "%s %s HTTP/1.%c\r\n"
                "Host: %s\r\n"
                "Connection: Close\r\n"
				"%s\r\n"
                "\r\n",
                (LPCTSTR)strMethod,
                (LPCTSTR)strFile,
                         m_cHttpVersion,
                (LPCTSTR)strDNS,
				(LPCTSTR)strHead);

            if (!m_pURLSocket->TimedSend((LPCTSTR)strRequest, strRequest.GetLength(), lSecond)) {
                m_pURLSocket->Close();
				ADD_RESULT_LOG(RESPONSE_CODE_SEND);
                continue;
            }
        
            m_edRequest.GetWindowText(str);
            str += strRequest;
            m_edRequest.SetWindowText(str);

            bool bGetResponseCode = false;
            while (true)
            {
                if (!m_pURLSocket->TimedReceive(szResponse, sizeof(szResponse)-1, lSecond))
                {
                    break;
                }

                DWORD dwRecvByte = m_pURLSocket->GetTotalByteReived();
                if (dwRecvByte > RESPONSE_SIZE_MAX) {
                    AddResponseLog(">>>数据太多，不再显示。。。");
                    break;
                }

                // HTTP/1.1 302
                if (!bGetResponseCode && (dwRecvByte > 8)) {
                    dwResponseCode = atoi(szResponse+sizeof("HTTP/1.1"));
                    bGetResponseCode = true;
                }                  
            }        

            if ((0 == dwResponseCode) && (m_pURLSocket->GetTotalByteReived() > sizeof("HTTP/1.1 302")))
                dwResponseCode = atoi(szResponse+sizeof("HTTP/1.1"));

            m_pURLSocket->Close();
			ADD_RESULT_LOG(dwResponseCode);
            //AddResultLog((LPCTSTR)strDNS, (LPCTSTR)strIP, dwResponseCode,
            //    m_pURLSocket->GetDNSTime(),m_pURLSocket->GetConnectTime(),m_pURLSocket->GetSendTime(),m_pURLSocket->GetRecvTime(),
            //    m_pURLSocket->GetTotalByteSent(), m_pURLSocket->GetTotalByteReived());
        }
    }

	SendMessage(WM_AFTER_WORK);
    //OnAfterWork();
    return 0;
}

void CURLTesterDlg::CloseThread()
{
    if (m_hWorkThread != INVALID_HANDLE_VALUE) {
        ::CloseHandle(m_hWorkThread);
        m_hWorkThread = INVALID_HANDLE_VALUE;
    }
}
void CURLTesterDlg::StartThread()
{
    CloseThread();

    m_hWorkThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)::ThreadProc, this, 0, &m_dwThreadId);
    if (INVALID_HANDLE_VALUE == m_hWorkThread) {
        AfxMessageBox(::GetLastErrorString());
    }
    else {
        OnWorking();
    }
}
void CURLTesterDlg::StopThread()
{
    if (m_hWorkThread != INVALID_HANDLE_VALUE) {
        ::TerminateThread(m_hWorkThread, 1);
        CloseThread();
    }
}

void CURLTesterDlg::AddRequestLog(const char* szLog)
{
    ADD_PROCESS_LOG(m_edRequest, szLog);

	CString str;
	str.Format("%d", m_edRequest.GetLineCount());
	m_StatusBar.SetText((LPCTSTR)str, 3, 0);

	//int nLineCount = m_edRequest.GetLineCount();
	//m_edRequest.LineScroll(nLineCount, 0);
	//m_edRequest.SetSel(m_edRequest.GetWindowTextLength(), m_edRequest.GetWindowTextLength());
}
void CURLTesterDlg::AddResponseLog(const char* szLog)
{
    ADD_PROCESS_LOG(m_edResponse, szLog);

	CString str;
	str.Format("%d", m_edResponse.GetLineCount());
	m_StatusBar.SetText((LPCTSTR)str, 4, 0);
}
void CURLTesterDlg::AddResultLog(const char* szDNS, const char* szIP, DWORD dwResponseCode, 
        DWORD dwDNSTime, DWORD dwConnectTime, DWORD dwSendTime, DWORD dwRecvTime,
        DWORD nSendByte, DWORD nRecvByte)
{
    int nRow = m_lvResult.InsertItem(m_lvResult.GetItemCount(), _T("1"));

    m_lvResult.SetItemText(nRow, 1, _T(szDNS));
    m_lvResult.SetItemText(nRow, 2, _T(szIP));
    m_lvResult.SetItemText(nRow, 3, DWORDToString(dwResponseCode));

    m_lvResult.SetItemText(nRow, 4, DWORDToString(dwDNSTime));
    m_lvResult.SetItemText(nRow, 5, DWORDToString(dwConnectTime));
    m_lvResult.SetItemText(nRow, 6, DWORDToString(dwSendTime));
    m_lvResult.SetItemText(nRow, 7, DWORDToString(dwRecvTime));

    m_lvResult.SetItemText(nRow, 8, DWORDToString(nSendByte));
    m_lvResult.SetItemText(nRow, 9, DWORDToString(nRecvByte));
	m_lvResult.SetCheck(nRow, TRUE);

	// overflow
	if (m_lvResult.GetItemCount() > RESULT_COUNT_MAX) {
		for (nRow=0; nRow<RESULT_COUNT_MAX/100; ++nRow)
			m_lvResult.DeleteItem(0);		
	}
}

void CURLTesterDlg::OnImportDns() 
{
	// TODO: Add your command handler code here
	ImportConfFromFile(false, false);
}

void CURLTesterDlg::OnImportIp() 
{
    ImportConfFromFile(true, false);
}

void CURLTesterDlg::OnExit() 
{
	// TODO: Add your command handler code here
	SendMessage(WM_CLOSE);
}

void CURLTesterDlg::ImportConfFromFile(bool bIP, bool bFromConf)
{
	// TODO: Add your command handler code here
    FILE* fp = NULL;
	CString strErrMsg;
    CString strFilename;

    if (bFromConf)
    {
        strFilename = m_szConfFilename;
    }
    else {    
	    //CFileDialog dlg(TRUE, NULL, NULL);
		CFileDialog dlg(TRUE, ".txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "文本文件(*.txt)|*.txt|列表文件(*.lst)|*.lst|所有文件(*.*)|*.*||", this);
        if (IDOK == dlg.DoModal()) {            
            strFilename = dlg.GetPathName();                    
        }
        else {
            return;
        }
    }

    fp = fopen((LPCTSTR)strFilename, "rt");        
    if (NULL == fp) {
        if (!bFromConf) {        
            strErrMsg.Format("不能打开“%s”。", strFilename);
            AfxMessageBox(strErrMsg);
        }
        return;
    }
                        
    int nSpacePositon = 0;
    char szLine[LINE_LENGTH_MAX] = {0};
    char* pszLine = szLine;
    bool bLeftBracketFound = false;
    bool bRightBracketFound = false;
    while (fgets(szLine, sizeof(szLine)-1, fp))
    {
        Trim(szLine);
        if (0 == szLine[0]) continue;
        if ('#' == szLine[0]) continue;
        if ('\'' == szLine[0]) continue;
        if ('\r' == szLine[0]) continue;
        if ('\n' == szLine[0]) continue;
        
        if ('{' == szLine[0]) {
            bLeftBracketFound = true;
            continue;
        }
        if (('}' == szLine[0]) && bLeftBracketFound) {
            bRightBracketFound = true;
            bLeftBracketFound = false;
            continue;
        }
        if (bLeftBracketFound) {
            if (!bRightBracketFound) {
                continue;
            }
            else {
                bLeftBracketFound = false;
                bRightBracketFound = false;
            }
        }
                    
        if (!bFromConf)
        {        
			char* pSpacePosition = strchr(szLine, ' ');
			if (pSpacePosition) *pSpacePosition = 0;

            if (bIP) {
                ADD_LISTCTRL_ITEM_BY_TEXT(szLine,m_lvIP,IsValidIP,"");
            }
            else {
                ADD_LISTCTRL_ITEM_BY_TEXT(szLine,m_lvDNS,IsValidDNS,"");
            }
        }
        else {
            // FROM CONF FILE
            if ('I' == szLine[0]) { // IP
                pszLine = szLine+1;
                ADD_LISTCTRL_ITEM_BY_TEXT(pszLine,m_lvIP,IsValidIP,"");
            }
            else if ('D' == szLine[0]) { // DNS
                pszLine = szLine+1;
                ADD_LISTCTRL_ITEM_BY_TEXT(pszLine,m_lvDNS,IsValidDNS,"");
            }
            else if ('P' == szLine[0]) { // Port
                if ('\0' == szLine[1]) {
                    pszLine = szLine+1;
                    AddPort(pszLine);
                }
                else if ('V' == szLine[1]) {                    
                    m_cHttpVersion = ('1' == szLine[2]) ? '1' : '0';
                    if ('0' == m_cHttpVersion)
                        ((CComboBox *)GetDlgItem(IDC_COMBO_HTTP_VERSION))->SetCurSel(0);
                    else
                        ((CComboBox *)GetDlgItem(IDC_COMBO_HTTP_VERSION))->SetCurSel(1);
                }
            }
            else if ('F' == szLine[0]) { // File
                pszLine = szLine+1;
                AddFile(pszLine);
            }
			else if ('H' == szLine[0]) { // Head
                pszLine = szLine+1;
                AddHead(pszLine);
            }
			else if ('W' == szLine[0]) {
				if ('S' == szLine[1]) { // Windows Size
					if ('1' == szLine[2])
						m_StartParam.WindowSize = 1;
					else if ('2' == szLine[2])
						m_StartParam.WindowSize = 2;
					else
						m_StartParam.WindowSize = 0;
				}
			}
        }
    }

    fclose(fp);    
}

void CURLTesterDlg::ExportConfToFile()
{
    FILE* fp = fopen(m_szConfFilename, "wt+");
    if (NULL == fp) return;

    ExportComboBoxToFile(fp, &m_cbPort);
    ExportComboBoxToFile(fp, &m_cbFile);
	ExportComboBoxToFile(fp, &m_cbHead);

    ExportListCtrlToFile(fp, &m_lvIP);
    ExportListCtrlToFile(fp, &m_lvDNS);

	IsMaximized() ? fputs("WS1\n", fp): fputs("WS2\n", fp);
    fprintf(fp, "PV%c\n", m_cHttpVersion);
    fclose(fp);
}

void CURLTesterDlg::ExportComboBoxToFile(FILE* fp, CComboBox* pComboBox)
{
    char text[1024] = {0};
    char* textp = text+1;

    if (pComboBox == &m_cbPort)
        text[0] = 'P';
    else if (pComboBox == &m_cbFile)
        text[0] = 'F';
	else if (pComboBox == &m_cbHead)
        text[0] = 'H';
    else
        return;

    for (int i=0; i<pComboBox->GetCount(); ++i) {
        int nTextLen = pComboBox->GetLBTextLen(i);
        pComboBox->GetLBText(i, textp);  
        textp[nTextLen] = '\n';
        textp[nTextLen+1] = '\0';
        
        fputs(text, fp);
    }
}
void CURLTesterDlg::ExportListCtrlToFile(FILE* fp, CListCtrl* pListCtrl)
{
    char text[100] = {0};
    char* textp = text+1;

    if (pListCtrl == &m_lvIP)
        text[0] = 'I';
    else if (pListCtrl == &m_lvDNS)
        text[0] = 'D';
    else
        return;

    for (int i=0; i<pListCtrl->GetItemCount(); ++i) {
        int nTextLen = pListCtrl->GetItemText(i, 1, textp, sizeof(text)-4);
        textp[nTextLen] = '\n';
        textp[nTextLen+1] = '\0';
        fputs(text, fp);
    }
}

void CURLTesterDlg::AddItemToComboBox(CComboBox* pComboBox, const char* str)
{
	CString str2;
	bool bExist = false;
	for (int i=0; i<pComboBox->GetCount(); ++i) {		
		pComboBox->GetLBText(i, str2);
		if (!strcmp(str, (LPCTSTR)str2)) {
			bExist = true;
			break;
		}
	}
	if (!bExist) {
		if (pComboBox->GetCount() > LIST_COUNT_MAX) {
			AfxMessageBox("数据太多，不能再插入。");
			return;
		}
		pComboBox->AddString(str);
	}
}
void CURLTesterDlg::AddPort(const char* str)
{
	AddItemToComboBox(&m_cbPort, str);
}
void CURLTesterDlg::AddFile(const char* str)
{
	AddItemToComboBox(&m_cbFile, str);
}
void CURLTesterDlg::AddHead(const char* str)
{
	AddItemToComboBox(&m_cbHead, str);
}

void CURLTesterDlg::OnButtonResultSelectAll() 
{
	// TODO: Add your control notification handler code here
	m_lvResult.CheckAll();
	UpdateStatusBar();
}

void CURLTesterDlg::OnButtonResultUnselect() 
{
	// TODO: Add your control notification handler code here
	m_lvResult.Uncheck();
	UpdateStatusBar();
}

void CURLTesterDlg::OnButtonResultDelete() 
{
	// TODO: Add your control notification handler code here
	m_lvResult.DeleteSelected();
	UpdateStatusBar();
}

void CURLTesterDlg::OnButtonResultExport() 
{
	// TODO: Add your control notification handler code here
	if (0 == m_lvResult.GetCheckCount()) {
		AfxMessageBox("请选择需要导出“测试结果”的。");
		return;
	}
	CFileDialog dlg(FALSE, ".csv", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "逗号分隔文件(*.csv)|*.csv|文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||", this);
	if (dlg.DoModal() != IDOK) return;

	CString strLine;
	CString strFilename = dlg.GetPathName();
	FILE* fp = fopen((LPCTSTR)strFilename, "wt");
	if (NULL == fp) {
		strLine.Format("导出测试结果失败，因为不能创建文件“%s”。", (LPCTSTR)strFilename);
		AfxMessageBox(strLine);
		return;
	}

	// Write Title
	fputs("编号,域名,IP地址,响应代码,域名时长,连接时长,发送时长,接收时长,发送字节,接收字节", fp);
	fputs("\n", fp);

	int nCheckCount = 0;
	for (int i=0; i<m_lvResult.GetItemCount(); ++i) {
		if (!m_lvResult.GetCheck(i)) continue;

		++nCheckCount;
		strLine.Format("%d,", nCheckCount);

		strLine += m_lvResult.GetItemText(i, 1); // 域名
		strLine += ",";
		strLine += m_lvResult.GetItemText(i, 2); // IP地址
		strLine += ",";
		strLine += m_lvResult.GetItemText(i, 3); // 响应代码
		strLine += ",";
		strLine += m_lvResult.GetItemText(i, 4); // 域名时长
		strLine += ",";
		strLine += m_lvResult.GetItemText(i, 5); // 连接时长
		strLine += ",";
		strLine += m_lvResult.GetItemText(i, 6); // 发送时长
		strLine += ",";
		strLine += m_lvResult.GetItemText(i, 7); // 接收时长
		strLine += ",";
		strLine += m_lvResult.GetItemText(i, 8); // 发送字节
		strLine += ",";
		strLine += m_lvResult.GetItemText(i, 9); // 接收字节
		//strLine += "\r\n";

		fputs((LPCTSTR)strLine, fp);
		fputs("\n", fp);
	}

	fclose(fp);
	strLine.Format("“%d”条测试结果被保存到文件“%s”中。", nCheckCount, (LPCTSTR)strFilename);
	AfxMessageBox(strLine);
}

void CURLTesterDlg::OnButtonResultClear() 
{
	// TODO: Add your control notification handler code here
	if (m_lvResult.GetItemCount() < 1) {
		AfxMessageBox("没有可清除的目标。"); return;
	}
	m_lvResult.DeleteAllItems();
}

void CURLTesterDlg::OnHelpAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}

void CURLTesterDlg::OnHelpManual() 
{
	// TODO: Add your command handler code here 
	CString strManual;
	strManual.Format("%s\\%s", m_szHomeDir, "URLTester.doc");

	SHELLEXECUTEINFO see;
	memset(&see, 0, sizeof(see));
	see.cbSize = sizeof(see);
	see.fMask = SEE_MASK_NOCLOSEPROCESS;
	see.hwnd = m_hWnd;
	see.lpVerb = NULL;
	see.lpDirectory = NULL;
	see.hInstApp = AfxGetInstanceHandle();
	see.nShow = SW_SHOWNORMAL;
	see.lpParameters = NULL;
	see.lpFile = (LPCSTR)strManual;
	if (!ShellExecuteEx(&see)) {
		//AfxMessageBox(GetLastErrorString());
		;
	}
}

UINT CAboutDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	return CDialog::OnNcHitTest(point);
}

void CURLTesterDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	if (!IsIconic()) {
		lpMMI->ptMinTrackSize.x = m_MinRect.right-m_MinRect.left;
		lpMMI->ptMinTrackSize.y = m_MinRect.bottom-m_MinRect.top;
	}
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CURLTesterDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

bool CURLTesterDlg::IsMaximized()
{
	int nScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	//int nScreenWidth  = GetSystemMetrics(SM_CXFULLSCREEN);
	//int nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	int nTitleHeight  = GetSystemMetrics(SM_CYCAPTION);

	RECT rect;
	GetWindowRect(&rect);

	return (nScreenWidth > rect.right-rect.left) || (nScreenHeight > (rect.bottom-rect.top) + nTitleHeight) ? false : true;
}
void CURLTesterDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!IsMaximized())
		ShowWindow(SW_SHOWMAXIMIZED); // 最大化
	else
		ShowWindow(SW_SHOWNORMAL);
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CURLTesterDlg::EditInputEnd(CWnd* pWnd)
{	
	CAutoComplete* pAutoComplete = NULL;	

	if (pWnd == &m_edIP) pAutoComplete = &m_acIP;
	else if (pWnd == &m_edDNS) pAutoComplete = &m_acDNS;
	else if (pWnd == &m_cbFile) pAutoComplete = &m_acFile;
	else if (pWnd == &m_cbHead) pAutoComplete = &m_acHead;
	else return;

	CString str;
	pWnd->GetWindowText(str);

	bool bExist = false;
	CStringArray& strings = pAutoComplete->GetStringList();
	for (int i=0; i<strings.GetSize(); ++i) {
		if (strings[i] == str) {
			bExist = true;
			break;
		}
	}

	if (!bExist) pAutoComplete->GetStringList().Add(str);
}

void CURLTesterDlg::InitAutoComplete()
{
	m_acIP.Init(&m_edIP);
	m_acDNS.Init(&m_edDNS);
	m_acFile.Init(&m_cbFile);
	m_acHead.Init(&m_cbHead);
}

void CURLTesterDlg::OnTRAYSHOWorHIDE() 
{
	// TODO: Add your command handler code here
	CMenu& menu = m_trayIcon.GetMenu();
	//CMenu* pSubMenu = menu.GetSubMenu(0);
	CMenu* pSubMenu = &menu;

	if (IsWindowVisible()) {
		ShowWindow(SW_HIDE);
		//BringWindowToTop();
		SetForegroundWindow();
		m_trayIcon.SetIcon(m_hTrayIcon, "显示URLTester");
		pSubMenu->ModifyMenu(0, 0, 0, "显示URLTester");
		ModifyStyleEx(0, WS_EX_TOOLWINDOW);
	}
	else {
		ModifyStyleEx(WS_EX_TOOLWINDOW, 0);
		ShowWindow(SW_SHOWNORMAL);
		m_trayIcon.SetIcon(m_hTrayIcon, "隐藏URLTester");
		pSubMenu->ModifyMenu(0, 0, 0, "隐藏URLTester");
	}
}

void CURLTesterDlg::OnTrayExit() 
{
	// TODO: Add your command handler code here
	SendMessage(WM_CLOSE);
}

void CURLTesterDlg::OnSelchangeComboHttpVersion() 
{
	// TODO: Add your control notification handler code here  
    char szVer[] = { '0', '1' };
    int nCurSel = ((CComboBox *)GetDlgItem(IDC_COMBO_HTTP_VERSION))->GetCurSel();    
    if (nCurSel > sizeof(szVer))
        m_cHttpVersion = '0';
    else
        m_cHttpVersion = szVer[nCurSel];
}
