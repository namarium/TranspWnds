///\file ULStatusBarCtrl.cpp
///\brief cpp файл класса статусбара размещенного на плавающей панельке(2006)
#include "..\..\ULLib\Include\ULStatusBarCtrl.h"
namespace ULWnds
{
	namespace ULWndCtrls
	{
		CULStatusBarCtrl::CULStatusBarCtrl(void):CULWndCtrl()
		{
			MessageMap.AddMessage<CULStatusBarCtrl>(WM_SIZE,&CULStatusBarCtrl::OnSize);
		}

		CULStatusBarCtrl::CULStatusBarCtrl(CULStatusBarCtrl& statusBarCtrl):CULWndCtrl(statusBarCtrl),
			m_StatusBar(statusBarCtrl.m_StatusBar)
		{
		}

		CULStatusBarCtrl::~CULStatusBarCtrl(void)
		{
		}

		void CULStatusBarCtrl::operator=(CULStatusBarCtrl& statusBarCtrl)
		{
			m_StatusBar=statusBarCtrl.m_StatusBar;
			ULWnds::ULWndCtrls::CULWndCtrl::operator=(statusBarCtrl);
		}

		BOOL CULStatusBarCtrl::CreateStatusBar(HWND hParentWnd,short nStatusID,BOOL fGrip)
		{
			m_StatusBar.Create(hParentWnd,nStatusID,fGrip);
			RECT rcStatusBar;
			m_StatusBar.GetWindowRect(&rcStatusBar);
			POINT ptLT={rcStatusBar.left,rcStatusBar.top};
			ScreenToClient(&ptLT);
			POINT ptRB={rcStatusBar.right,rcStatusBar.bottom};
			ScreenToClient(&ptRB);
			Create(NULL,ptLT.x,ptLT.y,ptRB.x-ptLT.x,ptRB.y-ptLT.y,
				hParentWnd,CULWndCtrl::dsAllowDockBottom,TRUE,0);
			m_StatusBar.SetParent(*this);
			m_StatusBar.SetWindowPos(HWND_TOP,0,0,
				ptRB.x-ptLT.x,ptRB.y-ptLT.y,SWP_SHOWWINDOW);
			m_StatusBar.AutoSize();
			return 	(m_StatusBar!=NULL);
		};

		LRESULT CULStatusBarCtrl::OnSize(WPARAM /*nType*/,LPARAM /*size*/)
		{
			m_StatusBar.AutoSize();
			return TRUE;
		}
	}
}
