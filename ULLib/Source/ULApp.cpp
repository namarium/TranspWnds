///\file ULApp.cpp
///\brief cpp файл класса приложения(01.05.2007)
#include "..\..\ULLib\Include\ULApp.h"
#include "..\..\ULLib\Include\ULRes.h"
namespace ULApps
{	
	CULApp::CULApp(void):
		m_hInstance(NULL),
		m_lpCmdLine(NULL),
		m_nCmdShow(NULL)
	{
		m_pULApp=this;
		m_hGetMessageHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, NULL, GetCurrentThreadId());
	}

	CULApp::~CULApp(void)
	{
		if(m_pMainWnd!=NULL)
			delete m_pMainWnd;
		UnhookWindowsHookEx(m_hGetMessageHook);
	};

	BOOL CULApp::Run(MSG* /*pMsg*/)
	{
		return TRUE;
	};

	void CULApp::MessageLoop()
	{
		MSG msg;
		while ((m_pMainWnd->IsWindow())&&(GetMessage(&msg, NULL, 0, 0))) 
		{
			if(Run(&msg)) 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	};

	void CULApp::ProcessMessages()
	{
		MSG msg;
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	};
	LRESULT CALLBACK CULApp::GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
	{
		MSG  * msg = (PMSG) lParam;
		if(!CULApp::GetULApp()->MessageMap.ProcessPreTranslateMessage(msg))
			msg->message = WM_NULL; 
/*
		if((msg->message >= WM_KEYFIRST) && (msg->message <= WM_KEYLAST))
		{
			if(TranslateAccelerator(hWnd, hAccelTable, (PMSG)lParam))
			{   
				msg->message = WM_NULL; 
			}   
		}
*/		CallNextHookEx(CULApp::GetULApp()->m_hGetMessageHook, code, wParam, lParam ); 
		return false;
	}

	CULApp *CULApp::m_pULApp=NULL;

	CULApp* CULApp::GetULApp(void)
	{
		return m_pULApp;
	};
}
int APIENTRY WinMain(HINSTANCE hInstance,
					HINSTANCE /*hPrevInstance*/,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	ULOther::ULSetResourceHandle(hInstance);
	ULApps::CULApp::GetULApp()->m_hInstance=hInstance;
	ULApps::CULApp::GetULApp()->m_lpCmdLine=lpCmdLine;
	ULApps::CULApp::GetULApp()->m_nCmdShow=nCmdShow;
	if(!ULApps::CULApp::GetULApp()->InitInstance())
		return FALSE;
	// Главный цикл сообщений
	ULApps::CULApp::GetULApp()->MessageLoop();
	return 0;
};
