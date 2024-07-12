// URLListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "URLTester.h"
#include "URLListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamData)
{
	CURLListCtrl::TItemDataStruct* data1 = NULL;
	CURLListCtrl::TItemDataStruct* data2 = NULL;
	CURLListCtrl* pThis = (CURLListCtrl *)lParamData;
	int nSortedColumn = pThis->GetSortedColumn();
	bool bSortFlag = pThis->GetSortFlag(nSortedColumn);

	if (bSortFlag) {		
		data1 = (CURLListCtrl::TItemDataStruct *)lParam2;
		data2 = (CURLListCtrl::TItemDataStruct *)lParam1;		
	}
	else {	
		data1 = (CURLListCtrl::TItemDataStruct *)lParam1;
		data2 = (CURLListCtrl::TItemDataStruct *)lParam2;	
	}

	if (!data1 || !data2) return 0;
	if (!data1->data_array || !data2->data_array) return 0;	

	//((CURLHeaderCtrl *)pThis->GetHeaderCtrl())->SetSortArrow(nSortedColumn, bSortFlag);
	return strcmp(data1->data_array[nSortedColumn], data2->data_array[nSortedColumn]);
}

/////////////////////////////////////////////////////////////////////////////
// CURLListCtrl

CURLListCtrl::CURLListCtrl()
{	
	m_nSortedColumn = -1;
	m_SortFlagArray = NULL;
}
CURLListCtrl::~CURLListCtrl()
{
	delete []m_SortFlagArray;
}


BEGIN_MESSAGE_MAP(CURLListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CURLListCtrl)
	ON_NOTIFY_REFLECT(HDN_ITEMCHANGED, OnItemchanged)
	ON_NOTIFY_REFLECT(LVN_ODSTATECHANGED, OnOdstatechanged)
	ON_NOTIFY_REFLECT(HDN_ITEMCLICK, OnItemclick)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_SETFOCUS, OnSetfocus)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CURLListCtrl message handlers

void CURLListCtrl::SendMessageToParent(UINT message, WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(message, wParam, lParam);
}

void CURLListCtrl::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here	
	SendMessageToParent(WM_LISTCTRL_ITEM_CHANGED, (WPARAM)this, (LPARAM)phdn);
	*pResult = 0;
}

void CURLListCtrl::OnOdstatechanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVODSTATECHANGE* pStateChanged = (NMLVODSTATECHANGE*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CURLListCtrl::OnItemclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CURLListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnLButtonDown(nFlags, point);
}

void CURLListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	*pResult = 0;
	SendMessageToParent(WM_LISTCTRL_ITEM_CLICK, (WPARAM)this, (LPARAM)phdn);	
}

void CURLListCtrl::OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

int CURLListCtrl::GetCheckCount() const
{
    int CheckCount = 0;
    for (int i=0; i<GetItemCount(); ++i) {
        if (GetCheck(i))
            ++CheckCount;
    }
    
    return CheckCount;
}

void CURLListCtrl::Uncheck()
{
    for (int i=0; i<GetItemCount(); ++i)
    {
        if (GetCheck(i))
            SetCheck(i, FALSE);
        else
            SetCheck(i, TRUE);
    }
}
void CURLListCtrl::CheckAll()
{
    if (GetCheckCount() == GetItemCount())
    {    
        for (int i=0; i<GetItemCount(); ++i)
            SetCheck(i, FALSE);
    }
    else {
        for (int i=0; i<GetItemCount(); ++i)
            SetCheck(i, TRUE);
    }
}
void CURLListCtrl::DeleteSelected()
{
	if (GetCheckCount() < 1) {
		AfxMessageBox("请选中需要删除的目标。"); SetFocus(); return;
	}
    for (int i=0; i<GetItemCount(); ++i)
    {
        if (GetCheck(i)) {
            DeleteItem(i);
            i = -1;
        }
    }
}

int CURLListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CURLListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_URLHeaderCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());
	CListCtrl::PreSubclassWindow();		
}

void CURLListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_nSortedColumn = pNMListView->iSubItem;
	SortItems(CompareFunc, (DWORD)this);
	SetSortFlag(m_nSortedColumn, !GetSortFlag(m_nSortedColumn));
	*pResult = 0;
}

void CURLListCtrl::DeleteItemData(int nItem)
{
	TItemDataStruct* data =  (TItemDataStruct *)GetItemData(nItem);
	if (data) {
		for (int nItem=0; nItem<GetHeaderCtrl()->GetItemCount(); ++nItem)
			delete []data->data_array[nItem];
		delete []data->data_array;
		delete data;
		
		SetItemData(nItem, 0);
	}	
}
BOOL CURLListCtrl::DeleteAllItems()
{
	for (int nItem=0; nItem<GetItemCount(); ++nItem)
		DeleteItemData(nItem);
	return CListCtrl::DeleteAllItems();
}
BOOL CURLListCtrl::DeleteItem(int nItem)
{
	DeleteItemData(nItem);
	return CListCtrl::DeleteItem(nItem);
}
int CURLListCtrl::InsertItem(int nItem, LPCTSTR lpszItem)
{
	int nRow = CListCtrl::InsertItem(nItem, lpszItem);
	if (nRow < 0) return FALSE;

	TItemDataStruct* data = new TItemDataStruct;
	data->data_array =  new char*[GetHeaderCtrl()->GetItemCount()];
	data->data_array[0] = new char[strlen(lpszItem)+1];	
	strcpy(data->data_array[0], lpszItem);
	for (int i=1; i<GetHeaderCtrl()->GetItemCount(); ++i)
		data->data_array[i] = NULL;

	SetItemData(nItem, (DWORD)data);
	return nRow;
}

BOOL CURLListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{
	if (!CListCtrl::SetItemText(nItem, nSubItem, lpszText)) return FALSE;

	TItemDataStruct* data =  (TItemDataStruct *)GetItemData(nItem);
	if (data && data->data_array) {
		delete []data->data_array[nSubItem];
		data->data_array[nSubItem] = new char[strlen(lpszText)+1];
		strcpy(data->data_array[nSubItem], lpszText);
	}		
	
	return TRUE;
}

bool CURLListCtrl::GetSortFlag(int nSortedColumn) const
{
	if (NULL == m_SortFlagArray) return false;
	return m_SortFlagArray[nSortedColumn];
}
void CURLListCtrl::SetSortFlag(int nSortedColumn, bool bSortFlag)
{
	if (m_SortFlagArray != NULL)
		m_SortFlagArray[nSortedColumn] = bSortFlag;
}

void CURLListCtrl::RefreshColumnInfo()
{
	delete []m_SortFlagArray;
	m_SortFlagArray = new bool[GetHeaderCtrl()->GetItemCount()];
	for (int i=0; i<GetHeaderCtrl()->GetItemCount(); ++i)
		m_SortFlagArray[i] = false;
}

void CURLListCtrl::OnDestroy() 
{
	CListCtrl::OnDestroy();
	
	// TODO: Add your message handler code here
	DeleteAllItems();
}
