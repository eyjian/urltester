#if !defined(AFX_URLHEADERCTRL_H__32349BA3_CA93_494F_82DC_62968649998C__INCLUDED_)
#define AFX_URLHEADERCTRL_H__32349BA3_CA93_494F_82DC_62968649998C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// URLHeaderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CURLHeaderCtrl window

class CURLHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CURLHeaderCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CURLHeaderCtrl)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CURLHeaderCtrl();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// Generated message map functions
protected:
	//{{AFX_MSG(CURLHeaderCtrl)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void SetSortArrow(const int iSortColumn, const BOOL bSortAscending);

private:
	int m_iSortColumn;
	BOOL m_bSortAscending;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLHEADERCTRL_H__32349BA3_CA93_494F_82DC_62968649998C__INCLUDED_)
