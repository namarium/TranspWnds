///\file ULProgressBar.h
///\brief заголовочный фаил класса обёртки для ProgressBar
#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__
#include "ULSubClass.h"
namespace ULWnds
{
	namespace ULControls
	{
		///\class CULProgressBar
		///\brief Класс контрола прогрессбара
		class CULProgressBar:
			public ULWnds::CULSubClass
		{
		public:
			///\brief конструктор
			CULProgressBar();
			///\brief Создаёт прогрессбар
			///\param hParentWnd - хендл родительского окна
			///\param uID - ID слайдера
			///\param x,y - координата слайдера
			///\param cx,cy - размер слайдера
			///\param dwStyle - стиль слайдера
			///\param dwStyleEx - расширенный стиль 
			///\return TRUE в случае успеха
			BOOL Create(HWND hParentWnd,UINT uID,int x,int y,int cx,int cy,
				DWORD dwStyle=WS_CHILD|WS_VISIBLE,DWORD dwStyleEx=NULL);
			///\return Возвращает текущую позицию прогрессбара
			UINT GetPos();
			///\brief устанавливает новую абсолютную позицию
			///\param nPos - значение позиции
			///\return предыдущая позиция
			UINT SetPos(UINT nPos);
			///\brief Возвращает диапазон прогрессбара
			///\param pnLower,pnUpper - указатели на нижнее и верхнее значение
			void GetRange(int* pnLower,int* pnUpper);
			///\brief Возвращает диапазон прогрессбара
			///\param fWhichLimit - если TRUE, то вернётся нижняя граница, иначе FALSE
			///\return выбранная граница
			int GetRange(BOOL fWhichLimit);
			///\brief установка диапазона
			///\param nLower,nUpper - нижний и верхний предел диапазона
			void SetRange(short nLower,short nUpper);
			///\brief установка диапазона 32х битными значениями
			///\param nLower,nUpper - нижний и верхний предел диапазона
			void SetRange32(int nLower,int nUpper);
			///\brief устанавливает смещение от текущей позиции
			///\param nIncrement - смещение
			///\return предыдущее значение прогресса
			int DeltaPos(int nIncrement);
			///\brief устанавливает цвет фона прогрессбара
			///\param clrNew - новый цвет
			///\return старый цвет
			COLORREF SetBkColor(COLORREF clrNew);
#if _WIN32_WINNT >= 0x0501
			///\brief переключает режим отображения
			///\param fEnable - флаг переключения
			///return предыдущее значение
			BOOL SetMarquee(BOOL fEnable);
#endif
			///\brief установка новой позиции
			///\param nStep - значение новой позиции
			///\return предыдущая позиция
			int SetStep(int nStep);
			///\brief смещение позиции на одну единицу
			///\return предыдущая позиция
			int StepIt();
		};
	}
}
#endif //__PROGRESSBAR_H__
