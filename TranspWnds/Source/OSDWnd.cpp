/*
 * Copyright © 2009-2011 Bombizov Alexandr
 * This file is part of TranspWnds.
 *
 * TranspWnds is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TranspWnds is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with TranspWnds.  If not, see <http://www.gnu.org/licenses/>.
 */
#define _WIN32_WINNT  0x0500
#include "OSDWnd.h"

COSDWnd::COSDWnd(void):
	CULWnd(),
	m_constIdTimer(1),
	m_nCurTimout(500),
	m_Alpha(255),
	m_osdPos(osdpCenter),
	m_clrText(0xff0000),
	m_clrTextShadow(0xaaaaaa)
{
	m_rcPadding.left=30;
	m_rcPadding.top=30;
	m_rcPadding.right=30;
	m_rcPadding.bottom=30;
	
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT),sizeof(LOGFONT),&m_lf);
	m_lf.lfHeight=-50;
	
	MessageMap.AddMessage<COSDWnd>(WM_PAINT,&COSDWnd::OnPaint);
	MessageMap.AddMessage<COSDWnd>(WM_TIMER,&COSDWnd::OnTimer);
}

COSDWnd::~COSDWnd(void)
{
}

BOOL COSDWnd::Create(HWND hwndParent)
{
	WNDCLASSEX wcex;	    
	wcex.cbSize = sizeof(WNDCLASSEX); 		
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= ULOther::ULGetResourceHandle();
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= NULL;	
	wcex.lpszClassName	= _T("HINTWNDCLASS");
	wcex.hIconSm		= NULL;
	RegisterClassEx(&wcex);
//	if(!RegisterClassEx(&wcex))
//		return FALSE;

	return CULWnd::CreateEx(WS_EX_LAYERED,wcex.lpszClassName,NULL,WS_POPUP,100,10,500,200,hwndParent,NULL);
}
void COSDWnd::ShowText(LPCTSTR pszText)
{
	m_strText=pszText;


	RECT rc={0,0,0,0};
	CULClientDC dc(*this);

	CULFont font;
	font.CreateFontIndirect(&m_lf);
	HFONT hf=(HFONT)dc.SelectObject(font);

	dc.DrawText(m_strText,m_strText.GetLen(),&rc,DT_CALCRECT);
	dc.SelectObject(hf);
	int nScreenWidth=::GetSystemMetrics(SM_CXFULLSCREEN);
	int nScreenHeight=::GetSystemMetrics(SM_CYFULLSCREEN);
	switch(m_osdPos)
	{
		case osdpTopLeft:
			{
				MoveWindow(m_rcPadding.left,m_rcPadding.top,
					rc.right+m_rcPadding.right,rc.bottom+m_rcPadding.bottom,TRUE);
			}
			break;
		case osdpTopRight:
			{
				MoveWindow(nScreenWidth-rc.right-m_rcPadding.left-m_rcPadding.right,
					m_rcPadding.top,
					rc.right+m_rcPadding.right,
					rc.bottom+m_rcPadding.bottom,TRUE);
			}
			break;
		case osdpBotLeft:
			{
				MoveWindow(m_rcPadding.left,nScreenHeight-m_rcPadding.top-rc.bottom+m_rcPadding.bottom,
					rc.right+m_rcPadding.right,rc.bottom+m_rcPadding.bottom,TRUE);
			}
			break;
		case osdpBotRight:
			{
				MoveWindow(nScreenWidth-rc.right-m_rcPadding.left-m_rcPadding.right,
					nScreenHeight-m_rcPadding.top-rc.bottom+m_rcPadding.bottom,
					rc.right+m_rcPadding.right,
					rc.bottom+m_rcPadding.bottom,TRUE);
			}
			break;
		case osdpCenter:
			{
				MoveWindow(nScreenWidth/2-rc.right/2-m_rcPadding.left,
					nScreenHeight/2-rc.bottom/2-m_rcPadding.top,
					rc.right+(m_rcPadding.left+m_rcPadding.right),
					rc.bottom+(m_rcPadding.top+m_rcPadding.bottom),TRUE);
			}
			break;	
	}
	m_Alpha=255;
	m_nCurTimout=500;
	::SetLayeredWindowAttributes(*this,0x00ffffff,m_Alpha,LWA_COLORKEY|LWA_ALPHA);
	ShowWindow(SW_SHOW);
	SetTimer(m_constIdTimer,m_nCurTimout);
	SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	InvalidateRect(NULL,TRUE);
}

void COSDWnd::SetPos(enOSDPos osdPos)
{
	m_osdPos=osdPos;
}
COSDWnd::enOSDPos COSDWnd::GetPos()
{
	return m_osdPos;
}

void COSDWnd::SetTextColor(COLORREF clrText)
{
	m_clrText=clrText;
}

COLORREF COSDWnd::GetTextColor()
{
	return m_clrText;
}

void COSDWnd::SetTextShadowColor(COLORREF clrTextShadow)
{
	m_clrTextShadow=clrTextShadow;
}

COLORREF COSDWnd::GetTextShadowColor()
{
	return m_clrTextShadow;
}

void COSDWnd::SetFont(LOGFONT& lf)
{
	memcpy(&m_lf,&lf,sizeof(LOGFONT));
}
BOOL COSDWnd::GetFont(LOGFONT* plf)
{
	if(!plf)
		return FALSE;
	memcpy(plf,&m_lf,sizeof(LOGFONT));
	return TRUE;
}

//===================================
LRESULT COSDWnd::OnPaint(WPARAM,LPARAM)
{
	CULFont font;
	font.CreateFontIndirect(&m_lf);

	CULPaintDC dc(*this);

	HFONT hf=(HFONT)dc.SelectObject(font);

	dc.SetBkMode(CULPaintDC::bmTRANSPARENT);
	dc.SetTextColor(m_clrTextShadow);
	RECT rc;
	GetClientRect(&rc);
	dc.DrawText(m_strText,m_strText.GetLen(),&rc,DT_CENTER);

	dc.SetTextColor(m_clrText);
	int nTextShift=abs(m_lf.lfHeight/10);
	rc.left-=nTextShift;
	rc.top-=nTextShift;
	rc.right-=nTextShift;
	rc.bottom-=nTextShift;
	dc.DrawText(m_strText,m_strText.GetLen(),&rc,DT_CENTER);

	dc.SelectObject(hf);
	return 0;
}

LRESULT COSDWnd::OnTimer(WPARAM idTmr,LPARAM)
{
	if(idTmr==m_constIdTimer)
	{
		if(m_Alpha==0)
		{
			KillTimer(m_constIdTimer);
			ShowWindow(SW_HIDE);
			return 0;
		}
		m_Alpha=BYTE((m_Alpha)/1.1);
		::SetLayeredWindowAttributes(*this,0x00ffffff,m_Alpha,LWA_COLORKEY|LWA_ALPHA);		
		if(m_nCurTimout>1)
		{
			m_nCurTimout/=2;
			SetTimer(m_constIdTimer,m_nCurTimout);
		}
	}	
	return 0;
}

