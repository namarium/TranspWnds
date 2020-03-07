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

class COSDWnd :
	public CULWnd
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
protected:
	enOSDPos m_osdPos;

	CULStr m_strText;
	RECT m_rcPadding;
	const int m_constIdTimer;
	UINT m_nCurTimout;
	BYTE m_Alpha;
	COLORREF m_clrText;
	COLORREF m_clrTextShadow;
	LOGFONT m_lf;
public:
	COSDWnd(void);
	~COSDWnd(void);
	BOOL Create(HWND hwndParent);
	void ShowText(LPCTSTR pszText);
	void SetPos(enOSDPos osdPos);
	COSDWnd::enOSDPos GetPos();
	void SetTextColor(COLORREF clrText);
	COLORREF GetTextColor();
	void SetTextShadowColor(COLORREF clrTextShadow);
	COLORREF GetTextShadowColor();
	void SetFont(LOGFONT& lf);
	BOOL GetFont(LOGFONT* plf);
protected:
	virtual LRESULT OnPaint(WPARAM,LPARAM);
	virtual LRESULT OnTimer(WPARAM,LPARAM);
};
