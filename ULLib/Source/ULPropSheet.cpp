///\file ULPropSheet.cpp
///\brief cpp файл класса root окна(2006)
#include "..\..\ULLib\Include\ULPropSheet.h"
#include "..\..\ULLib\Include\ULRes.h"
namespace ULWnds
{
	namespace ULDlgs
	{
		CULPropSheet* g_thisPreCreate=NULL;
		HHOOK m_CBTHook=NULL;

		LRESULT CALLBACK PropSheetCBTProc(int nCode,WPARAM wParam,LPARAM lParam)
		{
			if(nCode==HCBT_CREATEWND)
			{
				g_thisPreCreate->Attach((HWND)wParam);
				g_thisPreCreate=NULL;
				UnhookWindowsHookEx(m_CBTHook);
			}
			return CallNextHookEx(m_CBTHook,nCode,wParam,lParam);
		}

		CULPropSheet::CULPropSheet():
			CULSubClass(),m_fWizard(FALSE)
		{
			MessageMap.AddMessage<CULPropSheet>(WM_DESTROY,&CULPropSheet::OnDestroy);
			memset(&m_psh,0,sizeof(m_psh));
		};

		CULPropSheet::~CULPropSheet()
		{
		};

		BOOL CULPropSheet::AddPage(HPROPSHEETPAGE hPropSheetPage)
		{
			if(hPropSheetPage!=NULL)
			{
				m_phPropSheetPage.Resize(m_phPropSheetPage.GetSize()+1);
				m_phPropSheetPage[m_phPropSheetPage.GetSize()-1]=hPropSheetPage;
				return TRUE;
			}
			return FALSE;
		}

		INT_PTR CULPropSheet::Create(HWND hParenWnd,
									LPCTSTR szCaption,
									HBITMAP hbmHeader,
									HBITMAP hbmWatermark,
									UINT nWizardFlags,
									BOOL fModal)
		{
			m_psh.dwSize=sizeof(m_psh);
			m_psh.hInstance=ULOther::ULGetResourceHandle();
			m_psh.hwndParent=hParenWnd;
			m_psh.dwFlags|=PSH_USEPSTARTPAGE;
			m_psh.nStartPage=0;
			m_psh.nPages=(UINT)m_phPropSheetPage.GetSize();
			m_psh.phpage=m_phPropSheetPage;
			if(szCaption)
			{
//				m_psh.dwFlags|=PSH_PROPTITLE;			
				m_psh.pszCaption=szCaption;
			}
			m_psh.dwFlags|=nWizardFlags;
			m_fWizard=(nWizardFlags)?TRUE:FALSE;
//			PropSheetHeader.dwFlags|=PSH_USECALLBACK;
//			PropSheetHeader.pfnCallback=(PFNPROPSHEETCALLBACK)PropSheetProc;
			if(hbmHeader!=NULL)
			{
				m_psh.dwFlags|=PSH_USEHBMHEADER;
				m_psh.dwFlags|=PSH_HEADER;
				m_psh.hbmHeader=hbmHeader;
			}
			if(hbmWatermark!=NULL)
			{
				m_psh.dwFlags|=PSH_USEHBMWATERMARK;
				m_psh.dwFlags|=PSH_WATERMARK;
				m_psh.hbmWatermark=hbmWatermark;
			}
			if(fModal!=TRUE)
				m_psh.dwFlags|=PSH_MODELESS;

			g_thisPreCreate=this;


			m_CBTHook=SetWindowsHookEx(WH_CBT,PropSheetCBTProc,
				ULOther::ULGetResourceHandle(),GetCurrentThreadId());

			return PropertySheet(&m_psh);		 
		}

		INT_PTR CULPropSheet::Create(HWND hParenWnd,
				LPCTSTR szCaption,
				int rscHeader,
				int rscWatermark,
				UINT nWizardFlags,
				BOOL fModal)
		{
			return Create(hParenWnd,szCaption,
				::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(rscHeader)),
				::LoadBitmap(ULOther::ULGetResourceHandle(),MAKEINTRESOURCE(rscWatermark)),
				nWizardFlags,fModal);
		}

		BOOL CULPropSheet::IsWizard()
		{
			return m_fWizard;
		}
		
		LRESULT CULPropSheet::OnDestroy(WPARAM,LPARAM)
		{
			memset(&m_psh,0,sizeof(m_psh));
			m_phPropSheetPage.Free();
			return 0;
		}
	}
}
