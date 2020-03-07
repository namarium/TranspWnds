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
#include "ULHttpFile.h"
#include "ULLibNS.h"

class CUpdater:
	public CULThread
{
public:
	enum enUpdateNotify
	{
		unCurrentVersion,
		unConnect,
		unRecieveData,
		unParseData,
		unLastVersion,
		unNewVersionAvail,
		unError
	};

	enum{constSectionSize=3};
	enum enUpdateType
	{
		utNever,
		utAtStart,
		utEvery24,
		utAtStartEvery24
	};
	enUpdateType m_UpdateType;
	HWND m_hwndNotify;
	UINT m_uMsg;
protected:
	int m_arLastVer[constSectionSize];
	int m_arCurrentVer[constSectionSize];
public:
	CUpdater(void);
	~CUpdater(void);
	BOOL GetLastVersion(int arVer[constSectionSize]);
	BOOL GetCurrentVersion(int arVer[constSectionSize]);
	BOOL CheckForUpdate(int arVer[constSectionSize]);
	BOOL SetNotifyWnd(HWND hwndNotify);
	BOOL SetNotifyMsg(UINT uMsg);
protected:
	BOOL Run();

};
