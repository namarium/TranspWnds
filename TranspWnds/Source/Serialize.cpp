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
#include "WorkWnd.h"
#include "Hook.h"

void CWorkWnd::LoadSettings()
{	

	DWORD dwVal=0;
	//загрузка горячих клавиш для тулзы прозрачности
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TranspAlt"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fAlt=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fAlt=TRUE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TranspCtrl"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fCtrl=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fCtrl=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TranspShift"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fShift=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fShift=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TranspWin"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fWin=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fWin=FALSE;
	CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_uMsg[0]=WM_MOUSEWHEEL;
	//загрузка горячих клавиш для тулзы топмост
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TopMostAlt"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fAlt=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fAlt=TRUE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TopMostCtrl"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fCtrl=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fCtrl=TRUE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TopMostShift"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fShift=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fShift=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TopMostWin"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fWin=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fWin=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("TopMostMsg"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_uMsg[0]=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_uMsg[0]=WM_LBUTTONDOWN;
	//загрузка горячих клавиш для тулзы move
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("MoveWndAlt"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_fAlt=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_fAlt=TRUE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("MoveWndCtrl"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_fCtrl=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_fCtrl=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("MoveWndShift"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_fShift=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_fShift=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("MoveWndWin"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_fWin=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_fWin=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("MoveWndMsg"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_uMsg[0]=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_uMsg[0]=WM_LBUTTONDOWN;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("MoveWndMsg1"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_uMsg[1]=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_uMsg[1]=WM_MOUSEMOVE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("MoveWndMsg2"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_uMsg[2]=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_uMsg[2]=WM_LBUTTONUP;
	//загрузка горячих клавиш для тулзы size
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("SizeWndAlt"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_fAlt=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_fAlt=TRUE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("SizeWndCtrl"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_fCtrl=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_fCtrl=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("SizeWndShift"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_fShift=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_fShift=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("SizeWndWin"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_fWin=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_fWin=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("SizeWndMsg"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_uMsg[0]=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_uMsg[0]=WM_RBUTTONDOWN;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("SizeWndMsg1"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_uMsg[1]=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_uMsg[1]=WM_MOUSEMOVE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("SizeWndMsg2"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_uMsg[2]=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_uMsg[2]=WM_RBUTTONUP;
	//загрузка горячих клавиш для тулзы ToggleCaption
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("ToggleCaptionAlt"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_fAlt=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_fAlt=TRUE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("ToggleCaptionCtrl"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_fCtrl=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_fCtrl=TRUE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("ToggleCaptionShift"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_fShift=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_fShift=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("ToggleCaptionWin"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_fWin=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_fWin=FALSE;
	if(m_ProfileReg.GetProfileInt(_T("HotKeys"),_T("ToggleCaptionMsg"),&dwVal))
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_uMsg[0]=(BOOL)dwVal;
	else
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_uMsg[0]=WM_RBUTTONDOWN;
	//загрузка параметра метода перемещения окна
	if(m_ProfileReg.GetProfileInt(_T("MoveSize"),_T("MoveMethod"),&dwVal))
		CHook::GetHook()->m_MoveMethod=(enMoveSizeMethod)dwVal;
	else
		CHook::GetHook()->m_MoveMethod=msmProgram;
	//загрузка параметра метода изменения размера окна
	if(m_ProfileReg.GetProfileInt(_T("MoveSize"),_T("SizeMethod"),&dwVal))
		CHook::GetHook()->m_SizeMethod=(enMoveSizeMethod)dwVal;
	else
		CHook::GetHook()->m_SizeMethod=msmProgram;
	//загрузка параметра метода изменения размера окна
	if(m_ProfileReg.GetProfileInt(_T("MoveSize"),_T("SizeMethodBy"),&dwVal))
		CHook::GetHook()->m_SizeMethodBy=(enSizeMethodBy)dwVal;
	else
		CHook::GetHook()->m_SizeMethodBy=smbCorner;	
	//загрузка параметров изменения прозрачности
	if(m_ProfileReg.GetProfileInt(_T("TranspVals"),_T("MinTranspVal"),&dwVal))
		CHook::GetHook()->m_bMinTranspVal=(BOOL)dwVal;
	else
		CHook::GetHook()->m_bMinTranspVal=100;
	if(m_ProfileReg.GetProfileInt(_T("TranspVals"),_T("TranspStep"),&dwVal))
		CHook::GetHook()->m_bTranspStep=(BYTE)dwVal;
	else
		CHook::GetHook()->m_bTranspStep=10;
	//если вдруг m_bTranspStep стала равна 0
	if(CHook::GetHook()->m_bTranspStep==0)
		CHook::GetHook()->m_bTranspStep=10;
	//загрузка параметров позиционирования OSD окна
	if(m_ProfileReg.GetProfileInt(_T("OSD"),_T("Align"),&dwVal))
		m_osdWnd.SetPos((COSDWnd::enOSDPos)dwVal);
	else
		m_osdWnd.SetPos(COSDWnd::osdpCenter);
	//загрузка цветов OSD окна
	if(m_ProfileReg.GetProfileInt(_T("OSD"),_T("TextColor"),&dwVal))
		m_osdWnd.SetTextColor((COLORREF)dwVal);
	else
		m_osdWnd.SetTextColor(0xff0000);
	if(m_ProfileReg.GetProfileInt(_T("OSD"),_T("TextShadowColor"),&dwVal))
		m_osdWnd.SetTextShadowColor((COLORREF)dwVal);
	else
		m_osdWnd.SetTextShadowColor(0xaaaaaa);	
	//загрузка шрифта OSD окна
	LOGFONT lf;
	DWORD dwSizeOfLf=sizeof(lf);
	if(m_ProfileReg.GetProfileBinary(_T("OSD"),_T("Font"),(void*)&lf,&dwSizeOfLf))
		m_osdWnd.SetFont(lf);
	//загрузка параметра обновления
	if(m_ProfileReg.GetProfileInt(_T("Update"),_T("UpdateType"),&dwVal))
		m_Updater.m_UpdateType=(CUpdater::enUpdateType)dwVal;
	else
		m_Updater.m_UpdateType=CUpdater::utAtStartEvery24;
}

void CWorkWnd::SaveSettings()
{
	//сохранение горячих клавишь для тулзы прозрачности
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TranspAlt"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fAlt);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TranspCtrl"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fCtrl);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TranspShift"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fShift);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TranspWin"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTransp].m_fWin);
	//сохранение горячих клавишь для тулзы топмост
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TopMostAlt"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fAlt);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TopMostCtrl"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fCtrl);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TopMostShift"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fShift);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TopMostWin"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_fWin);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("TopMostMsg"),
		CHook::GetHook()->m_arHotKeyInfo[hkoTopMost].m_uMsg[0]);
	//сохранение горячих клавишь для тулзы перемещения окна
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("MoveWndAlt"),
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_fAlt);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("MoveWndCtrl"),
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_fCtrl);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("MoveWndShift"),
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_fShift);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("MoveWndWin"),
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_fWin);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("MoveWndMsg"),
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_uMsg[0]);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("MoveWndMsg1"),
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_uMsg[1]);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("MoveWndMsg2"),
		CHook::GetHook()->m_arHotKeyInfo[hkoMoveWnd].m_uMsg[2]);
	//сохранение горячих клавишь для тулзы ресайза окна
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("SizeWndAlt"),
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_fAlt);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("SizeWndCtrl"),
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_fCtrl);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("SizeWndShift"),
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_fShift);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("SizeWndWin"),
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_fWin);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("SizeWndMsg"),
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_uMsg[0]);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("SizeWndMsg1"),
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_uMsg[1]);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("SizeWndMsg2"),
		CHook::GetHook()->m_arHotKeyInfo[hkoSizeWnd].m_uMsg[2]);
	//сохранение горячих клавишь для тулзы ToggleCaption
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("ToggleCaptionAlt"),
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_fAlt);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("ToggleCaptionCtrl"),
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_fCtrl);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("ToggleCaptionShift"),
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_fShift);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("ToggleCaptionWin"),
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_fWin);
	m_ProfileReg.WriteProfileInt(_T("HotKeys"),_T("ToggleCaptionMsg"),
		CHook::GetHook()->m_arHotKeyInfo[hkoToggleCaption].m_uMsg[0]);

	//сохранение параметра метода перемещения окна
	m_ProfileReg.WriteProfileInt(_T("MoveSize"),_T("MoveMethod"),
		CHook::GetHook()->m_MoveMethod);

	//сохранение параметра метода изменения размера окна
	m_ProfileReg.WriteProfileInt(_T("MoveSize"),_T("SizeMethod"),
		CHook::GetHook()->m_SizeMethod);

	//сохранение параметра метода изменения размера окна
	m_ProfileReg.WriteProfileInt(_T("MoveSize"),_T("SizeMethodBy"),
		CHook::GetHook()->m_SizeMethodBy);

	m_ProfileReg.WriteProfileInt(_T("TranspVals"),_T("MinTranspVal"),
		CHook::GetHook()->m_bMinTranspVal);
	m_ProfileReg.WriteProfileInt(_T("TranspVals"),_T("TranspStep"),
		CHook::GetHook()->m_bTranspStep);
	//сохранение параметров позиционирования OSD окна
	m_ProfileReg.WriteProfileInt(_T("OSD"),_T("Align"),(DWORD)m_osdWnd.GetPos());
	//сохранение цветов OSD окна
	m_ProfileReg.WriteProfileInt(_T("OSD"),_T("TextColor"),(DWORD)m_osdWnd.GetTextColor());
	m_ProfileReg.WriteProfileInt(_T("OSD"),_T("TextShadowColor"),(DWORD)m_osdWnd.GetTextShadowColor());
	//сохранение шрифта OSD окна
	LOGFONT lf;
	m_osdWnd.GetFont(&lf);
	DWORD dwSizeOfLf=sizeof(lf);
	m_ProfileReg.WriteProfileBinary(_T("OSD"),_T("Font"),(void*)&lf,dwSizeOfLf);
	//сохранение параметра обновления
	m_ProfileReg.WriteProfileInt(_T("Update"),_T("UpdateType"),(DWORD)m_Updater.m_UpdateType);
}
