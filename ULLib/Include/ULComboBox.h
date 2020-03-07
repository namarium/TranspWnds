///\file ULComboBox.h 
///\brief фаил объявления класса лист контрола
#ifndef _ULCOMBOBOX_H__
#define _ULCOMBOBOX_H__
#include "ULSubClass.h"
#include "commctrl.h"

namespace ULWnds
{
	namespace ULControls
	{
		///\class CULComboBox
		///\brief класс комбобокса
		class CULComboBox:
			public ULWnds::CULSubClass
		{
		public:
			///\brief Конструктор
			CULComboBox();
			///\brief Функция для создания комбобокса
			///\param hParentWnd - хендл родительского окна
			///\param uID - ID листконтрола
			///\param x,y - координата листконтрола
			///\param cx,cy - размер листконтрола
			///\param dwStyle - стиль листконтрола
			///\param dwStyleEx - расширенный стиль 
			///\return TRUE в случае успеха
			BOOL Create(HWND hParentWnd,UINT uID,int x,int y,int cx,int cy,DWORD dwStyle,DWORD dwStyleEx);
			///\brief добавляет строку в комбобокс
			///\param pszStr - строка
			///\return номер добавленой строки в случае успеха иначе CB_ERR или CB_ERRSPACE
			int AddString(LPCTSTR pszStr);
			///\brief вставляет строку в комбобокс
			///\param nItem - индекс итема
			///\param pszStr - строка
			///\return номер добавленой строки в случае успеха иначе CB_ERR или CB_ERRSPACE
			int InsertString(int nItem,LPCTSTR pszStr);
			///\brief удалят строку из комбобокса
			///\return колличество оставшихся строк. CB_ERR в случае ошибки
			int DeleteString(int nItem);
			///\brief привязывает данные к итему
			///\param nItem - номер итема
			///\param dwData - данные
			///\return TRUE  в случае успеха
			BOOL SetItemData(int nItem,DWORD dwData);
			///\brief возвращает данные привязыванные к итему
			///\param nItem - номер итема
			///\return даныне, в случае ошибке CB_ERR
			DWORD GetItemData(int nItem);
			///\brief выбирает строку из листа в комбобокс
			///\param nItem - выбираемый итем
			///\return индекс выбранного итема. если nItem не попадает в диапазон итемов, то CB_ERR 
			int SetCurSel(int nItem);
			///\brief получает номер выбранной строки в комбике
			///\return индекс выбранного итема. если ничего не выбрано, то  CB_ERR
			int GetCurSel();
			///\brief Удаляет все итемы
			void ResetContent();
			///\brief показывает/прячет листбокс
			///\param fShowIt - флаг (показывает/прячет)
			void ShowDropDown(BOOL fShowIt);
			///\brief Возвращает информациб о комбобоксе
			///\param pcbi - указатель на экземпляр структуры с информацией
			///\return TRUE в случае успеха
			BOOL GetComboBoxInfo(PCOMBOBOXINFO pcbi);
			///\brief Возвращает число элементов в комбобоксе
			///\return число элементов. CB_ERR в случае ошибки
			int GetCount();
			///\brief возвращает текст указанного итема листбокса
			///\param nItem - номер итема 
			///\param pszText - буфер для получаемого текста 
			///\return длина возвращаемой строки, в случае ошибки CB_ERR
			int GetLBText(int nItem,LPTSTR pszText);
			///\brief аозвращает длину строки в указанном итеме
			///\param nItem - номер итема 
			///\return длина строки, в случае ошибки CB_ERR
			int GetLBTextLen(int nItem);			
			///\brief функция ищет строку в листбоксе комбобокса
			///\param nItem - итем, с которого нчинается поиск
			///\param pszStr - строка поиска
			///\return индекс найденого итема в случае успеха, иначе CB_ERR
			int FindString(int nItem,LPCTSTR pszStr);
		};
	}
}

#endif //_ULCOMBOBOX_H__
