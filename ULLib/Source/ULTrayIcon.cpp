///\file ULTrayIcon.cpp
///\brief фаил реализации класса иконки в трее
#define _WIN32_IE 0x0500
#include "..\Include\ULTrayIcon.h"

/*
#if (_WIN32_IE >= 0x0501)
#define NIN_BALLOONSHOW     (WM_USER + 2)
#define NIN_BALLOONHIDE     (WM_USER + 3)
#define NIN_BALLOONTIMEOUT  (WM_USER + 4)
#define NIN_BALLOONUSERCLICK (WM_USER + 5)
#endif
*/
namespace ULOther
{
	CULTrayIcon::CULTrayIcon(void):
		m_nIconCount(0)
	{
		m_pNotifyIconData=new NOTIFYICONDATA;
//		::ZeroMemory(m_pNotifyIconData-,sizeof(NOTIFYICONDATA));
/*		m_pNotifyIconData->cbSize=sizeof(m_NotifyIconData);
		m_pNotifyIconData->uFlags=0;
		m_pNotifyIconData->uID=0;
		*/
	}

	CULTrayIcon::~CULTrayIcon(void)
	{
		for(UINT i=1;i<=m_nIconCount;++i)
			DeleteIcon(i);
		delete m_pNotifyIconData;
	}

	BOOL CULTrayIcon::Create(HWND hWnd,UINT uCallbackMessage)
	{
		if(!::IsWindow(hWnd))
			return FALSE;
		m_pNotifyIconData->hWnd=hWnd;
		if(uCallbackMessage<WM_USER)
			return FALSE;

		m_pNotifyIconData->cbSize=sizeof(NOTIFYICONDATA);
		m_pNotifyIconData->uID=0;

		m_pNotifyIconData->uCallbackMessage=uCallbackMessage;
		m_pNotifyIconData->uFlags=NIF_MESSAGE;
		return TRUE;
	}

	UINT CULTrayIcon::AddIcon(HICON hIcon)
	{
		m_nIconCount++;
		m_pNotifyIconData->hIcon=hIcon;
		m_pNotifyIconData->uFlags|=NIF_ICON;
		m_pNotifyIconData->uID=m_nIconCount;
		return (::Shell_NotifyIcon(NIM_ADD,m_pNotifyIconData))?
			m_pNotifyIconData->uID:0;
	}
	BOOL CULTrayIcon::ChangeIcon(UINT uID,HICON hIcon)
	{
		m_pNotifyIconData->hIcon=hIcon;
		m_pNotifyIconData->uFlags=NIF_ICON;
		m_pNotifyIconData->uID=uID;
		return ::Shell_NotifyIcon(NIM_MODIFY,m_pNotifyIconData);
	}
	BOOL CULTrayIcon::SetIconTip(UINT uID,LPCTSTR pszTip)
	{
		if(!pszTip)
			return FALSE;
		_tcscpy_s(m_pNotifyIconData->szTip,sizeof(m_pNotifyIconData->szTip)/sizeof(m_pNotifyIconData->szTip[0]),
			pszTip);
		m_pNotifyIconData->uFlags=NIF_TIP;
		m_pNotifyIconData->uID=uID;
		return ::Shell_NotifyIcon(NIM_MODIFY,m_pNotifyIconData);
	}
	BOOL CULTrayIcon::ShowBalloon(UINT uID,LPCTSTR pszInfoTitle,LPCTSTR pszInfo,DWORD dwInfoFlags)
	{
		m_pNotifyIconData->uFlags=NIF_INFO;
		if(pszInfoTitle)
			_tcscpy_s(m_pNotifyIconData->szInfoTitle,
				sizeof(m_pNotifyIconData->szInfoTitle)/sizeof(m_pNotifyIconData->szInfoTitle[0]),
				pszInfoTitle);
		if(pszInfo)
			_tcscpy_s(m_pNotifyIconData->szInfo,
				sizeof(m_pNotifyIconData->szInfo)/sizeof(m_pNotifyIconData->szInfo[0]),
				pszInfo);
		m_pNotifyIconData->dwInfoFlags=dwInfoFlags;
		m_pNotifyIconData->uID=uID;
		return ::Shell_NotifyIcon(NIM_MODIFY,m_pNotifyIconData);
	}

	BOOL CULTrayIcon::DeleteIcon(UINT uID)
	{
		m_pNotifyIconData->uID=uID;
		return ::Shell_NotifyIcon(NIM_DELETE,m_pNotifyIconData);
	}
}
