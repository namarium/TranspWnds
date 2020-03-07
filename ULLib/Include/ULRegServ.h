///\file ULRegServ.h
///\brief Заголовочный фаил класса регистрации COM - объекта
#pragma once
#ifndef _ULREGSERV_H__
#define _ULREGSERV_H__
#include <windows.h>

namespace ULCOMs
{
	///\class CULRegServ
	///\brief Класс регистрации COM - объекта
	class CULRegServ
	{
	protected:
		///\brief размер CLSID 
		enum
		{
			CLSID_STRING_SIZE=39
		};
		///\brief Конвертирует CLSID в строковую переменную
		///\param clsid - идентификатор класса
		///\param szCLSID - строковая переменная
		///\param nLen - длина строковой переменной
		void CLSIDtoTCHAR(const CLSID& clsid,
			LPTSTR szCLSID,
			int nLen);
		///\brief Создаёт ключ и присвает ему значение
		///\param szKey - сходный ключ
		///\param szSubkey - создаваемый ключ
		///\param szValue - присваемое значение ключа
		///\return TRUE в случае успеха, иначе FALSE
		BOOL SetKeyAndValue(LPCTSTR szKey,
							LPCTSTR szSubkey,
							LPCTSTR szValue);
		///\brief Удаляет дочерний ключ и все его подключи
		///\param hKeyParent - родительский ключ
		///\param lpszKeyChild - удаляемый дочерний ключ 
		///\return Код возврата от функций реестра
		LONG RecursiveDeleteKey(HKEY hKeyParent,LPCTSTR lpszKeyChild); 
	public:
		///\brief Конструктор
		CULRegServ(void);
		///\brief Деструктор
		virtual ~CULRegServ(void);
		///\brief Функция предназначена для регистрации компонента. Вызывается в DllRegisterServer.
		///\param hModule - Хендл модуля
		///\param clsid - Идентификатор компонента
		///\param szFriendlyName - Дружественное имя компонента
		///\param szVerIndProgID - Не Зависящий от версии ProgID
		///\param szProgID - "Дружественный" идентификатор(грубо говоря название)
		///\return Код возврата функций реестра
		HRESULT RegisterServer(HMODULE hModule, 
							   const CLSID& clsid, 
							   LPCTSTR szFriendlyName,
							   LPCTSTR szVerIndProgID,
							   LPCTSTR szProgID);
		///\brief Функция предназначена для анрегистрации компонента. Вызывается в DllUnregisterServer.
		///\param clsid - Идентификатор компонента
		///\param szVerIndProgID - Не Зависящий от версии ProgID
		///\param szProgID - "Дружественный" идентификатор(грубо говоря название)
		///\return Код возврата функций реестра
		HRESULT UnregisterServer(const CLSID& clsid,
								 LPCTSTR szVerIndProgID,
								 LPCTSTR szProgID) ;
	};
}
#endif//_ULREGSERV_H__
