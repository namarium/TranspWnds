///\file ULSlider.cpp
///\brief фаил реализации класса обёртки для slider control
#include "..\Include\ULSlider.h"
#include "..\Include\ULStates.h"
namespace ULWnds
{
	namespace ULControls
	{
		CULSlider::CULSlider():
			CULSubClass()
		{
		}
		BOOL CULSlider::Create(HWND hParentWnd,UINT uID,LPCTSTR szCaption,int x,int y,int cx,int cy,DWORD dwStyle,DWORD dwStyleEx)
		{
			m_hParentWnd=hParentWnd;
			m_hWnd = CreateWindowEx(dwStyleEx,TRACKBAR_CLASS,
				szCaption,
				dwStyle, 
				x, y, cx, cy, 
				hParentWnd,(HMENU)(LONG_PTR)uID,   
				ULOther::ULGetResourceHandle(), 
				NULL);
			return CULSubClass::Attach(m_hWnd);
		}

		void CULSlider::SetRange(int nMin,int nMax,BOOL fRedraw)
		{
			SendMessage(TBM_SETRANGE,(WPARAM)fRedraw,MAKELPARAM(nMin,nMax));
		}
		void CULSlider::SetRangeMin(int nMin,BOOL fRedraw)
		{
			SendMessage(TBM_SETRANGEMIN,(WPARAM)fRedraw,(LPARAM)nMin);
		}
		void CULSlider::SetRangeMax(int nMax,BOOL fRedraw)
		{
			SendMessage(TBM_SETRANGEMAX,(WPARAM)fRedraw,(LPARAM)nMax);
		}
		void CULSlider::SetPos(int nPos,BOOL fRedraw)
		{
			SendMessage(TBM_SETPOS,(WPARAM)fRedraw,(LPARAM)nPos);
		}
		int CULSlider::GetPos()
		{
			return (int)SendMessage(TBM_GETPOS);
		}
		void CULSlider::SetThumbLenght(UINT nLen)
		{
			ASSERT((GetWindowLong(GWL_STYLE)&TBS_FIXEDLENGTH)==TBS_FIXEDLENGTH);
			SendMessage(TBM_SETTHUMBLENGTH,(WPARAM)nLen,0);
		}
		void CULSlider::SetTicFreq(UINT nFreq)
		{
			ASSERT((GetWindowLong(GWL_STYLE)&TBS_NOTICKS)!=TBS_NOTICKS);
			SendMessage(TBM_SETTICFREQ,(WPARAM)nFreq,0);
		}
		UINT CULSlider::SetLineSize(UINT nLineSize) 
		{
			return (UINT)SendMessage(TBM_SETLINESIZE,0,(LPARAM)nLineSize);
		}
		UINT CULSlider::SetPageSize(UINT nPageSize) 
		{
			return (UINT)SendMessage(TBM_SETPAGESIZE,0,(LPARAM)nPageSize);
		}
		
	}
}
