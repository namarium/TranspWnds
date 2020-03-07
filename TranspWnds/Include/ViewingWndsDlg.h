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
class CViewingWndsDlg :
	public CULDlg
{
public:
	enum enColumnIds
	{
		ciTransparent=1000,
		ciTransparentLevel,
		ciTopMost,
		ciHideCaption
	};
protected:
	ULListCtrlEx::CULListCtrlEx m_listctrlViewingWnds;
public:
	CViewingWndsDlg(void);
	~CViewingWndsDlg(void);
	void DeleteItem(HWND hwndItem);
	void InsertItem(HWND hwndItem);
protected:
	void InitList();
protected:
	LRESULT OnInitDialog(WPARAM,LPARAM);
	LRESULT OnDestroy(WPARAM,LPARAM);

	void OnTransparent(WORD,HWND);
	void OnTransparentLevelComboSelenOk(WORD,HWND);
	void OnTopMost(WORD,HWND);
	void OnHideCaption(WORD,HWND);

	void OnCancel(WORD,HWND);

};
