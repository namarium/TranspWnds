///\file ULToolBarCtrl.h
///\brief Заголовочный файл класса тулбара размещенного на плавающей панельке(12.10.2007)
#pragma once
#ifndef __UL_ULTOOLBARCTRL_H__
#define __UL_ULTOOLBARCTRL_H__
#include "ULToolBar.h"
#include "ULWndCtrl.h"
namespace ULWnds
{
	namespace ULWndCtrls
	{
		///\class CULToolBarCtrl
		///\brief Класс тулбара размещенного на плавающей панельке(12.10.2007)
		class CULToolBarCtrl :
			public CULWndCtrl
		{
		protected:
			///\brief Константы необходимые для позиционирования тулбара 
			///на панельке
			enum enConsts
			{
				cNonGripperBorder	=2,
				cGripperBorder		=7,
				cBorderButtonDock	=4,
				cBorderButtonFloat	=7,
				cGripperSize		=16
			};
			///\brief Непосредственно сам тулбар
			ULBars::CULToolBar m_ToolBar;
		public:
			//================================================
			///\brief Конструктор
			//================================================
			CULToolBarCtrl(void);
			///\brief Конструктор копирования
			CULToolBarCtrl(CULToolBarCtrl&);
			///\brief Деструктор
			virtual ~CULToolBarCtrl(void);
			///\brief оператор копирования
			void operator=(CULToolBarCtrl&);
			///\brief Функция возвращает ссылку на тулбар
			inline ULBars::CULToolBar& GetToolBar(){return m_ToolBar;};
			///\brief Функция создания тулбарконтрола
			///\param hParentWnd - хендл родителя(носителя)
			///\param tbID - идентификатор тулбара
			///\param nXPos,nYPos - позиция тулбарконтрола при создании в
			///	плавающем состоянии
			///\param dxButton,dyButton - размер кнопки тулбара
			///\param dxBitmap,dyBitmap - размер картинки для кнопки тулюара
			///\param dwDockedState - назначение стыковки на момент создания
			///\param fDocked - состояние стыковки на момент создания
			///\param tbStyle - стиль тулбара
			///\param tbExStyle - рачширенный стиль тулбара
			///\param dwDockingStyles - стили панели
			BOOL CreateToolBarCtrl(HWND hParentWnd,
							short tbID,
							int nXPos,
							int nYPos,
							int dxButton,
							int dyButton,
							int dxBitmap,
							int dyBitmap,
							DWORD dwDockedState,
							BOOL fDocked=TRUE,
							DWORD tbStyle=WS_CHILD|WS_VISIBLE|TBSTYLE_TOOLTIPS|
											TBSTYLE_AUTOSIZE|CCS_NODIVIDER,
							DWORD tbExStyle=TBSTYLE_EX_DRAWDDARROWS,
							DWORD dwDockingStyles=dsDrawGripperDocked|
												dsDrawGripperFloating|
												dsUseBorders|dsBorderTop|dsBorderLeft|
												dsBorderBottom|dsBorderRight|
												dsAllowDockAll);
		protected:
			///\brief Обработчик WM_SIZE
			virtual LRESULT OnSize(WPARAM nType,LPARAM point);
		};
	}
}
#endif //__UL_ULTOOLBARCTRL_H__
