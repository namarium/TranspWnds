///\file ULProfileReg.cpp
///\brief cpp файл класса профиля приложения в реестре(17.08.2007)
#include "..\\..\\ULLib\\Include\\ULProfileReg.h"
#include <tchar.h>
namespace ULOther
{
	CULProfileReg::CULProfileReg():
		m_hAppKey(NULL),m_fAllUsers(FALSE)
	{
	}

	CULProfileReg::~CULProfileReg(void)
	{
		Close();
	}

	BOOL CULProfileReg::SetRegistryKey(LPCTSTR pcszCompanyName,LPCTSTR pcszAppName,BOOL fAllUsers)
	{
		if(pcszCompanyName==NULL)
			return FALSE;
		if(pcszAppName==NULL)
			return FALSE;
		BOOL fRet=TRUE;
		m_fAllUsers=fAllUsers;
		HKEY hSoftKey=NULL;
		HKEY hCompanyKey=NULL;
		if(RegOpenKeyEx(((fAllUsers)?HKEY_LOCAL_MACHINE:HKEY_CURRENT_USER), _T("Software"), 0, KEY_WRITE|KEY_READ,
			&hSoftKey)==ERROR_SUCCESS)
		{
			DWORD dw;
			if(RegCreateKeyEx(hSoftKey, pcszCompanyName, 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hCompanyKey, &dw) == ERROR_SUCCESS)
			{
				if(RegCreateKeyEx(hCompanyKey, pcszAppName, 0, REG_NONE,
					REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
					&m_hAppKey, &dw)!=ERROR_SUCCESS)
					fRet=FALSE;
			}
			else
				fRet=FALSE;
		}
		else
			fRet=FALSE;

		if (hSoftKey != NULL)
			RegCloseKey(hSoftKey);
		if (hCompanyKey != NULL)
			RegCloseKey(hCompanyKey);
		return TRUE;
	}

	HKEY CULProfileReg::GetSectionKey(LPCTSTR pcszSection)
	{
		if(pcszSection==NULL)
			return NULL;
		HKEY hSectionKey = NULL;
		DWORD dw;
		RegCreateKeyEx(m_hAppKey, pcszSection, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hSectionKey, &dw);
		return hSectionKey; 
	}

	BOOL CULProfileReg::WriteProfileString(LPCTSTR pcszSection,
		LPCTSTR pcszEntry,LPCTSTR pcszValue)
	{
		if(m_hAppKey==NULL)
			return FALSE;
		LONG lResult; 
		if(pcszEntry == NULL)
			lResult = ::RegDeleteKey(m_hAppKey, pcszSection);
		else
			if(pcszValue == NULL)
			{
				HKEY hSecKey=GetSectionKey(pcszSection);
				if (hSecKey==NULL)
					return FALSE;
				lResult = ::RegDeleteValue(hSecKey,(LPTSTR)pcszEntry);
				RegCloseKey(hSecKey);
			}
			else
			{
				HKEY hSecKey = GetSectionKey(pcszSection);
				if (hSecKey == NULL)
					return FALSE;
				lResult = RegSetValueEx(hSecKey, pcszEntry, NULL, REG_SZ,
					(LPBYTE)pcszValue, ((DWORD)_tcslen(pcszValue)+1)*sizeof(TCHAR));
				RegCloseKey(hSecKey);
			}
		return (lResult == ERROR_SUCCESS);
	}

	BOOL CULProfileReg::WriteProfileInt(LPCTSTR pcszSection, LPCTSTR pcszEntry,int nValue)
	{
		if(pcszEntry==NULL)
			return FALSE;
		HKEY hSecKey = GetSectionKey(pcszSection);
		if (hSecKey == NULL)
			return FALSE;
		LONG lResult = RegSetValueEx(hSecKey, pcszEntry, NULL, REG_DWORD,
			(LPBYTE)&nValue, sizeof(nValue));
		RegCloseKey(hSecKey);
		return (lResult == ERROR_SUCCESS);
	}

	BOOL CULProfileReg::WriteProfileBinary(LPCTSTR pcszSection,
		LPCTSTR pcszEntry,void* pValue,DWORD dwSizeVal)
	{
		if(m_hAppKey==NULL)
			return FALSE;
		LONG lResult; 
		if(pcszEntry==NULL)
			lResult = ::RegDeleteKey(m_hAppKey, pcszSection);
		else
			if(pValue==NULL)
			{
				HKEY hSecKey=GetSectionKey(pcszSection);
				if (hSecKey==NULL)
					return FALSE;
				lResult = ::RegDeleteValue(hSecKey,(LPTSTR)pcszEntry);
				RegCloseKey(hSecKey);
			}
			else
			{
				HKEY hSecKey=GetSectionKey(pcszSection);
				if (hSecKey==NULL)
					return FALSE;
				lResult=RegSetValueEx(hSecKey, pcszEntry, NULL, REG_BINARY,
					(LPBYTE)pValue, dwSizeVal);
				RegCloseKey(hSecKey);
			}
		return (lResult==ERROR_SUCCESS);
	}

	BOOL CULProfileReg::GetProfileString(LPCTSTR pcszSection, LPCTSTR pcszEntry,
		LPTSTR pszValue,LPDWORD lpdwValLen)
	{
		if(pcszEntry==NULL)
			return FALSE;
		HKEY hSecKey = GetSectionKey(pcszSection);
		if (hSecKey == NULL)
			return FALSE;
		DWORD dwType, dwCount;
		LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)pcszEntry, NULL, &dwType,
			NULL, &dwCount);
		if(pszValue==NULL)
		{
			*lpdwValLen=dwCount;
			return FALSE;
		}
		if ((lResult==ERROR_SUCCESS)&&(dwType==REG_SZ)&&(dwCount<=*lpdwValLen))
		{ 
			lResult = RegQueryValueEx(hSecKey, (LPTSTR)pcszEntry, NULL, &dwType,
				(LPBYTE)pszValue,lpdwValLen);
			RegCloseKey(hSecKey);	
		}
		else
		{
			RegCloseKey(hSecKey);	
			return FALSE;
		}
		return (lResult == ERROR_SUCCESS);
	}



	BOOL CULProfileReg::GetProfileInt(LPCTSTR pcszSection, LPCTSTR pcszEntry,DWORD* pdwValue)
	{
		if(pcszEntry==NULL)
			return FALSE;
		if(pdwValue==NULL)
			return FALSE;
		HKEY hSecKey = GetSectionKey(pcszSection);
		if (hSecKey == NULL)
			return FALSE;
		DWORD dwType;
		DWORD dwCount = sizeof(*pdwValue);
		LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)pcszEntry, NULL, &dwType,
			(LPBYTE)pdwValue, &dwCount);
		RegCloseKey(hSecKey);
		return (lResult == ERROR_SUCCESS);
	}

	BOOL CULProfileReg::GetProfileBinary(LPCTSTR pcszSection, LPCTSTR pcszEntry,
			void* pValue,DWORD* lpdwValLen)
	{
		if(pcszEntry==NULL)
			return FALSE;
		HKEY hSecKey = GetSectionKey(pcszSection);
		if (hSecKey == NULL)
			return FALSE;
		DWORD dwType, dwCount;
		LONG lResult=RegQueryValueEx(hSecKey, (LPTSTR)pcszEntry, NULL, &dwType,
			NULL, &dwCount);
		if(pValue==NULL)
		{
			*lpdwValLen=dwCount;
			return FALSE;
		}
		if ((lResult==ERROR_SUCCESS)&&(dwType==REG_BINARY)&&(dwCount<=*lpdwValLen))
		{ 
			lResult = RegQueryValueEx(hSecKey, (LPTSTR)pcszEntry, NULL, &dwType,
				(LPBYTE)pValue,lpdwValLen);
			RegCloseKey(hSecKey);	
		}
		else
		{
			RegCloseKey(hSecKey);	
			return FALSE;
		}
		return (lResult == ERROR_SUCCESS);
	}

	BOOL CULProfileReg::AddToAutoRun(LPCTSTR pcszName,LPCTSTR pcszFilePath)
	{
		if(pcszName==NULL)
			return FALSE;
		HKEY hKey;         
		TCHAR szKeyName[]=_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
		LONG lResult=RegOpenKeyEx(((m_fAllUsers)?HKEY_LOCAL_MACHINE:HKEY_CURRENT_USER),
			szKeyName,0,KEY_WRITE,&hKey);
		if(pcszFilePath==NULL)
		{
			lResult = ::RegDeleteValue(hKey,(LPTSTR)pcszName);
		}
		else
			if(lResult==ERROR_SUCCESS)
				lResult=RegSetValueEx(hKey,pcszName,0,REG_SZ,(BYTE*)pcszFilePath,(WORD)_tcslen(pcszFilePath)*sizeof(pcszFilePath[0]));
		::RegCloseKey(hKey);	
		return (lResult==ERROR_SUCCESS);
	};
	void CULProfileReg::Close()
	{
		if (m_hAppKey != NULL)
		{
			RegCloseKey(m_hAppKey);
			m_hAppKey=NULL;
		}
	}
	BOOL CULProfileReg::IsAutoRun(LPCTSTR pcszName)
	{
		if(pcszName==NULL)
			return FALSE;
		HKEY hKey;         
		TCHAR szKeyName[]=_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
		LONG lResult=RegOpenKeyEx(((m_fAllUsers)?HKEY_LOCAL_MACHINE:HKEY_CURRENT_USER),
			szKeyName,0,KEY_QUERY_VALUE,&hKey);
		TCHAR szValNameRet[0xffff]={0};
		bool fFind=false;
		for(DWORD i=0;;++i)
		{
			DWORD dwSize=0xffff;
			//RegEnumValue сделан потому, что у меня отсутвует RegGetValue в Advapi32.dll. 
			//причины не знаю
			lResult=RegEnumValue(hKey,i,szValNameRet,&dwSize,0,0,0,0);
			if(lResult==ERROR_SUCCESS)
			{
				if(_tcscmp(pcszName,szValNameRet)==0)
				{
					fFind=true;
					break;
				}
			}
			else
				if(lResult==ERROR_NO_MORE_ITEMS)
					break;
		}
		::RegCloseKey(hKey);	
		return fFind;
	}
}

