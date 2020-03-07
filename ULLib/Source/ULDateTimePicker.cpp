///\file ULDateTimePicker.cpp
///\brief фаил объявления класса елемента управления даты и времени
#include "..\..\ULLib\Include\ULDateTimePicker.h"
#include "..\..\ULLib\Include\ULRes.h"

namespace ULWnds
{
	namespace ULControls
	{
		CULDateTimePicker::CULDateTimePicker():CULSubClass()
		{		
			INITCOMMONCONTROLSEX icex;
			icex.dwSize = sizeof(icex);
			icex.dwICC = ICC_DATE_CLASSES;
			InitCommonControlsEx(&icex);
		}
		CULDateTimePicker::~CULDateTimePicker()
		{
		}
		BOOL CULDateTimePicker::Create(HWND hParentWnd,WORD wID,int x,int y,int cx,int cy,DWORD dwStyle)
		{
			m_hWnd = CreateWindowEx(0,
				DATETIMEPICK_CLASS,
				NULL,
				dwStyle,
				x,y,cx,cy,
				hParentWnd,
				(HMENU)wID,
				ULOther::ULGetResourceHandle(),
				this);
			if(!m_hWnd)
				return FALSE;
			return Attach(m_hWnd);
		}
	}
}
