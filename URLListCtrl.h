#if !defined(AFX_URLLISTCTRL_H__910BA1F3_0D90_4D12_BAC0_889423A58AB0__INCLUDED_)
#define AFX_URLLISTCTRL_H__910BA1F3_0D90_4D12_BAC0_889423A58AB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// URLListCtrl.h : header file
//
#include "URLHeaderCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CURLListCtrl window

class CURLListCtrl : public CListCtrl
{
public:
	typedef struct {
		char** data_array;
	}TItemDataStruct;
	
// Construction
public:
	CURLListCtrl();

// Attributes
public:

// Operations
public:
    int GetCheckCount() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CURLListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CURLListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CURLListCtrl)
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOdstatechanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public: // Parent Method Hided
	BOOL DeleteAllItems();
	BOOL DeleteItem(int nItem);
	int InsertItem(int nItem, LPCTSTR lpszItem);
	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
	//CURLHeaderCtrl* GetHeaderCtrl() { return &m_URLHeaderCtrl; };

public:
    void Uncheck();
    void CheckAll();    
    void DeleteSelected();
	int GetSortedColumn() const { return m_nSortedColumn; }
	bool GetSortFlag(int nSortedColumn) const;
	void SetSortFlag(int nSortedColumn, bool bSortFlag);
	void RefreshColumnInfo();

private:
	void DeleteItemData(int nItem);
	void SendMessageToParent(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

private:	
	int m_nSortedColumn;
	bool* m_SortFlagArray;
	CURLHeaderCtrl m_URLHeaderCtrl;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLLISTCTRL_H__910BA1F3_0D90_4D12_BAC0_889423A58AB0__INCLUDED_)
