///\file ULWndCtrl.cpp
///\brief cpp файл класса окна типа тулвиндоу(21.07.2007)
#include "..\..\ULLib\Include\ULWndCtrl.h"
#include "..\..\ULLib\Include\ULRes.h"
namespace ULWnds
{
	namespace ULWndCtrls
	{
		CULWndCtrl::CULWndCtrl():CULWnd(),
			m_fDragging(FALSE),
			m_fDocked(FALSE),
			m_fControl(FALSE),
			m_fOldDrawDocked(FALSE),
			m_fOldControl(FALSE),
			m_nDockedSize(0),
			m_dwDockedState(0),
			m_dwDockingStyle(0),
			m_fMouseMoved(0)
		{
			m_ptPos.x=0;
			m_ptPos.y=0;
			m_rcBorderDock.left=0;
			m_rcBorderDock.top=0;
			m_rcBorderDock.right=0;
			m_rcBorderDock.bottom=0;
			m_rcBorderFloat.left=0;
			m_rcBorderFloat.top=0;
			m_rcBorderFloat.right=0;
			m_rcBorderFloat.bottom=0;
			m_rcOldRect.left=0;
			m_rcOldRect.top=0;
			m_rcOldRect.right=0;
			m_rcOldRect.bottom=0;
			m_FloatingSize.cx=0;
			m_FloatingSize.cy=0;
			m_ptCursorPos.x=0;
			m_ptCursorPos.y=0;
			m_rcStart.left=0;
			m_rcStart.top=0;
			m_rcStart.right=0;
			m_rcStart.bottom=0;
			m_ptOldCursorPos.x=0;
			m_ptOldCursorPos.y=0;
			m_FrameSize.cx=0;
			m_FrameSize.cy=0;

			//====
			MessageMap.AddMessage<CULWndCtrl>(WM_NCLBUTTONDBLCLK,&CULWndCtrl::OnNCLButtonDblClick);
			MessageMap.AddMessage<CULWndCtrl>(WM_WINDOWPOSCHANGED,&CULWndCtrl::OnWindowPosChanged);
			MessageMap.AddMessage<CULWndCtrl>(WM_NCLBUTTONDOWN,&CULWndCtrl::OnNCLButtonDown);
			MessageMap.AddMessage<CULWndCtrl>(WM_MOUSEMOVE,&CULWndCtrl::OnMouseMove);
			MessageMap.AddMessage<CULWndCtrl>(WM_LBUTTONUP,&CULWndCtrl::OnLButtonUp);
			MessageMap.AddMessage<CULWndCtrl>(WM_GETMINMAXINFO,&CULWndCtrl::OnGetMinMaxInfo);
			MessageMap.AddMessage<CULWndCtrl>(WM_SETTINGCHANGE,&CULWndCtrl::OnSettingChange);
			MessageMap.AddMessage<CULWndCtrl>(WM_ERASEBKGND,&CULWndCtrl::OnEraseBkGnd);
			MessageMap.AddMessage<CULWndCtrl>(WM_NCHITTEST,&CULWndCtrl::OnNCHitTest);
			MessageMap.AddMessage<CULWndCtrl>(WM_CLOSE,&CULWndCtrl::OnClose);
		};

		CULWndCtrl::CULWndCtrl(CULWndCtrl& WndCtrl):CULWnd(WndCtrl),
			m_fDragging(WndCtrl.m_fDragging),
			m_fDocked(WndCtrl.m_fDocked),
			m_fControl(WndCtrl.m_fControl),
			m_fOldDrawDocked(WndCtrl.m_fOldDrawDocked),
			m_fOldControl(WndCtrl.m_fOldControl),
			m_nDockedSize(WndCtrl.m_nDockedSize),
			m_dwDockedState(WndCtrl.m_dwDockedState),
			m_FrameSize(WndCtrl.m_FrameSize),
			m_dwDockingStyle(WndCtrl.m_dwDockingStyle),
			m_fMouseMoved(WndCtrl.m_fMouseMoved)
		{
			m_ptPos.x=WndCtrl.m_ptPos.x;
			m_ptPos.y=WndCtrl.m_ptPos.y;
			m_rcBorderDock.left=WndCtrl.m_rcBorderDock.left;
			m_rcBorderDock.top=WndCtrl.m_rcBorderDock.top;
			m_rcBorderDock.right=WndCtrl.m_rcBorderDock.right;
			m_rcBorderDock.bottom=WndCtrl.m_rcBorderDock.bottom;
			m_rcBorderFloat.left=WndCtrl.m_rcBorderFloat.left;
			m_rcBorderFloat.top=WndCtrl.m_rcBorderFloat.top;
			m_rcBorderFloat.right=WndCtrl.m_rcBorderFloat.right;
			m_rcBorderFloat.bottom=WndCtrl.m_rcBorderFloat.bottom;
			m_rcOldRect.left=WndCtrl.m_rcOldRect.left;
			m_rcOldRect.top=WndCtrl.m_rcOldRect.top;
			m_rcOldRect.right=WndCtrl.m_rcOldRect.right;
			m_rcOldRect.bottom=WndCtrl.m_rcOldRect.bottom;
			m_FloatingSize.cx=WndCtrl.m_FloatingSize.cx;
			m_FloatingSize.cy=WndCtrl.m_FloatingSize.cy;
			m_ptCursorPos.x=WndCtrl.m_ptCursorPos.x;
			m_ptCursorPos.y=WndCtrl.m_ptCursorPos.y;
			m_rcStart.left=WndCtrl.m_rcStart.left;
			m_rcStart.top=WndCtrl.m_rcStart.top;
			m_rcStart.right=WndCtrl.m_rcStart.right;
			m_rcStart.bottom=WndCtrl.m_rcStart.bottom;
			m_ptOldCursorPos.x=WndCtrl.m_ptOldCursorPos.x;
			m_ptOldCursorPos.y=WndCtrl.m_ptOldCursorPos.y;
		}

		CULWndCtrl::~CULWndCtrl()
		{
		};

		void CULWndCtrl::operator=(CULWndCtrl& WndCtrl)
		{
			m_fDragging=WndCtrl.m_fDragging;
			m_fDocked=WndCtrl.m_fDocked;
			m_fControl=WndCtrl.m_fControl;
			m_fOldDrawDocked=WndCtrl.m_fOldDrawDocked;
			m_fOldControl=WndCtrl.m_fOldControl;
			m_nDockedSize=WndCtrl.m_nDockedSize;
			m_dwDockedState=WndCtrl.m_dwDockedState;
			m_FrameSize=WndCtrl.m_FrameSize;
			m_dwDockingStyle=WndCtrl.m_dwDockingStyle;
			m_fMouseMoved=WndCtrl.m_fMouseMoved;
			m_ptPos.x=WndCtrl.m_ptPos.x;
			m_ptPos.y=WndCtrl.m_ptPos.y;
			m_rcBorderDock.left=WndCtrl.m_rcBorderDock.left;
			m_rcBorderDock.top=WndCtrl.m_rcBorderDock.top;
			m_rcBorderDock.right=WndCtrl.m_rcBorderDock.right;
			m_rcBorderDock.bottom=WndCtrl.m_rcBorderDock.bottom;
			m_rcBorderFloat.left=WndCtrl.m_rcBorderFloat.left;
			m_rcBorderFloat.top=WndCtrl.m_rcBorderFloat.top;
			m_rcBorderFloat.right=WndCtrl.m_rcBorderFloat.right;
			m_rcBorderFloat.bottom=WndCtrl.m_rcBorderFloat.bottom;
			m_rcOldRect.left=WndCtrl.m_rcOldRect.left;
			m_rcOldRect.top=WndCtrl.m_rcOldRect.top;
			m_rcOldRect.right=WndCtrl.m_rcOldRect.right;
			m_rcOldRect.bottom=WndCtrl.m_rcOldRect.bottom;
			m_FloatingSize.cx=WndCtrl.m_FloatingSize.cx;
			m_FloatingSize.cy=WndCtrl.m_FloatingSize.cy;
			m_ptCursorPos.x=WndCtrl.m_ptCursorPos.x;
			m_ptCursorPos.y=WndCtrl.m_ptCursorPos.y;
			m_rcStart.left=WndCtrl.m_rcStart.left;
			m_rcStart.top=WndCtrl.m_rcStart.top;
			m_rcStart.right=WndCtrl.m_rcStart.right;
			m_rcStart.bottom=WndCtrl.m_rcStart.bottom;
			m_ptOldCursorPos.x=WndCtrl.m_ptOldCursorPos.x;
			m_ptOldCursorPos.y=WndCtrl.m_ptOldCursorPos.y;
			m_FrameSize.cx=WndCtrl.m_FrameSize.cx;
			m_FrameSize.cy=WndCtrl.m_FrameSize.cy;

			ULWnds::CULWnd::operator=(WndCtrl);
		}	

		LRESULT CULWndCtrl::OnNCHitTest(WPARAM Type,LPARAM CurPos)
		{
			UINT uHitTest=(UINT)::DefWindowProc(*this, WM_NCHITTEST, Type, CurPos);		
			if(uHitTest == HTCLIENT)
				uHitTest = HTCAPTION;
			return (BOOL)uHitTest;
		};

		LRESULT CULWndCtrl::OnClose(WPARAM,LPARAM)
		{
			return (::SendMessage(m_hParentWnd,CWM_CLOSE,WPARAM(m_hWnd),0)!=TRUE);
		};

		LRESULT CULWndCtrl::OnNCLButtonDblClick(WPARAM wParam,LPARAM)
		{
			if(wParam==HTCAPTION)
				ToggleDockingMode();
			return FALSE;
		}

		LRESULT CULWndCtrl::OnWindowPosChanged(WPARAM,LPARAM lParam)
		{
			WINDOWPOS* pWindowPos=(WINDOWPOS*)lParam;
			RECT rcBorder={0,0,0,0};
			RECT rcClient;

			if(!m_fDocked)
			{
				if(!(pWindowPos->flags & SWP_NOMOVE))
				{
					m_ptPos.x=pWindowPos->x;
					m_ptPos.y=pWindowPos->y;
				}
				if(!(pWindowPos->flags & SWP_NOSIZE))
				{
					m_FrameSize.cx=pWindowPos->cx;
					m_FrameSize.cy=pWindowPos->cy;
					RECT rect;
					GetClientRect(&rect);
					m_FloatingSize.cx=rect.right;
					m_FloatingSize.cy=rect.bottom;
				}
				if(m_dwDockingStyle&dsUseBorders)
				{
					rcBorder.left	+=m_rcBorderFloat.left;
					rcBorder.right	+=m_rcBorderFloat.right;
					rcBorder.top	+=m_rcBorderFloat.top;
					rcBorder.bottom	+=m_rcBorderFloat.bottom;
				}
				::SetRect(&rcClient,0,0,m_FloatingSize.cx,m_FloatingSize.cy);
			}
			else
			{
				if(m_dwDockingStyle&dsBorderTop)rcBorder.top		+=2;
				if(m_dwDockingStyle&dsBorderBottom)rcBorder.bottom+=2;
				if(m_dwDockingStyle&dsBorderLeft)rcBorder.left	+=2;
				if(m_dwDockingStyle&dsBorderRight)rcBorder.right	+=2;

				if(m_dwDockingStyle&dsUseBorders)
				{
					rcBorder.left	+=m_rcBorderDock.left;
					rcBorder.right	+=m_rcBorderDock.right;
					rcBorder.top	+=m_rcBorderDock.top;
					rcBorder.bottom	+=m_rcBorderDock.bottom;
				}
			}
			if(m_dwDockingStyle&(dsDrawGripperDocked|dsDrawGripperFloating))
				rcBorder.left+=7;
			if(!(pWindowPos->flags & SWP_NOSIZE))
				InvalidateRect(0, TRUE);
			return FALSE;
		};

		LRESULT CULWndCtrl::OnNCLButtonDown(WPARAM wParam,LPARAM)
		{
			HCURSOR hMovableCursor = LoadCursor(NULL, IDC_SIZEALL); 
			::SetCursor(hMovableCursor);
			m_fDragging=FALSE;
			if(m_dwDockingStyle&(dsForceDock|dsForceFloat))
				return FALSE; 
			if(m_fDocked)
			{
				m_fControl=FALSE;
				m_fOldDrawDocked=TRUE;
			}
			else
			{
				m_fControl=TRUE;
				m_fOldDrawDocked=FALSE;
			}
			m_fOldControl=m_fControl;
			if(wParam==HTCAPTION)
			{
				SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
				GetWindowRect(&m_rcOldRect);
				DrawXorFrame(&m_rcOldRect,m_fOldDrawDocked);
				::GetCursorPos(&m_ptCursorPos);
				GetWindowRect(&m_rcStart);
				::SetCapture(*this);
				m_fMouseMoved=FALSE;
				m_fDragging=TRUE;
				m_ptOldCursorPos=m_ptCursorPos;
			}
			return FALSE;
		};

		LRESULT CULWndCtrl::OnMouseMove(WPARAM,LPARAM)
		{
			if(m_fDragging)
			{
				POINT ptCursorPos;
				BOOL fIsDockable;
				RECT rc,rcDrag;
				UINT uiDockSide;
				::GetCursorPos(&ptCursorPos);
				if(m_ptOldCursorPos.x == ptCursorPos.x && 
					ptCursorPos.y == m_ptOldCursorPos.y &&
					m_fControl == m_fOldControl)
					return FALSE;
				m_fMouseMoved=TRUE;
				m_fControl = GetKeyState(VK_CONTROL) < 0;
				m_ptOldCursorPos=ptCursorPos;
				::CopyRect(&rcDrag, &m_rcStart);
				::OffsetRect(&rcDrag, ptCursorPos.x-m_ptCursorPos.x, 
					ptCursorPos.y-m_ptCursorPos.y);
				uiDockSide = IsDockable(&rcDrag,ptCursorPos);
				fIsDockable = uiDockSide != FALSE && m_fControl == FALSE;
				if(fIsDockable)
					if(uiDockSide&(dsAllowDockLeft|dsAllowDockRight))
						::OffsetRect(&rcDrag, 0, -(ptCursorPos.y-m_ptCursorPos.y));
					else
						::OffsetRect(&rcDrag, -(ptCursorPos.x-m_ptCursorPos.x),0);
				if((::EqualRect(&rcDrag, &m_rcOldRect) == 0) || (m_fOldControl != m_fControl))
				{
					//Erase where the dragging frame used to be..
					DrawXorFrame(&m_rcOldRect, m_fOldDrawDocked);
					if(fIsDockable)
					{
						::GetClientRect(m_hParentWnd, &rc);
						::MapWindowPoints(m_hParentWnd, 0, (POINT *)&rc, 2);
						if(uiDockSide&(dsAllowDockLeft|dsAllowDockRight))
						{
							rcDrag.top    = rc.top;
							rcDrag.bottom = rcDrag.top + (rc.bottom-rc.top);
							rcDrag.right  = rcDrag.left+ m_nDockedSize;
						}
						else
						{
							rcDrag.left   = rc.left;
							rcDrag.right  = rcDrag.left+ (rc.right-rc.left);
							rcDrag.bottom = rcDrag.top + m_nDockedSize;
						}
					}
					else
					{
						rcDrag.right  = rcDrag.left + m_FrameSize.cx;
						rcDrag.bottom = rcDrag.top  + m_FrameSize.cy;
					}
					DrawXorFrame(&rcDrag, fIsDockable);
					m_fOldDrawDocked=fIsDockable;
				}
				m_rcOldRect= rcDrag;
				m_fOldControl = m_fControl;
			}
			return FALSE;
		};

		LRESULT CULWndCtrl::OnLButtonUp(WPARAM,LPARAM)
		{
			if(m_fDragging)
			{
				POINT ptCursorPos;
				RECT rcDrag;

				m_fDragging=FALSE;
				
				DrawXorFrame(&m_rcOldRect,m_fOldDrawDocked);
				::GetCursorPos(&ptCursorPos);
				::CopyRect(&rcDrag, &m_rcStart);
				::OffsetRect(&rcDrag, ptCursorPos.x-m_ptCursorPos.x,
					ptCursorPos.y-m_ptCursorPos.y);
				if(m_fMouseMoved)
				{
					BOOL fToggleDock;
					BOOL fSetWindowPos;
					UINT uDockSide;
					m_fDragging = FALSE;
					uDockSide = IsDockable(&rcDrag,ptCursorPos);
					if(m_fDocked)
						if(m_fControl == TRUE || uDockSide == 0)
						{
							fToggleDock   = TRUE;
							fSetWindowPos = FALSE;
							m_ptPos.x= m_rcOldRect.left;
							m_ptPos.y= m_rcOldRect.top;
						}
						else
						{
							fToggleDock   = FALSE;
							fSetWindowPos = FALSE;
							m_dwDockedState = uDockSide;
						}
					else
						if(m_fControl == FALSE && uDockSide)
						{
							fToggleDock   = TRUE;
							fSetWindowPos = FALSE;
							m_dwDockedState = uDockSide;
						}
						else
						{
							fToggleDock   = FALSE;
							fSetWindowPos = TRUE;
						}
					if(fToggleDock)
						ToggleDockingMode();
					else 
						if(fSetWindowPos)
							SetWindowPos(0, m_rcOldRect.left, m_rcOldRect.top, 0, 0, 
								SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER | 
								SWP_DRAWFRAME | SWP_NOSENDCHANGING);
						else
							ResizeAllWndCtrls();
				}
				ReleaseCapture();
			}
			return FALSE;
		};

		LRESULT CULWndCtrl::OnGetMinMaxInfo(WPARAM,LPARAM lParam)
		{
			MINMAXINFO* pmmi=(MINMAXINFO*)lParam;
			if((!m_fDocked)&& (m_dwDockingStyle&dsNoResize))
			{
				pmmi->ptMaxTrackSize.x = m_FrameSize.cx;
				pmmi->ptMaxTrackSize.y = m_FrameSize.cy;
				pmmi->ptMinTrackSize.x = m_FrameSize.cx;
				pmmi->ptMinTrackSize.y = m_FrameSize.cy;
			}
			return FALSE;
		}

		LRESULT CULWndCtrl::OnEraseBkGnd(WPARAM wParam,LPARAM)
		{
			HDC hDC=(HDC)wParam;
			RECT rcClient;
			
			GetClientRect(&rcClient);
			if(m_fDocked)
			{
				UINT bf = 0;
				
				if(m_dwDockingStyle& dsBorderTop)		bf |= BF_TOP;
				if(m_dwDockingStyle& dsBorderBottom)	bf |= BF_BOTTOM;
				if(m_dwDockingStyle& dsBorderLeft)	bf |= BF_LEFT;
				if(m_dwDockingStyle& dsBorderRight)	bf |= BF_RIGHT;
				
				//draw whatever edges we need
				if(bf) 
					::DrawEdge(hDC, &rcClient, EDGE_ETCHED, bf | BF_ADJUST);	
			}
			::SetBkColor(hDC, ::GetSysColor(COLOR_BTNFACE));
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rcClient, NULL, 0, 0);

		/*	if(m_fDocked && (m_dwDockingStyle & dsDrawGripperDocked))
			{
				int y = 1, height = rcClient.bottom - 1;
				int x = 1;
				
				if(m_dwDockingStyle& dsBorderTop)		{ y += 3; height -= 3; }
				if(m_dwDockingStyle& dsBorderBottom)	{ height -= 2; }
				if(m_dwDockingStyle& dsBorderLeft)	{ x += 3; }
				
				DrawGripper(hDC,rcClient);
			}
			
		*/	
			if(m_fDocked && (m_dwDockingStyle & dsDrawGripperDocked))
				DrawGripper(hDC, rcClient);
			if((!m_fDocked) && (m_dwDockingStyle & dsDrawGripperFloating))
				DrawGripper(hDC, rcClient);

			return TRUE;
		};

		LRESULT CULWndCtrl::OnSettingChange(WPARAM,LPARAM)
		{
			if(!m_fDocked)
			{
				CalcFloatingRect();
				SetWindowPos(HWND_TOP, 
					m_ptPos.x, 
					m_ptPos.x,
					m_FrameSize.cx,
					m_FrameSize.cy,
					SWP_NOACTIVATE|SWP_NOZORDER);
			}
			return FALSE;
		}

		HWND CULWndCtrl::Create(TCHAR* szWndName,
								int x,int y,int cx,int cy,
								HWND hParentWnd,
								DWORD dwDockedState,
								BOOL fDocked,
								DWORD dwDockingStyles)
		{
			m_hParentWnd=hParentWnd;
		//	m_fDocked=fDocked;
			m_fDocked=FALSE;
			m_dwDockedState=dwDockedState;
			m_dwDockingStyle=dwDockingStyles;

			WNDCLASSEX	wndclass;

			::ZeroMemory(&wndclass, sizeof(wndclass));

			wndclass.cbSize			= sizeof(wndclass);
			wndclass.style			= 0;
			wndclass.lpfnWndProc	= (WNDPROC)WndProc;
			wndclass.cbClsExtra		= 0;
			wndclass.cbWndExtra		= 0;
			wndclass.hInstance		= ULOther::ULGetResourceHandle();
			wndclass.hIcon			= 0;
			wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
			wndclass.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
			wndclass.lpszMenuName	= 0;
			wndclass.lpszClassName	= _T("ULCTRLWND");
			wndclass.hIconSm		= 0;

			::RegisterClassEx(&wndclass);
			m_hWnd=::CreateWindowEx(
				(m_fDocked)?twsChildExStyles:twsPopupExStyles, 
				wndclass.lpszClassName, szWndName,
				(m_fDocked)?twsChildStyles:twsPopupStyles,
				x, y, cx, cy,
				hParentWnd, NULL, ULOther::ULGetResourceHandle(), this);
			SetWindowPos(HWND_TOP, x, y, cx, cy, SWP_SHOWWINDOW);

		//////////////////////////
			m_ptPos.x=x;
			m_ptPos.y=y;
			m_FloatingSize.cx=cx;
			m_FloatingSize.cy=cy;
			::SetRect(&m_rcBorderDock, 4, 3, 3, 3);
			::SetRect(&m_rcBorderFloat, 4, 3, 3, 3);
			CalcFloatingRect();
			m_nDockedSize=m_FloatingSize.cy;
			if(m_dwDockingStyle & dsBorderTop)    m_nDockedSize += 2;
			if(m_dwDockingStyle & dsBorderBottom) m_nDockedSize += 2;
			if(fDocked)
				ToggleDockingMode();
			if(m_fDocked)
			{
				RECT rect;
				::GetClientRect(m_hParentWnd, &rect);
				::PostMessage(m_hParentWnd, WM_SIZE,  SIZE_RESTORED, MAKELPARAM(rect.right-rect.left,rect.bottom-rect.top));
			}
			else
			{
				CalcFloatingRect();
				SetWindowPos(HWND_TOP, 
					m_ptPos.x, 
					m_ptPos.x,
					m_FrameSize.cx,
					m_FrameSize.cy,
					SWP_SHOWWINDOW);
			}
			ShowWindow(SW_SHOW);
		//////////////////////////
			return *this;
		}

		void CULWndCtrl::TogglePopupStyle()
		{
			DWORD dwStyle   = GetWindowLong(GWL_STYLE);
			DWORD dwExStyle = GetWindowLong(GWL_EXSTYLE);			
			if(dwStyle & WS_CHILD)
			{
				SetWindowLong(GWL_STYLE,   (dwStyle   & ~twsChildStyles)  |twsPopupStyles);
				SetWindowLong(GWL_EXSTYLE, (dwExStyle & ~twsChildExStyles)|twsPopupExStyles);
				//юзаем ::SetParent, чтоб не потерятьнастоящего родителя
				::SetParent(*this,NULL);
			}
			else
			{
				SetWindowLong(GWL_STYLE,   (dwStyle   & ~twsPopupStyles)  |twsChildStyles);
				SetWindowLong(GWL_EXSTYLE, (dwExStyle & ~twsPopupExStyles)|twsChildExStyles);
				::SetParent(*this,m_hParentWnd);
			}
			SetWindowPos(0, 0, 0, 0, 0,SWP_NOMOVE   | SWP_NOSIZE     |
												SWP_NOZORDER | SWP_NOACTIVATE |
												SWP_FRAMECHANGED);
		}

		void CULWndCtrl::ToggleDockingMode()
		{
			ShowWindow(SW_HIDE);
			TogglePopupStyle();
			m_fDocked=!m_fDocked;
			ResizeAllWndCtrls();
			if(!m_fDocked)
			{
				CalcFloatingRect();
				SetWindowPos(m_hParentWnd, 
					m_ptPos.x, 
					m_ptPos.y,
					m_FrameSize.cx,
					m_FrameSize.cy,
					0);
			}
			ShowWindow(SW_SHOW);
		}

		void CULWndCtrl::DrawXorFrame(RECT *rect, BOOL fDocked)
		{
			static WORD _dotPatternBmp1[] = 
			{
				0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055
		//		0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555
			};

			static WORD _dotPatternBmp2[] = 
			{
				0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
			};

			HBITMAP hbm;
			HBRUSH  hbr;
			HANDLE  hbrushOld;
			WORD    *bitmap;

			int width, height, x, y;
			int border;

			HDC hdc = ::GetDC(NULL);

			if(fDocked)
			{
				border = 1;
				bitmap = _dotPatternBmp2;
			}
			else
			{
				border = 3;
				bitmap = _dotPatternBmp1;
			}
			
			x = rect->left;
			y = rect->top;
			width  = rect->right-rect->left;
			height = rect->bottom-rect->top;

			hbm = ::CreateBitmap(8, 8, 1, 1, bitmap);
			hbr = ::CreatePatternBrush(hbm);

			::SetBrushOrgEx(hdc, x, y, 0);
			hbrushOld = ::SelectObject(hdc, hbr);

			::PatBlt(hdc, x+border,       y,               width-border,  border,        PATINVERT);
			::PatBlt(hdc, x+width-border, y+border,        border,        height-border, PATINVERT);
			::PatBlt(hdc, x,              y+height-border, width-border,  border,        PATINVERT);
			::PatBlt(hdc, x,              y,               border,        height-border, PATINVERT);

			::SelectObject(hdc, hbrushOld);
			::DeleteObject(hbr);
			::DeleteObject(hbm);
			::ReleaseDC(0, hdc);
		}

		UINT CULWndCtrl::IsDockable(RECT* rcDrag,POINT ptCursorPos)
		{
			RECT rc, rc1, rc2, inter;
			
			if(m_fDocked)
			{
				RECT rcClientParent;
				::GetClientRect(m_hParentWnd,&rcClientParent);
				POINT ptClientCursorPos;
				ptClientCursorPos=ptCursorPos;
				::ScreenToClient(m_hParentWnd,&ptClientCursorPos);
				if((ptClientCursorPos.x>=rcClientParent.left+50)&&
					(ptClientCursorPos.x<=rcClientParent.right-50)&&
					(ptClientCursorPos.y>=rcClientParent.top+50)&&
					(ptClientCursorPos.y<=rcClientParent.bottom-50))
					return FALSE;
			}

			::GetWindowRect(m_hParentWnd, &rc1);
			::GetClientRect(m_hParentWnd, &rc2);
			::MapWindowPoints(m_hParentWnd, 0, (POINT *)&rc2, 2);
			::InflateRect(&rc2, -2, -2);
			// Check intersection at top
			::SetRect(&rc, rc1.left,rc1.top,rc1.right,rc2.top);
			if(::IntersectRect(&inter, rcDrag, &rc))
				return dsAllowDockTop;
			
			// Check intersection at bottom
			::SetRect(&rc, rc1.left,rc2.bottom, rc1.right,rc1.bottom);
			if(::IntersectRect(&inter, rcDrag, &rc))
				return dsAllowDockBottom;
			
			// Check intersection at left
			::SetRect(&rc, rc1.left,rc2.top,rc2.left,rc2.bottom);
			if(::IntersectRect(&inter, rcDrag, &rc))
				return dsAllowDockLeft;
			
			// Check intersection at right
			::SetRect(&rc, rc2.right, rc2.top, rc1.right, rc2.bottom);
			if(::IntersectRect(&inter, rcDrag, &rc))
				return dsAllowDockRight;
			
			return 0;
		}

		void CULWndCtrl::CalcFloatingRect()
		{
			RECT rect;
			::SetRect(&rect, 0, 0, m_FloatingSize.cx, m_FloatingSize.cy);
			::AdjustWindowRectEx(&rect, (DWORD)twsPopupStyles, FALSE, (DWORD)twsPopupExStyles);
			m_FrameSize.cx= rect.right  - rect.left;
			m_FrameSize.cy= rect.bottom - rect.top;
		}

		void CULWndCtrl::DrawGripper(HDC hDC,RECT rcClient)
		{
			RECT rect;
			int x=1;
			int y=1;
			int width=0,height=0;
			int nOffsetX=0;
			int nOffsetY=0;
			if((!m_fDocked)||(m_dwDockedState&dsAllowDockBottom)||(m_dwDockedState&dsAllowDockTop))
			{
				width=4;
				height=rcClient.bottom;
				nOffsetX=3;
			}
			if(m_fDocked&&((m_dwDockedState&dsAllowDockLeft)||(m_dwDockedState&dsAllowDockRight)))
			{
				width=rcClient.right;
				height=4;
				nOffsetY=3;
				if(m_dwDockingStyle& dsBorderTop)		{ y += 3;  }
				if(m_dwDockingStyle& dsBorderLeft)	{ x += 2;width -= 1;}
				if(m_dwDockingStyle& dsBorderRight)	{ width -= 1;}
			}
			else
				if(m_fDocked)
				{
					if(m_dwDockingStyle& dsBorderTop)		{ y += 3; height -= 2; }
					if(m_dwDockingStyle& dsBorderBottom)	{ height -= 2; }
					if(m_dwDockingStyle& dsBorderLeft)	{ x += 3;}
				}
			SetRect(&rect, x,y,width,height);
			DrawEdge(hDC, &rect,  BDR_RAISEDINNER, BF_RECT);
		}

		void CULWndCtrl::ResizeAllWndCtrls(void)
		{
			RECT rect;
			::GetClientRect(m_hParentWnd, &rect);
			::SendMessage(m_hParentWnd, WM_SIZE,  SIZE_RESTORED,
				MAKELPARAM(rect.right-rect.left,rect.bottom-rect.top));
		}
	}
}
