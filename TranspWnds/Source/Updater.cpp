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
#include "Updater.h"
#include "resource.h"
#include <string>
#include <sstream>

CUpdater::CUpdater(void):
	m_hwndNotify(NULL),
	m_uMsg(0)
{
}

CUpdater::~CUpdater(void)
{
}

BOOL CUpdater::GetLastVersion(int arVer[constSectionSize])
{
	::PostMessage(m_hwndNotify,m_uMsg,(WPARAM)unConnect,-1);
	ULInet::CULHttpFile file;
	if(!file.Open(CULStrTable(IDS_DOWNLOADPAGE)))
	{
		::PostMessage(m_hwndNotify,m_uMsg,(WPARAM)unConnect,FALSE);
		return FALSE;
	}
	char  szData[101];
	DWORD dwBytesRead;
	DWORD dwBytesCounter=0;
	bool fEnd=false;
	int offsetSearch=0;
	std::string strContent;
	::PostMessage(m_hwndNotify,m_uMsg,(WPARAM)unConnect,TRUE);
	::SendMessage(m_hwndNotify,m_uMsg,(WPARAM)unRecieveData,0);
	while(file.ReadFile(szData,sizeof(szData)-1,&dwBytesRead)&&!fEnd)
	{
		if(dwBytesRead==0)
			break;
		szData[dwBytesRead]=0;
		strContent+=szData;
		dwBytesCounter+=dwBytesRead;
		::SendMessage(m_hwndNotify,m_uMsg,(WPARAM)unRecieveData,(LPARAM)dwBytesCounter);
		std::string::size_type nPos=strContent.find("version",offsetSearch);
		if((nPos!=std::string::npos)&&((strContent.size()-nPos)>=sizeof("version xx.xx.xx")))
		{
			std::string::size_type nPosEnd=0;			
			if((nPosEnd=strContent.find('.',nPos))==std::string::npos)
				offsetSearch=nPos+sizeof("version xx.xx.xx");
			else
				if((nPosEnd=strContent.find('.',nPosEnd+1))==std::string::npos)
					offsetSearch=nPos+sizeof("version xx.xx.xx");
				else
				{
					std::string strVersion;
					strVersion.append(strContent,nPos,nPosEnd+4-nPos);
					if(
						(strVersion.size()>=(sizeof("version x.x.x")-1))
						&&
						(strVersion.size()<=(sizeof("version xx.xx.xx")-1))
						)
					{ 
						nPos=sizeof("version");
						bool fEndParseVersion=true;
						for(int i=0;i<constSectionSize;++i)
						{
							std::string strSec;
							nPosEnd=strVersion.find('.',nPos);
							if(nPosEnd)
							{
								strSec.append(strVersion,nPos,nPosEnd-nPos);
								std::stringstream ss;
								ss<<strSec;
								ss>>m_arLastVer[i];
								nPos=nPosEnd+1;
							}
							else
							{
								::PostMessage(m_hwndNotify,m_uMsg,(WPARAM)unParseData,(LPARAM)FALSE);
								fEndParseVersion=false;
								break;
							}
						}
						if(fEndParseVersion)
						{
							::PostMessage(m_hwndNotify,m_uMsg,(WPARAM)unParseData,(LPARAM)TRUE);
							::PostMessage(m_hwndNotify,m_uMsg,(WPARAM)unLastVersion,(LPARAM)m_arLastVer);
							fEnd=true;
						}
					}
				}
		}
	}

	if(arVer)
		for(int i=0;i<constSectionSize;++i)
			arVer[i]=m_arLastVer[i];
	return TRUE;
}

BOOL CUpdater::GetCurrentVersion(int arVer[constSectionSize])
{
	CULFileVersionInfo FileVersionInfo;
	TCHAR szFileName[0xffff];
	GetModuleFileName(ULOther::ULGetResourceHandle(),szFileName,sizeof(szFileName)/sizeof(szFileName[0]));
	FileVersionInfo.SetFileName(szFileName);
	TCHAR szVer[100];
	if(!FileVersionInfo.GetProductVersion(szVer,sizeof(szVer)/sizeof(szVer[0])))
		return FALSE;

	std::basic_string<TCHAR> strVersion;
	strVersion.assign(szVer);
	std::basic_string<TCHAR>::size_type nPos=0;
	bool fEndParseVersion=true;
	for(int i=0;i<constSectionSize;++i)
	{
		std::basic_string<TCHAR> strSec;
		std::basic_string<TCHAR>::size_type nPosEnd=strVersion.find('.',nPos);
		if(nPosEnd)
		{
			strSec.append(strVersion,nPos,nPosEnd-nPos);
			std::basic_stringstream<TCHAR> ss;
			ss<<strSec;
			ss>>m_arCurrentVer[i];
			nPos=nPosEnd+1;
		}
		else
		{
			fEndParseVersion=false;
			break;
		}
	}
	if(!fEndParseVersion)
		return FALSE;
	if(arVer)
		for(int i=0;i<constSectionSize;++i)
			arVer[i]=m_arCurrentVer[i];

	::PostMessage(m_hwndNotify,m_uMsg,(WPARAM)unCurrentVersion,(LPARAM)m_arCurrentVer);
	
	return TRUE;
}

BOOL CUpdater::CheckForUpdate(int arVer[constSectionSize])
{
	if(!GetCurrentVersion(NULL))
	{
		::PostMessage(m_hwndNotify,m_uMsg,(WPARAM)unError,(LPARAM)0);
		return FALSE;
	}
	if(!GetLastVersion(NULL))
	{
		::PostMessage(m_hwndNotify,m_uMsg,(WPARAM)unError,(LPARAM)0);
		return FALSE;
	}
	BOOL fUpdate=FALSE;
	for(int i=0;i<constSectionSize;++i)
	{
		if(m_arLastVer[i]>m_arCurrentVer[i])
		{
			fUpdate=TRUE;
			break;
		}
	}
	if(arVer)
		for(int i=0;i<constSectionSize;++i)
			arVer[i]=m_arLastVer[i];
	::PostMessage(m_hwndNotify,m_uMsg,(WPARAM)unNewVersionAvail,(LPARAM)fUpdate);
	return fUpdate;
}

BOOL CUpdater::SetNotifyWnd(HWND hwndNotify)
{
	if(!::IsWindow(hwndNotify))
		return FALSE;
	m_hwndNotify=hwndNotify;
	return TRUE;
}

BOOL CUpdater::SetNotifyMsg(UINT uMsg)
{
	if(uMsg<WM_USER)
		return FALSE;
	m_uMsg=uMsg;
	return TRUE;
}


BOOL CUpdater::Run()
{
	CheckForUpdate(NULL);
	return FALSE;
}
