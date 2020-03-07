///\file ULDllApp.cpp
///\brief cpp файл класса динамической библиотеки(03.01.2007)
#include "..\..\ULLib\Include\ULDllApp.h"
#include "..\..\ULLib\Include\ULRes.h"
namespace ULApps
{
	HINSTANCE CULDllApp::m_hInstance=NULL;

	CULDllApp::CULDllApp(void)
	{
		
		m_pULDllApp=this;
	}

	CULDllApp::~CULDllApp(void)
	{
	};

	BOOL CULDllApp::OnProcessAttach(HINSTANCE hInstance)
	{
		ULOther::ULSetResourceHandle(hInstance);
		m_hInstance=hInstance;
		return TRUE;
	};

	BOOL CULDllApp::OnProcessDetach()
	{
		return TRUE;
	};

	BOOL CULDllApp::OnThreadAttach()
	{
		return TRUE;
	};

	BOOL CULDllApp::OnThreadDetach()
	{
		return TRUE;
	};

	CULDllApp *CULDllApp::m_pULDllApp=NULL;

	CULDllApp* CULDllApp::GetULDllApp(void)
	{
		return m_pULDllApp;
	};

	BOOL CULDllApp::DllMain(HINSTANCE hinstDLL,
						DWORD fdwReason)
	{
		switch(fdwReason)
		{
		case DLL_PROCESS_ATTACH:
			return OnProcessAttach(hinstDLL);
		case DLL_THREAD_ATTACH:
			return OnThreadAttach();
		case DLL_THREAD_DETACH:
			return OnThreadDetach();
		case DLL_PROCESS_DETACH:
			return OnProcessDetach();
		default:
			return TRUE;
		}
	};
}
