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
#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0501
#endif
#include "Hook.h"
#include "ULLibNS.h"

class CPropHotKeys :
	public CULPropPage 
{
public:
	HOTKEYINFO m_arHotkey[hkoCount];
public:
	CPropHotKeys(void);
	~CPropHotKeys(void);
protected:
	virtual LRESULT OnInitDialog(WPARAM,LPARAM);
	virtual LRESULT OnCtlColor(WPARAM,LPARAM);

	virtual LRESULT OnApply(BYTE nReturn);
};
