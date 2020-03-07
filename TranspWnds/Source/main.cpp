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
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include "resource.h"
#include <map>

LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

///\brief структура, содержащая информацию об окне
///\param bAlpha - альфаканал
///\param fAlpha - TRUE,если окну был присвоен стиль WS_EX_LAYERED 
///\param fTopMost - TRUE,если окну был присвоен стиль WS_EX_TOPMOST 
typedef struct tagWndInfo
{
	BYTE bAlpha;
	BOOL fAlpha;
	BOOL fTopMost;
}WNDINFO,*LPWNDINFO;
///\brief мап окно - информация о нём
std::map<HWND,WNDINFO> g_mapWndInfo;
///\brief хендл хука
HHOOK g_hhMouse;


HINSTANCE g_hInst;
/*
int __stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	//синглтон
	if(CreateMutex(NULL,FALSE,_T("TranspWndSinletone")))
		if(GetLastError()==ERROR_ALREADY_EXISTS)
			return 0;

	g_hInst=hInstance;
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW; 
	wc.lpfnWndProc = (WNDPROC) WindowProc; 
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hInstance = hInstance; 
	wc.hIcon=LoadIcon(NULL, IDI_APPLICATION); 
	wc.hCursor=LoadCursor(NULL, IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
	wc.lpszMenuName=NULL;
	wc.lpszClassName=_T("TranspWndClass");

	if (!RegisterClass(&wc)) 
		return FALSE; 

	HWND hWnd=CreateWindowEx(0,_T("TranspWndClass"),_T("Main Window"),
    WS_POPUP,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		(HWND) NULL,(HMENU) NULL,hInstance,NULL);
 
	if (!hWnd) 
		return FALSE; 

	MSG msg;
	while(GetMessage(&msg,0,0,0)&&IsWindow(hWnd))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return 0;
}
*/
/*
///\brief функция преднозначена для popup родителя
///\param hWnd - хендл окна на родителе, или определенной иерархии дочерних окон или самого popup
///\return хендл popup в случае успеха, если popup оказывается рабочий стол то вернет NULL
HWND GetPopup(HWND hWnd)
{
	while(((GetWindowLong(hWnd,GWL_STYLE)&WS_CHILD))&&IsWindow(hWnd))
		hWnd=GetParent(hWnd);
	if(!hWnd)
		return NULL;		
	static TCHAR szText[255];
	GetClassName(hWnd,szText,255);
	_tcslwr_s(szText,255);
	if(_tcscmp(szText,_T("progman"))==0)
		return NULL;
	return hWnd;
}*//*
///\brief процедура мышинного хука
LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	if((wParam==WM_MOUSEWHEEL)&&(HIWORD(GetKeyState(VK_MENU))))
	{
		PMSLLHOOKSTRUCT phs=(PMSLLHOOKSTRUCT)lParam;
		HWND hWnd=::WindowFromPoint(phs->pt);
		if((hWnd=GetPopup(hWnd))!=0)
		{
			if((GetWindowLong(hWnd,GWL_EXSTYLE)&WS_EX_LAYERED)==0)
			{
				if(((short)HIWORD(phs->mouseData))>0)
					return CallNextHookEx(g_hhMouse,nCode,wParam,lParam);
				::SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)|WS_EX_LAYERED);
				WNDINFO wi={0};
				wi.bAlpha=255;
				wi.fAlpha=TRUE;
				g_mapWndInfo[hWnd]=wi;
				SetLayeredWindowAttributes(hWnd,0,255,LWA_ALPHA);
				RedrawWindow(hWnd,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
				return 1;
			}
			else
			{
				std::map<HWND,WNDINFO>::const_iterator iterItem=g_mapWndInfo.find(hWnd);
				if((iterItem._Mynode()->_Myval.first!=hWnd)||(!iterItem._Mynode()->_Myval.second.fAlpha))
					return 1;				
			}
			BYTE bAlpha=g_mapWndInfo[hWnd].bAlpha;
			if(((short)HIWORD(phs->mouseData))<0)
			{
				if(bAlpha>100)
					bAlpha-=10;
			}
			else
			{
				if(bAlpha<=245)
					bAlpha+=10;
			}			
			SetLayeredWindowAttributes(hWnd,0,bAlpha,LWA_ALPHA);		
			g_mapWndInfo[hWnd].bAlpha=bAlpha;
			return 1;
		}
	}
	if((wParam==WM_LBUTTONDOWN)&&(HIWORD(GetKeyState(VK_MENU)))&&(HIWORD(GetKeyState(VK_CONTROL))))
	{
		PMSLLHOOKSTRUCT phs=(PMSLLHOOKSTRUCT)lParam;
		HWND hWnd=::WindowFromPoint(phs->pt);
		if((hWnd=GetPopup(hWnd))!=0)
			if((GetWindowLong(hWnd,GWL_EXSTYLE)&WS_EX_TOPMOST)==0)
			{
				std::map<HWND,WNDINFO>::const_iterator iterItem=g_mapWndInfo.find(hWnd);
				if((iterItem._Mynode()->_Myval.first==hWnd))
					iterItem._Mynode()->_Myval.second.fTopMost=TRUE;
				else
				{
					WNDINFO wi={0};
					wi.fTopMost=TRUE;
					g_mapWndInfo[hWnd]=wi;
				}
				::SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
				return 1;						
			}
			else
			{
				std::map<HWND,WNDINFO>::const_iterator iterItem=g_mapWndInfo.find(hWnd);
				if((iterItem._Mynode()->_Myval.first==hWnd))
				{
					iterItem._Mynode()->_Myval.second.fTopMost=FALSE;
					::SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
				}
			}

	}
	return CallNextHookEx(g_hhMouse,nCode,wParam,lParam);
}
*/
///\brief изменяет статус автозагрузки
///\param fAdd - TRUE,если надо добавить программу в автозагрузку, FALSE, если убрать
void AutoRun(BOOL fAdd)
{
	HKEY hKey;
	LPCTSTR szValName=_T("TranspWnds");
	TCHAR szFilePath[0xffff];
	::GetModuleFileName(g_hInst,szFilePath,0xffff);
	TCHAR szKeyName[]=_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	LONG lResult=RegOpenKeyEx(HKEY_CURRENT_USER,szKeyName,0,(fAdd)?KEY_WRITE:KEY_SET_VALUE,&hKey);
	if(lResult==ERROR_SUCCESS)
		if(!fAdd)
		{
			lResult = ::RegDeleteValue(hKey,(LPTSTR)szValName);
		}
		else
			lResult=RegSetValueEx(hKey,szValName,0,REG_SZ,
				(BYTE*)szFilePath,(WORD)_tcslen(szFilePath)*sizeof(szFilePath[0]));
}
///\brief возвращает статус автозагрузки
///\return true, если прграмма в автозагрузке, иначе false
bool IsAutoRun()
{
	HKEY hKey;
	LPCTSTR szValName=_T("TranspWnds");
	TCHAR szKeyName[]=_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	LONG lResult=RegOpenKeyEx(HKEY_CURRENT_USER,szKeyName,0,KEY_QUERY_VALUE,&hKey);
	TCHAR szValNameRet[0xffff]={0};
	bool fFind=false;
	for(DWORD i=0;;++i)
	{
		DWORD dwSize=0xffff;
		//RegEnumValue сделан потому, что у меня отсутвует RegGetValue в Advapi32.dll. 
		//причины не знаю
		lResult=RegEnumValue(hKey,i++,szValNameRet,&dwSize,0,0,0,0);
		if(lResult==ERROR_SUCCESS)
		{
			if(_tcscmp(szValName,szValNameRet)==0)
			{
				fFind=true;
				break;
			}
		}
		else
			break;
	}
	::RegCloseKey(hKey);	
	return fFind;
}
///\brief перечисление оконных собщений
//enum 
//{
//	IDM_ENABLE=1001,
//	IDM_DISABLE,
//	IDM_RESTORRE,
//	IDM_AUTORUN,
//	IDM_QUIT,	
//	NIM_MESSAGE=WM_APP+1
//};
///\brief оконная функция
/*
LRESULT CALLBACK WindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static HMENU hPupupMenu=NULL;
	static NOTIFYICONDATA niData={};

	switch(uMsg)
	{
	case WM_CREATE:
		{
			hPupupMenu=::CreatePopupMenu();
			::AppendMenu(hPupupMenu, MF_GRAYED|MF_DISABLED|MF_BYCOMMAND, IDM_ENABLE, _T("Enable"));
			::AppendMenu(hPupupMenu, MF_BYCOMMAND, IDM_DISABLE, _T("Disable"));
			::AppendMenu(hPupupMenu, MF_BYCOMMAND, IDM_RESTORRE, _T("Restore"));			
			::AppendMenu(hPupupMenu, ((IsAutoRun())?MF_CHECKED:0)|MF_BYCOMMAND, IDM_AUTORUN, _T("AutoRun"));			
			::AppendMenu(hPupupMenu, MF_BYCOMMAND, IDM_QUIT, _T("Quit"));
			
			niData.cbSize=sizeof(niData);
			niData.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
			niData.hWnd=hWnd;
			niData.hIcon=(HICON)LoadImage(g_hInst,MAKEINTRESOURCE(IDI_MAINICON),IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_DEFAULTCOLOR);
			if(niData.hIcon==NULL)
				::DestroyWindow(hWnd);
			_tcscpy_s(niData.szTip,64,_T("TranspWnds"));
			niData.uCallbackMessage=NIM_MESSAGE;
			niData.uID=NIM_MESSAGE;
			Shell_NotifyIcon(NIM_ADD,&niData);
			g_hhMouse=SetWindowsHookEx(WH_MOUSE_LL,MouseProc,g_hInst,0);

			//задается для проверки существавания окон в мапе
			SetTimer(hWnd,0,60000,NULL);
			break;
		}
	case WM_TIMER:
		for(std::map<HWND,WNDINFO>::iterator i=g_mapWndInfo.begin();i!=g_mapWndInfo.end();i++)
			if(!IsWindow(i->first))
				g_mapWndInfo.erase(i);
		break;
	case NIM_MESSAGE:
		{
			if((lParam==WM_LBUTTONDOWN)||(lParam==WM_RBUTTONDOWN))
			{
				POINT pt={0};
				GetCursorPos(&pt);
				SetForegroundWindow(hWnd);
				TrackPopupMenu(hPupupMenu,TPM_RIGHTBUTTON,pt.x,pt.y,0,hWnd,NULL);
				PostMessage(hWnd,WM_NULL,0,0);
			}
			return 1;
		}
	case WM_COMMAND:
		{
			static MENUITEMINFO mii={0};
			switch(LOWORD(wParam))
			{
			case IDM_ENABLE:
				g_hhMouse=SetWindowsHookEx(WH_MOUSE_LL,MouseProc,g_hInst,0);
				mii.cbSize=sizeof(MENUITEMINFO);
				mii.fMask=MIIM_STATE;
				mii.fState=MFS_GRAYED|MFS_DISABLED;
				SetMenuItemInfo(hPupupMenu,IDM_ENABLE,FALSE,&mii);
				mii.fState=MFS_ENABLED;
				SetMenuItemInfo(hPupupMenu,IDM_DISABLE,FALSE,&mii);
				break;
			case IDM_DISABLE:
				UnhookWindowsHookEx(g_hhMouse);
				mii.cbSize=sizeof(MENUITEMINFO);
				mii.fMask=MIIM_STATE;
				mii.fState=MFS_GRAYED|MFS_DISABLED;
				SetMenuItemInfo(hPupupMenu,IDM_DISABLE,FALSE,&mii);
				mii.fState=MFS_ENABLED;
				SetMenuItemInfo(hPupupMenu,IDM_ENABLE,FALSE,&mii);
				g_hhMouse=NULL;
				break;
			case IDM_RESTORRE:
				for(std::map<HWND,WNDINFO>::iterator i=g_mapWndInfo.begin();i!=g_mapWndInfo.end();i++)
					if(IsWindow(i->first))
					{
						if(i->second.fAlpha)
						{
							SetLayeredWindowAttributes(i->first,0,255,LWA_ALPHA);
							::SetWindowLong(i->first,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)&~WS_EX_LAYERED);
							RedrawWindow(i->first,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
						}
						if(i->second.fTopMost)
							::SetWindowPos(i->first,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
					}
					g_mapWndInfo.clear();
				break;
			case IDM_AUTORUN:
				{
					bool fAutoRun=IsAutoRun();
					AutoRun(!fAutoRun);
					mii.cbSize=sizeof(MENUITEMINFO);
					mii.fMask=MIIM_STATE;
					mii.fState=((IsAutoRun())?MF_CHECKED:0);
					SetMenuItemInfo(hPupupMenu,IDM_AUTORUN,FALSE,&mii);
					break;
				}
			case IDM_QUIT:
				DestroyWindow(hWnd);
				break;
			}
			break;
		}
	case WM_DESTROY:
		if(g_hhMouse)
			UnhookWindowsHookEx(g_hhMouse);
		Shell_NotifyIcon(NIM_DELETE,&niData);
		::PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
*/
