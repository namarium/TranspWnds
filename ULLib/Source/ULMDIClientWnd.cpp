///\file ULMDIClientWnd.cpp
///\brief cpp файл класса клиентского окна архетектуры MDI(26.07.2007)
#include "..\..\ULLib\Include\ULMDIClientWnd.h"
#include "..\..\ULLib\Include\ULRes.h"
namespace ULWnds
{
	namespace ULFrames
	{
		CULMDIClientWnd::CULMDIClientWnd(void):CULSubClass()
		{
			MessageMap.AddMessage<CULMDIClientWnd>(CULMDIChildWnd::CLM_DESTROYCHILD,
				&CULMDIClientWnd::OnDestroyChild);
		}
	
		CULMDIClientWnd::CULMDIClientWnd(CULMDIClientWnd& ClientWnd):
			CULSubClass(ClientWnd)
		{
			m_pMDIChilds=ClientWnd.m_pMDIChilds;
		};

		CULMDIClientWnd::~CULMDIClientWnd(void)
		{
			for(unsigned int i=0;i<m_pMDIChilds.GetSize();++i)
				delete m_pMDIChilds[i];
		}

		void CULMDIClientWnd::operator = (CULMDIClientWnd& ClientWnd)
		{
			m_pMDIChilds=ClientWnd.m_pMDIChilds;
			ULWnds::CULWnd::operator=(ClientWnd);
		}

		HWND CULMDIClientWnd::Create(HWND hParentWnd,
									UINT idSubMenu,
									UINT idFirstChild,
									DWORD dwClientStyle,
									DWORD dwClientStyleEx)
		{
			CLIENTCREATESTRUCT ccs;
			HMENU hMenu=::GetMenu(hParentWnd);
			ccs.hWindowMenu  = ::GetSubMenu(hMenu, idSubMenu);
			ccs.idFirstChild = idFirstChild;
		#ifndef UNICODE
			char szClassName[]="MDICLIENT";
		#else
			wchar_t szClassName[]=L"MDICLIENT";
		#endif//UNICODE
			m_hWnd = ::CreateWindowEx(dwClientStyleEx, szClassName,
				NULL,dwClientStyle,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				hParentWnd, NULL, ULOther::ULGetResourceHandle(), (LPVOID)&ccs);
			ShowWindow(SW_SHOW);	

			return ((Attach(m_hWnd))?m_hWnd:NULL);
		};

		int CULMDIClientWnd::AddChild(CULMDIChildWnd* pULMDIChildWnd)
		{
			m_pMDIChilds.Resize(m_pMDIChilds.GetSize()+1);
			m_pMDIChilds[m_pMDIChilds.GetSize()-1]=/*(CULMDIChildWnd*)*/(pULMDIChildWnd);
			return (int)m_pMDIChilds.GetSize();
		};

		LRESULT CULMDIClientWnd::OnDestroyChild(WPARAM wParam,LPARAM)
		{
			HWND hChildWnd=(HWND)wParam;
			//копируем созержаение массива
			ULOther::CULArr<CULMDIChildWnd*> pMDIChilds(m_pMDIChilds.GetSize());
			memcpy(pMDIChilds+0,m_pMDIChilds+0,sizeof(CULMDIChildWnd*)*m_pMDIChilds.GetSize());
			m_pMDIChilds.Resize(m_pMDIChilds.GetSize()-1);
			//убираем из массива уничтоженный чайлд и очищаем память
			int nCounter=0;
			for(unsigned int i=0;i<pMDIChilds.GetSize();++i)
				if((pMDIChilds[i])->m_hWnd!=hChildWnd)
					m_pMDIChilds[nCounter++]=pMDIChilds[i];
				else 
				{
					delete pMDIChilds[i];
					pMDIChilds[i]=NULL;
					continue;	
				}
			return TRUE;
		};
	}
}
