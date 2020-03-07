///\file ULStatic.cpp
///\brief cpp файл класса обёртки для STATIC контрола(23.12.2008)
#include "..\..\ULLib\Include\ULStatic.h"
#include "..\..\ULLib\Include\ULRes.h"

namespace ULWnds
{
	namespace ULControls
	{
		CULStatic::CULStatic():
			CULSubClass()
		{}

		BOOL CULStatic::Create(HWND hParentWnd,LPCTSTR pszText,UINT uID,int x,int y,
			int cx,int cy,DWORD dwStyle,DWORD dwStyleEx)
		{
			m_hParentWnd=hParentWnd;
			m_hWnd = CreateWindowEx(dwStyleEx,_T("STATIC"),
				NULL,
				dwStyle, 
				x, y, cx, cy, 
				hParentWnd,(HMENU)(LONG_PTR)uID,   
				ULOther::ULGetResourceHandle(), 
				NULL);
 			SetWindowText(const_cast<LPTSTR>(pszText)); 
			return CULSubClass::Attach(m_hWnd);
		};
	}
}
