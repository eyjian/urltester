////////////////////////////////////////////////////////////////
// VCKBASE -- August 2000
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably NT too.
//
// CHyperlink implements a simple text hyperlink
//
#ifndef _HYPRILNK_H
#define _HYPRILNK_H

//////////////////
// Simple text hyperlink derived from CString
//
class CHyperlink : public CString {
public:
	CHyperlink(LPCTSTR lpLink = NULL) : CString(lpLink) { }
	~CHyperlink() { }
	const CHyperlink& operator=(LPCTSTR lpsz) {
		CString::operator=(lpsz);
		return *this;
	}
	operator LPCTSTR() {
		return CString::operator LPCTSTR(); 
	}
	virtual HINSTANCE Navigate() {
		return IsEmpty() ? NULL :
			ShellExecute(0, _T("open"), *this, 0, 0, SW_SHOWNORMAL);
	}
};

#endif
