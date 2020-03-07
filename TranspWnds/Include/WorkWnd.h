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
#pragma once
#include "ULLibNS.h"
#include "ViewingWndsDlg.h"
#include "PropSheetOptions.h"
#include "OSDWnd.h"
#include "Updater.h"

class CWorkWnd :
	public CULWnd
{
public:
	enum 
	{
		NIM_MESSAGE=WM_APP+1,
		OSDM_MESSAGE,
		UDM_CHECKFORUPDATE,
		UDM_CHECKFORUPDATENOTIFY
	};
	COSDWnd m_osdWnd;
protected:
	enum enTimers
	{
		tCheckWnds=1,
		tCheckForUpdateStartProg,
		tCheckForUpdateAt24,		
	};
	enum enTimerVals
	{
		tvFotStart=5*60*1000,
		tvFotAt24=24*60*60*1000
	};
protected:
	CULMenu m_Menu;
	HICON m_hTrayIcon;
	UINT m_idTrayIcon;
	CULTrayIcon m_TrayIcon;

	CUpdater m_Updater;

	CULProfileReg m_ProfileReg;

	CViewingWndsDlg m_ViewingWndsDlg;
	CPropSheetOptions m_propsheetOptions;
public:
	CWorkWnd(void);
	~CWorkWnd(void);
	void InitUpdateTimers();
	void LoadSettings();
	void SaveSettings();
protected:
	LRESULT OnCreate(WPARAM,LPARAM);
	LRESULT OnTimer(WPARAM,LPARAM);
	LRESULT OnDestroy(WPARAM,LPARAM);
	LRESULT OnNIMessage(WPARAM,LPARAM);
	LRESULT OnOSDMessage(WPARAM,LPARAM);
	LRESULT OnCheckForUpdate(WPARAM,LPARAM);
	LRESULT OnCheckForUpdateNotify(WPARAM,LPARAM);
	LRESULT OnTaskBarCreated(WPARAM,LPARAM);

	void OnEnable(WORD,HWND);
	void OnDisable(WORD,HWND);
	void OnRestore(WORD,HWND);

	void OnViewingWnds(WORD,HWND);

	void OnOptions(WORD,HWND);
	
	void OnAbout(WORD,HWND);
	
	void OnQuit(WORD,HWND);

	LRESULT OnTouchPadMousewheel(WPARAM wParam,LPARAM lParam);
};
