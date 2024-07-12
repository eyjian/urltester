#if !defined(AFX_IPEDIT_H__55A37EF6_7F6E_4D28_84E8_08D7E773858C__INCLUDED_)
#define AFX_IPEDIT_H__55A37EF6_7F6E_4D28_84E8_08D7E773858C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IPEdit.h : header file
//

#include "MyEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CIPEdit window

class CIPEdit : public CMyEdit
{
// Construction
public:
	CIPEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIPEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIPEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPEDIT_H__55A37EF6_7F6E_4D28_84E8_08D7E773858C__INCLUDED_)
