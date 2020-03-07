///\file ULRegServ.cpp
///\brief cpp фаил класса регистрации COM - объекта
#include "..\\include\\ULRegServ.h"
#include <objbase.h>
#include <assert.h>
#include <tchar.h>
namespace ULCOMs
{
	CULRegServ::CULRegServ(void)
	{
	}

	CULRegServ::~CULRegServ(void)
	{
	}

	void CULRegServ::CLSIDtoTCHAR(const CLSID& clsid,
					 LPTSTR szCLSID,
					 int nLen)
	{
		assert(nLen >= CLSID_STRING_SIZE) ;
		//получаем CLSID
		LPOLESTR wszCLSID = NULL ;
		HRESULT hr = StringFromCLSID(clsid, &wszCLSID) ;
		assert(SUCCEEDED(hr)) ;	
	#ifndef UNICODE
		// конвертация из юникада в ANSII
		size_t nConverted;
		wcstombs_s(&nConverted,szCLSID,nLen,wszCLSID, nLen) ;
	#else
		wcscpy_s(szCLSID,nLen,wszCLSID);
	#endif
		// освобождение памяти
		CoTaskMemFree(wszCLSID) ;
	}

	BOOL CULRegServ::SetKeyAndValue(LPCTSTR szKey,
									LPCTSTR szSubkey,
									LPCTSTR szValue)
	{
		HKEY hKey;
		TCHAR szKeyBuf[1024] ;

		//копируем ключ в буфер
		_tcscpy_s(szKeyBuf,1024,szKey) ;

		//добавляем имя подключа в буфер
		if (szSubkey != NULL)
		{
			_tcscat_s(szKeyBuf,1024, _T("\\")) ;
			_tcscat_s(szKeyBuf,1024, szSubkey ) ;
		}

		//создаём и открываем подключ
		long lResult = RegCreateKeyEx(HKEY_CLASSES_ROOT ,
									  szKeyBuf, 
									  0, NULL, REG_OPTION_NON_VOLATILE,
									  KEY_ALL_ACCESS, NULL, 
									  &hKey, NULL) ;
		if (lResult != ERROR_SUCCESS)
			return FALSE ;

		//устанавливаем значение
		if (szValue != NULL)
			RegSetValueEx(hKey, NULL, 0, REG_SZ, 
						  (BYTE *)szValue, 
						  static_cast<DWORD>(_tcslen(szValue))*sizeof(szValue[0])) ;

		RegCloseKey(hKey) ;
		return TRUE ;
	}

	LONG CULRegServ::RecursiveDeleteKey(HKEY hKeyParent,
										LPCTSTR lpszKeyChild)  
	{
		//открываем дочерний ключ
		HKEY hKeyChild ;
		LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyChild, 0,
								 KEY_ALL_ACCESS, &hKeyChild) ;
		if (lRes != ERROR_SUCCESS)
		{
			return lRes ;
		}

		// перебираем все влоденый ключи этого ключа
		FILETIME time ;
		TCHAR szBuffer[256] ;
		DWORD dwSize = 256 ;
		while (RegEnumKeyEx(hKeyChild, 0, szBuffer, &dwSize, NULL,
							NULL, NULL, &time) == S_OK)
		{
			//удаляем вложенные ключи перебираемых ключей(рекурсия)
			lRes = RecursiveDeleteKey(hKeyChild, szBuffer) ;
			if (lRes != ERROR_SUCCESS)
			{
				//закрываем ключ перед выходом
				RegCloseKey(hKeyChild) ;
				return lRes;
			}
			dwSize = 256 ;
		}

		//Закрываем дочерний ключ
		RegCloseKey(hKeyChild) ;

		//удаляем этот дочерний ключ
		return RegDeleteKey(hKeyParent, lpszKeyChild) ;
	}

	HRESULT CULRegServ::RegisterServer(HMODULE hModule,
									   const CLSID& clsid,
									   LPCTSTR szFriendlyName,
									   LPCTSTR szVerIndProgID,
									   LPCTSTR szProgID)
	{
		//получаем местополложение сервера
		TCHAR szModule[512]={0};
		DWORD dwResult =::GetModuleFileName(hModule, 
			szModule,
			sizeof(szModule)/sizeof(szModule[0])) ;
		assert(dwResult!= 0);

		//конвертируем CLSID в строку
		TCHAR szCLSID[CLSID_STRING_SIZE] ;
		CLSIDtoTCHAR(clsid, szCLSID, sizeof(szCLSID)/sizeof(szCLSID[0])) ;

		//делаем ключ CLSID\\{...}
		TCHAR szKey[64] ;
		_tcscpy_s(szKey,64,_T("CLSID\\")) ;
		_tcscat_s(szKey,64,szCLSID) ;
	  
		//добавляем CLSID в реестр.
		SetKeyAndValue(szKey, NULL, szFriendlyName) ;

		// добавляем в CLSID подключ с именем фаила сервера.
		SetKeyAndValue(szKey, _T("InprocServer32"), szModule) ;

		// добавляем в CLSID подключ с ProgID
		SetKeyAndValue(szKey, _T("ProgID"), szProgID) ;
		
		// добавляем в CLSID подключ с независимый от версии ProgID
		SetKeyAndValue(szKey, _T("VersionIndependentProgID"),
					   szVerIndProgID) ;

		// добавляем в HKEY_CLASSES_ROOT подключ с независимый от версии ProgID
		SetKeyAndValue(szVerIndProgID, NULL, szFriendlyName) ; 
		SetKeyAndValue(szVerIndProgID, _T("CLSID"), szCLSID) ;
		SetKeyAndValue(szVerIndProgID, _T("CurVer"), szProgID) ;

		// добавляем в HKEY_CLASSES_ROOT подключ с ProgID
		SetKeyAndValue(szProgID, NULL, szFriendlyName) ; 
		SetKeyAndValue(szProgID, _T("CLSID"), szCLSID) ;

		return S_OK ;
	}

	LONG CULRegServ::UnregisterServer(const CLSID& clsid,         
									  LPCTSTR szVerIndProgID, 
									  LPCTSTR szProgID)       
	{
		// Конвертирум CLSID в строку.
		TCHAR szCLSID[CLSID_STRING_SIZE] ;
		CLSIDtoTCHAR(clsid, szCLSID, sizeof(szCLSID)/sizeof(szCLSID[0])) ;
		// формируем ключ CLSID\\{...}
		TCHAR szKey[64] ;
		_tcscpy_s(szKey,sizeof(szKey)/sizeof(szKey[0]),_T("CLSID\\")) ;
		_tcscat_s(szKey,sizeof(szKey)/sizeof(szKey[0]),szCLSID) ;

		// удаляем ключ CLSID - CLSID\{...}
		LONG lResult = RecursiveDeleteKey(HKEY_CLASSES_ROOT, szKey) ;
		assert((lResult == ERROR_SUCCESS) ||
			   (lResult == ERROR_FILE_NOT_FOUND)) ; // Подключа может не быть.

		// удаляем независимы от версии ProgID ключ.
		lResult = RecursiveDeleteKey(HKEY_CLASSES_ROOT, szVerIndProgID) ;
		assert((lResult == ERROR_SUCCESS) ||
			   (lResult == ERROR_FILE_NOT_FOUND)) ; // Подключа может не быть

		// удаляем ProgID ключ.
		lResult = RecursiveDeleteKey(HKEY_CLASSES_ROOT, szProgID) ;
		assert((lResult == ERROR_SUCCESS) ||
			   (lResult == ERROR_FILE_NOT_FOUND)) ; // Подключа может не быть

		return S_OK ;
	}
}
