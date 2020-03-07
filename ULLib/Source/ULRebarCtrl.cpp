///\file ULRebarCtrl.cpp
///\brief cpp файл класса ребара размещенного на плавающей панельке(31.08.2008)
#include "..\..\ULLib\Include\ULRebarCtrl.h"

namespace ULWnds
{
	namespace ULWndCtrls
	{
		CULRebarCtrl::CULRebarCtrl(void):CULWndCtrl(),m_fAutoSize(false)
		{
			MessageMap.AddMessage<CULRebarCtrl>(WM_SIZE,&CULRebarCtrl::OnSize);
		}

		///\brief Конструктор копирования
		CULRebarCtrl::CULRebarCtrl(CULRebarCtrl& rebarCtrl):CULWndCtrl(rebarCtrl),
			m_Rebar(rebarCtrl.m_Rebar),
			m_fAutoSize(rebarCtrl.m_fAutoSize),
			m_lpSubClassWndProc(rebarCtrl.m_lpSubClassWndProc)
		{
		}

		CULRebarCtrl::~CULRebarCtrl(void)
		{
		}

		///\brief оператор копирования
		void CULRebarCtrl::operator=(CULRebarCtrl& rebarCtrl)
		{
			m_Rebar=rebarCtrl.m_Rebar;
			m_fAutoSize=rebarCtrl.m_fAutoSize;
			m_lpSubClassWndProc=rebarCtrl.m_lpSubClassWndProc;
			ULWnds::ULWndCtrls::CULWndCtrl::operator=(rebarCtrl);
		}

		LRESULT CULRebarCtrl::ClientWndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
		{
			ULBars::CULRebar* pULWnd=(ULBars::CULRebar*)CULWnd::FromHandle(hWnd);			
			CULRebarCtrl* pRebarCtrl=(CULRebarCtrl*)pULWnd->GetProp(_T("ULRebarCtrl"));
			switch(uMsg)
			{
			case WM_SIZE:
				{
					RECT rect;
					pRebarCtrl->GetClientRect(&rect);
					pRebarCtrl->PostMessage(WM_SIZE,  SIZE_RESTORED,
						MAKELPARAM(rect.right-rect.left,rect.bottom-rect.top));
				}
				break;
			}
			return ::CallWindowProc(pRebarCtrl->m_lpSubClassWndProc,*pULWnd,uMsg,wParam,lParam);
		}

		BOOL CULRebarCtrl::CreateRebarCtrl(HWND hParentWnd,
							int nXPos,
							int nYPos,
							DWORD dwDockedState)
		{
			BOOL fDocked=TRUE;
			DWORD dwDockingStyles=
				dsUseBorders|dsBorderTop|dsBorderLeft|
				dsBorderBottom|dsBorderRight|dsAllowDockAll;

			ULWnds::ULBars::CULRebar::enAlignFlags rbAlignFlags=ULWnds::ULBars::CULRebar::afTop;
			switch(dwDockedState)
			{
			case ULWnds::ULWndCtrls::CULWndCtrl::dsAllowDockTop:
				rbAlignFlags=ULWnds::ULBars::CULRebar::afTop;
				break;
			case ULWnds::ULWndCtrls::CULWndCtrl::dsAllowDockBottom:
				rbAlignFlags=ULWnds::ULBars::CULRebar::afBottom;
				break;
			case ULWnds::ULWndCtrls::CULWndCtrl::dsAllowDockLeft:
				rbAlignFlags=ULWnds::ULBars::CULRebar::afLeft;
				break;
			case ULWnds::ULWndCtrls::CULWndCtrl::dsAllowDockRight:
				rbAlignFlags=ULWnds::ULBars::CULRebar::afRight;
				break;
			}

			m_Rebar.Create(hParentWnd,0,rbAlignFlags);
			if(!m_Rebar)
				return FALSE;
			m_Rebar.SetProp(_T("ULRebarCtrl"),(HANDLE)this);
			m_lpSubClassWndProc=(WNDPROC)(LONG_PTR)m_Rebar.SetWindowLong(GWL_WNDPROC,(LONG)(LONG_PTR)ClientWndProc);
			RECT rcRebar;
			m_Rebar.GetWindowRect(&rcRebar);
			POINT ptLT={rcRebar.left,rcRebar.top};
			::ScreenToClient(m_Rebar,&ptLT);
			POINT ptRB={rcRebar.right,rcRebar.bottom};
			::ScreenToClient(m_Rebar,&ptRB);
			Create(NULL,nXPos,nYPos,ptRB.x-ptLT.x,ptRB.y-ptLT.y,
				hParentWnd,dwDockedState,fDocked,dwDockingStyles);
			if(!*this)
				return FALSE;

			m_Rebar.SetParent(*this);
			return TRUE;
		}
		LRESULT CULRebarCtrl::OnSize(WPARAM /*nType*/,LPARAM /*size*/)
		{
			if(!m_fAutoSize)
			{
				m_Rebar.AutoSize();
				m_fAutoSize=true;
				return TRUE;
			}
			else
				m_fAutoSize=false;
			RECT rcRebar;			
			m_Rebar.GetWindowRect(&rcRebar);
			m_FloatingSize.cy=m_nDockedSize=rcRebar.bottom-rcRebar.top-2;
			m_FloatingSize.cx=rcRebar.right-rcRebar.left;
			ResizeAllWndCtrls();
			return TRUE;
		}
	}
}
