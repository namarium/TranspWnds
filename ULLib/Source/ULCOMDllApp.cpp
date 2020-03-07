///\file ULCOMDllApp.cpp
///\brief cpp файл класса динамической библиотеки COM-сервера(05.01.2007)
#include "..\\include\\ULCOMDllApp.h"

namespace ULApps
{
	CULCOMDllApp::CULCOMDllApp(): 
		ULApps::CULDllApp(),
		m_nComponentsLocks(0),
		m_nServerLocks(0)		
	{
	}

	CULCOMDllApp::~CULCOMDllApp()
	{
	}

	void CULCOMDllApp::ComponentLock()
	{
		InterlockedIncrement(&m_nComponentsLocks);
	};

	void CULCOMDllApp::ComponentUnlock()
	{
		InterlockedDecrement(&m_nComponentsLocks);
	};

	void CULCOMDllApp::ServerLock()
	{
		InterlockedIncrement(&m_nServerLocks);
	};

	void CULCOMDllApp::ServerUnlock()
	{
		InterlockedDecrement(&m_nServerLocks);
	};
//===================================================================================
	HRESULT CULCOMDllApp::OnCanUnloadNow()
	{
		if ((m_nComponentsLocks == 0) && (m_nServerLocks == 0))
			return S_OK;
		else
			return S_FALSE;
	}
//===================================================================================
	HRESULT CULCOMDllApp::DllRegisterServer()
	{	
		return static_cast<CULCOMDllApp*>(CULDllApp::GetULDllApp())->OnRegisterServer();
	}

	HRESULT CULCOMDllApp::DllUnregisterServer()
	{	
		return static_cast<CULCOMDllApp*>(CULDllApp::GetULDllApp())->OnUnregisterServer();
	}

	HRESULT CULCOMDllApp::DllGetClassObject(const CLSID& clsid,
										const IID& iid,
										void** ppvObject)
	{		
		return static_cast<CULCOMDllApp*>(CULDllApp::GetULDllApp())->OnGetClassObject(clsid,iid,ppvObject);
	}

	HRESULT CULCOMDllApp::DllCanUnloadNow()
	{
		return static_cast<CULCOMDllApp*>(CULDllApp::GetULDllApp())->OnCanUnloadNow();
	}
}
