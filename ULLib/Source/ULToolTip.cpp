///\file ULToolTip.cpp
///\brief cpp файл класса контрола подсказок(20.07.2007)
#include "..\\..\\ULLib\\Include\\ULToolTip.h"
#include "..\..\ULLib\Include\ULRes.h"
namespace ULWnds
{
	namespace ULControls
	{
		CULToolTip::CULToolTip(){};

		CULToolTip::~CULToolTip(){};
		BOOL CULToolTip::Create(HWND hParentWnd,BOOL fBalloon)
		{
			INITCOMMONCONTROLSEX iccex; 
			iccex.dwICC = ICC_WIN95_CLASSES;
			iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
			InitCommonControlsEx(&iccex);

			m_hWnd = CreateWindowEx(WS_EX_TOPMOST,
				TOOLTIPS_CLASS,
				NULL,
				WS_POPUP|TTS_NOPREFIX|TTS_ALWAYSTIP|((fBalloon)?TTS_BALLOON:0),		
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				hParentWnd,
				NULL,
				ULOther::ULGetResourceHandle(),
				NULL
				);
			SetWindowPos(HWND_TOPMOST,0,0,0,0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			return Attach(m_hWnd);//Сабклассируем
		};

		BOOL CULToolTip::AddTool(HWND hTargetWnd,LPCTSTR szMessage,UINT uID)
		{
			RECT rcTargetWnd;
			::GetClientRect(hTargetWnd, &rcTargetWnd);		
			m_ToolInfo.cbSize=sizeof(TOOLINFO);
			m_ToolInfo.uFlags=(((uID)&&(hTargetWnd==m_hParentWnd))?TTF_TRACK:TTF_SUBCLASS);
			m_ToolInfo.hwnd=hTargetWnd;
			m_ToolInfo.hinst=ULOther::ULGetResourceHandle();
			m_ToolInfo.uId=uID;
			m_ToolInfo.lpszText=const_cast<LPTSTR>(szMessage);
			m_ToolInfo.rect.left=rcTargetWnd.left;    
			m_ToolInfo.rect.top=rcTargetWnd.top;
			m_ToolInfo.rect.right=rcTargetWnd.right;
			m_ToolInfo.rect.bottom=rcTargetWnd.bottom;

			return(BOOL)SendMessage(TTM_ADDTOOL, 0, (LPARAM)&m_ToolInfo);
		};

		BOOL CULToolTip::UpdateTipText(HWND hTargetWnd,LPCTSTR szMessage)
		{
			TOOLINFO ti;
			ti.cbSize=sizeof(TOOLINFO);
			ti.hwnd=hTargetWnd;
			ti.hinst=ULOther::ULGetResourceHandle();
			ti.uId=0;
			ti.lpszText=const_cast<LPTSTR>(szMessage);
			return(BOOL)SendMessage(TTM_UPDATETIPTEXT,0,(LPARAM)&ti);
		};
	}
}
