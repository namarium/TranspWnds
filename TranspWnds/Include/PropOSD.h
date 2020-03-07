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
#include"ULLibNS.h"
class CPropOSD :
	public CULPropPage
{
public:
	enum enOSDPos
	{
		osdpTopLeft=0,
		osdpTopRight,
		osdpBotLeft,
		osdpBotRight,
		osdpCenter
	};
public:
	enOSDPos m_osdPos;
	LOGFONT m_LogFont;
	COLORREF m_clrText;
	COLORREF m_clrTextShadow;

	CULBrush m_brushTextColor;
	CULBrush m_brushTextShadowColor;
public:
	CPropOSD(void);
	~CPropOSD(void);
protected:
	virtual LRESULT OnInitDialog(WPARAM,LPARAM);
	virtual LRESULT OnCtlColorStatic(WPARAM,LPARAM);

	virtual LRESULT OnApply(BYTE nReturn);

	void OnBtnFont(WORD,HWND);
	void OnBtnTextColor(WORD,HWND);
	void OnBtnTextShadowColor(WORD,HWND);
};
