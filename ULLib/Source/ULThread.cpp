///\file ULThread.cpp
///\brief Файл реализации класса потока(03.10.2007)
#include "..\..\ULLib\Include\ULThread.h"
#include <process.h> 
namespace ULThreads
{
	CULThread::CULThread(void):
		m_hThread(NULL)
	{
		
	};

	CULThread::~CULThread(void)
	{
		Quit(0);
		CloseHandle(m_hThread);
	}

	unsigned WINAPI CULThread::ThreadProc(LPVOID param)
	{
		CULThread* pThread=(CULThread*)param;
		MSG msg;
		BOOL fDone=FALSE;
		while(!fDone)                                        
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))   
			{
				if (msg.message == WM_QUIT)                 
					fDone=TRUE;                              
			}
			else
				if (!pThread->Run())
					break;
		pThread->m_fRunThread=FALSE;
		return 0;
	}

	BOOL CULThread::Run()
	{
		return TRUE;
	};

	BOOL CULThread::Create(LPSECURITY_ATTRIBUTES lpThreadAttributes,
			unsigned int uiStackSize,
			DWORD dwCreationFlags)
	{
		m_hThread=(HANDLE)_beginthreadex( 
			lpThreadAttributes,
			uiStackSize,
			ThreadProc,
			(LPVOID)this,
			dwCreationFlags,
			(unsigned*)&m_dwThreadId); 
		if(m_hThread)
			m_fRunThread=TRUE;
		ResumeThread();
		return (m_hThread!=NULL);
	};

	BOOL CULThread::Quit(DWORD dwMilliseconds)
	{
		m_fRunThread=FALSE;
		SetThreadPriority(THREAD_PRIORITY_HIGHEST);
		PostThreadMessage(WM_QUIT);
		return (WaitForSingleObject(*this,dwMilliseconds)==WAIT_OBJECT_0);
	};
}
