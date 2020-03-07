///\file ULSlider.h
///\brief заголовочный фаил класса обёртки для slider control
#ifndef _ULSLIDER_H__
#define _ULSLIDER_H__
#include "ULSubClass.h"
#include <commctrl.h>

namespace ULWnds
{
	namespace ULControls
	{
		///\class CULSlider
		///\brief класс обёртка для slider control
		class CULSlider:
			public CULSubClass
		{
		public:
			///\brief конструктор
			CULSlider();
			///\brief Функция для создания слайдера
			///\param hParentWnd - хендл родительского окна
			///\param uID - ID слайдера
			///\param szCaption - заголовок слайдера
			///\param x,y - координата слайдера
			///\param cx,cy - размер слайдера
			///\param dwStyle - стиль слайдера
			///\param dwStyleEx - расширенный стиль 
			///\return TRUE в случае успеха
			BOOL Create(HWND hParentWnd,UINT uID,LPCTSTR szCaption,int x,int y,int cx,int cy,
				DWORD dwStyle=WS_CHILD|WS_VISIBLE|TBS_AUTOTICKS,DWORD dwStyleEx=NULL);
			///\brief функция задаёт диапазон для слайдера
			///\param nMin - минимальноe значение
			///\param nMax - максимальное значение
			///\param fRedraw - флаг перерисовки слайдера при изменении диапазона
			void SetRange(int nMin,int nMax,BOOL fRedraw=TRUE);
			///\brief функция задаёт минимальное значение для слайдера
			///\param nMin - минимальноe значение
			///\param fRedraw - флаг перерисовки слайдера при изменении диапазона
			void SetRangeMin(int nMin,BOOL fRedraw=TRUE);
			///\brief функция задаёт максимальное значение для слайдера
			///\param nMax - максимальное значение
			///\param fRedraw - флаг перерисовки слайдера при изменении диапазона
			void SetRangeMax(int nMax,BOOL fRedraw=TRUE);
			///\brief устанавливает позицию бегунка слайдера
			///\param nPos - позиция бегунка
			///\param fRedraw - флаг перерисовки слайдера при позиции бегунка
			void SetPos(int nPos,BOOL fRedraw=TRUE);
			///\brief возвращает позицию бегунка слайдера
			///\return позиция бегунка
			int GetPos();
			///\brief устанавливает длину бегунка
			///\param nLen - длина бегунка
			void SetThumbLenght(UINT nLen);
			///\brief устанавливает частоту отрисовки маркеров
			///\param nFreq - частота отрисовки маркеров
			void SetTicFreq(UINT nFreq);
			///\brief устанавливает шаг изменения позиции курсором клавиатуры
			///\param nLineSize - шаг изменения позиции
			///\return предыдущий шаг изменения позиции
			UINT SetLineSize(UINT nLineSize);
			///\brief устанавливает шаг изменения позиции кнопками PageUp/PageDown \n
			/// и кликом мыши по свободной области
			///\param nPageSize - шаг изменения позиции
			///\return предыдущий шаг изменения позиции
			UINT SetPageSize(UINT nPageSize);
		};
	}
}
#endif //_ULSLIDER_H__
