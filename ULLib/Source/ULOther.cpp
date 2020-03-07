///\file ULOther.cpp
///\brief cpp файл пространства имён просто для полезных функций(25.09.2007)

#include "..\..\ULLib\Include\ULOther.h"

BOOL ULOther::GetProcAddresses( HINSTANCE* hLibrary, LPCTSTR lpszLibrary, INT nCount, ... )
{
	va_list va;
	va_start( va, nCount );
	BOOL fRetFunc=TRUE;
	try
	{
		if ( ( *hLibrary = LoadLibrary(lpszLibrary) ) != NULL )
		{
			FARPROC * lpfProcFunction = NULL;
			LPSTR lpszFuncName = NULL;
			INT nIdxCount = 0;
			while ( nIdxCount < nCount )
			{
				lpfProcFunction = va_arg( va, FARPROC* );
				lpszFuncName = va_arg( va, LPSTR );
				if ( ( *lpfProcFunction = 
					GetProcAddress( *hLibrary, 
						lpszFuncName ) ) == NULL )
				{
					lpfProcFunction = NULL;				
					throw FALSE;
				}
				nIdxCount++;
			}
		}
		else
			throw FALSE;
	}
	catch(BOOL fRet)
	{
		fRetFunc=fRet;
	}
	va_end( va );
	return fRetFunc;
}

const char* ULOther::Rus(const TCHAR* pszIn)
{
    static char szBuffer[MAX_PATH];
    int nLen = (int)_tcslen(pszIn);	
	if (nLen  >= MAX_PATH)
        nLen  = MAX_PATH;
    CharToOemBuff(pszIn, szBuffer, nLen);
    szBuffer[nLen] = 0;
    return szBuffer;
}

LPTSTR ULOther::ParseError(UINT nErr)
{
	static TCHAR szBuffer[MAX_PATH];
	FormatMessage(/*FORMAT_MESSAGE_ALLOCATE_BUFFER | */FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		nErr,
		MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT),
		(LPTSTR) &szBuffer,
		MAX_PATH,
		NULL);
	return szBuffer;
}

DWORD ULOther::GetModuleFolderPath(HMODULE hModule,LPTSTR lpszFolderPath,DWORD dwSize)
{
	DWORD dwLen=::GetModuleFileName(hModule,lpszFolderPath,dwSize);
	while((dwLen)&&(*(lpszFolderPath+(--dwLen))!=_T('\\')));
	*(lpszFolderPath+dwLen+1)=0;
	return dwLen;
};
