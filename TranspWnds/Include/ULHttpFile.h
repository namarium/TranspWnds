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
#ifndef _ULHTTPFILE_H__
#define _ULHTTPFILE_H__
#include <windows.h>
#include <Wininet.h>

namespace ULInet
{
	class CULHttpFile
	{
	protected:
		HINTERNET m_hInternetSession;
		HINTERNET m_hFile;
		DWORD m_dwOpenFlags;
	public:
		CULHttpFile(void);
		~CULHttpFile(void);
		BOOL Open(LPCTSTR lpszUrl,DWORD dwFlags=INTERNET_FLAG_NO_CACHE_WRITE);
		BOOL Close();
		DWORD SetFilePointer(LONG lDistanceToMove,DWORD dwMoveMethod=FILE_BEGIN);
		BOOL ReadFile(LPVOID lpBuffer,DWORD dwNumberOfBytesToRead,LPDWORD lpdwNumberOfBytesRead);

	};
}
#endif //_ULHTTPFILE_H__
