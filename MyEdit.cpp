// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "URLTester.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers

bool CMyEdit::IsCopyOrPasteOrCut(UINT nChar)
{
	//unsigned char kbuf[256];
	//GetKeyboardState(kbuf);

	return ((3 == nChar) || (22 == nChar) || (24 == nChar)) ? true : false;
	//return ( (('v' == nChar) || ('p' == nChar) || ('x' == nChar)) & (kbuf[VK_CONTROL]&128)) ? true : false;
}
