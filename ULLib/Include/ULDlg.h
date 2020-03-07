///\file ULDlg.h
///\brief Заголовочный файл класса диалогого окна(2006)
#pragma once
#ifndef __UL_DLG_H__
#define __UL_DLG_H__

#include "ULSubClass.h"
namespace ULWnds
{
	namespace ULDlgs
	{
		///\class CULDlg
		///\brief Класс диалогого окна(2006)
		class CULDlg:public CULWnd
		{
		protected:
			///\brief Функция окна
			static LRESULT WndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam);
			///\brief Идентификатор шаблона диалога,
			///передаётся при создании окна 
			short m_idTempl;
			///\brief Идентификатор иконки,
			///передаётся при создании окна 
			short m_idIcon;
			///\brief Необходима для определения модальный или 
			///немодальный диалог
			BOOL m_fModal;
		public:
			///\brief Конструктор поумолчанию
			CULDlg(short idIcon=0);
			///\brief деструктор
			virtual ~CULDlg(void);
			///\brief Создаёт немодальный диалог
			///\param idTempl - идентификатор шаблона диалога
			///\param hParentWnd - хэндл родительского окна
			HWND Create(short idTempl,	
				HWND hParentWnd=NULL);	
			///\brief Создаёт модальный диалог
			///\param idTempl - идентификатор шаблона диалога
			///\param hParentWnd - хэндл родительского окна
			///\return IDOK или IDCANCEL
			int CreateModal(short idTempl,
				HWND hParentWnd=NULL);	  
			///\brief Служит для определения модальности диалога
			///\return TRUE если модально
			inline BOOL IsModal()
				{return m_fModal;}
		//==================inline functions============================
			///\brief отправка сообщения итему лежащему на диалоге
			///\param nIDDlgItem - ID итема 
			///\param Msg - Сообщение
			///\param wParam - Параметр 1
			///\param lParam - Параметр 2
			///\return ответ обработчика сообщения
			inline LRESULT SendDlgItemMessage(int nIDDlgItem,UINT Msg,
				WPARAM wParam,LPARAM lParam)
				{return ::SendDlgItemMessage(*this,nIDDlgItem,Msg,wParam,lParam);};
			///\brief Получает текст из контрола на диалоге
			///\param nIDDlgItem - ID диалога
			///\param lpString - страка
			///\param nMaxCount - длина строки
			///\return колличество символов TCHAR в случае успеха, иначе NULL
			inline UINT GetDlgItemText(int nIDDlgItem,LPTSTR lpString,int nMaxCount)
				{return ::GetDlgItemText(*this,nIDDlgItem,lpString,nMaxCount);}
			///\brief Устанавливает текст в контрол на диалоге
			///\param nIDDlgItem - ID диалога
			///\param lpString - страка
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL SetDlgItemText(int nIDDlgItem,LPCTSTR lpString)
				{return ::SetDlgItemText(*this,nIDDlgItem,lpString);}
			///\brief Устанавливает текст на контрол диалога интерпритируя его из числа
			///\param nIDDlgItem - ID диалога
			///\param uValue - число
			///\param fSigned - знаковость
			///\return TRUE в случае успеха, иначе FALSE
			inline BOOL SetDlgItemInt(int nIDDlgItem,UINT uValue,BOOL fSigned)
				{return ::SetDlgItemInt(*this,nIDDlgItem,uValue,fSigned);}
			///\brief Возвращает число содержащееся на контроле диалога
			///\param nIDDlgItem - ID диалога
			///\param lpTranslated - возвращемый результат операции(TRUE в случае успеха, иначе FALSE)
			///\param fSigned - знаковость
			///\return число с контрола
			inline UINT GetDlgItemInt(int nIDDlgItem,BOOL *lpTranslated,BOOL fSigned)
				{return ::GetDlgItemInt(*this,nIDDlgItem,lpTranslated,fSigned);}
			///\brief Передаёт фокус дочернему контролу диалога
			///\param hWndCtrl - Хэндл контрола
			inline void GotoDlgCtrl(HWND hWndCtrl)
				{SendMessage(WM_NEXTDLGCTL, (WPARAM)hWndCtrl, TRUE);}
			///\brief Передаёт фокус следующему дочернему 
			///	контролу диалога согласно WS_TABSTOP
			inline void NextDlgCtrl()
				{SendMessage(WM_NEXTDLGCTL);}
			///\brief Передаёт фокус предыдущему дочернему 
			///	контролу диалога согласно WS_TABSTOP
			inline void PrevDlgCtrl()
				{SendMessage(WM_NEXTDLGCTL, 1);}
		protected:
			///\brief Обработчик WM_INITDIALOG 
			virtual LRESULT OnInitDialog(WPARAM,LPARAM /*lParam*/);
			///\brief Обработчик WM_DESTROY
			virtual LRESULT OnDestroy(WPARAM /*wParam*/,LPARAM /*lParam*/);
			///\brief Обработчик BN_CLICKED кнопки IDOK 
			virtual void OnOK(WORD id,HWND hControl);
			///\brief Обработчик BN_CLICKED кнопки IDCANCEL 
			virtual void OnCancel(WORD id,HWND hControl);
		};
	}
}
#endif//__UL_DLG_H__
