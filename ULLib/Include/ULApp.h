///\file ULApp.h
///\brief Заголовочный файл класса приложения(01.05.2007)
#pragma once
#ifndef __ULAPP__H_
#define __ULAPP__H_
#include <windows.h>
#include "ULWnd.h"
namespace ULApps
{
	///\class CULApp
	///\brief Абстрактный класс приложения(01.05.2007)
	class CULApp
	{
		///\brief для доступа из WinMain
		static CULApp* m_pULApp;
	public:
		///\brief Конструктор
		CULApp(void);
		///\brief Десдруктор
		virtual ~CULApp(void);
		///\brief HINSTANCE приложения, передаваемый в WinMain
		HINSTANCE m_hInstance;
		///\brief Командная строка запуска, передаваемая в WinMain
		LPSTR m_lpCmdLine;
		///\brief Для GUI, передаваемая в WinMain
		int m_nCmdShow;
		///\brief Указатель на главное окно, для цикла сообщений
		ULWnds::CULWnd* m_pMainWnd;
		///\brief объект карты сообщений вызова PreTranslateMessage
		CULMessageMap<ULWnds::CULWnd> MessageMap;
		///\brief хендл хука WH_GETMESSAGE
		HHOOK m_hGetMessageHook;
		///\brief Функций WinMain сделана френдом CULApp, чтоб 
		///при подключении заголовочного файла к разным 
		///модулям не было повторения определения
		friend int APIENTRY WinMain(HINSTANCE hInstance,
						 HINSTANCE /*hPrevInstance*/,
						 LPSTR lpCmdLine,
						 int nCmdShow);
		///\brief Служит для инициализации приложения
		virtual BOOL InitInstance()=0;
		///\brief Функция вызываемая перед TranslateMessage
		///\param pMsg - структура сообщения
		///\return TRUE если сообщение должно быть отправлено обработчикам сообщений
		virtual BOOL Run(MSG* pMsg);
		///\brief Реализует цикл сообщений
		virtual void MessageLoop();
		///\brief Освобождает очередь сообщений
		void ProcessMessages();
		///brief функция перехвата очереди сообщений (для хука WH_GETMESSAGE)
		static LRESULT CALLBACK GetMsgProc(int /*code*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		///\brief Служит для получения указателя на класс приложения
		///из любого модуля
		static CULApp* GetULApp(void);
	};
}
#endif//__ULAPP__H_
