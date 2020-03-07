///\file ULComboBox.cpp
///\brief фаил реализации класса лист контрола
#include "..\..\ULLib\Include\ULComboBox.h"
#include "..\..\ULLib\Include\ULRes.h"

namespace ULWnds
{
	namespace ULControls
	{
		CULComboBox::CULComboBox():
			ULWnds::CULSubClass()
		{

		}

		BOOL CULComboBox::Create(HWND hParentWnd,UINT uID,int x,int y,
			int cx,int cy,DWORD dwStyle,DWORD dwStyleEx)
		{
			m_hParentWnd=hParentWnd;
			m_hWnd = CreateWindowEx(dwStyleEx,_T("COMBOBOX"),
				NULL,
				dwStyle, 
				x, y, cx, cy, 
				hParentWnd,(HMENU)(LONG_PTR)uID,   
				ULOther::ULGetResourceHandle(), 
				NULL);
			return CULSubClass::Attach(m_hWnd);
		}

		int CULComboBox::AddString(LPCTSTR pszStr)
		{
			return (int)SendMessage(CB_ADDSTRING,0,(LPARAM)pszStr);
		}

		int CULComboBox::InsertString(int nItem,LPCTSTR pszStr)
		{
			return (int)SendMessage(CB_INSERTSTRING,(WPARAM)nItem,(LPARAM)pszStr);
		}

		int CULComboBox::DeleteString(int nItem)
		{
			return (int)SendMessage(CB_DELETESTRING,(WPARAM)nItem);
		}

		BOOL CULComboBox::SetItemData(int nItem,DWORD dwData)
		{
			return (SendMessage(CB_SETITEMDATA,(WPARAM)nItem,(LPARAM)dwData)!=CB_ERR);
		}

		DWORD CULComboBox::GetItemData(int nItem)
		{
			return (DWORD)SendMessage(CB_GETITEMDATA,(WPARAM)nItem);
		}
					
		int CULComboBox::SetCurSel(int nItem)
		{
			return (int)SendMessage(CB_SETCURSEL,(WPARAM)nItem);
		}

		int CULComboBox::GetCurSel()
		{
			return (int)SendMessage(CB_GETCURSEL);
		}
		void CULComboBox::ResetContent()
		{
			SendMessage(CB_RESETCONTENT);
		}
		void CULComboBox::ShowDropDown(BOOL fShowIt)
		{
			SendMessage(CB_SHOWDROPDOWN,(WPARAM)fShowIt);
		}
		BOOL CULComboBox::GetComboBoxInfo(PCOMBOBOXINFO pcbi)
		{
			return ::GetComboBoxInfo(*this,pcbi);
		}
		int CULComboBox::GetCount()
		{
			return (int)SendMessage(CB_GETCOUNT);
		}
		int CULComboBox::GetLBText(int nItem,LPTSTR pszText)
		{
			return (int)SendMessage(CB_GETLBTEXT,(WPARAM)nItem,(LPARAM)pszText);
		}
		int CULComboBox::GetLBTextLen(int nItem)
		{
			return (int)SendMessage(CB_GETLBTEXT,(WPARAM)nItem);
		}
		int CULComboBox::FindString(int nItem,LPCTSTR pszStr)
		{
			return (int)SendMessage(CB_FINDSTRING,(WPARAM)nItem,(LPARAM)pszStr);
		}
	}
}
