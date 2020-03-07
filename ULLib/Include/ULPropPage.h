///\file ULPropPage.h
///\brief Заголовочный файл класса root окна(21.07.2007)
#pragma once
#ifndef __UL_PROPPAGE_H__
#define __UL_PROPPAGE_H__
#include "ULDlg.h"
namespace ULWnds
{
	namespace ULDlgs
	{
		///\class CULPropPage
		///\brief Класс страницы для окна свойств(2006)
		class CULPropPage:public CULDlg
		{
		protected:
			///\brief Функция окна для созданной страницы
			static LRESULT WndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam);
		public:
			///\brief Конструктор
			CULPropPage();
			///\brief Деструктор
			virtual ~CULPropPage();
			///\brief функция создаёт страницу свойств
			///\param TemplID - ID ресурса диалога
			///\param szTitle - загловок
			///\param szSubTitle - подзагловок для режима мастера
			///\param hIcon - хендл иконки
			///\param fHelp - флаг наличия кнопки "Помощь"
			///\return хендл на созданную страницу свойств
			HPROPSHEETPAGE Create(short TemplID, 
				LPCTSTR szTitle=NULL,
				LPCTSTR szSubTitle=NULL, 
				HICON hIcon=NULL,
				BOOL fHelp=FALSE);
			///\brief Делает активной Apply
			void DoChange();
			///\brief Делает неактивной Apply
			void DoUnChange();
		protected:
			///\brief обработчик сообщений от WM_ONINITDIALOGz
			virtual LRESULT OnInitDialog(WPARAM,LPARAM);
			///\brief обработчик сообщений от WM_NOTIFY
			virtual LRESULT OnNotify(WPARAM,LPARAM);
			///\brief Обработчик сообщений от PSN_SETACTIVE
			///	возникает когда страницу делают текущей в окне 
			///	страниц свойств
			///\param dwFlags - флаг для того чтобы выставить 
			///	кнопки для окна страницы свойств
			///	PSWIZB_NEXT,PSWIZB_BACK,PSWIZB_FINISH
			virtual LRESULT OnSetActive(DWORD dwFlags);
			///\brief Обработчик сообщений от PSN_WIZNEXT,т.е. от кнопки Next
			///\param nResult - результат выполнения страницы, если -1,
			///	то страница не переключится может быть ID диалога следующей страницы
			virtual LRESULT OnWizNext(short nResult);
			///\brief Обработчик сообщений от PSN_WIZFINISH,т.е.от кнопки Finish
			///\param fFinish - результат выполнения страницы
			///	0/!0 - успешно/не закроется
			virtual LRESULT OnWizFinish(BOOL fFinish);
			///\brief обработчик сообщений от PSN_RESET,т.е.от кнопки
			///	Cancel или "X"
			///\param fXorCancel - по МСДН 1 = "X", 0=Cancel , но у меня 
			///	только 0
			virtual LRESULT OnReset(BOOL fXorCancel);
			///\briefВызывается при нажатии OK или Apply
			///\param nReturn - возвращаемое значение, говорящее об 
			///	отсутствии ошибок на странице
			virtual LRESULT OnApply(BYTE nReturn);
			///\brief Вызывается при нажатии по кнопке "Справка"
			///\param hPropSheet - хендл на шит
			virtual LRESULT OnHelp(HWND hPropSheet);
			///\brief Вызывается,когда текущая страница перекрывается 
			///	следующей страницей или нажимается OK или Apply,
			///	если в DWL_MSGRESULT стоит не 0
			///\param fProblem - TRUE запрещает переход на следующую страницу без активации
			virtual LRESULT OnKillActivate(BOOL fProblem);
		};
	}
}
#endif //__UL_PROPPAGE_H__
