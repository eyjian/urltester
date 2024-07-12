#if !defined(AFX_URLCOMBOBOXEX_H__70A4A82F_33D0_4880_B4CD_6C18D93710CA__INCLUDED_)
#define AFX_URLCOMBOBOXEX_H__70A4A82F_33D0_4880_B4CD_6C18D93710CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// URLComboBoxEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CURLComboBoxEx window

class CURLComboBoxEx : public CComboBoxEx
{
// Construction
public:
	CURLComboBoxEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CURLComboBoxEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CURLComboBoxEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CURLComboBoxEx)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditchange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLCOMBOBOXEX_H__70A4A82F_33D0_4880_B4CD_6C18D93710CA__INCLUDED_)
