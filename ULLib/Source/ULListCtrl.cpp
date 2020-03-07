///\file ULListCtrl.cpp
///\brief фаил реализации класса лист контрола
#include "..\..\ULLib\Include\ULListCtrl.h"
#include "..\..\ULLib\Include\ULRes.h"

namespace ULWnds
{
	namespace ULControls
	{

		CULListCtrl::CULListCtrl():
			ULWnds::CULSubClass()
		{

		}

		BOOL CULListCtrl::Create(HWND hParentWnd,UINT uID,int x,int y,
			int cx,int cy,DWORD dwStyle,DWORD dwStyleEx)
		{
			m_hParentWnd=hParentWnd;
			m_hWnd = CreateWindowEx(dwStyleEx,WC_LISTVIEW,
				NULL,
				dwStyle, 
				x, y, cx, cy, 
				hParentWnd,(HMENU)(LONG_PTR)uID,   
				ULOther::ULGetResourceHandle(), 
				NULL);
			return CULSubClass::Attach(m_hWnd);
		}

		int CULListCtrl::InsertColumn(int nCol,LPCTSTR lpszColumnHeading,int nFormat,int nWidth,int nSubItem)
		{
			LVCOLUMN lvc;
			lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM|LVCF_FMT;
			lvc.pszText=(LPTSTR)lpszColumnHeading;
			lvc.cchTextMax=static_cast<int>(_tcslen(lpszColumnHeading)+1);
			lvc.fmt=nFormat;
			lvc.cx=nWidth;
			lvc.iSubItem=nSubItem;
			return (int)SendMessage(LVM_INSERTCOLUMN,(WPARAM)nCol,(LPARAM)&lvc);
		}

		BOOL CULListCtrl::SetColumnWidth(int nCol,int cx)
		{
			return (BOOL)SendMessage(LVM_SETCOLUMNWIDTH,(WPARAM)nCol,(LPARAM)cx);
		}

		int CULListCtrl::GetColumnWidth(int nCol)
		{
			return (int)SendMessage(LVM_GETCOLUMNWIDTH,(WPARAM)nCol);
		}

		int CULListCtrl::InsertItem(int nItem,LPCTSTR lpszItem)
		{
			LVITEM lvi;
			lvi.mask=LVIF_TEXT;
			lvi.iItem=nItem;
			lvi.iSubItem=0;
			lvi.pszText=(LPTSTR)lpszItem;
			lvi.cchTextMax=static_cast<int>(_tcslen(lpszItem)+1);
			return (int)SendMessage(LVM_INSERTITEM,0,(LPARAM)&lvi);
		}

		BOOL CULListCtrl::SetItemText(int nItem,int nSubItem,LPCTSTR lpszText)
		{
			LVITEM lvi;
			lvi.mask=LVIF_TEXT;
			lvi.iItem=nItem;
			lvi.iSubItem=nSubItem;
			lvi.pszText=(LPTSTR)lpszText;
			lvi.cchTextMax=static_cast<int>(_tcslen(lpszText)+1);
			return (BOOL)SendMessage(LVM_SETITEM,0,(LPARAM)&lvi);
		}

		BOOL CULListCtrl::GetItemText(int nItem,int nSubItem,LPTSTR lpszText,int nLen)
		{
			LVITEM lvi;
			lvi.mask=LVIF_TEXT;
			lvi.iItem=nItem;
			lvi.iSubItem=nSubItem;
			lvi.pszText=(LPTSTR)lpszText;
			lvi.cchTextMax=nLen;
			return (BOOL)SendMessage(LVM_GETITEM,0,(LPARAM)&lvi);
		}


		int CULListCtrl::GetItemCount()
		{
			return (int)SendMessage(LVM_GETITEMCOUNT);
		}

		DWORD CULListCtrl::SetExtendedStyle(DWORD dwNewStyle)
		{
			return (DWORD)SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,(WPARAM)dwNewStyle,(LPARAM)dwNewStyle);
		}

		BOOL CULListCtrl::DeleteAllItems()
		{
			return (BOOL)SendMessage(LVM_DELETEALLITEMS);
		}

		BOOL CULListCtrl::GetColumn(int nCol,LPLVCOLUMN pCol)
		{
			return (BOOL)SendMessage(LVM_GETCOLUMN,(WPARAM)nCol,(LPARAM)pCol);	
		}

		BOOL CULListCtrl::GetItemRect(int nItem,LPRECT lpRect,UINT nCode)
		{
			lpRect->left = nCode;
			return (BOOL)SendMessage(LVM_GETITEMRECT, (WPARAM)nItem,(LPARAM)lpRect);
		}

		BOOL CULListCtrl::GetSubItemRect(int iItem, int iSubItem, int nArea, RECT* lpRect)
		{
			lpRect->top = iSubItem;
			lpRect->left = nArea;
			return (BOOL)SendMessage(LVM_GETSUBITEMRECT,iItem, (LPARAM)lpRect);
		}
		int CULListCtrl::GetSubItemCount()
		{
			HWND hwndHeader=(HWND)SendMessage(LVM_GETHEADER);
			if(!hwndHeader)
				return -1;
			return (int)::SendMessage(hwndHeader,HDM_GETITEMCOUNT,0,0);			
		}
		int CULListCtrl::HitTest(LPLVHITTESTINFO pInfo)
		{
			return(int)SendMessage(LVM_HITTEST,0,(LPARAM)pInfo);
		}
		int CULListCtrl::SubItemHitTest(LPLVHITTESTINFO pInfo)
		{
			return(int)SendMessage(LVM_SUBITEMHITTEST,0,(LPARAM)pInfo);
		}
		BOOL CULListCtrl::SetItemData(int nItem,DWORD_PTR dwData)
		{
			LVITEM it={0};
			it.mask=LVIF_PARAM;
			it.iItem=nItem;
			it.lParam=dwData;
			return (BOOL)SendMessage(LVM_SETITEM,0,(LPARAM)&it);
		}
		DWORD_PTR CULListCtrl::GetItemData(int nItem)
		{
			LVITEM it={0};
			it.mask=LVIF_PARAM;
			it.iItem=nItem;
			SendMessage(LVM_GETITEM,0,(LPARAM)&it);
			return it.lParam;
		}
		int CULListCtrl::GetFirstSelectedItemPosition()
		{
			return (int)SendMessage(LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_SELECTED);
		}

		int CULListCtrl::GetNextSelectedItemPosition(int pos)
		{
			return (int)SendMessage(LVM_GETNEXTITEM,(WPARAM)pos,(LPARAM)LVNI_SELECTED);
		}

		BOOL CULListCtrl::DeleteItem(int nItem)
		{
			return (BOOL)SendMessage(LVM_DELETEITEM,(WPARAM)nItem);
		}



	}
}
