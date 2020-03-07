///\file ULFileVersionInfo.h
///\brief Заголовочный файл класса версии фаила(29.12.2007)
#pragma once
#ifndef __ULFILE_VERSION_INFO__H_
#define __ULFILE_VERSION_INFO__H_
#include <windows.h>
#include <tchar.h>
#pragma comment(lib, "version.lib")

namespace ULOther
{
	///\class CULFileVersionInfo
	///\brief Класс версии фаила
	class CULFileVersionInfo
	{
	protected:
		///\brief Полное имя и путь к фаилу
		LPTSTR m_lpszFileName;
	public:
		///\brief Конструктор
		///\param lpszFileName Полное имя и путь к фаилу
		CULFileVersionInfo(LPTSTR lpszFileName);
		///\brief Конструктор
		CULFileVersionInfo();
		///\brief Деструктор
		~CULFileVersionInfo();
		///\brief Получает информацию о фаиле по заданному типу
		///\param szType - тип получаемой инф
		///\param szBuffer - буфер, в который будет скопирована полученная информация
		///\param nLength - длина буфера
		///\return буфер, в который будет скопирована полученная информация в случае успеха, иначе NULL
		LPTSTR GetVersionString(LPTSTR szType, LPTSTR szBuffer, ULONG nLength);
		///\brief Устанавливает полное имя и путь к фаилу
		///\param lpszFileName - полное имя и путь к фаилу
		///\return Предыдущее полное имя и путь к фаилу
		LPTSTR SetFileName(LPTSTR lpszFileName);
		///\brief Возвращает текущее полное имя и путь к фаилу
		///\param lpszFileName - буфер, в который будет скопированы имя и путь к фаилу
		///\param nLen - длина буфера
		///\return Текущее полное имя и путь к фаилу, иначе NULL
		LPTSTR GetFileName(LPTSTR lpszFileName,UINT nLen);
		///\brief Возвращает описание фаила
		///\param lpszDescription - буфер, в который будет скопировано описание фаила
		///\param nLen - длина буфера
		///\return Описание фаила, иначе NULL
		LPTSTR GetFileDescription(LPTSTR lpszDescription,UINT nLen);
		///\brief Возвращает версия фаила
		///\param lpszVersion - буфер, в который будет скопирована версия фаила
		///\param nLen - длина буфера
		///\return Версия фаила, иначе NULL
		LPTSTR GetFileVersion(LPTSTR lpszVersion,UINT nLen);
		///\brief Возвращает внутреннее имя фаила
		///\param lpszInternalName - буфер, в который будет скопировано внутреннее имя фаила
		///\param nLen - длина буфера
		///\return Внутреннее имя фаила, иначе NULL
		LPTSTR GetInternalName(LPTSTR lpszInternalName,UINT nLen);
		///\brief Возвращает название продукта
		///\param lpszProductName - буфер, в который будет скопировано название продукта
		///\param nLen - длина буфера
		///\return Название продукта, иначе NULL
		LPTSTR GetProductName(LPTSTR lpszProductName,UINT nLen);
		///\brief Возвращает версия продукта
		///\param lpszProductVersion - буфер, в который будет скопирована версия продукта
		///\param nLen - длина буфера
		///\return Версия продукта, иначе NULL
		LPTSTR GetProductVersion(LPTSTR lpszProductVersion,UINT nLen);
		///\brief Возвращает коментарии	
		///\param lpszComments - буфер, в который будет скопированы коментарии
		///\param nLen - длина буфера
		///\return Коментарии, иначе NULL
		LPTSTR GetComments(LPTSTR lpszComments,UINT nLen);
		///\brief Возвращает владельца авторских прав	
		///\param lpszLegalCopyright - буфер, в который будет скопирован владелец авторских прав	
		///\param nLen - длина буфера
		///\return Владелец авторских прав, иначе NULL
		LPTSTR GetLegalCopyright(LPTSTR lpszLegalCopyright,UINT nLen);
		///\brief Возвращает название компании производителя
		///\param lpszCompanyName - буфер, в который будет скопирован название компании производителя
		///\param nLen - длина буфера
		///\return название компании производителя, иначе NULL
		LPTSTR GetCompanyName(LPTSTR lpszCompanyName,UINT nLen);	
	};
};
#endif //__ULFILE_VERSION_INFO__H_
