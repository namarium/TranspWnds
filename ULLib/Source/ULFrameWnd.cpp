///\file ULFrameWnd.cpp
///\brief cpp файл класса главного окна(2006)
#include "..\..\ULLib\Include\ULFrameWnd.h"
#include "..\..\ULLib\Include\ULRes.h"
namespace ULWnds
{
	namespace ULFrames
	{
		CULFrameWnd::CULFrameWnd(void):	CULWnd()
		{
			MessageMap.AddMessage<CULFrameWnd>(WM_CREATE,&CULFrameWnd::OnCreate);
			MessageMap.AddMessage<CULFrameWnd>(WM_DESTROY,&CULFrameWnd::OnDestroy);
		}

		CULFrameWnd::CULFrameWnd(CULFrameWnd& FrameWnd):CULWnd(FrameWnd)
		{
		}

		CULFrameWnd::~CULFrameWnd(void)
		{
		}

		void CULFrameWnd::operator = (CULFrameWnd& FrameWnd)
		{
			ULWnds::CULWnd::operator=(FrameWnd);
		}

		BOOL CULFrameWnd::Create(LPCTSTR szWindowName,
								short MenuID,
								short IconID,
								short IconSmID,
								short BGColor,
								DWORD dwStyle,
								DWORD dwStyleEx)
		{
			WNDCLASSEX wcex;
			HINSTANCE hInstance=ULOther::ULGetResourceHandle(); 
		    
			wcex.cbSize = sizeof(WNDCLASSEX); 		

			wcex.style			= CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc	= (WNDPROC)WndProc;
			wcex.cbClsExtra		= 0;
			wcex.cbWndExtra		= 0;
			wcex.hInstance		= hInstance;
			wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IconID);
			wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground	= (HBRUSH)BGColor;
			wcex.lpszMenuName	= MAKEINTRESOURCE(MenuID);	
			wcex.lpszClassName	= _T("ULFRAMEWND");
			wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IconSmID);
			RegisterClassEx(&wcex);
		//==============================================================
			m_hWnd=CreateWindowEx(dwStyleEx,
				wcex.lpszClassName, szWindowName, dwStyle,
				CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, this);
			return (m_hWnd!=NULL);
		}

		LRESULT CULFrameWnd::OnDestroy(WPARAM /*wParam*/,LPARAM /*lParam*/)
		{
			return FALSE;
		}

		LRESULT CULFrameWnd::OnCreate(WPARAM /*wParam*/,LPARAM)
		{
			return FALSE;
		}
	}
}
