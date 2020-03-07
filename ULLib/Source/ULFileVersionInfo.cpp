///\file ULFileVersionInfo.cpp
///\brief cpp файл класса версии фаила(29.12.2007)

#include "..\\include\\ULFileVersionInfo.h"

ULOther::CULFileVersionInfo::CULFileVersionInfo(LPTSTR lpszFileName):
	m_lpszFileName(lpszFileName)
{
}

ULOther::CULFileVersionInfo::CULFileVersionInfo():
	m_lpszFileName(NULL)
{
}

ULOther::CULFileVersionInfo::~CULFileVersionInfo()
{
}

LPTSTR ULOther::CULFileVersionInfo::SetFileName(LPTSTR lpszFileName)
{
	LPTSTR lpszOldName=m_lpszFileName;
	m_lpszFileName=lpszFileName;
	return lpszOldName;
}

LPTSTR ULOther::CULFileVersionInfo::GetFileName(LPTSTR lpszFileName,UINT nLen)
{
	size_t nFileNameLen=_tcslen(m_lpszFileName);
	if((UINT)nLen<nFileNameLen)
		return NULL;
	_tcscpy_s(lpszFileName,nLen,m_lpszFileName);
	return lpszFileName;
}

LPTSTR ULOther::CULFileVersionInfo::GetVersionString(LPTSTR szType, LPTSTR szBuffer, ULONG nLength)
{
	UINT nLen;
	PVOID pVer;	
	DWORD* pdwCodePage;
	TCHAR szFmt[0x40];
	PVOID ptr = 0;
	BOOL fRes = FALSE;	
	szBuffer[0] = '\0';
	nLen = GetFileVersionInfoSize(m_lpszFileName, 0);
	if(nLen == 0 || (pVer = malloc(nLen)) == 0)
		return NULL;
	if(GetFileVersionInfo(m_lpszFileName, 0, nLen, pVer))
	{
		if(VerQueryValue(pVer, _T("\\VarFileInfo\\Translation"), (LPVOID*)&pdwCodePage, &nLen))
		{
			wsprintf(szFmt, _T("\\StringFileInfo\\%04x%04x\\%s"), (DWORD)(*pdwCodePage) & 0xFFFF, 
					(DWORD)(*pdwCodePage) >> 16, szType);
			
			if(VerQueryValue(pVer, szFmt, &ptr, &nLen))
			{
				lstrcpyn(szBuffer, (TCHAR*)ptr, min(nLength, nLen));
				fRes = TRUE;
			}
		}
	}
	free(pVer);
	return fRes ? szBuffer : NULL;
}

LPTSTR ULOther::CULFileVersionInfo::GetFileDescription(LPTSTR lpszDescription,UINT nLen)
{
	return GetVersionString(_T("FileDescription"),lpszDescription,nLen);
}

LPTSTR ULOther::CULFileVersionInfo::GetFileVersion(LPTSTR lpszVersion,UINT nLen)
{
	return GetVersionString(_T("FileVersion"),lpszVersion,nLen);
}
LPTSTR ULOther::CULFileVersionInfo::GetInternalName(LPTSTR lpszInternalName,UINT nLen)
{
	return GetVersionString(_T("InternalName"),lpszInternalName,nLen);
}
LPTSTR ULOther::CULFileVersionInfo::GetProductName(LPTSTR lpszProductName,UINT nLen)
{
	return GetVersionString(_T("ProductName"),lpszProductName,nLen);
}
LPTSTR ULOther::CULFileVersionInfo::GetProductVersion(LPTSTR lpszProductVersion,UINT nLen)
{
	return GetVersionString(_T("ProductVersion"),lpszProductVersion,nLen);
}

LPTSTR ULOther::CULFileVersionInfo::GetComments(LPTSTR lpszComments,UINT nLen)
{
	return GetVersionString(_T("Comments"),lpszComments,nLen);
}

LPTSTR ULOther::CULFileVersionInfo::GetLegalCopyright(LPTSTR lpszLegalCopyright,UINT nLen)
{
	return GetVersionString(_T("LegalCopyright"),lpszLegalCopyright,nLen);
}

LPTSTR ULOther::CULFileVersionInfo::GetCompanyName(LPTSTR lpszCompanyName,UINT nLen)
{
	return GetVersionString(_T("CompanyName"),lpszCompanyName,nLen);
}
