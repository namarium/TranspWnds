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
//------------------------------------------------------------------------------------------//
///\file TranspWndsHKH.cpp
///\date 30.04.09 (20.23)
///\author Volodin Oleg
//------------------------------------------------------------------------------------------//
#define TRANSP_WNDS_HKH_EXPORTS

#include <windows.h>

#include "TranspWndsHKH.h"
#include <sstream>

typedef struct tagBlockSize
{
	HWND hTargetWnd;
	SIZE szWnd;
	BOOL fEnable;
}BLOCKSIZE;

#pragma data_seg("Shared")
HINSTANCE g_hInstance		= NULL;
HWND g_hHostWnd				= NULL;
HHOOK g_hCallWndProcHook	= NULL;
HHOOK g_hGetMsgHookProc=NULL;
BLOCKSIZE g_BlockSize={0};
#pragma data_seg()
#pragma comment( linker, "/section:Shared,rws" )


BOOL WINAPI DllMain(
	HINSTANCE hInstanceDLL,
	DWORD fdwReason,
	LPVOID lpvReserved
)
{
	g_hInstance = hInstanceDLL;
	return TRUE;
}

LRESULT CALLBACK CallWndProcHookFunc(
	int nCode,
	WPARAM wParam,
	LPARAM lParam
)
{
	if ( nCode >= 0 )
	{
		PCWPSTRUCT pcws = (PCWPSTRUCT)lParam;

		if ( pcws->message == WM_MOUSEWHEEL )
		{
			PostMessage( g_hHostWnd, NWM_TOUCHPADMOUSEWHEEL, pcws->wParam, pcws->lParam );
		}
/*
		if((pcws->message==WM_WINDOWPOSCHANGING)||
			(pcws->message==WM_WINDOWPOSCHANGED))
		{
			if(g_BlockSize.fEnable&&(pcws->hwnd==g_BlockSize.hTargetWnd))
			{
				//MessageBeep(MB_OK);
				WINDOWPOS* pwp=(WINDOWPOS*)pcws->lParam;
				if((pwp->cx=g_BlockSize.szWnd.cx)||(pwp->cy!=g_BlockSize.szWnd.cy))
				{
					//LRESULT lRes=CallNextHookEx( g_hCallWndProcHook, nCode, wParam, lParam );
					MessageBeep(MB_OK);
//					std::basic_stringstream<TCHAR> ss;
//					ss<<pwp->cx<<"   "<<pwp->cy<<"   "<<g_BlockSize.szWnd.cx<<"   "<<g_BlockSize.szWnd.cy;
//					SetWindowText(pcws->hwnd,ss.str().c_str());
					pwp->cx=g_BlockSize.szWnd.cx;
					pwp->cy=g_BlockSize.szWnd.cy;
					return 1;//lRes;
				}
				else
					MessageBeep(MB_ICONHAND);
				return 1;					
			}
			//MessageBeep(MB_OK);
		}
		*/
/*		if(pcws->message==WM_NCCALCSIZE)
		{
		 if(g_BlockSize.fEnable&&(pcws->hwnd==g_BlockSize.hTargetWnd))
		 {
				if(pcws->wParam)
				{
					LPNCCALCSIZE_PARAMS lpcsp=(LPNCCALCSIZE_PARAMS)pcws->lParam;
	//				lpcsp->rgrc[0];
						std::basic_stringstream<TCHAR> ss;
						ss<<lpcsp->rgrc[0].left<<"   "<<lpcsp->rgrc[0].top
							<<"   "<<lpcsp->rgrc[0].right<<"   "<<lpcsp->rgrc[0].bottom<<" : "
							<<lpcsp->rgrc[1].left<<"   "<<lpcsp->rgrc[1].top
							<<"   "<<lpcsp->rgrc[1].right<<"   "<<lpcsp->rgrc[1].bottom<<" : "
							<<lpcsp->rgrc[2].left<<"   "<<lpcsp->rgrc[2].top
							<<"   "<<lpcsp->rgrc[2].right<<"   "<<lpcsp->rgrc[2].bottom;
						SetWindowText(pcws->hwnd,ss.str().c_str());

						lpcsp->rgrc[0].bottom=lpcsp->rgrc[0].top+26;//g_BlockSize.szWnd.cy;
						lpcsp->rgrc[1].bottom=lpcsp->rgrc[0].top+26;//g_BlockSize.szWnd.cy;
						lpcsp->rgrc[2].bottom=lpcsp->rgrc[0].top+26;//g_BlockSize.szWnd.cy;
						WINDOWPOS* pwp=(WINDOWPOS*)lpcsp->lppos;
						pwp->cy=26;
						MessageBeep(MB_OK);
						return 0;

				}
				else
				{
	//				LPRECT lprc=(LPRECT)pcws->lParam;
	//				lprc->bottom=lprc->top+26;

				}
			}
			
		}*/
	}

	return CallNextHookEx( g_hCallWndProcHook, nCode, wParam, lParam );
}

TRANSP_WNDS_HKH_API
BOOL SetTranspWndsHookForTouchpad(
	HWND hHostWnd
)
{
	if (
		(g_hHostWnd = hHostWnd) != NULL &&
		(g_hCallWndProcHook = SetWindowsHookEx( WH_CALLWNDPROC, CallWndProcHookFunc, g_hInstance, NULL )) != NULL
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

TRANSP_WNDS_HKH_API
BOOL UnSetTranspWndsHookForTouchpad()
{
	return UnhookWindowsHookEx( g_hCallWndProcHook );
}

LRESULT CALLBACK GetMsgHookProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	PMSG pmsg=(PMSG)lParam;
	if((pmsg->message==WM_SIZE))
		MessageBeep(MB_OK);	
	if((pmsg->message==WM_WINDOWPOSCHANGING)||
		(pmsg->message==WM_WINDOWPOSCHANGED))
		{
			if(g_BlockSize.fEnable&&(pmsg->hwnd==g_BlockSize.hTargetWnd))
			{//MessageBeep(MB_OK);
				WINDOWPOS* pwp=(WINDOWPOS*)pmsg->lParam;
				if((pwp->cx=g_BlockSize.szWnd.cx)||(pwp->cy!=g_BlockSize.szWnd.cy))
				{
					//LRESULT lRes=CallNextHookEx( g_hCallWndProcHook, nCode, wParam, lParam );
					
					std::basic_stringstream<TCHAR> ss;
					ss<<pwp->cx<<"   "<<pwp->cy<<"   "<<g_BlockSize.szWnd.cx<<"   "<<g_BlockSize.szWnd.cy;
					SetWindowText(pmsg->hwnd,ss.str().c_str());
					pwp->cx=g_BlockSize.szWnd.cx;
					pwp->cy=g_BlockSize.szWnd.cy;
					//return 1;//lRes;
				}
			}
MessageBeep(MB_ICONHAND);
		}
/*	if(pmsg->message!=WM_SETTEXT)
	{
			std::basic_stringstream<TCHAR> ss;
			ss<<std::hex<<pmsg->message;
			SetWindowText(pmsg->hwnd,ss.str().c_str());
	}
*/
//				MessageBeep(MB_ICONHAND);
	return CallNextHookEx( g_hGetMsgHookProc, nCode, wParam, lParam );
}

TRANSP_WNDS_HKH_API void EnableBlockSize(HWND hTargetWnd,SIZE szWnd,BOOL fEnable)
{
	g_BlockSize.hTargetWnd=hTargetWnd;
	g_BlockSize.szWnd=szWnd;
	g_BlockSize.fEnable=fEnable;
/*	if(fEnable)
	{
		DWORD dwProcId=0;
//		::GetWindowThreadProcessId(hTargetWnd,&dwProcId);
		g_hGetMsgHookProc=SetWindowsHookEx(WH_GETMESSAGE,GetMsgHookProc,g_hInstance,dwProcId);
	}
	else
		UnhookWindowsHookEx(g_hGetMsgHookProc);
*/
//	if(fEnable)
//		MessageBeep(MB_ICONHAND);

}
