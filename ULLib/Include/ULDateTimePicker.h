///\file ULDateTimePicker.h 
///\brief фаил объявления класса елемента управления даты и времени
#ifndef _ULDATETIMEPICKER_H__
#define _ULDATETIMEPICKER_H__
#include "ULSubClass.h"
#include "commctrl.h"

namespace ULWnds
{
	namespace ULControls
	{
		///\class CULDateTimePicker
		///\brief класс елемента управления даты и времени
		class CULDateTimePicker:
			public ULWnds::CULSubClass
		{
		public:
			///\brief Конструктор
			CULDateTimePicker();
			///\brief деструктор
			~CULDateTimePicker();
			///\brief Создает елемент управления даты и времени
			///\param hParentWnd - хендл окна родителя
			///\param wID - идентификатор контрола
			///\param x,y - координаты контрола
			///\param cx,cy - размеры
			///\param dwStyle - стиль
			///\return TRUE в случае успеха
			BOOL Create(HWND hParentWnd,WORD wID,int x,int y,int cx,int cy,DWORD dwStyle=
				WS_BORDER|WS_CHILD|WS_VISIBLE|DTS_SHOWNONE);
		};
	}
}

#endif //_ULDATETIMEPICKER_H__
