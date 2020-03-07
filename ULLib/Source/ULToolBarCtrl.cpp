///\file ULToolBarCtrl.cpp
///\brief cpp файл класса тулбара размещенного на плавающей панельке(04.08.2007)
#include "..\..\ULLib\Include\ULToolBarCtrl.h"
namespace ULWnds
{
	namespace ULWndCtrls
	{
		CULToolBarCtrl::CULToolBarCtrl(void):CULWndCtrl()
		{
			MessageMap.AddMessage<CULToolBarCtrl>(WM_SIZE,&CULToolBarCtrl::OnSize);
		}

		CULToolBarCtrl::CULToolBarCtrl(CULToolBarCtrl& toolBarCtrl):CULWndCtrl(toolBarCtrl),
			m_ToolBar(toolBarCtrl.m_ToolBar)
		{
		}

		CULToolBarCtrl::~CULToolBarCtrl(void)
		{
		}

		void CULToolBarCtrl::operator=(CULToolBarCtrl& toolBarCtrl)
		{
			m_ToolBar=toolBarCtrl.m_ToolBar;
			ULWnds::ULWndCtrls::CULWndCtrl::operator=(toolBarCtrl);
		}

		BOOL CULToolBarCtrl::CreateToolBarCtrl(HWND hParentWnd,
										short tbID,
										int nXPos,
										int nYPos,
										int dxButton,//ширина кнопки
										int dyButton,//высота кнопки
										int dxBitmap,//ширина картинки для кнопки
										int dyBitmap,//высота картинки для кнопки
										DWORD dwDockedState,
										BOOL fDocked,
										DWORD tbStyle,
										DWORD tbExStyle,
										DWORD dwDockingStyles)
		{
			m_ToolBar.Create(hParentWnd,tbID,dxButton,dyButton,
				dxBitmap,dyBitmap,
				(((dwDockedState&dsAllowDockLeft)||(dwDockedState&dsAllowDockRight))?
				ULBars::CULToolBar::afLeft:ULBars::CULToolBar::afTop)
				|ULBars::CULToolBar::afNon,
				tbStyle,tbExStyle);
			if(!m_ToolBar)
				return FALSE;
			RECT rcToolBar;
			m_ToolBar.GetWindowRect(&rcToolBar);
			POINT ptLT={rcToolBar.left,rcToolBar.top};
			::ScreenToClient(m_ToolBar,&ptLT);
			POINT ptRB={rcToolBar.right,rcToolBar.bottom};
			::ScreenToClient(m_ToolBar,&ptRB);
			int rmvl=(fDocked)?cBorderButtonDock:cBorderButtonFloat;
			Create(NULL,nXPos,nYPos,ptRB.x-ptLT.x,ptRB.y-ptLT.y+rmvl,
				hParentWnd,dwDockedState,fDocked,dwDockingStyles);
			if(!*this)
				return FALSE;
			m_ToolBar.SetParent(*this);
			m_ToolBar.SetWindowPos(HWND_TOP,
				((dwDockedState&dsAllowDockLeft)||(dwDockedState&dsAllowDockRight))?
					cNonGripperBorder:cGripperBorder,
				((dwDockedState&dsAllowDockLeft)||(dwDockedState&dsAllowDockRight))?
					cNonGripperBorder:cGripperBorder,
				ptRB.x-ptLT.x,ptRB.y-ptLT.y,0);
			return 	(m_ToolBar!=NULL);
		};

		LRESULT CULToolBarCtrl::OnSize(WPARAM /*nType*/,LPARAM size)
		{
		//=========Переворачиваем тулбар=====================
			if((m_fDocked)&&((m_dwDockedState&dsAllowDockLeft)||
				(m_dwDockedState&dsAllowDockRight)))
			{
				DWORD dwStyle=(DWORD)m_ToolBar.SendMessage(TB_GETSTYLE,0,0);
				dwStyle&=~ULBars::CULToolBar::afTop;

				for(unsigned int i=0;i<m_ToolBar.GetButtonCount();i++)
				{
					TBBUTTON tbbut;
					m_ToolBar.SendMessage(TB_GETBUTTON,WPARAM(i),LPARAM(&tbbut));
					tbbut.fsState|=TBSTATE_WRAP;
					m_ToolBar.SendMessage(TB_SETSTATE,WPARAM(tbbut.idCommand),
						LPARAM(tbbut.fsState));
					m_ToolBar.SendMessage(TB_SETSTYLE,0,
						LPARAM(dwStyle|ULBars::CULToolBar::afLeft));
				}
			}
			else
			{
				DWORD dwStyle=(DWORD)m_ToolBar.SendMessage(TB_GETSTYLE,0,0);
				dwStyle&=~ULBars::CULToolBar::afLeft;
				::SendMessage(m_ToolBar,TB_SETSTYLE,0,
					LPARAM(dwStyle|ULBars::CULToolBar::afTop));
				for(unsigned int i=0;i<m_ToolBar.GetButtonCount();i++)
				{
					TBBUTTON tbbut;
					m_ToolBar.SendMessage(TB_GETBUTTON,WPARAM(i),LPARAM(&tbbut));
					tbbut.fsState&=~TBSTATE_WRAP;
					m_ToolBar.SendMessage(TB_SETSTATE,WPARAM(tbbut.idCommand),
						LPARAM(tbbut.fsState));
				}
			}
		//===================================================
			SIZE szWndTBC={0,0};
			RECT rcBtn,rcTBWidth;
			if(m_ToolBar.GetButtonCount()!=0)
			{
				m_ToolBar.GetItemRect(m_ToolBar.GetButtonCount()-1,&rcTBWidth);
				m_ToolBar.GetItemRect(0,&rcBtn);
				if(m_fDocked)
				{
					RECT rcClient;
					GetClientRect(&rcClient);
					szWndTBC.cx+=/*((m_dwDockedState&dsAllowDockLeft)||(m_dwDockedState&dsAllowDockRight))?
						rcClient.bottom:*/rcClient.right;
					szWndTBC.cy+=cBorderButtonDock;		
		//==========пересчёт размера прямоугольника для стыковки=======
					m_FloatingSize.cx=(((m_dwDockedState&dsAllowDockLeft)||(m_dwDockedState&dsAllowDockRight))?
						rcTBWidth.bottom:rcTBWidth.right)+cGripperSize;
					m_nDockedSize=((m_dwDockedState&dsAllowDockLeft)||(m_dwDockedState&dsAllowDockRight))?
						(rcBtn.bottom+5):(rcBtn.bottom+5);
					m_FloatingSize.cy=::GetSystemMetrics(SM_CYCAPTION)+cBorderButtonFloat;
		//==========Позиционируем тулбар на CtrlWnd======
					m_ToolBar.SetWindowPos(HWND_TOP,
						((m_dwDockedState&dsAllowDockLeft)||(m_dwDockedState&dsAllowDockRight))?
							cNonGripperBorder:cGripperBorder,
						((m_dwDockedState&dsAllowDockLeft)||(m_dwDockedState&dsAllowDockRight))?
							cGripperBorder:cNonGripperBorder,
						0,0,SWP_NOSIZE);
				}
				else
				{
					szWndTBC.cx+=rcTBWidth.right+cGripperSize;
					szWndTBC.cy+=::GetSystemMetrics(SM_CYCAPTION);
					szWndTBC.cy+=cBorderButtonFloat;
		//==========Позиционируем тулбар на CtrlWnd======
					m_ToolBar.SetWindowPos(HWND_TOP,
						cGripperBorder,cNonGripperBorder,0,0,SWP_NOSIZE|SWP_NOREDRAW);
				}
				szWndTBC.cy+=((m_fDocked)&&((m_dwDockedState&dsAllowDockLeft)||(m_dwDockedState&dsAllowDockRight)))?
					rcTBWidth.right:rcTBWidth.bottom;
		//==========Подгоняем размер CtrlWnd под размер тулбара======
				if((m_fDocked)&&((m_dwDockedState&dsAllowDockLeft)||(m_dwDockedState&dsAllowDockRight)))
					SetWindowPos(NULL,0,0,szWndTBC.cx,/*szWndTBC.cy+1000*/HIWORD(size),SWP_NOZORDER|SWP_NOMOVE);
				else
					SetWindowPos(NULL,0,0,szWndTBC.cx,szWndTBC.cy,SWP_NOZORDER|SWP_NOMOVE);
			}
			return TRUE;
		}
	}
}
