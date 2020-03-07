///\file ULStatic.h
///\brief Заголовочный файл класса обёртки для STATIC контрола(23.12.2008)
#pragma once
#ifndef _STATIC_H__
#define _STATIC_H__
#include "ULSubClass.h"
namespace ULWnds
{
	namespace ULControls
	{
		///\class CULStatic
		///\brief Класс обёртка для STATIC контрола(23.12.2008)
		class CULStatic:
			public CULSubClass
		{
		public:
			///\brief Конструктор
			CULStatic();
			///\brief Функция для создания эдита
			///\param hParentWnd - хендл родительского окна
			///\param pszText - текст
			///\param uID - идентификатор контрола
			///\param x,y - координата контрола
			///\param cx,cy - размер 
			///\param dwStyle - стиль 
			///\param dwStyleEx - расширенный стиль 
			///\return TRUE в случае успеха
			BOOL Create(HWND hParentWnd,LPCTSTR pszText,UINT uID,int x,int y,
				int cx,int cy,DWORD dwStyle=WS_CHILD|WS_VISIBLE,
				DWORD dwStyleEx=WS_EX_CLIENTEDGE|WS_EX_ACCEPTFILES);
		};
	}
}
#endif //_STATIC_H__
