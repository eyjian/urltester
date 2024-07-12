#if !defined(AFX_PORTEDIT_H__ABB341B9_5839_4D81_BC38_4D1AB8B63196__INCLUDED_)
#define AFX_PORTEDIT_H__ABB341B9_5839_4D81_BC38_4D1AB8B63196__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPortEdit window

class CPortEdit : public CEdit
{
// Construction
public:
	CPortEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPortEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPortEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTEDIT_H__ABB341B9_5839_4D81_BC38_4D1AB8B63196__INCLUDED_)
