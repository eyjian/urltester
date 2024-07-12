// URLTester.h : main header file for the URLTESTER application
//

#if !defined(AFX_URLTESTER_H__18AB6F3B_A5A9_40D2_9881_F914C822A777__INCLUDED_)
#define AFX_URLTESTER_H__18AB6F3B_A5A9_40D2_9881_F914C822A777__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CURLTesterApp:
// See URLTester.cpp for the implementation of this class
//

class CURLTesterApp : public CWinApp
{
public:
	CURLTesterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CURLTesterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CURLTesterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLTESTER_H__18AB6F3B_A5A9_40D2_9881_F914C822A777__INCLUDED_)
