///\file ULQueueThread.h
///\brief Заголовочный файл класса потока с очередью(27.08.2007)
#pragma once
#ifndef __ULQUEUETHREAD__H_
#define __ULQUEUETHREAD__H_
#include "ULMessageMap.inl"
#include "ULThread.h"
namespace ULThreads
{
	///\class CULQueueThread
	///\brief Класс потока с очередью(27.08.2007)
	class CULQueueThread:public CULThread
	{
	private:
		///\brief хэндл события запуска цикла сообщений
		HANDLE m_hStartedEvent;
	protected:
		///\brief Фукнция потока
		static DWORD ThreadProc(LPVOID param);
		///\brief Объект класса добавления и обработки сообщений.
		///Cообщения добавляются в конструкторе класса
		CULMessageMap<CULQueueThread> MessageMap;
	public:
		///\brief Конструктор
		CULQueueThread(void);
		///\brief Десдруктор
		virtual ~CULQueueThread(void);
		///\brief Создаёт поток
		///\param lpThreadAttributes атрибуты доступа
		///\param dwStackSize размер стэка
		///\param dwCreationFlags флаг
		///\return TRUE в случае успеха
		BOOL Create(LPSECURITY_ATTRIBUTES lpThreadAttributes=NULL,
			SIZE_T dwStackSize=0,
			DWORD dwCreationFlags=CREATE_SUSPENDED);
	};
}
#endif//__ULQUEUETHREAD__H_
