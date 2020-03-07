///\file ULMDIChildWnd.cpp
///\brief cpp файл класса дочернего окна архетектуры MDI(03.08.2007)
#include "..\..\ULLib\Include\ULMDIChildWnd.h"
#include "..\..\ULLib\Include\ULApp.h"
#include "..\..\ULLib\Include\ULFrameWnd.h"
#include "..\..\ULLib\Include\ULRes.h"
namespace ULWnds
{
	namespace ULFrames
	{
		CULMDIChildWnd::CULMDIChildWnd(void):CULWnd()
		{
			MessageMap.AddMessage<CULMDIChildWnd>(WM_CREATE,&CULMDIChildWnd::OnCreate);
			MessageMap.AddMessage<CULMDIChildWnd>(WM_NCDESTROY,&CULMDIChildWnd::OnNcDestroy);
			MessageMap.AddMessage<CULMDIChildWnd>(WM_SHOWWINDOW,&CULMDIChildWnd::OnShowWindow);
		}

		CULMDIChildWnd::CULMDIChildWnd(CULMDIChildWnd& MDIChildWnd):CULWnd(MDIChildWnd)
		{}

		CULMDIChildWnd::~CULMDIChildWnd(void)
		{
		}

		void CULMDIChildWnd::operator = (CULMDIChildWnd& ChildWnd)
		{
			ULWnds::CULWnd::operator=(ChildWnd);
		}

		LRESULT CULMDIChildWnd::WndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
		{
			LONG dwNewLong;
			CULWnd *pULWnd=NULL;
			switch(uMsg)
			{
			case WM_CREATE:
				{
					dwNewLong=(LONG)(LONG_PTR)((LPMDICREATESTRUCT)
						((LPCREATESTRUCT) lParam)->lpCreateParams)->lParam;
					::SetWindowLong (hWnd, GWL_USERDATA,dwNewLong);
					// получим указатель на класс
					pULWnd = (CULWnd*)(LONG_PTR)dwNewLong; 
					pULWnd->m_hWnd=hWnd;
					break;
				}
			default:
				{
					pULWnd=FromHandle(hWnd);
					if(pULWnd!=NULL)
					{
						LRESULT lRes=pULWnd->OnMessage(uMsg,wParam,lParam);
						if(lRes)
							return lRes;

						if(uMsg==WM_COMMAND)
						{
							BOOL fRet=pULWnd->MessageMap.ProcessCommand(pULWnd,wParam,lParam);
							if(fRet)
								return fRet;
						}
						else 
							if(uMsg==WM_NOTIFY)
							{
								BOOL fRet=pULWnd->MessageMap.ProcessNotify(pULWnd,(NMHDR*)lParam);
								if(fRet)
									return fRet;
							}
						BOOL fRet=pULWnd->MessageMap.ProcessMessage(pULWnd,uMsg,wParam,lParam);
						if(fRet)
							return fRet;
						else
							return DefMDIChildProc(hWnd, uMsg, wParam, lParam);
					}
					else
						return DefMDIChildProc(hWnd, uMsg, wParam, lParam);
				}
			}
			return FALSE;	
		};

		HWND CULMDIChildWnd::Create(TCHAR* szWindowName,
									int x,
									int y,
									int cx,
									int cy,
									short IconID,
									HWND hClientWnd,
									short BGColor,
									DWORD dwChildStyle)
		{
			m_hParentWnd=hClientWnd;

			WNDCLASS wc; 

			wc.style         = 0; 
			wc.lpfnWndProc   = (WNDPROC) WndProc; 
			wc.cbClsExtra    = 0; 
			wc.cbWndExtra    = 0; 
			wc.hInstance     = ULOther::ULGetResourceHandle(); 
			wc.hIcon         = LoadIcon(wc.hInstance,MAKEINTRESOURCE(IconID)); 
			wc.hCursor       = LoadCursor( NULL, IDC_ARROW); 
			wc.hbrBackground = (HBRUSH) BGColor; 

			wc.lpszMenuName  = NULL; 
			
		#ifndef UNICODE
			wc.lpszClassName = "ULCHILDWND";  
		#else
			wc.lpszClassName = L"ULCHILDWND";  
		#endif//UNICODE	
			RegisterClass(&wc);
			m_hWnd=CreateMDIWindow(wc.lpszClassName,szWindowName,dwChildStyle,
				x,y,cx,cy,hClientWnd,ULOther::ULGetResourceHandle(),LPARAM(this));

			return m_hWnd;
		}

		LRESULT CULMDIChildWnd::OnCreate(WPARAM /*wParam*/,LPARAM)
		{
			return FALSE;
		}

		LRESULT CULMDIChildWnd::OnNcDestroy(WPARAM ,LPARAM)
		{
			::PostMessage(m_hParentWnd,CLM_DESTROYCHILD,WPARAM(m_hWnd),NULL);
			return TRUE;
		}	

		LRESULT CULMDIChildWnd::OnShowWindow(WPARAM ,LPARAM)
		{
			//обновляем регион при включенных стилях XP
			RECT rect;
			GetWindowRect(&rect);
			POINT ptLT={rect.left,rect.top};
			POINT ptRB={rect.right,rect.bottom};
			::ScreenToClient(m_hParentWnd,&ptLT);
			::ScreenToClient(m_hParentWnd,&ptRB);
			MoveWindow(ptLT.x,ptLT.y+1,
				ptRB.x-ptLT.x,ptRB.y-ptLT.y,TRUE);
			MoveWindow(ptLT.x,ptLT.y-1,
				ptRB.x-ptLT.x,ptRB.y-ptLT.y,TRUE);
			return FALSE;
		}	
	}
}
