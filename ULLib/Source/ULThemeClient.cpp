///\file ULThemeClient.cpp
///\brief файл реализации класса оконной темы(06.08.2008)
#include "..\..\ULLib\Include\ULThemeClient.h"
namespace ULWnds
{
	CULThemeClient::CULThemeClient():
		m_clrInner(0xeeeeee),
		m_clrOuter(0xa0a0a0),
		m_nRadius(10),
		m_pClientWnd(NULL)
	{
	}

	CULThemeClient::CULThemeClient(CULThemeClient& ThemeClient):
		m_clrInner(ThemeClient.m_clrInner),
		m_clrOuter(ThemeClient.m_clrOuter),
		m_nRadius(ThemeClient.m_nRadius),
		m_pClientWnd(ThemeClient.m_pClientWnd)
	{
	}
	CULThemeClient::CULThemeClient(HWND hClientWnd):
		m_clrInner(0xeeeeee),
		m_clrOuter(0xa0a0a0),
		m_nRadius(10),
		m_pClientWnd(NULL)
	{
		Create(hClientWnd);
	}
	void CULThemeClient::operator = (CULThemeClient& ThemeClient)
	{
		m_clrInner=ThemeClient.m_clrInner;
		m_clrOuter=ThemeClient.m_clrOuter;
		m_nRadius=ThemeClient.m_nRadius;
		m_pClientWnd=ThemeClient.m_pClientWnd;
	}
	CULThemeClient::~CULThemeClient()
	{
	}

	LRESULT CULThemeClient::ClientWndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
	{
		CULSubClass* pULWnd=(CULSubClass*)CULWnd::FromHandle(hWnd);			
		CULThemeClient* pThemeClient=(CULThemeClient*)pULWnd->GetProp(_T("ULThemeClient"));
		switch(uMsg)
		{
		case WM_ERASEBKGND:
			{
				ULGDI::ULDC::CULDC dc((HDC&)wParam);
				if(pThemeClient->OnEraseBkGndClient(dc)&&dc.Detach())
					return TRUE;
			}
			break;
		case WM_NCHITTEST:
				return HTCAPTION;
		case WM_COMMAND:
				if(LOWORD(wParam)==CTM_MINIMIZE)
				{
					pULWnd->SendMessage(WM_SYSCOMMAND,SC_MINIMIZE,NULL);
					return TRUE;
				}
		}
		return ::CallWindowProc(pThemeClient->m_lpSubClassWndProc,*pULWnd,uMsg,wParam,lParam);
	}
	void CULThemeClient::CreateLeftUpCorner()
	{
		TRIVERTEX tv[4];
		tv[0].x=0;
		tv[0].y=0;
		tv[0].Red=BYTE(m_clrOuter)<<8;
		tv[0].Green=BYTE(m_clrOuter>>8)<<8;
		tv[0].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[0].Alpha=0;

		tv[1].x=m_nRadius;
		tv[1].y=0;
		tv[1].Red=BYTE(m_clrOuter)<<8;
		tv[1].Green=BYTE(m_clrOuter>>8)<<8;
		tv[1].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[1].Alpha=0;

		tv[2].x=m_nRadius;
		tv[2].y=m_nRadius;
		tv[2].Red=BYTE(m_clrInner)<<8;
		tv[2].Green=BYTE(m_clrInner>>8)<<8;
		tv[2].Blue=BYTE(m_clrInner>>16)<<8;
		tv[2].Alpha=0;		

		tv[3].x=0;
		tv[3].y=m_nRadius;
		tv[3].Red=BYTE(m_clrOuter)<<8;
		tv[3].Green=BYTE(m_clrOuter>>8)<<8;
		tv[3].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[3].Alpha=0;		

		GRADIENT_TRIANGLE gt[2];
		gt[0].Vertex1 = 0;
		gt[0].Vertex2 = 1;
		gt[0].Vertex3 = 2;
		gt[1].Vertex1 = 0;
		gt[1].Vertex2 = 2;
		gt[1].Vertex3 = 3;

		m_MemDC.GradientFill(tv,4,&gt,2,GRADIENT_FILL_TRIANGLE);
	}

	void CULThemeClient::CreateRightUpCorner()
	{
		RECT rc;
		m_pClientWnd->GetWindowRect(&rc);
		rc.right-=rc.left;
		rc.bottom-=rc.top;

		TRIVERTEX tv[4];
		tv[0].x=rc.right-m_nRadius;
		tv[0].y=0;
		tv[0].Red=BYTE(m_clrOuter)<<8;
		tv[0].Green=BYTE(m_clrOuter>>8)<<8;
		tv[0].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[0].Alpha=0;

		tv[1].x=rc.right;
		tv[1].y=0;
		tv[1].Red=BYTE(m_clrOuter)<<8;
		tv[1].Green=BYTE(m_clrOuter>>8)<<8;
		tv[1].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[1].Alpha=0;

		tv[2].x=rc.right;
		tv[2].y=m_nRadius;
		tv[2].Red=BYTE(m_clrOuter)<<8;
		tv[2].Green=BYTE(m_clrOuter>>8)<<8;
		tv[2].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[2].Alpha=0;		

		tv[3].x=rc.right-m_nRadius;
		tv[3].y=m_nRadius;
		tv[3].Red=BYTE(m_clrInner)<<8;
		tv[3].Green=BYTE(m_clrInner>>8)<<8;
		tv[3].Blue=BYTE(m_clrInner>>16)<<8;
		tv[3].Alpha=0;		

		GRADIENT_TRIANGLE gt[2];
		gt[1].Vertex1 = 1;
		gt[1].Vertex2 = 0;
		gt[1].Vertex3 = 3;
		gt[0].Vertex1 = 1;
		gt[0].Vertex2 = 2;
		gt[0].Vertex3 = 3;

		m_MemDC.GradientFill(tv,4,&gt,2,GRADIENT_FILL_TRIANGLE);
	}

	void CULThemeClient::CreateLeftDownCorner()
	{
		RECT rc;
		m_pClientWnd->GetWindowRect(&rc);
		rc.right-=rc.left;
		rc.bottom-=rc.top;

		TRIVERTEX tv[4];
		tv[0].x=0;
		tv[0].y=rc.bottom-m_nRadius;
		tv[0].Red=BYTE(m_clrOuter)<<8;
		tv[0].Green=BYTE(m_clrOuter>>8)<<8;
		tv[0].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[0].Alpha=0;

		tv[1].x=m_nRadius;
		tv[1].y=rc.bottom-m_nRadius;
		tv[1].Red=BYTE(m_clrInner)<<8;
		tv[1].Green=BYTE(m_clrInner>>8)<<8;
		tv[1].Blue=BYTE(m_clrInner>>16)<<8;
		tv[1].Alpha=0;

		tv[2].x=m_nRadius;
		tv[2].y=rc.bottom;
		tv[2].Red=BYTE(m_clrOuter)<<8;
		tv[2].Green=BYTE(m_clrOuter>>8)<<8;
		tv[2].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[2].Alpha=0;		

		tv[3].x=0;
		tv[3].y=rc.bottom;
		tv[3].Red=BYTE(m_clrOuter)<<8;
		tv[3].Green=BYTE(m_clrOuter>>8)<<8;
		tv[3].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[3].Alpha=0;		

		GRADIENT_TRIANGLE gt[2];
		gt[1].Vertex1 = 3;
		gt[1].Vertex2 = 0;
		gt[1].Vertex3 = 1;
		gt[0].Vertex1 = 3;
		gt[0].Vertex2 = 2;
		gt[0].Vertex3 = 1;

		m_MemDC.GradientFill(tv,4,&gt,2,GRADIENT_FILL_TRIANGLE);
	}

	void CULThemeClient::CreateRightDownCorner()
	{
		RECT rc;
		m_pClientWnd->GetWindowRect(&rc);
		rc.right-=rc.left;
		rc.bottom-=rc.top;

		TRIVERTEX tv[4];
		tv[0].x=rc.right-m_nRadius;
		tv[0].y=rc.bottom-m_nRadius;
		tv[0].Red=BYTE(m_clrInner)<<8;
		tv[0].Green=BYTE(m_clrInner>>8)<<8;
		tv[0].Blue=BYTE(m_clrInner>>16)<<8;
		tv[0].Alpha=0;

		tv[1].x=rc.right;
		tv[1].y=rc.bottom-m_nRadius;
		tv[1].Red=BYTE(m_clrOuter)<<8;
		tv[1].Green=BYTE(m_clrOuter>>8)<<8;
		tv[1].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[1].Alpha=0;

		tv[2].x=rc.right;
		tv[2].y=rc.bottom;
		tv[2].Red=BYTE(m_clrOuter)<<8;
		tv[2].Green=BYTE(m_clrOuter>>8)<<8;
		tv[2].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[2].Alpha=0;		

		tv[3].x=rc.right-m_nRadius;
		tv[3].y=rc.bottom;
		tv[3].Red=BYTE(m_clrOuter)<<8;
		tv[3].Green=BYTE(m_clrOuter>>8)<<8;
		tv[3].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[3].Alpha=0;		

		GRADIENT_TRIANGLE gt[2];
		gt[0].Vertex1 = 0;
		gt[0].Vertex2 = 1;
		gt[0].Vertex3 = 2;
		gt[1].Vertex1 = 0;
		gt[1].Vertex2 = 2;
		gt[1].Vertex3 = 3;

		m_MemDC.GradientFill(tv,4,&gt,2,GRADIENT_FILL_TRIANGLE);
	}

	void CULThemeClient::CreateBorder()
	{
		RECT rc;
		m_pClientWnd->GetWindowRect(&rc);
		rc.right-=rc.left;
		rc.bottom-=rc.top;
		//левый бордюр
		TRIVERTEX tv[2];
		GRADIENT_RECT gr;
		tv[0].x=0;
		tv[0].y=m_nRadius;
		tv[0].Red=BYTE(m_clrOuter)<<8;
		tv[0].Green=BYTE(m_clrOuter>>8)<<8;
		tv[0].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[0].Alpha=0;
		tv[1].x=m_nRadius;
		tv[1].y=rc.bottom-m_nRadius;
		tv[1].Red=BYTE(m_clrInner)<<8;
		tv[1].Green=BYTE(m_clrInner>>8)<<8;
		tv[1].Blue=BYTE(m_clrInner>>16)<<8;
		tv[1].Alpha=0;
		gr.UpperLeft  = 1;
		gr.LowerRight = 0;
		m_MemDC.GradientFill(tv,2,&gr,1,GRADIENT_FILL_RECT_H);
		//правый бордюр
		tv[0].x=rc.right-m_nRadius;
		tv[0].y=m_nRadius;
		tv[0].Red=BYTE(m_clrInner)<<8;
		tv[0].Green=BYTE(m_clrInner>>8)<<8;
		tv[0].Blue=BYTE(m_clrInner>>16)<<8;
		tv[0].Alpha=0;
		tv[1].x=rc.right;
		tv[1].y=rc.bottom-m_nRadius;
		tv[1].Red=BYTE(m_clrOuter)<<8;
		tv[1].Green=BYTE(m_clrOuter>>8)<<8;
		tv[1].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[1].Alpha=0;
		gr.UpperLeft  = 0;
		gr.LowerRight = 1;
		m_MemDC.GradientFill(tv,2,&gr,1,GRADIENT_FILL_RECT_H);
		//верхний бордюр
		tv[0].x=m_nRadius;
		tv[0].y=0;
		tv[0].Red=BYTE(m_clrOuter)<<8;
		tv[0].Green=BYTE(m_clrOuter>>8)<<8;
		tv[0].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[0].Alpha=0;
		tv[1].x=rc.right-m_nRadius;
		tv[1].y=m_nRadius;
		tv[1].Red=BYTE(m_clrInner)<<8;
		tv[1].Green=BYTE(m_clrInner>>8)<<8;
		tv[1].Blue=BYTE(m_clrInner>>16)<<8;
		tv[1].Alpha=0;
		gr.UpperLeft  = 0;
		gr.LowerRight = 1;
		m_MemDC.GradientFill(tv,2,&gr,1,GRADIENT_FILL_RECT_V);
		//ниднеий бордюр
		tv[0].x=m_nRadius;
		tv[0].y=rc.bottom;
		tv[0].Red=BYTE(m_clrOuter)<<8;
		tv[0].Green=BYTE(m_clrOuter>>8)<<8;
		tv[0].Blue=BYTE(m_clrOuter>>16)<<8;
		tv[0].Alpha=0;
		tv[1].x=rc.right-m_nRadius;
		tv[1].y=rc.bottom-m_nRadius;
		tv[1].Red=BYTE(m_clrInner)<<8;
		tv[1].Green=BYTE(m_clrInner>>8)<<8;
		tv[1].Blue=BYTE(m_clrInner>>16)<<8;
		tv[1].Alpha=0;
		gr.UpperLeft  = 0;
		gr.LowerRight = 1;
		m_MemDC.GradientFill(tv,2,&gr,1,GRADIENT_FILL_RECT_V);
	}
//================================================
	BOOL CULThemeClient::Create(HWND hClientWnd)
	{
		if(!hClientWnd)
			return FALSE;
		m_pClientWnd=ULWnds::CULWnd::FromHandle(hClientWnd);
		m_pClientWnd->SetProp(_T("ULThemeClient"),(HANDLE)this);
		m_lpSubClassWndProc=(WNDPROC)(LONG_PTR)m_pClientWnd->SetWindowLong(GWL_WNDPROC,(LONG)(LONG_PTR)ClientWndProc);
			m_pClientWnd->ModifyStyle(WS_BORDER|DS_3DLOOK|WS_DLGFRAME,0);
		m_pClientWnd->ModifyStyleEx(WS_EX_CLIENTEDGE,0);
			ULGDI::ULDC::CULWindowDC dcDT(::GetDesktopWindow());
		m_MemDC.CreateCompatibleDC(dcDT);
		ULGDI::ULGObjs::CULBitmap Bitmap;
		RECT rc;
		m_pClientWnd->GetWindowRect(&rc);
		rc.right-=rc.left;
		rc.bottom-=rc.top;
		Bitmap.CreateCompatibleBitmap(dcDT,rc.right,rc.bottom);
		Bitmap.m_hBitmap=(HBITMAP)m_MemDC.SelectObject(Bitmap);

		if(!OnCreateRgn())
		return FALSE;
		if(!OnCreateBkGnd())
			return FALSE;
			
		if(m_pClientWnd->GetWindowLong(GWL_STYLE)&WS_SYSMENU)
		{
			assert(m_pClientWnd->GetDlgItem(IDCANCEL)==NULL);
			if(m_pClientWnd->GetDlgItem(IDCANCEL))
				return FALSE;
			assert(m_hlX.CreateHyperLink(*m_pClientWnd,rc.right-m_nRadius-12,m_nRadius,_T("X"),NULL,IDCANCEL));
			m_hlX.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDefault,ULWnds::ULControls::ULButtons::CULHyperLink::sBold,m_clrOuter);
			m_hlX.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDown,ULWnds::ULControls::ULButtons::CULHyperLink::sNone,m_clrOuter);
			m_hlX.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tHiLite,ULWnds::ULControls::ULButtons::CULHyperLink::sBold,m_clrOuter);
			if(m_pClientWnd->GetWindowLong(GWL_STYLE)&WS_MINIMIZEBOX)
			{
				assert(m_pClientWnd->GetDlgItem(CTM_MINIMIZE)==NULL);
				if(m_pClientWnd->GetDlgItem(CTM_MINIMIZE))
					return FALSE;
				assert(m_hl_.CreateHyperLink(*m_pClientWnd,rc.right-m_nRadius-12-12-1,m_nRadius-2,_T("_"),NULL,CTM_MINIMIZE));
				m_hl_.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDefault,ULWnds::ULControls::ULButtons::CULHyperLink::sBold,m_clrOuter);
				m_hl_.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDown,ULWnds::ULControls::ULButtons::CULHyperLink::sNone,m_clrOuter);
				m_hl_.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tHiLite,ULWnds::ULControls::ULButtons::CULHyperLink::sBold,m_clrOuter);
			}
		}
		return TRUE;
	}

	BOOL CULThemeClient::OnEraseBkGndClient(ULGDI::ULDC::CULDC& dc)
	{
		RECT rc;
		m_pClientWnd->GetClientRect(&rc);
		dc.BitBlt(0,0,rc.right,rc.bottom,m_MemDC,0,0,SRCCOPY);
		return TRUE;
	}
	BOOL CULThemeClient::OnCreateRgn()
	{
		RECT rc;
		m_pClientWnd->GetWindowRect(&rc);
		rc.right-=rc.left;
		rc.bottom-=rc.top;

		ULGDI::ULGObjs::CULRgn Rgn;
		Rgn.CreateRoundRectRgn(0,0,rc.right,rc.bottom,m_nRadius,m_nRadius);
		m_pClientWnd->SetWindowRgn(Rgn,TRUE);
		Rgn.Detach();

		if(m_hlX.m_hWnd)
			m_hlX.SetWindowPos(NULL,rc.right-m_nRadius-10,m_nRadius,0,0,SWP_NOZORDER|SWP_NOSIZE);

		if(m_hl_.m_hWnd)
			m_hl_.SetWindowPos(NULL,rc.right-m_nRadius-10-12-1,m_nRadius-2,0,0,SWP_NOZORDER|SWP_NOSIZE);

		return TRUE;
	}

	BOOL CULThemeClient::OnCreateBkGnd()
	{
		CreateLeftUpCorner();
		CreateRightUpCorner();
		CreateLeftDownCorner();
		CreateRightDownCorner();
		CreateBorder();

		RECT rc;
		m_pClientWnd->GetWindowRect(&rc);
		rc.right-=rc.left;
		rc.bottom-=rc.top;
		RECT rcInner={m_nRadius,m_nRadius,rc.right-m_nRadius,rc.bottom-m_nRadius};
		m_MemDC.FillRect(&rcInner,::CreateSolidBrush(m_clrInner));

		if(m_hlX.m_hWnd)
		{
			m_hlX.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDefault,ULWnds::ULControls::ULButtons::CULHyperLink::sBold,m_clrOuter);
			m_hlX.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDown,ULWnds::ULControls::ULButtons::CULHyperLink::sNone,m_clrOuter);
			m_hlX.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tHiLite,ULWnds::ULControls::ULButtons::CULHyperLink::sBold,m_clrOuter);
		}

		if(m_hl_.m_hWnd)
		{
			m_hl_.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDefault,ULWnds::ULControls::ULButtons::CULHyperLink::sBold,m_clrOuter);
			m_hl_.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tDown,ULWnds::ULControls::ULButtons::CULHyperLink::sNone,m_clrOuter);
			m_hl_.SetStyles(ULWnds::ULControls::ULButtons::CULHyperLink::tHiLite,ULWnds::ULControls::ULButtons::CULHyperLink::sBold,m_clrOuter);
		}
		return TRUE;
	}
	void CULThemeClient::SetColors(COLORREF clrInner,COLORREF clrOuter)
	{
		m_clrInner=clrInner;
		m_clrOuter=clrOuter;
		OnCreateBkGnd();
	}
	void CULThemeClient::SetRadius(int nRadius)
	{
		m_nRadius=nRadius;
		OnCreateRgn();
	}
}
