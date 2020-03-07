///\file ULProgressBar.cpp
///\brief фаил реализации класса обёртки для ProgressBar
#define _WIN32_WINNT 0x0501
#include "..\Include\ULProgressBar.h"
#include "..\Include\ULStates.h"
#include <commctrl.h>
namespace ULWnds
{
	namespace ULControls
	{
		CULProgressBar::CULProgressBar():
			ULWnds::CULSubClass()
		{
		}

		BOOL CULProgressBar::Create(HWND hParentWnd,UINT uID,int x,int y,int cx,int cy,DWORD dwStyle,DWORD dwStyleEx)
		{
			m_hParentWnd=hParentWnd;
			m_hWnd = CreateWindowEx(dwStyleEx,PROGRESS_CLASS,
				NULL,
				dwStyle, 
				x, y, cx, cy, 
				hParentWnd,(HMENU)(LONG_PTR)uID,   
				ULOther::ULGetResourceHandle(), 
				NULL);
			return CULSubClass::Attach(m_hWnd);
		}

		UINT CULProgressBar::GetPos()
		{
			return (UINT)SendMessage(PBM_GETPOS);
		}

		UINT CULProgressBar::SetPos(UINT nPos)
		{
			return (UINT)SendMessage(PBM_SETPOS,(LPARAM)nPos,0);
		}

		void CULProgressBar::GetRange(int* pnLower,int* pnUpper)
		{
			ASSERT(pnLower);
			ASSERT(pnUpper);
			PBRANGE pbr;
			SendMessage(PBM_GETRANGE,0,(LPARAM)&pbr);
			*pnLower=pbr.iLow;
			*pnUpper=pbr.iHigh;
		}

		int CULProgressBar::GetRange(BOOL fWhichLimit)
		{
			return (int)SendMessage(PBM_GETRANGE,fWhichLimit);
		}

		void CULProgressBar::SetRange(short nLower,short nUpper)
		{
			SendMessage(PBM_SETRANGE,0,MAKELPARAM(nLower,nUpper));
		}

		void CULProgressBar::SetRange32(int nLower,int nUpper)
		{
			SendMessage(PBM_SETRANGE32,(WPARAM)nLower,(LPARAM)nUpper);
		}

		int CULProgressBar::DeltaPos(int nIncrement)
		{
			return (int)SendMessage(PBM_DELTAPOS,(WPARAM)nIncrement);
		}

		COLORREF CULProgressBar::SetBkColor(COLORREF clrNew)
		{
			return (COLORREF)SendMessage(PBM_SETBKCOLOR,0,(LPARAM)clrNew);
		}
#if _WIN32_WINNT >= 0x0501
		BOOL CULProgressBar::SetMarquee(BOOL fEnable)
		{
			return (BOOL)SendMessage(PBM_SETMARQUEE,(WPARAM)fEnable);
		}
#endif
		int CULProgressBar::SetStep(int nStep)
		{
			return (int)SendMessage(PBM_SETSTEP,(WPARAM)nStep);
		}

		int CULProgressBar::StepIt()
		{
			return (int)SendMessage(PBM_STEPIT);
		}
	}
}
