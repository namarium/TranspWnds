///\file ULWndCtrlList.cpp
///\brief cpp файл класса контейнера для плавающих панелек(03.08.2007)
#include "..\..\ULLib\Include\ULWndCtrlList.h"
#include "malloc.h"
namespace ULWnds
{
	namespace ULWndCtrls
	{
		WNDPROC CULWndCtrlList::lpOldParentProc=NULL;
		BOOL CULWndCtrlList::m_fActive=FALSE;
		RECT CULWndCtrlList::m_rcParentClientFreeRect={0,0,0,0};
		ULOther::CULArr<CULWndCtrl*> CULWndCtrlList::m_arWndCtrl=NULL;

		LRESULT CULWndCtrlList::SubClassParentProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
		{
			if(uMsg==WM_NOTIFY)
				for(unsigned int i=0;i<m_arWndCtrl.GetSize();i++)
					m_arWndCtrl[i]->SendMessage(WM_NOTIFY,wParam,lParam);
			if(uMsg==WM_SIZE)
			{
				HWND hClient=(HWND)::SendMessage(hWnd,CULWndCtrlList::MDIM_GETCLIENT,0,0);
				HDWP hdwp = BeginDeferWindowPos((int)m_arWndCtrl.GetSize()+((hClient!=NULL)?1:0));
				//=======================================
				m_rcParentClientFreeRect.left=0;
				m_rcParentClientFreeRect.top=0;
				m_rcParentClientFreeRect.right=LOWORD(lParam);
				m_rcParentClientFreeRect.bottom=HIWORD(lParam);
				//воткнуть ресайз клинта
				for(unsigned int i=0;i<m_arWndCtrl.GetSize();i++)
					if(m_arWndCtrl[i]->IsWindow()&&m_arWndCtrl[i]->IsDocked())
					{
						int nResDockSize = m_arWndCtrl[i]->GetDockedSize();
						switch(m_arWndCtrl[i]->GetDockedState())
						{
						case CULWndCtrl::dsAllowDockTop:
							{
								m_arWndCtrl[i]->DeferWindowPos(hdwp,0, m_rcParentClientFreeRect.left, 
									m_rcParentClientFreeRect.top, 
									m_rcParentClientFreeRect.right-
									m_rcParentClientFreeRect.left,
									nResDockSize, SWP_NOZORDER);//((wParam==SIZE_RESTORED)?0:SWP_NOREDRAW)
								m_rcParentClientFreeRect.top+=nResDockSize;
								break;
							}
						case CULWndCtrl::dsAllowDockBottom:
							{
								m_arWndCtrl[i]->DeferWindowPos(hdwp, 0, m_rcParentClientFreeRect.left, 
									m_rcParentClientFreeRect.bottom-nResDockSize,
									m_rcParentClientFreeRect.right-
									m_rcParentClientFreeRect.left,
									nResDockSize, SWP_NOZORDER);
								m_rcParentClientFreeRect.bottom-=nResDockSize;
								break;
							}
						}
					}
				for(unsigned int i=0;i<m_arWndCtrl.GetSize();i++)
					if(::IsWindow(*m_arWndCtrl[i])&&m_arWndCtrl[i]->IsDocked())
					{
						int nResDockSize = m_arWndCtrl[i]->GetDockedSize();
						switch(m_arWndCtrl[i]->GetDockedState())
						{
						case CULWndCtrl::dsAllowDockLeft:
							{
								m_arWndCtrl[i]->DeferWindowPos(hdwp, 0, 
									m_rcParentClientFreeRect.left, 
									m_rcParentClientFreeRect.top, 
									nResDockSize,
									m_rcParentClientFreeRect.bottom-
									m_rcParentClientFreeRect.top,
									SWP_NOZORDER/*|SWP_NOREDRAW*/);
								m_rcParentClientFreeRect.left+=nResDockSize;
								break;
							}
						case CULWndCtrl::dsAllowDockRight:
							{
								m_arWndCtrl[i]->DeferWindowPos(hdwp,0,
									m_rcParentClientFreeRect.right-nResDockSize, 
									m_rcParentClientFreeRect.top, 
									nResDockSize,
									m_rcParentClientFreeRect.bottom-
									m_rcParentClientFreeRect.top,
									SWP_NOZORDER/*|SWP_NOREDRAW*/);
								m_rcParentClientFreeRect.right-=nResDockSize;
								break;
							}
						}
					}
					if(hClient!=NULL)
						DeferWindowPos(hdwp,hClient,NULL,
							m_rcParentClientFreeRect.left,
							m_rcParentClientFreeRect.top,
							m_rcParentClientFreeRect.right-
							m_rcParentClientFreeRect.left,
							m_rcParentClientFreeRect.bottom-
							m_rcParentClientFreeRect.top,
							SWP_NOZORDER);
				//=======================================
				EndDeferWindowPos(hdwp);
				::BringWindowToTop(hClient);
			}
			//Делаем родительское окно активным, пока активно приложение
			if(uMsg==WM_ACTIVATEAPP)
			{
				m_fActive=(BOOL)wParam;
				::PostMessage(hWnd,WM_NCACTIVATE,wParam,0);
				::UpdateWindow(hWnd);
			}
			if(uMsg==WM_NCACTIVATE)
			{
				wParam=m_fActive;
				for(unsigned int i=0;i<m_arWndCtrl.GetSize();i++)
					if(m_arWndCtrl[i]->IsWindow()&&!m_arWndCtrl[i]->IsDocked())
						m_arWndCtrl[i]->PostMessage(WM_NCACTIVATE,wParam);
			}
			return ::CallWindowProc(lpOldParentProc,hWnd,uMsg,wParam,lParam);
		};

		CULWndCtrlList::CULWndCtrlList()
		{
			lpOldParentProc=NULL;
			m_fActive=FALSE;
		}

		CULWndCtrlList::CULWndCtrlList(const CULWndCtrlList& WndCtrlList)
		{
//#pragma message(__FILE__" не понял, почему варнинг")
			lpOldParentProc=WndCtrlList.lpOldParentProc;
			m_fActive=WndCtrlList.m_fActive;
			m_arWndCtrl=WndCtrlList.m_arWndCtrl;
			m_rcParentClientFreeRect=WndCtrlList.m_rcParentClientFreeRect;
		}

		CULWndCtrlList::~CULWndCtrlList()
		{
		}

		void CULWndCtrlList::operator=(const CULWndCtrlList& WndCtrlList)
		{
//#pragma message(__FILE__" не понял, почему варнинг")
			lpOldParentProc=WndCtrlList.lpOldParentProc;
			m_fActive=WndCtrlList.m_fActive;
			m_arWndCtrl=WndCtrlList.m_arWndCtrl;
			m_rcParentClientFreeRect=WndCtrlList.m_rcParentClientFreeRect;
		}

		BOOL CULWndCtrlList::Create(HWND hOwnerWnd)
		{
			m_fActive=TRUE;
			lpOldParentProc=(WNDPROC)(LONG_PTR)::SetWindowLong(hOwnerWnd,GWL_WNDPROC,
				(LONG)(LONG_PTR)&SubClassParentProc);
			return (lpOldParentProc!=NULL);
		};

		BOOL CULWndCtrlList::AddWndCtrl(CULWndCtrl* pULWndCtrl)
		{
			m_arWndCtrl.Resize(m_arWndCtrl.GetSize()+1);
			m_arWndCtrl[m_arWndCtrl.GetSize()-1]=pULWndCtrl;
			return (m_arWndCtrl!=NULL);
		}
	}
}
