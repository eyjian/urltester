// PortEdit.cpp : implementation file
//

#include "stdafx.h"
#include "URLTester.h"
#include "PortEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortEdit

CPortEdit::CPortEdit()
{
}

CPortEdit::~CPortEdit()
{
}


BEGIN_MESSAGE_MAP(CPortEdit, CEdit)
	//{{AFX_MSG_MAP(CPortEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortEdit message handlers

void CPortEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
