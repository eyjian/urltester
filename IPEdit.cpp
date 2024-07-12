// IPEdit.cpp : implementation file
//

#include "stdafx.h"
#include "URLTester.h"
#include "IPEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIPEdit

CIPEdit::CIPEdit()
{
}

CIPEdit::~CIPEdit()
{
}


BEGIN_MESSAGE_MAP(CIPEdit, CEdit)
	//{{AFX_MSG_MAP(CIPEdit)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPEdit message handlers

void CIPEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if ( ('.' == nChar) || (8 == nChar)
		|| ((nChar >= '0') && (nChar <= '9')) 
		|| IsCopyOrPasteOrCut(nChar) ) {
		if ('.' == nChar) {
			CString str;
			this->GetWindowText(str);
			if (!str.IsEmpty()) {
				if ('.' == str.GetAt(str.GetLength()-1))
					return;
				int nDotCount = 0;
				for (int i=0; i<str.GetLength(); ++i) {
					if ('.' == str[i])
						++nDotCount;
					if (nDotCount > 2)
						return;
				}
			}
			else 
			{
				return;
			}
		}
		CMyEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}
