#if !defined(AFX_DNSEDIT_H__A55DB383_443E_410C_BCDD_57BF675618ED__INCLUDED_)
#define AFX_DNSEDIT_H__A55DB383_443E_410C_BCDD_57BF675618ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DNSEdit.h : header file
//

#include "MyEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDNSEdit window

class CDNSEdit : public CMyEdit
{
// Construction
public:
	CDNSEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDNSEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDNSEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDNSEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DNSEDIT_H__A55DB383_443E_410C_BCDD_57BF675618ED__INCLUDED_)
