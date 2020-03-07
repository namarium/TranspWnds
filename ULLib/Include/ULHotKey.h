///\file ULHotKey.h
///\brief Заголовочный файл класса обёртки лдя HOTKEY контрола(09.09.2007)
#pragma once
#ifndef __HOTKEY__H_
#define __HOTKEY__H_
#include "ULSubClass.h"
#include <commctrl.h>
namespace ULWnds
{
	namespace ULControls
	{
		///\class CULHotKey
		///\brief Класс обёртка для контрола горячих клавиш(16.07.2007)
		class CULHotKey:public CULSubClass
		{
		public:
			///\brief варианты запретов
			enum enCombInv
			{
				ciA=	HKCOMB_A,
				ciC=	HKCOMB_C,
				ciCA=	HKCOMB_CA,
				ciNone=	HKCOMB_NONE,
				ciS=	HKCOMB_S,
				ciSA=	HKCOMB_SA,
				ciSC=	HKCOMB_SC,
				ciSCA=	HKCOMB_SCA
			};
			///\brief варианты дополнительных горячих клавиш
			enum enModifierKey
			{
				mfAlt		=HOTKEYF_ALT,
				mfControl	=HOTKEYF_CONTROL,
				mfExt		=HOTKEYF_EXT,
				mfShift		=HOTKEYF_SHIFT
			};
			///\brief структура горячих клавиш
			///\param wHotkey - горячие клавиши упакованные в слово
			///\param hkStruct - разделенные горячие клавиши
			union unHotKey
			{
				WORD wHotkey;
				///\brief структура разделенных горячих клавиш
				///\param bVirtualKey - горячая клавиша
				///\param bModifierKey - дополнительная горячая клавиша
				struct tagHotKey
				{
					BYTE bVirtualKey;
					BYTE bModifierKey;
				}hkStruct;
			};
		public:
			///\brief Конструктор
			CULHotKey();
			///\brief Деструктор
			virtual ~CULHotKey();
			///\brief Функция для создания хоткея
			///\param hParentWnd - хендл родительского окна
			///\param uID - ID кнопки
			///\param x,y - координаты
			///\param cx,cy - размер 
			///\param dwStyle - стиль 
			///\param dwStyleEx - расширенный 
			///\return TRUE в случае успеха
			BOOL Create(HWND hParentWnd,UINT uID,int x,int y,
				int cx,int cy,DWORD dwStyle=WS_CHILD|WS_VISIBLE|WS_TABSTOP,
				DWORD dwStyleEx=WS_EX_CLIENTEDGE);
		//==================inline functions============================
			///\brief возвращает текущую горячую клавишу
			///\return CULHotKey::unHotKey
			inline unHotKey GetHotKey()
			{	
				unHotKey HotKey;
				HotKey.wHotkey=(WORD)SendMessage(HKM_GETHOTKEY);
				return HotKey;
			}
			///\brief устанавливает горячую клавишу
			///\param HotKey - горячая клавиша
			inline void SetHotKey(unHotKey HotKey)
				{SendMessage(HKM_SETHOTKEY,(WPARAM)HotKey.wHotkey);}
			///\brief устанавливает горячую клавишу
			///\param bVirtualKey - горячая клавиша
			///\param bModifierKey - дополнительная клавиша
			inline void SetHotKey(BYTE bVirtualKey,BYTE bModifierKey)
				{SendMessage(HKM_SETHOTKEY,MAKELPARAM(bVirtualKey,bModifierKey));}
			///\brief устанавливает запреты на выбор горячей клавиши
			///\param bCombInv - варианты запретов
			///\param bModifierKey - дополнительная клавиша, которая добавиться к запрещенной 
			inline void SetRules(BYTE bCombInv,BYTE bModifierKey)
				{SendMessage(HKM_SETHOTKEY,(WPARAM)bCombInv,(LPARAM)bModifierKey);}
		};
	}
}
#endif //__HOTKEY__H_
