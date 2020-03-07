///\file ULStatusBarCtrl.h
///\brief Заголовочный файл класса статусбара размещенного на плавающей панельке(12.10.2007)
#pragma once
#ifndef __UL_ULSTATUSBARCTRL_H__
#define __UL_ULSTATUSBARCTRL_H__
#include "ULWndCtrl.h" 
#include "ULStatusBar.h" 
namespace ULWnds
{
	namespace ULWndCtrls
	{
		///\class CULStatusBarCtrl
		///\brief Класс статусбара размещенного на плавающей панельке(12.10.2007)
		class CULStatusBarCtrl:
			public CULWndCtrl
		{
		protected:
			///\brief Непосредственно сам статубар
			ULBars::CULStatusBar m_StatusBar;
		public:
			///\brief Конструктор
			CULStatusBarCtrl(void);
			///\brief Конструктор копирования
			CULStatusBarCtrl(CULStatusBarCtrl&);
			///\brief Деструктор
			virtual ~CULStatusBarCtrl(void);
			///\brief оператор копирования
			void operator=(CULStatusBarCtrl&);
			///\brief Функция возвращает ссылку на статубар
			inline ULBars::CULStatusBar& GetStatusBar(){return m_StatusBar;};
			///\brief Функция создания статусбарконтрола
			///\param hParentWnd - хендл родителя(носителя)
			///\param nStatusID - ID статусбара
			///\param fGrip - разметка на статусбаре для изменения размеров
			BOOL CreateStatusBar(HWND hParentWnd,short nStatusID,BOOL fGrip=TRUE);
		protected:
			///\brief Обработчик WM_SIZE
			virtual LRESULT OnSize(WPARAM nType,LPARAM size);
		};
	}
}
#endif //__UL_ULSTATUSBARCTRL_H__
