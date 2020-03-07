///\file ULStates.h
///\brief Заголовочный файл пространства имен классов состояния памяти,времени 
///выполнения и лог-файла(21.07.2007)
#pragma once
#ifndef __STATES__H_
#define __STATES__H_

#include <windows.h>
#include <stdio.h>
#include <crtdbg.h>
#include <TCHAR.H>
#include <assert.h>
#include "ULOther.h"
#include "ULRes.h"
///\namespace ULStates
/*!\brief Пространство имен классов состояния памяти,времени 
выполнения и лог-файла(21.07.2007)*/
namespace ULStates
{
	///\class CULMemState
	///\brief Класс для отслеживания утечки памяти(2006)
	class CULMemState
	{
	protected:
		///\brief Структура хранящая снимок памяти при вызове MemCkeck
		_CrtMemState m_OldState;
		///\brief Структура хранящая снимок памяти при вызове MemVerify
		_CrtMemState m_NewState;
	public:
		///\brief Функция делает начальный снимок памяти и сохраняет в m_OldState
		void MemCkeck()
		{
			_CrtMemCheckpoint(&m_OldState);
		};
		/*!\brief Функция делает начальный снимок памяти и сохраняет
		в m_NewState, а затем сравнивает с m_OldState*/
		///\return количество неосвобожденной памяти
		int MemVerify()
		{
			_CrtMemCheckpoint(&m_NewState);
			if(_CrtMemDifference(&m_NewState, &m_OldState, &m_NewState)!=0)
				return int(m_NewState.lSizes[1]);
			else
				return 0;
		}
		///\brief Функция возвращает количество неосвобожденной памяти
		///\return количество неосвобожденной памяти
		int GetLeak()
		{
			return (int)m_NewState.lSizes[1];
		}
	};
}
	//=============================================
	//макросы для отладки ПО
	//=============================================
	#ifdef _DEBUG
		#include <stdio.h>
		/*!\brief объект класса CULMemState для работы макросов 
		MEM_CHECK и MEM_VERIFY*/
		static ULStates::CULMemState m_MemProtect;
		///\brief Функция делает начальный снимок памяти
		#define MEM_CHECK	m_MemProtect.MemCkeck();
		/*!\brief Функция делает конечный снимок памяти и в случае
		утечки выводит MessageBox*/
		#define MEM_VERIFY			if(m_MemProtect.MemVerify()!=0){ \
									char buffer[MAX_PATH];			\
									_stprintf_s(buffer,_T("Неосвобождённой памяти %d байт(а)"),int(m_MemProtect.GetLeak()));\
									::MessageBox(0,buffer,_T("Утечка памяти!"),MB_ICONHAND);};

	#else
		///\brief Функция делает начальный снимок памяти
		#define MEM_CHECK 
		/*!\brief Функция делает конечный снимок памяти и в случае
		утечки выводит MessageBox*/
		#define MEM_VERIFY 
	#endif //_DEBUG

namespace ULStates
{
	///\class CULTimeState
	/*!\brief Класс для определения времени выполнения кода
	с учетом вытесняющей многозадачности, 
	т.е. только выделенные кванты времени (2006)*/
	class CULTimeState
	{
	protected:
		///\brief Начальные показатели времени
		FILETIME ftKernelTimeStart, ftKernelTimeEnd; 
		///\brief Конечные показатели времени
		FILETIME ftUserTimeStart, ftUserTimeEnd; 
		FILETIME ftDummy; 
		///\brief Функция для перевода из FILETIME в _int64
		///\param pFT - FILETIME
		///\return формат в _int64
		_int64 FileTimeToQuadWord(FILETIME* pFT)
		{
			return ((unsigned long long(pFT->dwHighDateTime)<<32)|pFT->dwLowDateTime);
		};
	public:
		///\brief Конструктор
		///\атоматически запускает счётчик времени
		CULTimeState()
		{
			TimeCheck();
		};
		///Функция для запоминания начального момента времени
		///\return TRUE в случае успеха
		BOOL TimeCheck()
		{
			// получаем начальные показатели времени 
			return ::GetThreadTimes(::GetCurrentThread(), &ftDummy, &ftDummy, 
				&ftKernelTimeStart, &ftUserTimeStart);
		};
		///\brief Функция для получения конечного момента времени и сравнения с начальным
		///\return время в милисекундах
		double TimeVerify()
		{
			_int64 qwKernelTimeElapsed, qwUserTimeElapsed, qwTotalTimeElapsed; 
			// получаем конечные показатели времени 
			::GetThreadTimes(::GetCurrentThread(), &ftDummy, &ftDummy, 
				&ftKernelTimeEnd, &ftUserTimeEnd); 
			// получаем общее время, складывая время выполнения ядра и User 
			qwKernelTimeElapsed = FileTimeToQuadWord(&ftKernelTimeEnd) - FileTimeToQuadWord(&ftKernelTimeStart); 
			qwUserTimeElapsed = FileTimeToQuadWord(&ftUserTimeEnd) - FileTimeToQuadWord(&ftUserTimeStart); 
			qwTotalTimeElapsed = qwKernelTimeElapsed + qwUserTimeElapsed; 
			//возвращаем в мс
			return (double(qwTotalTimeElapsed)/10000);
		};
	};
	///\class CULWatchState
	/*!\brief Класс для определения времени выполнения кода
	без учета вытесняющей многозадачности (2006)*/
	class CULWatchState
	{ 
	protected:
		///\brief Количество отсчетов в секунду 
		LARGE_INTEGER m_liPerfFreq; 
		///\brief Начальный отсчет 
		LARGE_INTEGER m_liPerfStart; 
	public: 
		///\brief Конструктор
		///автоматически запускает счетчик времени
		CULWatchState() 
		{ 
			WatchCheck(); 
		} 
		///\brief Функция для запоминания начального момента времени
		///\return TRUE в случае успеха
		BOOL WatchCheck() 
		{
			QueryPerformanceFrequency(&m_liPerfFreq); 
			return QueryPerformanceCounter(&m_liPerfStart); 
		};
		/*!\brief Функция для получения конечного момента времени
			и сравнения с начальным
			возвращает число миллисекунд после вызова WatchCheck */
		double WatchVerify() const 
		{ 

			LARGE_INTEGER liPerfNow; 
			QueryPerformanceCounter(&liPerfNow); 
			return (double((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) /m_liPerfFreq.QuadPart); 
		} 
	}; 
	///\class CULLogFile
	/*!\brief Класс для ведения отладочного лога:
		утечки памяти и времени выполнения кода(2006)*/
	class CULLogFile
	{
	protected:
		///\brief Структура для хранения полей лога
		///\param lfdMemState - следит за утечкой памяти
		///\param lfdTimeState - следит за временем выполнения
		///\param lfdszName - имя поля лога
		struct tagLogFileData
		{
			CULMemState lfdMemState;
			CULTimeState lfdTimeState;
			CULWatchState lfdWatchState;
			TCHAR* lfdszName;
		};
		///\brief Массив структур для хранения полей лога
		ULOther::CULArr<tagLogFileData> m_pLogFileData;
		///\brief Указатель идентификатор файла лога
		FILE* m_LogFile;
	public:
		///\brief Конструктор
		CULLogFile():
		m_pLogFileData(NULL)
		{
			//определение имениисполняемого файла
			TCHAR szPath[5][MAX_PATH];
			::GetModuleFileName(ULOther::ULGetResourceHandle(),szPath[0],MAX_PATH);
			 _tsplitpath_s(szPath[0],szPath[1],MAX_PATH,szPath[2],MAX_PATH,szPath[3],MAX_PATH,szPath[4],MAX_PATH);
			//имя файла лога = имя исполняемого файла.log
			_tcscat_s(szPath[3],MAX_PATH,szPath[4]);
			_tcscat_s(szPath[3],MAX_PATH,_T(".log"));
			_tfopen_s(&m_LogFile,szPath[3],_T("w+"));
#ifdef UNICODE
			unsigned short usLittleEndian=0xfeff;
			fwrite(&usLittleEndian,2,1,m_LogFile);
#endif
		};
		///\brief Деструктор
		~CULLogFile()
		{
			fclose(m_LogFile);
		}
		///\brief Функция добавляет поле лога
		///\param szLogName имя поля лога
		///\return TRUE в случае успеха
		BOOL AddLog(TCHAR* szLogName)
		{
			if(ResetLog(szLogName))
				return FALSE;
			m_pLogFileData.Resize(m_pLogFileData.GetSize()+1);
			if(m_pLogFileData!=NULL)
			{
				m_pLogFileData[m_pLogFileData.GetSize()-1].lfdMemState.MemCkeck();
				m_pLogFileData[m_pLogFileData.GetSize()-1].lfdTimeState.TimeCheck();
				m_pLogFileData[m_pLogFileData.GetSize()-1].lfdWatchState.WatchCheck();
				m_pLogFileData[m_pLogFileData.GetSize()-1].lfdszName=szLogName;
				return TRUE;
			}
			else
				return FALSE;
		};
		///\brief Функция сбрасывает поле лога
		///\param szLogName имя поля лога
		///\return TRUE в случае успеха
		BOOL ResetLog(TCHAR* szLogName)
		{
			for(unsigned int i=0;i<m_pLogFileData.GetSize();i++)
				if((_tcsspn(m_pLogFileData[i].lfdszName,szLogName)==0)&&
					(_tcslen(m_pLogFileData[i].lfdszName)==_tcslen(szLogName)))
				{
					m_pLogFileData[i].lfdMemState.MemCkeck();
					m_pLogFileData[i].lfdTimeState.TimeCheck();
					m_pLogFileData[i].lfdWatchState.WatchCheck();
					return TRUE;
				}
			return FALSE;
		};
		///\brief Функция сохраняет поле лога
		///\param szLogName имя поля лога
		///\return TRUE в случае успеха
		BOOL SaveLog(TCHAR* szLogName)
		{
			for(unsigned int i=0;i<m_pLogFileData.GetSize();i++)
				if((_tcscmp(m_pLogFileData[i].lfdszName,szLogName)==0))
				{
					TCHAR szSaveLog[MAX_PATH];
					_stprintf_s(szSaveLog,MAX_PATH,_T("%s: утечка памяти %d байт,время выполнения %.3f мс,полное время %.3f мс\r"),
						m_pLogFileData[i].lfdszName,
						//исключаем из лога издержки LOGFILEDATA
						m_pLogFileData[i].lfdMemState.MemVerify()-(m_pLogFileData.GetSize()-1-i)*sizeof(tagLogFileData),
						m_pLogFileData[i].lfdTimeState.TimeVerify(),
						m_pLogFileData[i].lfdWatchState.WatchVerify());
					fwrite(szSaveLog,_tcslen(szSaveLog)*sizeof(TCHAR),1,m_LogFile);
					return TRUE;
				}
			return FALSE;
		}
	};
}	//=============================================
	//макросы для отладки ПО(2006)
	//=============================================
	#ifdef _DEBUG
		/*!\brief объект класса CULLogFile для работы макросов 
		LOGFILE_ADD, LOGFILE_RESET и LOGFILE_SAVE*/
		template <typename T>
		T &GetLogFile()
		{
			static T g_LogFile;
			return g_LogFile;
		}
		///\brief Макрос добавляет поле лога
		#define LOGFILE_ADD(szLogName)		\
			GetLogFile<ULStates::CULLogFile>().AddLog(szLogName);
		///\brief Макрос сбрасывает поле лога
		#define LOGFILE_RESET(szLogName)	\
			GetLogFile<ULStates::CULLogFile>().ResetLog(szLogName);
		///\brief Функция сохраняет поле лога
		#define LOGFILE_SAVE(szLogName)		\
			GetLogFile<ULStates::CULLogFile>().SaveLog(szLogName);
		///\brief определение ассерта
		#define ASSERT assert
	#else
		///\brief Макрос добавляет поле лога
		#define LOGFILE_ADD(szLogName)
		///\brief Макрос сбрасывает поле лога
		#define LOGFILE_RESET(szLogName)
		///\brief Функция сохраняет поле лога
		#define LOGFILE_SAVE(szLogName)
		///\brief определение ассерта
		#define ASSERT
	#endif //_DEBUG

#endif //__STATES__H_
