// DNSEdit.cpp : implementation file
//

#include "stdafx.h"
#include "URLTester.h"
#include "DNSEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDNSEdit

CDNSEdit::CDNSEdit()
{
}

CDNSEdit::~CDNSEdit()
{
}


BEGIN_MESSAGE_MAP(CDNSEdit, CEdit)
	//{{AFX_MSG_MAP(CDNSEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDNSEdit message handlers

void CDNSEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if ( ('.' == nChar) || (8 == nChar)
		|| ((nChar >= '0') && (nChar <= '9'))
		|| ((nChar >= 'A') && (nChar <= 'Z'))
		|| ((nChar >= 'a') && (nChar <= 'z'))
		|| IsCopyOrPasteOrCut(nChar) )
		CMyEdit::OnChar(nChar, nRepCnt, nFlags);
}
