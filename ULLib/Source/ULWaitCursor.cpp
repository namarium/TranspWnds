///\file ULWaitCursor.cpp
///\brief фаил реализации класса курсора ожидания
#include "..\..\ULLib\Include\ULWaitCursor.h"

namespace ULOther
{
	CULWaitCursor::CULWaitCursor()
	{
		m_hWaitCursor=::LoadCursor(NULL,IDC_WAIT);
		m_hCursor=::SetCursor(m_hWaitCursor);
	}
	CULWaitCursor::~CULWaitCursor()
	{
		::SetCursor(m_hCursor);
	}
	void CULWaitCursor::Restore()
	{
		::SetCursor(m_hCursor);
	}
}
