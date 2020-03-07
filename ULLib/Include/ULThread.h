///\file ULThread.h
///\brief Заголовочный файл класса потока (03.10.2007)
#pragma once
#ifndef __ULTHREAD__H_
#define __ULTHREAD__H_
#include <windows.h>
namespace ULThreads
{
	///\class CULThread
	///\brief Класс потока(27.08.2007)
	class CULThread
	{
	private:
		///\brief Флаг, состояния потока
		///	если FALSE, то обработчики сообщений потока должны
		///	немедленно завершать свою работу
		volatile BOOL m_fRunThread;
	protected:
		///\brief Фукнция потока
		static unsigned WINAPI ThreadProc(LPVOID param);
		///\brief Рабочая функция
		///\return FALSE, если функция завершает работу потока
		virtual BOOL Run();
	public:
		///\brief хэндл потока
		HANDLE m_hThread;
		///\brief ID потока
		DWORD m_dwThreadId;
	public:
		///\brief Конструктор
		CULThread(void);
		///\brief Десдруктор
		virtual ~CULThread(void);
		///\brief Создаёт поток
		///\param lpThreadAttributes атрибуты доступа
		///\param uiStackSize размер стэка
		///\param dwCreationFlags флаг
		///\return TRUE в случае успеха
		BOOL Create(LPSECURITY_ATTRIBUTES lpThreadAttributes=NULL,
			unsigned int uiStackSize=0,
			DWORD dwCreationFlags=CREATE_SUSPENDED);
		///\brief Выполняет завершение потока
		///\param dwMilliseconds - время в миллисекундах ожидания завершения потока
		///\return TRUE, если поток завершился коректно
		virtual BOOL Quit(DWORD dwMilliseconds);
		///\brief Отправляет сообщение потоку
		///\param Msg Сообщение
		///\param wParam параметр 1
		///\param lParam параметр 2
		///\return TRUE в случае успеха
		inline BOOL PostThreadMessage(UINT Msg,WPARAM wParam=0,LPARAM lParam=0)
			{return ::PostThreadMessage(m_dwThreadId,Msg,wParam,lParam);};
		///\brief Приостанавливает поток
		///\return TRUE в случае успеха
		inline BOOL SuspendThread()
			{return (::SuspendThread(m_hThread)!=-1); };
		///\brief Приостанавливает поток
		///\return TRUE в случае успеха
		inline BOOL ResumeThread()
			{return (::ResumeThread(m_hThread)!=-1);};
		///\brief Устанавливает приоритет
		///\param nPiority Приоритет
		///\return TRUE в случае успеха
		inline BOOL SetThreadPriority(int nPiority)
			{return ::SetThreadPriority(m_hThread,nPiority);};
		///\brief Возвращает хендл потока
		inline operator HANDLE() const{return m_hThread;};
		inline BOOL IsRun()
			{return m_fRunThread;}
	};
}
#endif//__ULTHREAD__H_
