///\file ULMDIChildWnd.h
///\brief Заголовочный файл класса дочернего окна архетектуры MDI(03.08.2007)
#pragma once
#ifndef __UL_ULMDICHILDWND_H__
#define __UL_ULMDICHILDWND_H__
#include "ULWnd.h"
namespace ULWnds
{
	namespace ULFrames
	{
		///\class CULMDIChildWnd
		///\brief Класс дочернего окна архетектуры MDI(03.08.2007)
		class CULMDIChildWnd:
			public CULWnd
		{
		protected:
			///\brief Функция дочернего окна
			static LRESULT WndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam);
		public:
			///\brief Сообщение, которое чайлд посылает клиенту при закрытии
			///\param WPARAM - HWND чайлда
			///\param LPARAM - не используеться
			enum{CLM_DESTROYCHILD=WM_APP+1};
			///\brief Конструктор
			CULMDIChildWnd(void);
			///\brief Конструктор копирования
			CULMDIChildWnd(CULMDIChildWnd& MDIChildWnd);
			///\brief Деструктор
			virtual ~CULMDIChildWnd(void);
			///\brief Оператор копирования
			virtual void operator = (CULMDIChildWnd&);
			///\brief Функция создания дочернего окна
			///\param szWindowName - заголовок окна
			///\param x,y - координаты окна
			///\param cx,cy - размеры окна
			///\param IconID - ID иконки из ресурсов
			///\param hClientWnd - клиентское окно
			///\param BGColor - цвет клиентской области
			///\param dwChildStyle - стиль дочернего  окна
			///\return хкндл созданного окна
			HWND Create(TCHAR* szWindowName,
						int x,
						int y,
						int cx,
						int cy,
						short IconID,
						HWND hClientWnd,
						short BGColor=COLOR_WINDOW,
						DWORD dwChildStyle=MDIS_ALLCHILDSTYLES|WS_CLIPCHILDREN);
		protected:
			///\brief Обработчик WM_CREATE
			virtual LRESULT OnCreate(WPARAM /*wParam*/,LPARAM);
			///\brief Обработчик WM_NCDESTROY
			virtual LRESULT OnNcDestroy(WPARAM ,LPARAM);	
			///\brief Обработчик WM_SHOWWINDOW
			virtual LRESULT OnShowWindow(WPARAM ,LPARAM);
		};
	}
}
#endif//__UL_ULMDICHILDWND_H__
