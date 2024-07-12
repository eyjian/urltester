// URLTesterDlg.h : header file
//

#if !defined(AFX_URLTESTERDLG_H__904391F8_8426_4248_B0E3_A23B7A1E2B59__INCLUDED_)
#define AFX_URLTESTERDLG_H__904391F8_8426_4248_B0E3_A23B7A1E2B59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPEdit.h"
#include "DNSEdit.h"
#include "TrayIcon.h"
#include "MyDialog.h"
#include "PortEdit.h"
#include "URLSocket.h"
#include "AutoCompl.h"
#include "URLListCtrl.h"
#include "URLComboBoxEx.h"
#include "TextProgressCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CURLTesterDlg dialog

class CURLTesterDlg : public ClxDialog
{    
	struct {
		unsigned char WindowSize;
	}m_StartParam;
private:
    friend DWORD ThreadProc(void *param);
    DWORD ThreadProc();
    void StartThread();
    void StopThread();
    void CloseThread();
    void OnWorking();
    void OnAfterWork();
	void OnListCtrlItemClick(WPARAM wParam, LPARAM lParam);
	void OnListCtrlItemChanged(WPARAM wParam, LPARAM lParam);

// Construction
public:
	CURLTesterDlg(CWnd* pParent = NULL);	// standard constructor
    ~CURLTesterDlg();

// Dialog Data
	//{{AFX_DATA(CURLTesterDlg)
	enum { IDD = IDD_URLTESTER_DIALOG };
	CButton	m_buPause;
	CTextProgressCtrl	m_ctrlProgress;
	CButton	m_buUnselectIP;
	CButton	m_buStop;
	CButton	m_buStartQuick;
	CButton	m_buStartBatch;
	CButton	m_buSELECTALLIP;
	CButton	m_buDelIP;
	CButton	m_buExit;
	CButton	m_buUnselectDNS;
	CButton	m_buSELECTALLDNS;
	CButton	m_buDelDNS;
	CButton	m_buAddIP;
	CButton	m_buAddDNS;
	CEdit	        m_edRequest;
	CEdit	        m_edResponse;
	CPortEdit	    m_edPort;
	CURLComboBoxEx	m_cbTimeout;
	CURLComboBoxEx	m_cbPort;
    CURLComboBoxEx  m_cbMethod;
	CURLComboBoxEx	m_cbHead;
	CURLComboBoxEx	m_cbFile;
	CIPEdit	        m_edIP;
	CDNSEdit	    m_edDNS;
	CURLListCtrl	m_lvIP;
	CURLListCtrl	m_lvDNS;
	CURLListCtrl	m_lvResult;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CURLTesterDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CURLTesterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonAddDns();
	afx_msg void OnButtonDnsSelectAll();
	afx_msg void OnButtonDnsUnselect();
	afx_msg void OnButtonDnsDelete();
	afx_msg void OnButtonAddIp();
	afx_msg void OnButtonIpSelectAll();
	afx_msg void OnButtonUnselect();
	afx_msg void OnButtonIpDelete();
	afx_msg void OnButtonStartBatch();
	afx_msg void OnButtonStartQuick();
	afx_msg void OnButtonExit();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnImportDns();
	afx_msg void OnImportIp();
	afx_msg void OnHelpAbout();
	afx_msg void OnHelpManual();
	afx_msg void OnButtonResultSelectAll();
	afx_msg void OnButtonResultUnselect();
	afx_msg void OnButtonResultDelete();
	afx_msg void OnButtonResultExport();
	afx_msg void OnButtonResultClear();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonPause();
	afx_msg void OnTRAYSHOWorHIDE();
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTrayExit();
	afx_msg void OnExit();
	afx_msg void OnSelchangeComboHttpVersion();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    void AddRequestLog(const char* szLog);
    void AddResponseLog(const char* szLog);
    void AddResultLog(const char* szDNS, const char* szIP, DWORD dwResponseCode, 
        DWORD dwDNSTime, DWORD dwConnectTime, DWORD dwSendTime, DWORD dwRecvTime,
        DWORD nSendByte, DWORD nRecvByte);

private:
	void InitListViewOfIP();
	void InitListViewOfDNS();
	void InitListViewOfResult();

    bool CheckFile();
	bool CheckPort();
    bool QuickCheck();
    bool CanBatchCheck();
   
    void AddPort(const char* str);
    void AddFile(const char* str);
	void AddHead(const char* str);
	void AddItemToComboBox(CComboBox* pComboBox, const char* str);
        
    void KeepConf();
    void ExportConfToFile();    
    void ImportConfFromFile(bool bIP, bool bFromConf);    
    void ExportComboBoxToFile(FILE* fp, CComboBox* pComboBox);
    void ExportListCtrlToFile(FILE* fp, CListCtrl* pListCtrl);

	bool IsMaximized();
	void UpdateStatusBar();
	void InitAutoComplete();
	void EditInputEnd(CWnd* pWnd);
	void FilterHead(CString& strHead);	

private:    
    bool m_bQuickTest;
    DWORD m_dwThreadId;
    HANDLE m_hWorkThread;
    CURLSocket* m_pURLSocket;
	char m_szHomeDir[1024];
    char m_szConfFilename[1024];

private:
	RECT m_MinRect;
	HICON m_hTrayIcon;
	CTrayIcon m_trayIcon;
	CToolTipCtrl m_tipOfFile;
	CToolTipCtrl m_tipOfHead;
	CStatusBarCtrl m_StatusBar;

private:
	CAutoComplete m_acIP;
	CAutoComplete m_acDNS;
	CAutoComplete m_acFile;
	CAutoComplete m_acHead;

private:
    char m_cHttpVersion;
};

extern CString GetLastErrorString();

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLTESTERDLG_H__904391F8_8426_4248_B0E3_A23B7A1E2B59__INCLUDED_)
