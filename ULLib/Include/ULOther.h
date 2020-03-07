///\file ULOther.h
///\brief Заголовочный файл пространства имён просто для полезных функций(25.08.2007)
#pragma once
#ifndef __ULOTHER__H_
#define __ULOTHER__H_
#include <windows.h>
#include "ULProfileReg.h"
#include "ULPtr.inl"
#include "ULFileVersionInfo.h"
#include "ULWaitCursor.h"
#include "ULRes.h"

#include "ULTrayIcon.h"
///\namespace ULOther
///\brief Пространство имён просто для полезных функций(25.08.2007)
namespace ULOther
{
	///\brief Функция для подключения длл и импортирования функций(2006)
	///\param hLibrary -хендл подключенной библиотеки 
	///\param lpszLibrary-имя файла подключаемой библиотеки 
	///\param nCount-колличество импортируемых функций 
	///\param ... - указатель на прототип функции, имя функции
	///\return TRUE в случае успеха
	BOOL GetProcAddresses(HINSTANCE *hLibrary, LPCTSTR lpszLibrary, INT nCount, ... );
	///\brief Функция для перевода rus текста в консольный(30.07.2007)
	///\param pszIn - указатель на буфер с текстом
	///\return консольный текст
	const char* Rus(const TCHAR* pszIn);
	///\brief Функция возвращает описание ошибки(29.12.2007)
	///\param nErr - номер ошибки
	///\return описание последней ошибки
	LPTSTR ParseError(UINT nErr);
	///\brief Для получения директории, в которой расположен указанный модуль
	///\param hModule - хенжл модуля
	///\param lpszFolderPath - возвращаемая строка с именем директории
	///\param dwSize - размер буфера
	///\return длина строки пути к возвращаемой директории 
	DWORD GetModuleFolderPath(HMODULE hModule,LPTSTR lpszFolderPath,DWORD dwSize);	
};
#endif//__ULOTHER__H_
