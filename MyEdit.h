#if !defined(AFX_MYEDIT_H__715138E9_5D6F_474A_92AA_827DFDA85448__INCLUDED_)
#define AFX_MYEDIT_H__715138E9_5D6F_474A_92AA_827DFDA85448__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyEdit window

class CMyEdit : public CEdit
{
// Construction
public:
	CMyEdit();

// Attributes
public:

// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	bool IsCopyOrPasteOrCut(UINT nChar);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__715138E9_5D6F_474A_92AA_827DFDA85448__INCLUDED_)
