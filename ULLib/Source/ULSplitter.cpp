///\file ULSplitter.cpp
///\brief фаил реализации класса оконного разделителя
#include "..\..\ULLib\Include\ULSplitter.h"
#include "..\..\ULLib\Include\ULRes.h"
namespace ULWnds
{
	namespace ULControls
	{
		CULSplitter::CULSplitter():CULWnd(),
			m_nSplitterPos(0),
			m_nSplitterBorder(3),
			m_oldPos(-4),
			m_fMoved(FALSE),
			m_fDragMode(FALSE),
			m_SplitterOrientation(soHorz),
			m_fResize(TRUE),
			propResize(this)
		{		
			m_arPane[0]=NULL;
			m_arPane[1]=NULL;

			MessageMap.AddMessage<CULSplitter>(WM_LBUTTONDOWN,&CULSplitter::OnLButtonDown);
			MessageMap.AddMessage<CULSplitter>(WM_LBUTTONUP,&CULSplitter::OnLButtonUp);
			MessageMap.AddMessage<CULSplitter>(WM_MOUSEMOVE,&CULSplitter::OnMouseMove);
			MessageMap.AddMessage<CULSplitter>(WM_SIZE,&CULSplitter::OnSize);
			MessageMap.AddMessage<CULSplitter>(WM_WINDOWPOSCHANGED,&CULSplitter::OnWindowPosChanged);
			MessageMap.AddMessage<CULSplitter>(WM_SETCURSOR,&CULSplitter::OnSetCursor);
		}
		BOOL CULSplitter::Create(HWND hParentWnd,WORD wID,int x,int y,int cx,int cy,
			enSplitterOrientation SplitterOrientation,int nSplitterPos,BOOL fResize)
		{
			m_nSplitterPos=nSplitterPos;
			m_SplitterOrientation=SplitterOrientation;
			m_fResize=fResize;

			WNDCLASSEX wcex;		    
			wcex.cbSize = sizeof(WNDCLASSEX); 		
			wcex.style				=CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc	=(WNDPROC)WndProc;
			wcex.cbClsExtra		=0;
			wcex.cbWndExtra		=0;
			wcex.hInstance		=ULOther::ULGetResourceHandle();
			wcex.hIcon				=NULL;
			if(fResize)
			{
				if(SplitterOrientation==soHorz)
					wcex.hCursor		=LoadCursor(NULL,IDC_SIZENS);
				else
					if(SplitterOrientation==soVert)
						wcex.hCursor	=LoadCursor(NULL,IDC_SIZEWE);
			}
			else
				wcex.hCursor			=LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground=(HBRUSH)(COLOR_3DFACE+1);
			wcex.lpszMenuName	=NULL;	
			wcex.lpszClassName=_T("ULSPLITTER");
			wcex.hIconSm			=NULL;
			RegisterClassEx(&wcex);
		//==============================================================
			m_hWnd=CreateWindowEx(0,
				wcex.lpszClassName, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,
				x,y,cx,cy,hParentWnd,(HMENU)wID,ULOther::ULGetResourceHandle(),this);
			return (m_hWnd!=NULL);
		}

		BOOL CULSplitter::SetPane(int nPos,ULWnds::CULWnd* pwndPane)
		{			
			if(((nPos!=0)||(nPos!=1))&&pwndPane)
			{
				m_arPane[nPos]=pwndPane;
				pwndPane->SetParent(*this);
				return TRUE;
			}
			else
				return FALSE;
		}

		void CULSplitter::SizeWindowContents(int nWidth, int nHeight)
		{
			if(m_SplitterOrientation==soHorz)
			{
				if(m_arPane[0])
					m_arPane[0]->MoveWindow(0,0,nWidth,m_nSplitterPos,TRUE);
				if(m_arPane[1])
					m_arPane[1]->MoveWindow(0,m_nSplitterPos+m_nSplitterBorder,nWidth,
						nHeight-m_nSplitterPos-m_nSplitterBorder,TRUE);
			}
			else
				if(m_SplitterOrientation==soVert)
				{
					if(m_arPane[0])
						m_arPane[0]->MoveWindow(0,0,m_nSplitterPos,nHeight,TRUE);
					if(m_arPane[1])
						m_arPane[1]->MoveWindow(m_nSplitterPos+m_nSplitterBorder,0,
							nWidth-m_nSplitterPos-m_nSplitterBorder,nHeight,TRUE);
				}
			::SendMessage(GetParent(),WM_COMMAND,MAKELPARAM(GetWindowLong(GWL_ID),scPaneSized),(LPARAM)m_hWnd);
		}

		void CULSplitter::DrawXorBar(HDC hDC, int x1, int y1, int width, int height)
		{
			static WORD _dotPatternBmp[8] = 
			{ 
				0x00aa,0x0055,0x00aa,0x0055, 
				0x00aa,0x0055,0x00aa,0x0055
			};


			HBITMAP hbm=CreateBitmap(8,8,1,1,_dotPatternBmp);
			HBRUSH  hbr=CreatePatternBrush(hbm);

			SetBrushOrgEx(hDC,x1,y1,0);
			HBRUSH  hbrushOld=(HBRUSH)SelectObject(hDC, hbr);

			PatBlt(hDC, x1, y1, width, height, PATINVERT);

			SelectObject(hDC, hbrushOld);

			DeleteObject(hbr);
			DeleteObject(hbm);
		}

		void CULSplitter::SetSplitterPos(int nSplitterPos)
		{
			m_nSplitterPos=nSplitterPos;
			RECT rc;
			GetClientRect(&rc);
			SizeWindowContents(rc.right,rc.bottom);
		}

		void CULSplitter::SetResize(BOOL* pfEnable)
		{
			m_fResize=*pfEnable;
		}

		void CULSplitter::GetResize(BOOL* pfEnable)
		{
			*pfEnable=m_fResize;
		}

		LRESULT CULSplitter::OnMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
		{
			switch(uMsg)
			{
/*			case WM_CTLCOLORBTN:
			case WM_CTLCOLOREDIT:
			case WM_CTLCOLORDLG:
			case WM_CTLCOLORLISTBOX:
			case WM_CTLCOLORSCROLLBAR:
			case WM_CTLCOLORSTATIC:
			case WM_DRAWITEM:
			case WM_MEASUREITEM:
			case WM_DELETEITEM:
			case WM_COMPAREITEM:
			case WM_CHARTOITEM:
			case WM_VKEYTOITEM:
			case WM_HSCROLL:
			case WM_VSCROLL:
			case WM_PARENTNOTIFY:*/
			case WM_COMMAND:
			case WM_NOTIFY:
				return ::SendMessage(GetParent(),uMsg,wParam,lParam);
			default:
				return 0;
			}
		}

		LRESULT CULSplitter::OnLButtonDown(WPARAM,LPARAM lParam)
		{
			if(!m_fResize)
				return FALSE;
			POINT pt={(short)LOWORD(lParam),(short)HIWORD(lParam)};
			RECT rect;
			GetWindowRect(&rect);
			ClientToScreen(&pt);
			pt.x-=rect.left;
			pt.y-=rect.top;
			OffsetRect(&rect,-rect.left,-rect.top);

			if(m_SplitterOrientation==soHorz)
			{
				if(pt.y<0)
					pt.y=0;
				if(pt.y>rect.bottom-4) 
					pt.y=rect.bottom-4;
			}
			else
				if(m_SplitterOrientation==soVert)
				{
					if(pt.x<0)
						pt.x=0;
					if(pt.x>rect.right-4) 
						pt.x=rect.right-4;
				}

			m_fDragMode=TRUE;

			SetCapture();
			HDC hDC = GetWindowDC(*this);
			if(m_SplitterOrientation==soHorz)
				DrawXorBar(hDC,1,pt.y-2,rect.right-2,4);
			else
				if(m_SplitterOrientation==soVert)
					DrawXorBar(hDC,pt.x-2,1,4,rect.bottom-2);
			ReleaseDC(*this, hDC);	
			m_oldPos=(m_SplitterOrientation==soHorz)?pt.y:pt.x;
			return FALSE;
		}

		LRESULT CULSplitter::OnLButtonUp(WPARAM,LPARAM lParam)
		{
			if(!m_fResize)
				return FALSE;

			POINT pt={(short)LOWORD(lParam),(short)HIWORD(lParam)};

			if(m_fDragMode == FALSE)
				return 0;
			RECT rect;
			GetWindowRect(&rect);

			ClientToScreen(&pt);
			pt.x-=rect.left;
			pt.y-=rect.top;

			OffsetRect(&rect,-rect.left,-rect.top);

			if(m_SplitterOrientation==soHorz)
			{
				if(pt.y<0)
					pt.y=0;
				if(pt.y>rect.bottom-4) 
					pt.y=rect.bottom-4;
			}
			else
				if(m_SplitterOrientation==soVert)
				{
					if(pt.x<0)
						pt.x=0;
					if(pt.x>rect.right-4) 
						pt.x=rect.right-4;
				}

			HDC hDC=GetWindowDC(*this);
			if(m_SplitterOrientation==soHorz)
				DrawXorBar(hDC,1,pt.y-2,rect.right-2,4);
			else
				if(m_SplitterOrientation==soVert)
					DrawXorBar(hDC,pt.x-2,1,4,rect.bottom-2);		
			ReleaseDC(*this,hDC);

			m_oldPos=(m_SplitterOrientation==soHorz)?pt.y:pt.x;
			m_fDragMode=FALSE;
			GetWindowRect(&rect);
			pt.x+=rect.left;
			pt.y+=rect.top;

			ScreenToClient(&pt);
			GetClientRect(&rect);
			m_nSplitterPos=(m_SplitterOrientation==soHorz)?pt.y:pt.x;
	
			SizeWindowContents(rect.right,rect.bottom);

			ReleaseCapture();
			return FALSE;
		}

		LRESULT CULSplitter::OnMouseMove(WPARAM wParam,LPARAM lParam)
		{
			if(!m_fResize)
				return FALSE;

			if(m_fDragMode==FALSE) 
				return 0;

			POINT pt={(short)LOWORD(lParam),(short)HIWORD(lParam)};

			RECT rect;
			GetWindowRect(&rect);

			ClientToScreen(&pt);
			pt.x-=rect.left;
			pt.y-=rect.top;

			OffsetRect(&rect,-rect.left,-rect.top);

			if(m_SplitterOrientation==soHorz)
			{
				if(pt.y<0)
					pt.y=0;
				if(pt.y>rect.bottom-4) 
					pt.y=rect.bottom-4;
			}
			else
				if(m_SplitterOrientation==soVert)
				{
					if(pt.x<0)
						pt.x=0;
					if(pt.x>rect.right-4) 
						pt.x=rect.right-4;
				}

			if(pt.y!=m_oldPos&&wParam&MK_LBUTTON)
			{
				HDC hDC=GetWindowDC(*this);
				if(m_SplitterOrientation==soHorz)
				{
					DrawXorBar(hDC,1,m_oldPos-2,rect.right-2,4);
					DrawXorBar(hDC,1,pt.y-2,rect.right-2,4);
				}
				else
					if(m_SplitterOrientation==soVert)
					{
						DrawXorBar(hDC,m_oldPos-2,1,4,rect.bottom-2);
						DrawXorBar(hDC,pt.x-2,1,4,rect.bottom-2);		
					}

				ReleaseDC(*this,hDC);

				m_oldPos=(m_SplitterOrientation==soHorz)?pt.y:pt.x;
			}

			return FALSE;
		}

		LRESULT CULSplitter::OnSize(WPARAM,LPARAM lParam)
		{
			SizeWindowContents(LOWORD(lParam), HIWORD(lParam));
			InvalidateRect();
			return FALSE;
		}

		LRESULT CULSplitter::OnWindowPosChanged(WPARAM,LPARAM lParam)
		{
			SizeWindowContents(LOWORD(lParam), HIWORD(lParam));
			InvalidateRect();
			return FALSE;
		}
		LRESULT CULSplitter::OnSetCursor(WPARAM /*wParam*/,LPARAM /*lParam*/)
		{
			return !m_fResize;
		}
	}
}
