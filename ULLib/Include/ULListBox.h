///\file ULListBox.h
///\brief Заголовочный файл класса обёртки лдя LISTBOX контрола(08.09.2007)
#pragma once
#ifndef __LISTBOX__H_
#define __LISTBOX__H_
#include "ULSubClass.h"
namespace ULWnds
{
	namespace ULControls
	{
		///\class CULListBox
		///\brief Класс обёртка лдя LISTBOX контрола(08.09.2007)
		class CULListBox:public CULSubClass
		{
		public:
			///\brief Конструктор
			CULListBox();
			///\brief Деструктор
			virtual ~CULListBox();
			///\brief Функция для создания листбокса
			///\param hParentWnd - хендл родительского окна
			///\param uID - ID кнопки
			///\param x,y - координата кнопки
			///\param cx,cy - размер кнопки
			///\param dwStyle - стиль эдита
			///\param dwStyleEx - расштренный стиль эдита
			///\return TRUE в случае успеха
			BOOL Create(HWND hParentWnd,UINT uID,int x,int y,
				int cx,int cy,DWORD dwStyle=WS_CHILD|WS_VISIBLE|WS_TABSTOP|LBS_HASSTRINGS,
				DWORD dwStyleEx=WS_EX_CLIENTEDGE);
		//==================inline functions============================

			///\brief добавляет строку в листбокс
			///\param pcszStr - строка
			///\return номер добавленной строки
			inline int AddString(LPCTSTR pcszStr)
				{return (int)SendMessage(LB_ADDSTRING,0,(LPARAM)pcszStr);}
			///\brief удаляет  строку из листбокса
			///\param nItem - номер строки
			///\return колличество оставшихся строк
			inline int DeleteString(int nItem)
				{return (int)SendMessage(LB_DELETESTRING,(WPARAM)nItem);}
			///\brief вставляет  строку в определённую позицию
			///\param nItem - номер строки
			///\param pcszStr - строка
			///\return номер вставленной строки
			inline int InsertString(int nItem,LPCTSTR pcszStr)
				{return (int)SendMessage(LB_INSERTSTRING,(WPARAM)nItem,(LPARAM)pcszStr);}
			///\brief возвращает колличество элементов 
			///\return колличество элементов 
			inline int GetCount()
				{return (int)SendMessage(LB_GETCOUNT);}
			///\brief возвращает выбранный элемент при режиме одиночного выделения
			///\return выделенный элемент, если такой есть. Если нет то вернёт LB_ERR(-1)
			inline int GetCurSel()
				{return (int)SendMessage(LB_GETCURSEL);}
			///\brief устанавливает выбранный элемент при режиме одиночного выделения
			///\param nItem - номер строки
			///\return в случае ошибки вернёт LB_ERR(-1)
			inline int SetCurSel(int nItem)
				{return (int)SendMessage(LB_SETCURSEL,(WPARAM)nItem);}
			///\brief перемещает на передний план итемы, чтоб было видно указанный итем
			///\param nItem - номер строки
			///\return в случае ошибки вернёт LB_ERR(-1)
			inline int SetTopIndex(int nItem)
				{return (int)SendMessage(LB_SETTOPINDEX,(WPARAM)nItem);}
			///\brief возвращает расположение указанноой строки
			///\param nItem - номер строки
			///\param lpRect - расположение
			///\return в случае ошибки вернёт LB_ERR(-1)
			inline int GetItemRect(int nItem,LPRECT lpRect)
				{return (int)SendMessage(LB_GETITEMRECT,(WPARAM)nItem,(LPARAM)lpRect);}
			///\brief возвращает высоту строки
			///\return высота строки
			inline int GetItemHeight()
				{return (int)SendMessage(LB_GETITEMHEIGHT);}		
			///\brief Удаляет все итемы
			inline void ResetContent()
				{SendMessage(LB_RESETCONTENT);}
			///\brief возвращает строку из определённой позиции
			///\param nItem - номер строки
			///\param pcszStr - строка
			///\return число скопированных символов в буфер,в случае ошибки вернёт LB_ERR(-1)
			inline int GetText(int nItem,LPCTSTR pcszStr)
				{return (int)SendMessage(LB_GETTEXT,(WPARAM)nItem,(LPARAM)pcszStr);}
			///\brief возвращает длину строки из указанной позиции
			///\param nItem - номер строки
			///\return число символов,в случае ошибки вернёт LB_ERR(-1)
			inline int GetTextLen(int nItem)
				{return (int)SendMessage(LB_GETTEXTLEN,(WPARAM)nItem);}
			///\brief возвращает номер итема по указанной абсолютной координате
			///\param pt - абсолютная координата
			///\return номер итема,в случае ошибки вернёт в старшем слове 1
			inline int ItemFromPoint(POINT pt)
				{return (int)SendMessage(LB_ITEMFROMPOINT,0,MAKELPARAM(pt.x,pt.y));}
			///\brief Устанавливает значение для ячейки
			///\param nItem - номер ячейки
			///\param pData - значение
			///\return TRUE в случае успеха
			inline BOOL SetItemData(int nItem,LPVOID pData)
				{return ((SendMessage(LB_SETITEMDATA,WPARAM(nItem),LPARAM(pData)))!=LB_ERR);}
		};
	}
}
#endif //__LISTBOX__H_
