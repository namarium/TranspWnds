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
#include "ULHttpFile.h"
#include <ULStates.h>

#pragma comment(lib,"Wininet.lib")

namespace ULInet
{
	CULHttpFile::CULHttpFile(void):
		m_hInternetSession(NULL),
		m_hFile(NULL),
		m_dwOpenFlags(NULL)
	{
	}

	CULHttpFile::~CULHttpFile(void)
	{
		Close();
	}

	BOOL CULHttpFile::Open(LPCTSTR lpszUrl,DWORD dwFlags)
	{
		m_dwOpenFlags=dwFlags;
		m_hInternetSession=InternetOpen(_T("ULHttpFile"),INTERNET_OPEN_TYPE_PRECONFIG,NULL,0,0);
		if(!m_hInternetSession)
			return FALSE;
		m_hFile=InternetOpenUrl(m_hInternetSession,lpszUrl,NULL,0,dwFlags,NULL);
		if(!m_hFile)
		{
			InternetCloseHandle(m_hInternetSession);
			m_hInternetSession=NULL;
			return FALSE;
		}
		return TRUE;
	}
	BOOL CULHttpFile::Close()
	{
		if(!(m_hInternetSession&&m_hFile))
			return FALSE;
		if(!InternetCloseHandle(m_hInternetSession))
			return FALSE;
		if(!InternetCloseHandle(m_hFile))
			return FALSE;
		return TRUE;
	}
	DWORD CULHttpFile::SetFilePointer(LONG lDistanceToMove,DWORD dwMoveMethod)
	{
		ASSERT(!(m_dwOpenFlags&(INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_NO_CACHE_WRITE)));
		return InternetSetFilePointer(m_hFile,lDistanceToMove,NULL,dwMoveMethod,NULL);
	}
	BOOL CULHttpFile::ReadFile(LPVOID lpBuffer,DWORD dwNumberOfBytesToRead,LPDWORD lpdwNumberOfBytesRead)
	{
		if((!lpBuffer)||(!lpdwNumberOfBytesRead))
			return FALSE;
		return InternetReadFile(m_hFile,lpBuffer,dwNumberOfBytesToRead,lpdwNumberOfBytesRead);
	}
}
