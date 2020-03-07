///\file ULThreads.h
///\brief Заголовочный файл пространства имён ULThreads и содержащй класс критическрй секции(27.08.2007)
#pragma once
#ifndef __ULTHREADS__H_
#define __ULTHREADS__H_

#include "ULQueueThread.h"
///\namespace ULThreads
///\brief Пространство имён классов связанных с потоками и 	их синхронизацией(27.08.2007)
namespace ULThreads
{
	///\class CULCriticalSection
	///\brief Класс критической секции(27.08.2007)
	class CULCriticalSection
	{
	private:
		///\brief ID критической секции
		CRITICAL_SECTION m_CS;
	public:
		///\brief Конструктор
		CULCriticalSection()
		{
			::InitializeCriticalSection(&m_CS);
		}
		///\brief Деструктор
		virtual ~CULCriticalSection()
		{
			::DeleteCriticalSection(&m_CS);
		}
		///\brief Вход в критичемкую секцию
		inline void Enter()
		{
			::EnterCriticalSection(&m_CS);
		}
		///\brief Выход из критической секции
		inline void Leave()
		{
			::LeaveCriticalSection(&m_CS);
		}
		///\brief Ожидает освобождение критической секции
		///\param dwSeconds - время в секундах
		///\return TRUE, если выход произошел 
		///	до истечения заданного времени
#if(_WIN32_WINNT >= 0x0400)
		inline BOOL WaitEnter(DWORD dwSeconds)
		{
			while(!TryEnter()&&(dwSeconds--))
				Sleep(1000);
			return (dwSeconds!=0);
		}
#endif
		///\brief Проверка критической секции
		///\return FALSE, если критическая секция свободна
		///	TRUE, если если осуществлен вход в критическую секцию
#if(_WIN32_WINNT >= 0x0400)
		inline BOOL TryEnter()
		{
			return ::TryEnterCriticalSection(&m_CS);
		}
#endif
	};
	///\class CULEvent
	///\brief Класс события(11.10.2007)
	class CULEvent
	{
	private:
		///\brief Флаг уже говорящий о том что событие создано
		BOOL m_fExists;
	protected:
		///\brief Хэндл события
		HANDLE m_hEvent;
	public:
		///\brief Конструктор
		CULEvent():m_hEvent(NULL),m_fExists(FALSE){}
		///\brief Конструктор копирования
		CULEvent(HANDLE hEvent):m_hEvent(hEvent),m_fExists(FALSE){}
		///\brief Деструктор
		inline virtual ~CULEvent(){}
		///\brief Для подстановки объекта класса там где требуется 
		///только HANDLE
		inline operator HANDLE() const{return m_hEvent;}
		///\brief Создаёт событие
		///\param fManualReset - если TRUE, то сбрасывать придёться при помощи ResetEvent
		///\param fInitialState - начально состояние события при создании 
		///\param lpName - имя для именных, межпроцессовых событий
		///\param lpEventAttributes - кароче в NULL
		///\return TRUE в случае успеха,иначе FALSE 
		inline BOOL Create(LPCTSTR lpName=NULL,
			BOOL fManualReset=FALSE,
			BOOL fInitialState=FALSE,
			LPSECURITY_ATTRIBUTES lpEventAttributes=NULL)
		{
			m_hEvent=::CreateEvent(lpEventAttributes,fManualReset,fInitialState,lpName);
			m_fExists=(::GetLastError()==ERROR_ALREADY_EXISTS);
			return(m_hEvent!=NULL);
		}
		///\brief Говорит о том что событие уже создано(после вызова CreateEvent)
		///\return TRUE в случае созданного события
		inline BOOL IsAlraedyExists()
			{return m_fExists;};
		///\brief Открывает ранее созданное событие
		///\param lpName - Имя события
		///\param dwDesiredAccess - Права доступа
		///\param bInheritHandle - флаг начледования события дочерним процессом
		///\return TRUE в случае успеха, иначе FALSE
		BOOL Open(LPCTSTR lpName,DWORD dwDesiredAccess=EVENT_ALL_ACCESS,BOOL bInheritHandle=FALSE)
			{return ((m_hEvent=::OpenEvent(dwDesiredAccess,bInheritHandle,lpName))!=NULL);};
		///\brief Функция устанавливает событие
		///\return TRUE в случае успеха,иначе FALSE 
		inline BOOL Set()
			{return ::SetEvent(m_hEvent);}
		///\brief Функция сбрасывает событие
		///\return TRUE в случае успеха,иначе FALSE 
		inline BOOL Reset()
			{return ::ResetEvent(m_hEvent);}
		///\brief Проверяет состояние события
		///\return TRUE если установлено, иначе FALSE
		inline BOOL CheckEvent()
			{return Wait(0);}
		///\brief Ожидает переход события в установленное состояние
		///\param dwMillisecond - время ожидания в мс
		///\return TRUE если за установленное время событие установилось, иначе FALSE
		inline BOOL Wait(DWORD dwMillisecond=INFINITE)
			{return (::WaitForSingleObject(m_hEvent,dwMillisecond)==WAIT_OBJECT_0);}
		///\brief Закрывает событие
		///\return TRUE в случае успеха,иначе FALSE 
		inline BOOL Close()
			{return ::CloseHandle(m_hEvent);}
	};
	///\class CULMutex
	///\brief Класс события(11.10.2007)
	class CULMutex
	{
	private:
		///\brief Флаг уже говорящий о том что мютекс создан
		BOOL m_fExists;
	protected:
		///\brief Хэндл мютекса
		HANDLE m_hMutex;
	public:
		///\brief Конструктор
		CULMutex():m_hMutex(NULL){}
		///\brief Конструктор копирования
		CULMutex(HANDLE hMutex):m_hMutex(hMutex){}
		///\brief Деструктор
		inline virtual ~CULMutex(){}
		///\brief Для подстановки объекта класса там где требуется 
		///только HANDLE
		inline operator HANDLE() const{return m_hMutex;}
		///\brief Создаёт мютекс
		///\param lpName - имя для именных, межпроцессовых мютексов
		///\param fInitialOwner - начально состояние мютекса при создании 
		///\param lpEventAttributes - кароче NULL
		///\return TRUE в случае успеха,иначе FALSE 
		inline BOOL Create(LPCTSTR lpName=NULL,
			BOOL fInitialOwner=FALSE,
			LPSECURITY_ATTRIBUTES lpEventAttributes=NULL)
		{
			m_hMutex=::CreateMutex(lpEventAttributes,fInitialOwner,lpName);
			m_fExists=(::GetLastError()==ERROR_ALREADY_EXISTS);
			return(m_hMutex!=NULL);
		}
		///\brief Говорит о том что мютекс уже создан(после вызова CreateMutex)
		///\return TRUE в случае созданного события
		inline BOOL IsAlraedyExists()
			{return m_fExists;};
		///\brief Открывает ранее созданный мютекс
		///\param lpName - Имя события
		///\param dwDesiredAccess - Права доступа
		///\param bInheritHandle - флаг начледования события дочерним процессом
		///\return TRUE в случае успеха, иначе FALSE
		BOOL Open(LPCTSTR lpName,DWORD dwDesiredAccess=EVENT_ALL_ACCESS,BOOL bInheritHandle=FALSE)
			{return ((m_hMutex=::OpenMutex(dwDesiredAccess,bInheritHandle,lpName))!=NULL);};
		///\brief Функция освобождает мютекс
		///\return TRUE в случае успеха,иначе FALSE 
		inline BOOL Release()
			{return ::ReleaseMutex(m_hMutex);}
		///\brief Проверяет состояние мютекса
		///\return TRUE если установлено, иначе FALSE
		inline BOOL CheckMutex()
		{
			BOOL fRet=Wait(0);
			if(fRet)
				Release();
			return fRet;
		}
		///\brief Ожидает переход мютекса в установленное состояние
		///\param dwMillisecond - время ожидания в мс
		///\return TRUE если за установленное время мютекс установился, иначе FALSE
		inline BOOL Wait(DWORD dwMillisecond=INFINITE)
			{return (::WaitForSingleObject(m_hMutex,dwMillisecond)==WAIT_OBJECT_0);}
		///\brief Закрывает мютекс
		///\return TRUE в случае успеха,иначе FALSE 
		inline BOOL Close()
			{return ::CloseHandle(m_hMutex);}
	};
}
#endif //__ULTHREADS__H_
