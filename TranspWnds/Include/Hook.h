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

#include <windows.h>
#include <map>

typedef struct tagHotKeyInfo
{
	bool IsHotKey(PMSLLHOOKSTRUCT lpMouseHookStruct);
	bool IsMsg(int nMsg,UINT uMsg);
	BOOL m_fAlt;
	BOOL m_fCtrl;
	BOOL m_fShift;
	BOOL m_fWin;
	UINT m_uMsg[3];
	tagHotKeyInfo& operator=(tagHotKeyInfo& HotKeyInfo);
}HOTKEYINFO,*LPHOTKEYINFO;

enum enHotKeyOperations
{
	hkoTransp,
	hkoTopMost,
	hkoMoveWnd,
	hkoSizeWnd,
	hkoToggleCaption,
	hkoThroughClick,
	hkoCollapseWnd,
	hkoCount
};

enum enMoveSizeMethod
{
	msmProgram,
	msmSystem
};

enum enSizeMethodBy
{
	smbCorner,
	msmBorder
};

class CHook
{
public:
	HHOOK m_hMouse;
public:
	///\brief структура, содержащая информацию об окне
	///\param bAlpha - альфаканал
	///\param fAlpha - TRUE,если окну был присвоен стиль WS_EX_LAYERED 
	///\param fTopMost - TRUE,если окну был присвоен стиль WS_EX_TOPMOST 
	///\param dwStyle - стиль окна,если у окна был убран заголовок, иначе 0
	///\param dwNonCollapseHeight - высота окна до свёртывания высоты
	typedef struct tagWndInfo
	{
		BYTE bAlpha;
		BOOL fAlpha;
		BOOL fTopMost;
		DWORD dwStyle;
		DWORD dwNonCollapseHeight;
	}WNDINFO,*LPWNDINFO;
	///\brief мап окно - информация о нём
	std::map<HWND,WNDINFO> m_mapWndInfo;

	HOTKEYINFO m_arHotKeyInfo[hkoCount];

	BYTE m_bMinTranspVal;
	BYTE m_bTranspStep;

	enMoveSizeMethod m_MoveMethod;
	enMoveSizeMethod m_SizeMethod;
	enSizeMethodBy m_SizeMethodBy;
public:
	CHook(void);
	~CHook(void);

	void Enable();
	void Disable();
	void Restore();

	static CHook* GetHook();
public:
	LRESULT ProcessTransp( UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct );
	LRESULT ProcessTransp( UINT uMsg, POINT ptMouseAction, DWORD dwMouseData );
	LRESULT ProcessTopMost(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	LRESULT ProcessMoveWnd(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	LRESULT ProcessSizeWnd(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	LRESULT ProcessToggleCaption(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	LRESULT ProcessThroughClick(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
	///\brief функция обработки свёртывания окна
	LRESULT ProcessCollapseWnd(UINT uMsg, PMSLLHOOKSTRUCT lpMouseHookStruct);
};
