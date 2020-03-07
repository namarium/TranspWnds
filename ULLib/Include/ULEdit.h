///\file ULEdit.h
///\brief Заголовочный файл класса обёртки для EDIT контрола(16.08.2007)
#pragma once
#ifndef __EDIT__H_
#define __EDIT__H_
#include "ULSubClass.h"
namespace ULWnds
{
	namespace ULControls
	{
		///\class CULEdit
		///\brief Класс обёртка для EDIT контрола(16.07.2007)
		class CULEdit:public CULSubClass
		{
		public:
			///\brief Конструктор
			CULEdit();
			///\brief Деструктор
			virtual ~CULEdit();
			///\brief Функция для создания эдита
			///\param hParentWnd - хендл родительского окна
			///\param pszText - текст
			///\param uID - ID эдита
			///\param x,y - координата кнопки
			///\param cx,cy - размер 
			///\param dwStyle - стиль 
			///\param dwStyleEx - расширенный стиль эдита
			///\return TRUE в случае успеха
			BOOL Create(HWND hParentWnd,LPCTSTR pszText,UINT uID,int x,int y,
				int cx,int cy,DWORD dwStyle=WS_CHILD|WS_VISIBLE|ES_LEFT|WS_TABSTOP|ES_AUTOHSCROLL,
				DWORD dwStyleEx=WS_EX_CLIENTEDGE|WS_EX_ACCEPTFILES);
		//==================inline functions============================
			///\brief Выделяет строку
			///\param nStartChar - начальный символ выделения
			///\param nEndChar - конечный символ выделения
			inline void SetSel(int nStartChar,int nEndChar)
				{SendMessage(EM_SETSEL,(WPARAM)nStartChar,(LPARAM)nEndChar);}
			///\brief Возвращает границы выделения строки
			///\param nStartChar - начальный символ выделения
			///\param nEndChar - конечный символ выделения
			inline void GetSel(int* nStartChar,int* nEndChar)
				{SendMessage(EM_GETSEL,(WPARAM)nStartChar,(LPARAM)nEndChar);}
			///\brief Устанавливает предел колличества символов
			///\param nLimit - предел колличества символов
			inline void SetLimitText(int nLimit)
				{SendMessage(EM_LIMITTEXT,(WPARAM)nLimit,0);}
			///\brief Возвращает предел колличества символов
			///\return предел колличества символов
			inline int GetLimitText()
				{return (int)SendMessage(EM_GETLIMITTEXT,0,0);}
			///\brief Определяет номер символа и строки
			///\param xPos,yPos - координаты
			///\param pnChar - возвращаемый номер символа
			///\param pnLine - возвращаемый номер строки
			inline void CharFromPos(int xPos,int yPos,WORD* pnChar,WORD* pnLine)
			{
				int ret=(int)SendMessage(EM_CHARFROMPOS,0,MAKELPARAM(xPos,yPos));
				if(pnChar!=NULL)
					*pnChar=LOWORD(ret);
				if(pnLine!=NULL)
					*pnLine=HIWORD(ret);
			}		
			///\brief Определяет координаты символа
			///\param nChar - номер символа
			///\param pXPos,pYPos - возвращаемые координаты
			inline void PosFromChar(int nChar,int* pXPos,int* pYPos)
			{
				int ret=(int)SendMessage(EM_POSFROMCHAR,WPARAM(nChar));
				if(pXPos!=NULL)
					*pXPos=LOWORD(ret);
				if(pYPos!=NULL)
					*pYPos=HIWORD(ret);
			}		
			///\brief Устанавливает форматируемую область
			///\param rc - форматируемая область
			inline void SetRect(RECT rc)
				{SendMessage(EM_SETRECT,(WPARAM)0,(LPARAM)&rc);}
			///\brief Получает форматируемую область
			///\param lpRect - форматируемая область
			inline void GetRect(LPRECT lpRect)
				{SendMessage(EM_GETRECT,(WPARAM)0,(LPARAM)lpRect);}
			///\brief Устанавливает/снимает режим только для чтения
			///\param fReadOnly - флаг режима 
			inline void SetReadOnly(BOOL fReadOnly=TRUE)
				{SendMessage(EM_SETREADONLY,(WPARAM)fReadOnly);}
			
		protected:
			///\brief Обработчик сообщения WM_DROPFILES
			virtual LRESULT OnDropFiles(WPARAM,LPARAM);
		};
	}
}
#endif //__EDIT__H_
