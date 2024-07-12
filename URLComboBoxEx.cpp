// URLComboBoxEx.cpp : implementation file
//

#include "stdafx.h"
#include "URLTester.h"
#include "URLComboBoxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CURLComboBoxEx

CURLComboBoxEx::CURLComboBoxEx()
{
}

CURLComboBoxEx::~CURLComboBoxEx()
{
}


BEGIN_MESSAGE_MAP(CURLComboBoxEx, CComboBoxEx)
	//{{AFX_MSG_MAP(CURLComboBoxEx)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, OnEditchange)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CURLComboBoxEx message handlers

void CURLComboBoxEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CComboBoxEx::OnChar(nChar, nRepCnt, nFlags);
}

void CURLComboBoxEx::OnEditchange() 
{
	// TODO: Add your control notification handler code here
	
}

void CURLComboBoxEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CComboBoxEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
