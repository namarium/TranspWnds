///\file ULSkinDlg.h
///\brief Заголовочный файл класса скин-диалогого окна(17.10.2007)
#pragma once
#ifndef __UL_SKIN_DLG_H__
#define __UL_SKIN_DLG_H__


#include "ULDlg.h"
#include "ULGDI.h"
namespace ULWnds
{
	namespace ULDlgs
	{
		///\class CULSkinDlg
		///\brief Класс скин-диалогого окна(17.10.2007)
		class CULSkinDlg:
			public CULDlg
		{
		private:
			///\brief Хендл битмапа для региона диалогового окна
			HBITMAP m_hRgnBitmap;
			///\brief Хендл битмапа для заднего фона диалогового окна
			HBITMAP m_hBGBitmap;
			///\brief контекст памяти
			ULGDI::ULDC::CULDC m_MemDC;
			///\brief размер диалога после установки картинки
			SIZE m_SizeDlg;
		public:
			///\brief Конструктор
			CULSkinDlg();
			///\brief Деструктор
			virtual ~CULSkinDlg();
			///\brief Получает регион из битмапа
			///\param hRgnBitmap - битмап для региона
			///\return TRUE если регион создан
			BOOL SetBitmapRgn(HBITMAP hRgnBitmap);
			///\brief устанавливает задний фон
			///\param hBGBitmap - битмап фона
			void SetBitmapBG(HBITMAP hBGBitmap);
		protected:
			///\brief Обработчик сообхения WM_INITDIALOG
			virtual LRESULT OnInitDialog(WPARAM wParam,LPARAM lParam);
			///\brief Обработчик сообхения WM_ERASEBKGND
			virtual LRESULT OnEraseBkGnd(WPARAM,LPARAM);	
		};
	}
}
#endif//__UL_SKIN_DLG_H__
