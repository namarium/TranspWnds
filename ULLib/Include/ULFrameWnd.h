///\file ULFrameWnd.h
///\brief Заголовочный файл класса главного окна(2006)
#pragma once

#ifndef __UL_ULFRAMEWND_H__
#define __UL_ULFRAMEWND_H__
#include "ULWnd.h"
namespace ULWnds
{
	namespace ULFrames
	{
		///\class CULFrameWnd
		///\brief Класс главного окна(2006)
		class CULFrameWnd:
			public CULWnd
		{
		public:
			///\brief Конструктор
			CULFrameWnd(void);
			///\brief Конструктор копирования
			CULFrameWnd(CULFrameWnd& FrameWnd);
			///\brief Деструктор
			virtual ~CULFrameWnd(void);
			///\brief Оператор копирования
			virtual void operator = (CULFrameWnd&);
			///\brief Функция создания главного окна
			///\param szWindowName - заголовок окна
			///\param MenuID - ID меню из ресурсов
			///\param IconID - ID иконки из ресурсов
			///\param IconSmID - ID маленькой иконки из ресурсов
			///\param BGColor - цвет клиентской области
			///\param dwStyle - стиль окна
			///\param dwStyleEx - расширенный стиль окна		
			///\return TRUE в случае успеха
			BOOL Create(LPCTSTR szWindowName,				
						short MenuID, 						
						short IconID, 						
						short IconSmID,						
						short BGColor,						
						DWORD dwStyle=WS_OVERLAPPEDWINDOW,	
						DWORD dwStyleEx=WS_EX_APPWINDOW);	
		protected:
			///\brief Обработчик WM_DESTROY
			virtual LRESULT OnDestroy(WPARAM /*wParam*/,LPARAM /*lParam*/);
			///\brief Обработчик WM_CREATE
			virtual LRESULT OnCreate(WPARAM /*wParam*/,LPARAM);
		};
	}
}
#endif//__UL_ULFRAMEWND_H__
