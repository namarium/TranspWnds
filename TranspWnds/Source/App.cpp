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
#include "App.h"
#include <ULLibLink.h>
#include "WorkWnd.h"
CApp::CApp(void)
{
}

CApp::~CApp(void)
{
	
}

BOOL CApp::InitInstance()
{
	//синглтон
	if(CreateMutex(NULL,FALSE,_T("TranspWndSinletone")))
		if(GetLastError()==ERROR_ALREADY_EXISTS)
			return FALSE;

	SetThreadLocale(MAKELCID(0x040c, SORT_DEFAULT));


	m_pMainWnd=new CWorkWnd;
	return m_pMainWnd->Create(NULL,NULL,WS_POPUP,GetSystemMetrics(SM_CXFULLSCREEN)/2-100,
		GetSystemMetrics(SM_CYFULLSCREEN)/2-100,0,0,NULL,NULL);
}

CApp theApp;
