///\file ULDllApp.h
///\brief Заголовочный файл класса динамической библиотеки(03.01.2007)
#pragma once
#ifndef __ULDLLAPP__H_
#define __ULDLLAPP__H_
#include <windows.h>
#include "ULWnd.h"
namespace ULApps
{
	///\class CULDllApp
	///\brief Абстрактный класс динамической библиотеки(03.01.2007)
	class CULDllApp
	{
		///\brief для доступа из DllMain
		static CULDllApp* m_pULDllApp;
	public:
		///\brief Конструктор
		CULDllApp(void);
		///\brief Десдруктор
		virtual ~CULDllApp(void);
		///\brief HINSTANCE приложения, передаваемый в WinMain
		static HINSTANCE m_hInstance;
		///\brief Вызывается при загрузке длл процессом
		virtual BOOL OnProcessAttach(HINSTANCE hInstance);
		///\brief Вызывается при выгрузке длл процессом
		virtual BOOL OnProcessDetach();
		///\brief Вызывается при инициализации длл потоком
		virtual BOOL OnThreadAttach();
		///\brief Вызывается при выгрузке длл потоком
		virtual BOOL OnThreadDetach();
		///\brief Функций DllMain сделана френдом CULDllApp, чтоб 
		///при подключении заголовочного файла к разным 
		///модулям не было повторения определения
		BOOL DllMain(HINSTANCE /*hinstDLL*/,
			DWORD /*fdwReason*/);
		///\brief Служит для получения указателя на класс приложения
		///из любого модуля
		static CULDllApp* GetULDllApp(void);
	};
}
///\brief Объявление функции DllMain
#define UL_DECLARE_DLLMAIN() \
	BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID /*lpvReserved*/)\
		{return ULApps::CULDllApp::GetULDllApp()->DllMain(hinstDLL,fdwReason);}

	
#endif//__ULDLLAPP__H_
