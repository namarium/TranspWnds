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
#include "Hook.h"
#include "WorkWnd.h"
#include "resource.h"
#include "PropHotKeys.h"
#include "PropSystem.h"
#include "PropTransparent.h"
#include "PropOSD.h"

#include "TranspWndsHKH.h"

#include <sstream>

#include "AboutDlg.h"

#define TOUCHPAD 


CWorkWnd::CWorkWnd(void):
	CULWnd()
{
	MessageMap.AddMessage<CWorkWnd>(WM_CREATE,&CWorkWnd::OnCreate);
	MessageMap.AddMessage<CWorkWnd>(WM_DESTROY,&CWorkWnd::OnDestroy);
	MessageMap.AddMessage<CWorkWnd>(NIM_MESSAGE,&CWorkWnd::OnNIMessage);
	MessageMap.AddMessage<CWorkWnd>(OSDM_MESSAGE,&CWorkWnd::OnOSDMessage);
	MessageMap.AddMessage<CWorkWnd>(UDM_CHECKFORUPDATE,&CWorkWnd::OnCheckForUpdate);
	MessageMap.AddMessage<CWorkWnd>(UDM_CHECKFORUPDATENOTIFY,&CWorkWnd::OnCheckForUpdateNotify);

	MessageMap.AddMessage<CWorkWnd>(WM_TIMER,&CWorkWnd::OnTimer);

	MessageMap.AddCommand<CWorkWnd>(IDM_ENABLE,&CWorkWnd::OnEnable);
	MessageMap.AddCommand<CWorkWnd>(IDM_DISABLE,&CWorkWnd::OnDisable);
	MessageMap.AddCommand<CWorkWnd>(IDM_RESTORE,&CWorkWnd::OnRestore);

	MessageMap.AddCommand<CWorkWnd>(IDM_VIEWING,&CWorkWnd::OnViewingWnds);	

	MessageMap.AddCommand<CWorkWnd>(IDM_OPTIONS,&CWorkWnd::OnOptions);

	MessageMap.AddCommand<CWorkWnd>(IDM_ABOUT,&CWorkWnd::OnAbout);
		
	MessageMap.AddCommand<CWorkWnd>(IDM_QUIT,&CWorkWnd::OnQuit);

	MessageMap.AddMessage<CWorkWnd>( NWM_TOUCHPADMOUSEWHEEL, &CWorkWnd::OnTouchPadMousewheel );
}

CWorkWnd::~CWorkWnd(void)
{
}

void CWorkWnd::InitUpdateTimers()
{
	switch(m_Updater.m_UpdateType)
	{
	case CUpdater::utAtStart:
		KillTimer(tCheckForUpdateAt24);
		SetTimer(tCheckForUpdateStartProg,tvFotStart);
		break;
	case CUpdater::utEvery24:
		KillTimer(tCheckForUpdateStartProg);
		SetTimer(tCheckForUpdateAt24,tvFotAt24);
		break;
	case CUpdater::utAtStartEvery24:
		SetTimer(tCheckForUpdateStartProg,tvFotStart);
		SetTimer(tCheckForUpdateAt24,tvFotAt24);
		break;
	}
}

LRESULT CWorkWnd::OnCreate(WPARAM,LPARAM)
{	
	HICON hIcon=(HICON)::LoadImage(ULGetResourceHandle(),MAKEINTRESOURCE(IDI_MAINICON),
		IMAGE_ICON,0,0,LR_DEFAULTSIZE);
	SetIcon(hIcon,TRUE);

	m_Menu.CreatePopupMenu();
	m_Menu.AppendMenu(MF_GRAYED|MF_DISABLED|MF_BYCOMMAND,IDM_ENABLE,CULStrTable(IDS_MENU_ENABLE));
	m_Menu.AppendMenu(MF_BYCOMMAND,IDM_DISABLE,CULStrTable(IDS_MENU_DISABLE));
	m_Menu.AppendMenu(MF_BYCOMMAND,IDM_RESTORE,CULStrTable(IDS_MENU_RESTORE));			
	m_Menu.AppendMenu(MF_BYPOSITION|MF_SEPARATOR,3,NULL);			
	m_Menu.AppendMenu(MF_BYCOMMAND,IDM_VIEWING,CULStrTable(IDS_MENU_VIEWING));			
	m_Menu.AppendMenu(MF_BYPOSITION|MF_SEPARATOR,3,NULL);			
	m_Menu.AppendMenu(MF_BYCOMMAND,IDM_OPTIONS,CULStrTable(IDS_MENU_OPTIONS));			
	m_Menu.AppendMenu(MF_BYPOSITION|MF_SEPARATOR,5,NULL);
	m_Menu.AppendMenu(MF_BYCOMMAND,IDM_ABOUT,CULStrTable(IDS_MENU_ABOUT));			
	m_Menu.AppendMenu(MF_BYPOSITION|MF_SEPARATOR,5,NULL);
	
	m_Menu.AppendMenu(MF_BYCOMMAND,IDM_QUIT,CULStrTable(IDS_MENU_QUIT));


	//грузим трэй иконку
	m_hTrayIcon=(HICON)LoadImage(ULGetResourceHandle(),MAKEINTRESOURCE(IDI_MAINICON),IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_DEFAULTCOLOR);
	//подписываемся на сообщение создание панели задач
	UINT WM_TASKBARCREATED = RegisterWindowMessage(_T("TaskbarCreated"));
	MessageMap.AddMessage<CWorkWnd>(WM_TASKBARCREATED,&CWorkWnd::OnTaskBarCreated);
	//создаём иконку
	PostMessage(WM_TASKBARCREATED);
	

	m_ProfileReg.SetRegistryKey(CULStrTable(IDS_COMPANY_NAME),CULStrTable(IDS_APP_NAME));
	LoadSettings();
#ifdef TOUCHPAD
	SetTranspWndsHookForTouchpad(m_hWnd);
#endif

	CHook::GetHook()->Enable();

	SetTimer(tCheckWnds,2000);

	ASSERT(m_osdWnd.Create(*this));

	InitUpdateTimers();

	return 0;
}

LRESULT CWorkWnd::OnTimer(WPARAM idTimer,LPARAM)
{
	switch(idTimer)
	{
	case tCheckWnds:
		for(std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=
			CHook::GetHook()->m_mapWndInfo.begin();
			iterItem!=CHook::GetHook()->m_mapWndInfo.end();iterItem++)
			if(!::IsWindow(iterItem->first))
			{
				if(m_ViewingWndsDlg.IsWindow())
					m_ViewingWndsDlg.DeleteItem(iterItem->first);
				CHook::GetHook()->m_mapWndInfo.erase(iterItem->first);
				break;
			}
			else
				if(m_ViewingWndsDlg.IsWindow())
					m_ViewingWndsDlg.InsertItem(iterItem->first);
		break;
	case tCheckForUpdateStartProg:
		KillTimer(tCheckForUpdateStartProg);
		PostMessage(UDM_CHECKFORUPDATE,(WPARAM)m_hWnd,(LPARAM)UDM_CHECKFORUPDATENOTIFY);
		break;
	case tCheckForUpdateAt24:
		PostMessage(UDM_CHECKFORUPDATE,(WPARAM)m_hWnd,(LPARAM)UDM_CHECKFORUPDATENOTIFY);
		break;
	}		
	return 1;
}


LRESULT CWorkWnd::OnDestroy(WPARAM,LPARAM)
{
	KillTimer(tCheckWnds);
	::PostQuitMessage(0);
	return 0;
}

LRESULT CWorkWnd::OnNIMessage(WPARAM,LPARAM lParam)
{
	SetForegroundWindow();
	if((lParam==WM_LBUTTONDOWN)||(lParam==WM_RBUTTONDOWN))
	{
		POINT pt={0};
		GetCursorPos(&pt);
		m_Menu.TrackPopupMenu(TPM_RIGHTBUTTON,pt,*this);
		PostMessage(WM_NULL);
	}
	if(lParam==NIN_BALLOONUSERCLICK)
		::ShellExecute(NULL, _T("open"), CULStrTable(IDS_DOWNLOADPAGE), NULL, NULL, SW_SHOWNORMAL);
	return 1;
}

LRESULT CWorkWnd::OnOSDMessage(WPARAM wParam,LPARAM lParam)
{
	switch(lParam)
	{
	case hkoTopMost:
//		if(CHook::GetHook()->m_mapWndInfo[(HWND)wParam].fTopMost)
		if(::GetWindowLong((HWND)wParam,GWL_EXSTYLE)&WS_EX_TOPMOST)
			m_osdWnd.ShowText(_T("TopMost On"));
		else
			m_osdWnd.ShowText(_T("TopMost Off"));
		break;
	case hkoToggleCaption:
		if(::GetWindowLong((HWND)wParam,GWL_STYLE)&WS_CAPTION)
			m_osdWnd.ShowText(_T("Caption On"));
		else
			m_osdWnd.ShowText(_T("Caption Off"));
		break;
	case hkoTransp:
		{
			std::map<HWND,CHook::WNDINFO>::const_iterator iterItem=CHook::GetHook()->m_mapWndInfo.find((HWND)wParam);
			if((iterItem==CHook::GetHook()->m_mapWndInfo.end())||(!iterItem._Mynode()->_Myval.second.fAlpha))
			{
				m_osdWnd.ShowText(_T("Disable"));
				break;
			}
			std::basic_stringstream<TCHAR> ss;
			ss<<int(100*double(255-CHook::GetHook()->CHook::GetHook()->m_mapWndInfo[(HWND)wParam].bAlpha)/255);
			ss<<_T("%");
			m_osdWnd.ShowText(ss.str().c_str());
		}
		break;
	}
	return 1;
}

LRESULT CWorkWnd::OnCheckForUpdate(WPARAM hWnd,LPARAM uMsg)
{
	m_Updater.SetNotifyWnd((HWND)hWnd);
	m_Updater.SetNotifyMsg((UINT)uMsg);
	m_Updater.Create();
	return NULL;
}

LRESULT CWorkWnd::OnCheckForUpdateNotify(WPARAM idEvent,LPARAM idEventParam)
{
	switch(idEvent)
	{
		case CUpdater::unNewVersionAvail:
			if(idEventParam==TRUE)
				m_TrayIcon.ShowBalloon(1,CULStrTable(IDS_DOWNLOAD_NOTIFY_CAPTION),
					CULStrTable(IDS_DOWNLOAD_NOTIFY),NIIF_INFO);		
			break;
	}
	return 1;
}

LRESULT CWorkWnd::OnTaskBarCreated(WPARAM,LPARAM)
{
	m_TrayIcon.Create(*this,NIM_MESSAGE);
	m_idTrayIcon=m_TrayIcon.AddIcon(m_hTrayIcon);
	m_TrayIcon.SetIconTip(m_idTrayIcon,CULStrTable(IDS_APP_NAME));
	return 1;
}

void CWorkWnd::OnEnable(WORD,HWND)
{
	CHook::GetHook()->Enable();
	m_Menu.EnableMenuItem(IDM_ENABLE,MFS_GRAYED|MFS_DISABLED);
	m_Menu.EnableMenuItem(IDM_DISABLE,MFS_ENABLED);
}

void CWorkWnd::OnDisable(WORD,HWND)
{
	CHook::GetHook()->Disable();
	m_Menu.EnableMenuItem(IDM_DISABLE,MFS_GRAYED|MFS_DISABLED);
	m_Menu.EnableMenuItem(IDM_ENABLE,MFS_ENABLED);
}

void CWorkWnd::OnRestore(WORD,HWND)
{
	CHook::GetHook()->Restore();
}

void CWorkWnd::OnViewingWnds(WORD,HWND)
{
	if(m_ViewingWndsDlg.IsWindow())
	{
		m_ViewingWndsDlg.SetForegroundWindow();
		m_ViewingWndsDlg.SetActiveWindow();
		return;
	}
	m_ViewingWndsDlg.Create(IDD_VIEWING_WINDOWS,NULL);
	m_ViewingWndsDlg.ShowWindow(SW_SHOW);
}

void CWorkWnd::OnOptions(WORD,HWND)
{
	if(m_propsheetOptions.IsWindow())
	{
		m_propsheetOptions.SetForegroundWindow();
		m_propsheetOptions.SetActiveWindow();
		return;
	}
	CPropHotKeys propHotKeys;
	for(int i=0;i<hkoCount;++i)
		propHotKeys.m_arHotkey[i]=CHook::GetHook()->m_arHotKeyInfo[i];
	m_propsheetOptions.AddPage(propHotKeys.Create(IDD_PROPPAGE_HOTKEYS));	

	CPropTransparent propTransparent;
	propTransparent.m_nMinTransparentLevel=int(100*double(255-CHook::GetHook()->m_bMinTranspVal)/255);
	propTransparent.m_nTransparentLevelStep=int(100*double(CHook::GetHook()->m_bTranspStep)/255);
	m_propsheetOptions.AddPage(propTransparent.Create(IDD_PROPPAGE_TRANSPARENT));		

	CPropOSD propOSD;
	propOSD.m_osdPos=(CPropOSD::enOSDPos)m_osdWnd.GetPos();
	propOSD.m_clrText=m_osdWnd.GetTextColor();
	propOSD.m_clrTextShadow=m_osdWnd.GetTextShadowColor();
	m_osdWnd.GetFont(&propOSD.m_LogFont);
	m_propsheetOptions.AddPage(propOSD.Create(IDD_PROPPAGE_OSD));		

	CPropSystem propSystem;
	propSystem.m_fAutoRun=m_ProfileReg.IsAutoRun(CULStrTable(IDS_APP_NAME));
	propSystem.m_UpdateType=(CPropSystem::enUpdateType)m_Updater.m_UpdateType;
	m_propsheetOptions.AddPage(propSystem.Create(IDD_PROPPAGE_SYSTEM));	

	m_propsheetOptions.m_psh.dwFlags=PSH_NOAPPLYNOW|PSH_NOCONTEXTHELP;
	if(m_propsheetOptions.Create(*this,CULStrTable(IDS_OPTIONSDLG_CAPTION),0)==IDOK)
	{
		for(int i=0;i<hkoCount;++i)
			CHook::GetHook()->m_arHotKeyInfo[i]=propHotKeys.m_arHotkey[i];

		CHook::GetHook()->m_bMinTranspVal=255-int(255*double(propTransparent.m_nMinTransparentLevel)/100);
		CHook::GetHook()->m_bTranspStep=int(255*double(propTransparent.m_nTransparentLevelStep)/100);
		m_osdWnd.SetPos((COSDWnd::enOSDPos)propOSD.m_osdPos);
		m_osdWnd.SetTextColor(propOSD.m_clrText);
		m_osdWnd.SetTextShadowColor(propOSD.m_clrTextShadow);
		m_osdWnd.SetFont(propOSD.m_LogFont);
		m_Updater.m_UpdateType=(CUpdater::enUpdateType)propSystem.m_UpdateType;
		SaveSettings();
		if(propSystem.m_fAutoRun)
		{
			TCHAR szFileName[0xffff];
			::GetModuleFileName(NULL,szFileName,0xffff);
			m_ProfileReg.AddToAutoRun(CULStrTable(IDS_APP_NAME),szFileName);
		}
		else
			m_ProfileReg.AddToAutoRun(CULStrTable(IDS_APP_NAME),NULL);
		InitUpdateTimers();
	}
}

void CWorkWnd::OnAbout(WORD,HWND)
{
	CAboutDlg dlg;
	dlg.CreateModal(IDD_ABOUTBOX,*this);
}

void CWorkWnd::OnQuit(WORD,HWND)
{
	CHook::GetHook()->Restore();
#ifdef TOUCHPAD
	ASSERT(UnSetTranspWndsHookForTouchpad());
#endif
	DestroyWindow();
}

LRESULT CWorkWnd::OnTouchPadMousewheel(WPARAM wParam,LPARAM lParam)
{
	POINT pt = { LOWORD(lParam), HIWORD(lParam) };
	CHook::GetHook()->ProcessTransp( WM_MOUSEWHEEL, pt, GET_WHEEL_DELTA_WPARAM(wParam) );
	return 0;
}
