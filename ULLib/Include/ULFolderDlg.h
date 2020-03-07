///\file FolderDlg.h
///\brief заголовочный фаил для класса выбора директории
#ifndef _ULFOLDERDLG_H__
#define _ULFOLDERDLG_H__
#include <shlobj.h>
#include "ULDlg.h"
namespace ULWnds
{
	namespace ULDlgs
	{
		///\class CULFolderDlg
		///\brief класс выбора директории
		class CULFolderDlg:
			public CULDlg
		{
		protected:
			BROWSEINFO m_bi;
			///\brief Заглушка для ULWnds::ULDlgs::CULDlg::Create
			HWND Create(short /*idTempl*/,	
				HWND /*hParentHWND=NULL*/)
			{return NULL;}
		public:
			///\brief конструктор
			CULFolderDlg(HWND hParentWnd=NULL,UINT ulFlags=/*BIF_NEWDIALOGSTYLE|*/BIF_DONTGOBELOWDOMAIN/*|BIF_USENEWUI*/);
			///\brief деструктор
			~CULFolderDlg();
			///\brief отображает диалог выбора директории
			///\returun IDOK/IDCANCEL
			int CreateModal();
			///\brief возвращает выбранный путь
			///\return выбранный путь
			LPCTSTR GetPath();
			///\brief возвращает выбранный путь
			///\param lpszPath - указатель на буфер, в который будет скопирован путь
			///\param nLen - размер буфера
			///\return TRUE в случае успеха
			BOOL GetPath(LPTSTR lpszPath,WORD nLen);
		protected:
			///\brief Функция окна
			static LRESULT WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
			///\brief callback для сабклассинга
			static int CALLBACK BrowseCallbackProc(HWND hDlg,UINT uMsg,LPARAM lParam,LPARAM lpData);
		protected:
			///\brief функция обработчик инициализации диалога
			virtual void OnInitialized();
			///\brief для обработчика сообщения WM_COMMAND
			virtual LRESULT OnMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
			///\brief заглушка для IDOK
			virtual void OnOK(WORD,HWND);
			///\brief заглушка для IDCANCEL
			virtual void OnCancel(WORD,HWND);
		};
	}
}

#endif //_ULFOLDERDLG_H__
