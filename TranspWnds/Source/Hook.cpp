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
#define _WIN32_WINNT 0x0500
#include "Hook.h"
#include "WorkWnd.h"
#include "TranspWndsHKH.h"

LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam);


bool tagHotKeyInfo::IsHotKey( PMSLLHOOKSTRUCT lpMouseHookStruct )
{
	BOOL fKeyState=HIWORD(GetKeyState(VK_MENU));
	if((m_fAlt&&!fKeyState)||(!m_fAlt&&fKeyState))
		return false;
	fKeyState=HIWORD(GetKeyState(VK_CONTROL));
	if((m_fCtrl&&!fKeyState)||(!m_fCtrl&&fKeyState))
		return false;
	fKeyState=HIWORD(GetKeyState(VK_SHIFT));
	if((m_fShift&&!fKeyState)||(!m_fShift&&fKeyState))
		return false;
	fKeyState=HIWORD(GetKeyState(VK_LWIN))||HIWORD(GetKeyState(VK_RWIN));
	if((m_fWin&&!fKeyState)||(!m_fWin&&fKeyState))
		return false;			
	return true;
}
bool tagHotKeyInfo::IsMsg(int nMsg,UINT uMsg)
{
	return (m_uMsg[nMsg]==uMsg);
}

tagHotKeyInfo& tagHotKeyInfo::operator=(tagHotKeyInfo& HotKeyInfo)
{
	m_fAlt=HotKeyInfo.m_fAlt;
	m_fCtrl=HotKeyInfo.m_fCtrl;
	m_fShift=HotKeyInfo.m_fShift;
	m_fWin=HotKeyInfo.m_fWin;
	m_uMsg[0]=HotKeyInfo.m_uMsg[0];
	m_uMsg[1]=HotKeyInfo.m_uMsg[1];
	m_uMsg[2]=HotKeyInfo.m_uMsg[2];
	return *this;
}

CHook::CHook(void):
	m_hMouse(NULL)
{
	m_arHotKeyInfo[hkoThroughClick].m_fAlt=TRUE;
	m_arHotKeyInfo[hkoThroughClick].m_fCtrl=TRUE;
	m_arHotKeyInfo[hkoThroughClick].m_fShift=TRUE;

	m_arHotKeyInfo[hkoCollapseWnd].m_fAlt=TRUE;
	m_arHotKeyInfo[hkoCollapseWnd].m_fCtrl=TRUE;
	m_arHotKeyInfo[hkoCollapseWnd].m_fShift=TRUE;
	m_arHotKeyInfo[hkoCollapseWnd].m_uMsg[0]=WM_LBUTTONDOWN;
}

CHook::~CHook(void)
{
	::UnhookWindowsHookEx(m_hMouse);
}

void CHook::Enable()
{
	m_hMouse=SetWindowsHookEx(WH_MOUSE_LL,MouseProc,ULGetResourceHandle(),0);
}

void CHook::Disable()
{
	::UnhookWindowsHookEx(m_hMouse);
}

void CHook::Restore()
{
	for(std::map<HWND,WNDINFO>::iterator i=m_mapWndInfo.begin();i!=m_mapWndInfo.end();i++)
		if(IsWindow(i->first))
		{
			if(i->second.fAlpha)
			{
				SetLayeredWindowAttributes(i->first,0,255,LWA_ALPHA);
				::SetWindowLong(i->first,GWL_EXSTYLE,GetWindowLong(i->first,GWL_EXSTYLE)&~WS_EX_LAYERED);
				RedrawWindow(i->first,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
			}

			if(i->second.fTopMost)
				::SetWindowPos(i->first,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			if(i->second.dwStyle)
			{
				::SetWindowLong(i->first,GWL_STYLE,
					::GetWindowLong(i->first,GWL_STYLE)|i->second.dwStyle);
				::SetWindowPos(i->first,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);
			}
		}
		m_mapWndInfo.clear();
}

static CHook theHook;

CHook* CHook::GetHook()
{
	return &theHook;
}

///\brief функция предназначена для поиска popup родителя
///\param hWnd - хендл окна на родителе, или определенной иерархии дочерних окон или самого popup
///\return хендл popup в случае успеха, если popup оказывается рабочий стол то вернет NULL
HWND GetPopup(HWND hWnd)
{
	while(((GetWindowLong(hWnd,GWL_STYLE)&WS_CHILD))&&IsWindow(hWnd))
		hWnd=GetParent(hWnd);
	//hWnd=GetAncestor(hWnd,GA_ROOT);
	if(!hWnd)
		return NULL;		
	static TCHAR szText[255];
	GetClassName(hWnd,szText,255);
	if(_tcsicmp(szText,_T("progman"))==0)
		return NULL;
	return hWnd;
}

///\brief процедура мышинного хука
LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	LRESULT lRes=0;
	lRes=theHook.ProcessTransp((UINT)wParam,(PMSLLHOOKSTRUCT)lParam);
	if(lRes)
		return lRes;
	lRes=theHook.ProcessTopMost((UINT)wParam,(PMSLLHOOKSTRUCT)lParam);
	if(lRes)
		return lRes;
	lRes=theHook.ProcessMoveWnd((UINT)wParam,(PMSLLHOOKSTRUCT)lParam);
	if(lRes)
		return lRes;
	lRes=theHook.ProcessSizeWnd((UINT)wParam,(PMSLLHOOKSTRUCT)lParam);
	if(lRes)
		return lRes;
	lRes=theHook.ProcessToggleCaption((UINT)wParam,(PMSLLHOOKSTRUCT)lParam);
	if(lRes)
		return lRes;
/*	//не сделал еще :)
	lRes=theHook.ProcessThroughClick((UINT)wParam,(PMSLLHOOKSTRUCT)lParam);
	if(lRes)
		return lRes;
*/
	lRes=theHook.ProcessCollapseWnd((UINT)wParam,(PMSLLHOOKSTRUCT)lParam);
	if(lRes)
		return lRes;
	 
	return CallNextHookEx(theHook.m_hMouse,nCode,wParam,lParam);
}

LRESULT CHook::ProcessTransp(
	UINT uMsg,
	PMSLLHOOKSTRUCT lpMouseHookStruct
)
{
	return this->ProcessTransp( uMsg, lpMouseHookStruct->pt, lpMouseHookStruct->mouseData );
}

LRESULT CHook::ProcessTransp(
	UINT uMsg,
	POINT ptMouseAction,
	DWORD dwMouseData
)
{
	if(!m_arHotKeyInfo[hkoTransp].IsHotKey(NULL))
		return 0;
	if(!m_arHotKeyInfo[hkoTransp].IsMsg(0,uMsg))
		return 0;

	HWND hWnd=::WindowFromPoint( ptMouseAction );
	if((hWnd=GetPopup(hWnd))==0)
		return 0;

	if((GetWindowLong(hWnd,GWL_EXSTYLE)&WS_EX_LAYERED)==0)
	{
		if(((short)HIWORD(dwMouseData))>0)
			return 0;

		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if(iterItem!=m_mapWndInfo.end())
		{
			iterItem._Mynode()->_Myval.second.bAlpha=255;
			iterItem._Mynode()->_Myval.second.fAlpha=TRUE;
		}
		else
		{
			CHook::WNDINFO wi={0};
			wi.bAlpha=255;
			wi.fAlpha=TRUE;
			m_mapWndInfo[hWnd]=wi;
		}
		::SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)|WS_EX_LAYERED);
		CULApp::GetULApp()->m_pMainWnd->PostMessage(CWorkWnd::OSDM_MESSAGE,
			(WPARAM)hWnd,(LPARAM)hkoTransp);
		SetLayeredWindowAttributes(hWnd,0,255,LWA_ALPHA);
		RedrawWindow(hWnd,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
		return 1;
	}
	else
	{
		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if((iterItem==m_mapWndInfo.end())||(!iterItem._Mynode()->_Myval.second.fAlpha))
		{
			CULApp::GetULApp()->m_pMainWnd->PostMessage(CWorkWnd::OSDM_MESSAGE,
				(WPARAM)hWnd,(LPARAM)hkoTransp);
			return 0;                               
		}
	}
	BYTE bAlpha=m_mapWndInfo[hWnd].bAlpha;
	if(((short)HIWORD( dwMouseData ))<0)
	{
		if(bAlpha>=(m_bMinTranspVal+m_bTranspStep))
			bAlpha-=m_bTranspStep;
	}
	else
	{
		if(bAlpha<=(255-m_bTranspStep))
			bAlpha+=m_bTranspStep;
		else
			bAlpha=255;
	}                     
	CULApp::GetULApp()->m_pMainWnd->PostMessage(CWorkWnd::OSDM_MESSAGE,
		(WPARAM)hWnd,(LPARAM)hkoTransp);
	SetLayeredWindowAttributes(hWnd,0,bAlpha,LWA_ALPHA);            
	m_mapWndInfo[hWnd].bAlpha=bAlpha;
/*	if(m_mapWndInfo[hWnd].bAlpha==255)
	{
		::SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)&~WS_EX_LAYERED);
		m_mapWndInfo[hWnd].fAlpha=FALSE;
	}
	*/
	return 1;
}

LRESULT CHook::ProcessTopMost(UINT uMsg,PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	if(!m_arHotKeyInfo[hkoTopMost].IsHotKey(lpMouseHookStruct))
		return 0;
	if(!m_arHotKeyInfo[hkoTopMost].IsMsg(0,uMsg))
		return 0;
	HWND hWnd=::WindowFromPoint(lpMouseHookStruct->pt);
	if((hWnd=GetPopup(hWnd))==0)
		return 0;
	if((GetWindowLong(hWnd,GWL_EXSTYLE)&WS_EX_TOPMOST)==0)
	{
		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if(iterItem!=m_mapWndInfo.end())
			iterItem._Mynode()->_Myval.second.fTopMost=TRUE;
		else
		{
			CHook::WNDINFO wi={0};
			wi.fTopMost=TRUE;
			m_mapWndInfo[hWnd]=wi;
		}
		::SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		CULApp::GetULApp()->m_pMainWnd->PostMessage(CWorkWnd::OSDM_MESSAGE,
			(WPARAM)hWnd,(LPARAM)hkoTopMost);
		return 1;						
	}
	else
	{
		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if(iterItem!=m_mapWndInfo.end())
		{
			iterItem._Mynode()->_Myval.second.fTopMost=FALSE;
			::SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			CULApp::GetULApp()->m_pMainWnd->PostMessage(CWorkWnd::OSDM_MESSAGE,
				(WPARAM)hWnd,(LPARAM)hkoTopMost);
		}
	}
	return 1;
}

LRESULT CHook::ProcessMoveWnd(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	static bool fStartMoveWnd=false;
	static POINT ptStart={0,0};
	static HWND hWnd=NULL;
	if(m_MoveMethod==msmProgram)
	{
		if(fStartMoveWnd&&m_arHotKeyInfo[hkoMoveWnd].IsMsg(1,uMsg))
		{
			RECT rc;
			::GetWindowRect(hWnd,&rc);
			int nWidth=rc.right-rc.left;
			int nHeight=rc.bottom-rc.top;
			rc.left+=lpMouseHookStruct->pt.x-ptStart.x;
			rc.top+=lpMouseHookStruct->pt.y-ptStart.y;
			::MoveWindow(hWnd,rc.left,rc.top,nWidth,nHeight,TRUE);
			ptStart.x=lpMouseHookStruct->pt.x;
			ptStart.y=lpMouseHookStruct->pt.y;
		}
		else
			fStartMoveWnd=false;
		
		if(m_arHotKeyInfo[hkoMoveWnd].IsMsg(2,uMsg)&&fStartMoveWnd)
		{
			fStartMoveWnd=false;
			RedrawWindow(hWnd,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
			return 0;
		}
	}
	if(!m_arHotKeyInfo[hkoMoveWnd].IsHotKey(lpMouseHookStruct))
		return 0;
	if(!m_arHotKeyInfo[hkoMoveWnd].IsMsg(0,uMsg))
		return 0;
	hWnd=::WindowFromPoint(lpMouseHookStruct->pt);
	if((hWnd=GetPopup(hWnd))==0)
		return 0;
	
	if(m_MoveMethod==msmSystem)
		SendMessage(hWnd,WM_SYSCOMMAND,SC_SIZE+9,0);

	fStartMoveWnd=true;
	ptStart=lpMouseHookStruct->pt;

	return 1;
}

LRESULT CHook::ProcessSizeWnd(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	static bool fStartSizeWnd=false;
	static POINT ptStart={0,0};
	static HWND hWnd=NULL;
	if(m_SizeMethod==msmProgram)
	{
		if(fStartSizeWnd&&m_arHotKeyInfo[hkoSizeWnd].IsMsg(1,uMsg))
		{
			RECT rc;
			::GetWindowRect(hWnd,&rc);
			int nWidth=rc.right-rc.left;
			int nHeight=rc.bottom-rc.top;
			nWidth+=lpMouseHookStruct->pt.x-ptStart.x;
			nHeight+=lpMouseHookStruct->pt.y-ptStart.y;
			::MoveWindow(hWnd,rc.left,rc.top,nWidth,nHeight,TRUE);
			ptStart.x=lpMouseHookStruct->pt.x;
			ptStart.y=lpMouseHookStruct->pt.y;			
		}
		else
			fStartSizeWnd=false;
		
		if(m_arHotKeyInfo[hkoSizeWnd].IsMsg(2,uMsg)&&fStartSizeWnd)
		{
			RedrawWindow(hWnd,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
			fStartSizeWnd=false;
			
			return 0;
		}
	}
	if(!m_arHotKeyInfo[hkoSizeWnd].IsHotKey(lpMouseHookStruct))
		return 0;
	if(!m_arHotKeyInfo[hkoSizeWnd].IsMsg(0,uMsg))
		return 0;
	hWnd=::WindowFromPoint(lpMouseHookStruct->pt);
	if((hWnd=GetPopup(hWnd))==0)
		return 0;

	

	fStartSizeWnd=true;
	
	if(m_SizeMethod==msmSystem)
	{
		RECT rc;
		::GetWindowRect(hWnd,&rc);
		int nWidth=rc.right-rc.left;
		int nHeight=rc.bottom-rc.top;
		int x=lpMouseHookStruct->pt.x-rc.left;
		int y=lpMouseHookStruct->pt.y-rc.top;

		if(m_SizeMethodBy==smbCorner)
		{
			if(y<nHeight/2)
			{
				if(x<nWidth/2)
					SendMessage(hWnd,WM_SYSCOMMAND,SC_SIZE|WMSZ_TOPLEFT,0);
				else
					SendMessage(hWnd,WM_SYSCOMMAND,SC_SIZE|WMSZ_TOPRIGHT,0);
			}
			else
			{
				if(x<nWidth/2)
					SendMessage(hWnd,WM_SYSCOMMAND,SC_SIZE|WMSZ_BOTTOMLEFT,0);
				else
					SendMessage(hWnd,WM_SYSCOMMAND,SC_SIZE|WMSZ_BOTTOMRIGHT,0);
			}
		}
		else
		{
			int deltaLeft=x;
			int deltaRight=nWidth-x;
			int deltaTop=y;
			int deltaBottom=nHeight-y;

			if((deltaLeft<deltaRight)&&(deltaLeft<deltaTop)&&
				(deltaLeft<deltaBottom))
				SendMessage(hWnd,WM_SYSCOMMAND,SC_SIZE|WMSZ_LEFT,0);
			else
				if((deltaRight<deltaTop)&&
					(deltaRight<deltaBottom))
					SendMessage(hWnd,WM_SYSCOMMAND,SC_SIZE|WMSZ_RIGHT,0);
				else
					if(deltaTop<deltaBottom)
						SendMessage(hWnd,WM_SYSCOMMAND,SC_SIZE|WMSZ_TOP,0);
					else
						SendMessage(hWnd,WM_SYSCOMMAND,SC_SIZE|WMSZ_BOTTOM,0);
		}
	}

	ptStart=lpMouseHookStruct->pt;
	return 1;
}

LRESULT CHook::ProcessToggleCaption(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	if(!m_arHotKeyInfo[hkoToggleCaption].IsHotKey(lpMouseHookStruct))
		return 0;
	if(!m_arHotKeyInfo[hkoToggleCaption].IsMsg(0,uMsg))
		return 0;
	HWND hWnd=::WindowFromPoint(lpMouseHookStruct->pt);
	if((hWnd=GetPopup(hWnd))==0)
		return 0;
	if((GetWindowLong(hWnd,GWL_STYLE)&WS_CAPTION)==WS_CAPTION)
	{
		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if(iterItem!=m_mapWndInfo.end())
			iterItem._Mynode()->_Myval.second.dwStyle=(DWORD)::GetWindowLong(hWnd,GWL_STYLE);
		else
		{
			CHook::WNDINFO wi={0};
			wi.dwStyle=(DWORD)::GetWindowLong(hWnd,GWL_STYLE);
			m_mapWndInfo[hWnd]=wi;
		}
		::SetWindowLong(hWnd,GWL_STYLE,
			::GetWindowLong(hWnd,GWL_STYLE)&~WS_CAPTION);
		::SetWindowPos(hWnd,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);
		CULApp::GetULApp()->m_pMainWnd->PostMessage(CWorkWnd::OSDM_MESSAGE,
			(WPARAM)hWnd,(LPARAM)hkoToggleCaption);

		return 1;						
	}
	else
	{
		std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
		if(iterItem!=m_mapWndInfo.end())
		{
			::SetWindowLong(hWnd,GWL_STYLE,
				::GetWindowLong(hWnd,GWL_STYLE)|iterItem._Mynode()->_Myval.second.dwStyle);
			::SetWindowPos(hWnd,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);
			iterItem._Mynode()->_Myval.second.dwStyle=0;
			CULApp::GetULApp()->m_pMainWnd->PostMessage(CWorkWnd::OSDM_MESSAGE,
				(WPARAM)hWnd,(LPARAM)hkoToggleCaption);
		}
	}
	RedrawWindow(hWnd,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	return 1;
}

LRESULT CHook::ProcessCollapseWnd(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	if(!m_arHotKeyInfo[hkoCollapseWnd].IsHotKey(lpMouseHookStruct))
		return 0;
	if(!m_arHotKeyInfo[hkoCollapseWnd].IsMsg(0,uMsg))
		return 0;
	HWND hWnd=::WindowFromPoint(lpMouseHookStruct->pt);
	if((hWnd=GetPopup(hWnd))==0)
		return 0;
	RECT rc;
	GetWindowRect(hWnd,&rc);

	std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=m_mapWndInfo.find(hWnd);
	if(iterItem==m_mapWndInfo.end())
	{
		CHook::WNDINFO wi={0};
		m_mapWndInfo[hWnd]=wi;
	}
//	if((GetWindowLong(hWnd,GWL_STYLE)&WS_MINIMIZE)==0)
//		m_mapWndInfo[hWnd].dwNonCollapseHeight=0;

	if(m_mapWndInfo[hWnd].dwNonCollapseHeight==0)
	{
//		SetWindowLong(hWnd,GWL_STYLE,GetWindowLong(hWnd,GWL_STYLE)|WS_ICONIC);
//		CloseWindow(hWnd);
//		SetWindowLong(hWnd,GWL_STYLE,GetWindowLong(hWnd,GWL_STYLE)|WS_MINIMIZE);
//		SetWindowPos(hWnd,NULL,0,0,rc.right-rc.left,
//			20,SWP_NOZORDER|SWP_NOMOVE|SWP_FRAMECHANGED);
		
//		ShowWindow(hWnd,SW_SHOWMINNOACTIVE);
//		SetWindowLong(hWnd,GWL_STYLE,GetWindowLong(hWnd,GWL_STYLE)|WS_VISIBLE);
//		SetWindowPos(hWnd,NULL,0,0,rc.right-rc.left,
//			20,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
//		if(IsWindowEnabled(hWnd))
//			MessageBeep(MB_ICONASTERISK);
//		SetActiveWindow(hWnd);
		WINDOWPLACEMENT wp;
		wp.length=sizeof(wp);
		wp.flags=WPF_RESTORETOMAXIMIZED;
		wp.ptMaxPosition.x=0;
		wp.ptMaxPosition.y=0;
		wp.rcNormalPosition.left=rc.left;
		wp.rcNormalPosition.top=rc.top;
		wp.rcNormalPosition.right=rc.right;
		wp.rcNormalPosition.bottom=100;//rc.top+30;
		wp.showCmd=SW_SHOWNORMAL;
//		SetWindowPlacement(hWnd,&wp);
		
//		SendMessage(hWnd,WM_SIZE,SIZE_RESTORED,MAKELPARAM(rc.right-rc.left,0));
		//rc.bottom=900;
//		SendMessage(hWnd,WM_NCCALCSIZE,FALSE,(LPARAM)&rc);
		
//		SetWindowPlacement(hWnd,&wp);

//		SetWindowLong(hWnd,GWL_STYLE,GetWindowLong(hWnd,GWL_STYLE)|WS_VISIBLE);
		SIZE sz={rc.right-rc.left,26};
//		EnableBlockSize(hWnd,sz,TRUE);
//		SetWindowPlacement(hWnd,&wp);
		RECT rcClient;
		GetClientRect(hWnd,&rcClient);
		ValidateRect(hWnd,&rcClient);

		SetWindowPos(hWnd,NULL,0,0,rc.right-rc.left,
			26,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSENDCHANGING|SWP_NOACTIVATE);
		
//		EnableBlockSize(hWnd,sz,FALSE);

			//GetWindowRect(hWnd,&rc);
//			if((rc.right-rc.left)>50)
//				MessageBeep(1);
/*		SetWindowPos(hWnd,NULL,0,0,rc.right-rc.left,26,
			SWP_NOZORDER|SWP_NOMOVE|SWP_FRAMECHANGED|SWP_NOACTIVATE);

		SetWindowPos(hWnd,NULL,0,0,rc.right-rc.left,
			26,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSENDCHANGING|SWP_NOACTIVATE);
*/

		m_mapWndInfo[hWnd].dwNonCollapseHeight=rc.bottom-rc.top;
	}
	else
	{
		SIZE sz={0,0};
//		EnableBlockSize(hWnd,sz,FALSE);
		//ShowWindow(hWnd,SW_RESTORE);
		SetWindowPos(hWnd,NULL,0,0,rc.right-rc.left,
			m_mapWndInfo[hWnd].dwNonCollapseHeight,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSENDCHANGING|SWP_NOACTIVATE);

	/*		SetWindowLong(hWnd,GWL_STYLE,GetWindowLong(hWnd,GWL_STYLE)&~WS_ICONIC);
			SetWindowPos(hWnd,NULL,0,0,rc.right-rc.left,
				rc.bottom-rc.top,SWP_NOZORDER|SWP_NOMOVE|SWP_FRAMECHANGED);
				*/
		m_mapWndInfo[hWnd].dwNonCollapseHeight=0;
	}


	return 1;
}






//////!!!!!!!!!!!!!!это пробное для сквозного клика!!!!


///\class CEnumWindows
///\brief класс для поиска окна под окном, который под курсором мыши
///\note идея заключается в пеереборе окон и проверки всех последующих \n
/// в попадание под курсор.\n
/// При попадании под курсор производится поиск дочернего окна
struct CEnumWindows
{
	HWND m_hwndParent;
	HWND m_hwndNext;
	POINT m_ptClick;
	bool m_fFindParent;
	CEnumWindows():m_hwndParent(NULL),m_hwndNext(NULL),m_fFindParent(false)
	{}
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd,LPARAM lParam)
	{
		CEnumWindows* pEnumWindows=(CEnumWindows*)lParam;
		if(!pEnumWindows->m_fFindParent)
		{
			if(hWnd==pEnumWindows->m_hwndParent)
				pEnumWindows->m_fFindParent=true;
		}
		else
		{
			RECT rc;
			GetWindowRect(hWnd,&rc);
			if(::PtInRect(&rc,pEnumWindows->m_ptClick))
			{
				pEnumWindows->m_hwndNext=hWnd;
				return FALSE;
			}
		}

		return TRUE;
	}
	HWND GetNext(HWND hWnd,POINT pt)
	{
		m_hwndParent=hWnd;
		m_ptClick=pt;
		m_hwndNext=NULL;
		m_fFindParent=false;
		EnumWindows((WNDENUMPROC)EnumWindowsProc,(LPARAM)this);
		return m_hwndNext;
	}
	HWND GetChild(HWND hWnd,POINT pt)
	{
		if(!m_hwndNext)
			if(!GetNext(hWnd,pt))
				return FALSE;
		HWND hwndChild=m_hwndNext;
		for(;;)
		{
/*		
			FLASHWINFO fi={0};
			fi.cbSize=sizeof(fi);
			fi.hwnd=hwndChild;
			fi.uCount=4;
			FlashWindowEx(&fi);
			*/
			HWND hwndTmp=ChildWindowFromPoint(hwndChild,pt);
			if(hwndTmp&&(hwndTmp!=hwndChild))
				hwndChild=hwndTmp;
			else
				break;
		}
		return hwndChild;
	}
};
CEnumWindows g_EnumWindows;

LRESULT CHook::ProcessThroughClick(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct)
{
	if(!m_arHotKeyInfo[hkoThroughClick].IsHotKey(lpMouseHookStruct))
		return 0;
	if(!((uMsg==WM_LBUTTONUP)||(uMsg==WM_LBUTTONDOWN)))
		return 0;

	HWND hWnd=::WindowFromPoint(lpMouseHookStruct->pt);
	if((hWnd=GetPopup(hWnd))==0)
		return 0;

	HWND hwndNext=g_EnumWindows.GetNext(hWnd,lpMouseHookStruct->pt);
	HWND hwndChild=g_EnumWindows.GetChild(hWnd,lpMouseHookStruct->pt);
	if(!hWnd)
		return 0;
//	::SetForegroundWindow(hWnd);
//	::SwitchToThisWindow(hwndNext,TRUE);
	::SetForegroundWindow(hwndNext);
//	::SetActiveWindow(hwndNext);
//	::SetFocus(hwndNext);
//	::BringWindowToTop(hwndNext);
//	::SetWindowPos(hwndNext,hWnd,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
//	::SetForegroundWindow(hwndNext);
//	ShowWindow(hWnd,SW_HIDE);
	if(uMsg==WM_LBUTTONDOWN&&0)
	{
		::SendMessage(hwndChild,WM_LBUTTONDOWN,0,MAKELPARAM(lpMouseHookStruct->pt.x,
			lpMouseHookStruct->pt.y));
		::SendMessage(hwndChild,WM_LBUTTONUP,0,MAKELPARAM(lpMouseHookStruct->pt.x,
			lpMouseHookStruct->pt.y));
	}
//::SetForegroundWindow(hWnd);
//		::SetActiveWindow(hwndNext);
//	::SetFocus(hwndNext);
	return 1;
}
/////////////////////окончание функций сквозного клика


