///\file ULToolTip.h
///\brief Заголовочный файл класса контрола подсказок(06.09.2007)
#pragma once
#ifndef __ULTOOLTIP_H__
#define __ULTOOLTIP_H__
#include <windows.h>
#include <commctrl.h>
#include "ULSubClass.h"
#pragma comment(lib,"comctl32.lib")
namespace ULWnds
{
	namespace ULControls
	{
		///\class CULToolTip
		///\brief Класс контрола подсказок(06.09.2007)
		class CULToolTip: public CULSubClass
		{
		public:
			TOOLINFO m_ToolInfo;
			///brief Типы иконок в заголовке
			///\param itNon - Без иконки
			///\param itInfo - Иконка "Инфо"
			///\param itWarning - Иконка "Внимание"
			///\param itError - Иконка "Ошибка"
			enum enIconType
			{
				itNon		=0,
				itInfo		=1,
				itWarning	=2,
				itError		=3
			};
			///\brief Типы устанавливаемых задержек
			///\param dtAutomatic - распределяет задерку по определенным 
			///	пропорциям
			///\param dtAutoPop - устанавливает время удержания для 
			///	появения подсказки
			///\param dtInitial - вроде устанавливает задерку между 
			///	остановкой курсора и его перехода в стационарное 
			///	состояние
			///\param dtReshow - устанавливает задерку между исчезновением
			///	одной и появлением другой подсказки
			enum enDelayTime
			{
				dtAutomatic	=TTDT_AUTOMATIC,
				dtAutoPop	=TTDT_AUTOPOP,
				dtInitial	=TTDT_INITIAL,
				dtReshow	=TTDT_RESHOW
			};
			///Конструктор
			CULToolTip();
			///Деструктор
			virtual ~CULToolTip();
			///\brief Функция создания окна
			///\param hParentWnd - родительское окно
			///\param fBalloon - тип отображения окна
			///\return TRUE в случае успеха
			BOOL Create(HWND hParentWnd, BOOL fBalloon=TRUE);	
			///\brief Функция добавления подсказки
			///	если uID==0,то устанавливается флаг TTF_SUBCLASS
			///	если uID!=0,то устанавливается флаг TTF_TRACK
			///	что позволяет использовать TrackPosition и TrackActivate
			///\param hTargetWnd - окно, для которого подсказка
			///\param szMessage - текст подсказки
			///\param uID - идентификатор подсказки
			///\return TRUE в случае успеха
			BOOL AddTool(HWND hTargetWnd, LPCTSTR szMessage,UINT uID=0);	
			///\brief Функция обновления подсказки
			///\param hTargetWnd - окно, для которого подсказка
			///\param szMessage - текст подсказки
			///\return TRUE в случае успеха
			BOOL UpdateTipText(HWND hTargetWnd,	LPCTSTR szMessage);				
		//==================inline functions============================
			///\brief Функция задаёт цвет фона
			///\param clrBK - цвет фона
			inline void SetTipBkColor(COLORREF clrBK)
				{SendMessage(TTM_SETTIPBKCOLOR,clrBK,0);};
			///\brief Функция задаёт цвет текста
			///\param clrText - цвет текста	
			inline void SetTipTextColor(COLORREF clrText)
				{SendMessage(TTM_SETTIPTEXTCOLOR,clrText,0);};
			///\brief Функция задаёт заголовок для подсказки и тип отображаемой иконки
			///\param it- тип отображаемой иконки
			///\param szTitle - текст заголовка
			inline BOOL SetTitle(enIconType it, TCHAR* szTitle)
				{return(BOOL)SendMessage(TTM_SETTITLE,WPARAM(it),(LPARAM)szTitle);};
			///\brief Функция устанавливает задржки появления...подсказки
			///\param nmsTime - задержка в мс
			///\param dt - тип задержки
			inline int SetDelayTime(int nmsTime,enDelayTime dt=dtAutomatic)
				{return (int)SendMessage(TTM_SETDELAYTIME,WPARAM(dt),(LPARAM)nmsTime);};
			///\brief Функция устанавливает позицию подсказки
			///\param x,y - абсолютные координаты
			inline void TrackPosition(int x,int y)
				{SendMessage(TTM_TRACKPOSITION,0,(LPARAM)(DWORD) MAKELONG(x,y));};
			///\brief Функция де/активизирует выбраную подсказку
			///\param hWnd - Хэндл окна, которому принадлежит подсказка
			///\param uID - ID подсказки 
			///\param fActivate - TRUE/FALSE - активизировать/деактивизировать
			inline void TrackActivate(HWND hWnd,UINT uID,BOOL fActivate)
				{m_ToolInfo.hwnd=hWnd;m_ToolInfo.uId=uID;
						SendMessage(TTM_TRACKACTIVATE,(WPARAM)fActivate,(LPARAM)&m_ToolInfo);};
		};
	}
}
#endif //__ULTOOLTIP_H__
