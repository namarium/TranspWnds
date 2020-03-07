///\file ULSubClass.cpp
///\brief cpp файл класса для сабклассирования контролов(07.10.2007)
#include "..\..\ULLib\Include\ULSubClass.h"
#include <commctrl.h>
namespace ULWnds
{

  CULSubClass::CULSubClass():
		ULWnds::CULWnd(),
		m_lpOldWndProc(NULL)
  {};

	CULSubClass::CULSubClass(CULSubClass& SubClass):
		ULWnds::CULWnd(SubClass),
		m_lpOldWndProc(SubClass.m_lpOldWndProc)
	{};

	void CULSubClass::operator = (CULSubClass& SubClass)
	{
		m_lpOldWndProc=SubClass.m_lpOldWndProc;
		ULWnds::CULWnd::operator=(SubClass);
	}

	CULSubClass::~CULSubClass(){};

	LRESULT CULSubClass::WndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
	{
		CULSubClass* pULWnd=(CULSubClass*)FromHandle(hWnd);

		LRESULT lRes=pULWnd->OnMessage(uMsg,wParam,lParam);
		if(lRes)
			return lRes;

		if(uMsg==WM_COMMAND)
		{
			BOOL fRet=pULWnd->MessageMap.ProcessCommand(pULWnd,wParam,lParam);
			if(fRet)
				return fRet;
		}
		else 
			if(uMsg==WM_NOTIFY)
			{
				BOOL fRet=pULWnd->MessageMap.ProcessNotify(pULWnd,(NMHDR*)lParam);
				if(fRet)
					return fRet;
			}
		BOOL fRet=pULWnd->MessageMap.ProcessMessage(pULWnd,uMsg,wParam,lParam);
		if(fRet)
			return fRet;
		return pULWnd->CallWindowProc(uMsg,wParam,lParam);
	};
	LRESULT CULSubClass::ParentWndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
	{
		CULWnd* pULWnd=FromHandle(hWnd);
		switch(uMsg)
		{
		case WM_DRAWITEM:
			{
				//You can use the GetDlgItem function with any parent-child window pair, not just with dialog boxes. 
				CULSubClass* pChildWnd=(CULSubClass*)FromHandle(::GetDlgItem(hWnd,(int)wParam));
				if(pChildWnd!=NULL)
				{	
					BOOL fRet;
					fRet=pChildWnd->MessageMap.ProcessReflectMessage(pChildWnd,uMsg,wParam,lParam);
					if(fRet==FALSE)
						break;
					else 
						//If an application processes this message, it should return TRUE. 
						return fRet;
				}
			}
		case WM_CTLCOLORBTN:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
			{
				CULSubClass* pChildWnd=(CULSubClass*)FromHandle((HWND)lParam);
				if(pChildWnd!=NULL)
				{	
					BOOL fRet;
					fRet=pChildWnd->MessageMap.ProcessReflectMessage(pChildWnd,uMsg,wParam,lParam);
					if(fRet==FALSE)
						break;
					else 
						//If an application processes this message, it returns a handle to a brush.
						//The system uses the brush to paint the background of the control.
						return fRet;
				}
			}
		case WM_MEASUREITEM:
			{
				//You can use the GetDlgItem function with any parent-child window pair, not just with dialog boxes. 
				CULSubClass* pChildWnd=(CULSubClass*)FromHandle(::GetDlgItem(hWnd,(int)wParam));
				if(pChildWnd!=NULL)
				{	
					BOOL fRet;
					fRet=pChildWnd->MessageMap.ProcessReflectMessage(pChildWnd,uMsg,wParam,lParam);
					if(fRet==FALSE)
						break;
					else 
						//If an application processes this message, it should return TRUE. 
						return fRet;
				}
			}
		case WM_DELETEITEM:
			{
				//You can use the GetDlgItem function with any parent-child window pair, not just with dialog boxes. 
				CULSubClass* pChildWnd=(CULSubClass*)FromHandle(::GetDlgItem(hWnd,(int)wParam));
				if(pChildWnd!=NULL)
				{	
					BOOL fRet;
					fRet=pChildWnd->MessageMap.ProcessReflectMessage(pChildWnd,uMsg,wParam,lParam);
					if(fRet==FALSE)
						break;
					else 
						//If an application processes this message, it should return TRUE. 
						return fRet;
				}
			}
		case WM_COMPAREITEM:
			{
				//You can use the GetDlgItem function with any parent-child window pair, not just with dialog boxes. 
				CULSubClass* pChildWnd=(CULSubClass*)FromHandle(::GetDlgItem(hWnd,(int)wParam));
				if(pChildWnd!=NULL)
				{	
					BOOL fRet;
					fRet=pChildWnd->MessageMap.ProcessReflectMessage(pChildWnd,uMsg,wParam,lParam);
					if(fRet==FALSE)
						break;
					else 
						//Value	Meaning 
						//-1	Item 1 precedes item 2 in the sorted order. 
						//0		Items 1 and 2 are equivalent in the sorted order. 
						//1		Item 1 follows item 2 in the sorted order. 
						//для совместимости при обработке WM_COMPAREITEM -1,0,1 должны замениться
						//на 1,2,3 соответственно
						return fRet;
				}
			}
		case WM_CHARTOITEM:
			{
				CULSubClass* pChildWnd=(CULSubClass*)FromHandle((HWND)lParam);
				if(pChildWnd!=NULL)
				{	
					BOOL fRet;
					fRet=pChildWnd->MessageMap.ProcessReflectMessage(pChildWnd,uMsg,wParam,lParam);
					if(fRet==FALSE)
						break;
					else 
						//The return value specifies the action that the application 
						//performed in response to the message. A return value of –1 or –2 
						//indicates that the application handled all aspects of selecting
						//the item and requires no further action by the list box. 
						//A return value of 0 or greater specifies the zero-based index
						//of an item in the list box and indicates that the list box should
						//perform the default action for the keystroke on the specified item.
						//для совместимости при обработке WM_VKEYTOITEM -2,-1,0 должны замениться
						//на -3,-2,-1 соответственно
						return fRet;
				}
			}
		case WM_VKEYTOITEM:
			{
				CULSubClass* pChildWnd=(CULSubClass*)FromHandle((HWND)lParam);
				if(pChildWnd!=NULL)
				{	
					BOOL fRet;
					fRet=pChildWnd->MessageMap.ProcessReflectMessage(pChildWnd,uMsg,wParam,lParam);
					if(fRet==FALSE)
						break;
					else 
						//The return value specifies the action that the application performed in response
						//to the message. A return value of –2 indicates that the application
						//handled all aspects of selecting the item and requires no further 
						//action by the list box. A return value of –1 indicates that the 
						//list box should perform the default action in response to the 
						//keystroke. A return value of 0 or greater specifies the index of 
						//an item in the list box and indicates that the list box should 
						//perform the default action for the keystroke on the specified item. 
						//для совместимости при обработке WM_VKEYTOITEM -2,-1,0 должны замениться
						//на -3,-2,-1 соответственно
						return fRet-2;
				}
			}
		case WM_HSCROLL:
		case WM_VSCROLL:
			{
				CULSubClass* pChildWnd=(CULSubClass*)FromHandle((HWND)lParam);
				if(pChildWnd!=NULL)
				{	
					BOOL fRet;
					fRet=pChildWnd->MessageMap.ProcessReflectMessage(pChildWnd,uMsg,wParam,lParam);
					if(fRet==TRUE)
						break;
					else 
						//If an application processes this message, it should return zero. 
						return fRet;
				}
			}
		case WM_PARENTNOTIFY:
			{
				if((wParam!=WM_CREATE)||(wParam!=WM_DESTROY))
					break;
				CULSubClass* pChildWnd=(CULSubClass*)FromHandle((HWND)lParam);
				if(pChildWnd!=NULL)
				{	
					BOOL fRet;
					fRet=pChildWnd->MessageMap.ProcessReflectMessage(pChildWnd,uMsg,wParam,lParam);
					if(fRet==TRUE)
						break;
					else 
						//If an application processes this message, it should return zero. 
						return fRet;
				}
			}
		case WM_COMMAND:
			{
				CULSubClass* pChildWnd=(CULSubClass*)FromHandle((HWND)lParam);
				if(pChildWnd!=NULL)
					if(pChildWnd->MessageMap.ProcessReflectCommand(pChildWnd,HIWORD(wParam))==TRUE)
						return TRUE;
				break;
			}
		case WM_NOTIFY:
			{
				CULSubClass* pChildWnd=(CULSubClass*)FromHandle(((NMHDR*)lParam)->hwndFrom);
				if(pChildWnd!=NULL)
				{
					BOOL fRet;
					if(pChildWnd->MessageMap.ProcessReflectNotify(pChildWnd,(NMHDR*)lParam,&fRet))
						return fRet;
				}
				break;
			}
		}		
		return pULWnd->CallWindowProc(uMsg,wParam,lParam);
	};

	BOOL CULSubClass::Attach(HWND hWnd)
	{
		if(hWnd==NULL)
			return FALSE;
		m_hWnd=hWnd;
		m_hParentWnd=GetParent();

		if(::GetWindowLong(hWnd,GWL_USERDATA)!=NULL)
			return FALSE;
		::SetWindowLong(hWnd,GWL_USERDATA,(LONG)(LONG_PTR)this);

		if(MessageMap.IsAllowMessage())
		{
			PreSubClass();
			m_lpOldWndProc=(WNDPROC)(LONG_PTR)::SetWindowLong(hWnd,
				GWL_WNDPROC,(LONG)(LONG_PTR)WndProc);
			if(m_lpOldWndProc==NULL)
				return FALSE;
		}
		if(MessageMap.IsAllowReflect())
		{
			CULWnd* pParentWnd=FromHandle(m_hParentWnd);
			if(pParentWnd==NULL)
				return FALSE;
			if(pParentWnd->m_lpSubClassWndProc==NULL)
			{
				pParentWnd->m_lpSubClassWndProc=(WNDPROC)(LONG_PTR)::GetWindowLong(*pParentWnd,GWL_WNDPROC);
				if(pParentWnd->m_lpSubClassWndProc==NULL)
				{
					pParentWnd->m_lpSubClassWndProc=(WNDPROC)(LONG_PTR)::GetWindowLong(*pParentWnd,DWL_DLGPROC);
					::SetWindowLong(*pParentWnd,DWL_DLGPROC,(LONG)(LONG_PTR)ParentWndProc);
				}
				else
          ::SetWindowLong(*pParentWnd,GWL_WNDPROC,(LONG)(LONG_PTR)ParentWndProc);
			}
		}
		return TRUE;
	}

	void CULSubClass::PreSubClass()
	{

	};
}
