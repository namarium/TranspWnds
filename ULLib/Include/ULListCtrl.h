///\file ULListCtrl.h 
///\brief фаил объявления класса лист контрола
#ifndef _ULLISTCTRL_H__
#define _ULLISTCTRL_H__
#include "ULSubClass.h"
#include "commctrl.h"

namespace ULWnds
{
	namespace ULControls
	{
		///\class CULListCtrl
		///\brief класс лист контрола
		class CULListCtrl:
			public ULWnds::CULSubClass
		{
		public:
			///\brief Конструктор
			CULListCtrl();
			///\brief Функция для создания листконтрола
			///\param hParentWnd - хендл родительского окна
			///\param uID - ID листконтрола
			///\param x,y - координата листконтрола
			///\param cx,cy - размер листконтрола
			///\param dwStyle - стиль листконтрола
			///\param dwStyleEx - расширенный стиль 
			///\return TRUE в случае успеха
			BOOL Create(HWND hParentWnd,UINT uID,int x,int y,int cx,int cy,DWORD dwStyle,DWORD dwStyleEx);
			///\brief вставляет столбец в листконтрол
			///\param nCol - позиция вставки
			///\param lpszColumnHeading - Название столбца
			///\param nFormat - флаг выравнивания
			///\param nWidth - ширина столбца
			///\param nSubItem - индекс сабитема ассоциированного со столбцом
			///\return индекс нового столбца в случае успеха, иначе -1
			int InsertColumn(int nCol,LPCTSTR lpszColumnHeading,int nFormat = LVCFMT_LEFT,
				int nWidth = -1,int nSubItem = -1);
			///\brief задает ширину столбца
			///\param nCol - номер столбца 
			///\param cx - ширина столбца
			///\	может принимать значения LVSCW_AUTOSIZE и LVSCW_AUTOSIZE_USEHEADER
			///\return TRUE в случае успеха
			BOOL SetColumnWidth(int nCol,int cx);
			///\brief получает ширину столбца
			///\param nCol - номер столбца 
			///\return ширина столбца
			int GetColumnWidth(int nCol);
			///\brief вставляет итем 
			///\param nItem - позиция вставки
			///\param lpszItem - строка
			///\return номер вставленного итема
			int InsertItem(int nItem,LPCTSTR lpszItem);	
			///\brief устанавливает текст в нужную позицию
			///\param nItem - № итема для вставки
			///\param nSubItem - № сабитема для вставки
			///\param lpszText - текст для вставки
			///\return TRUE в случае успеха
			BOOL SetItemText(int nItem,int nSubItem,LPCTSTR lpszText);
			///\brief получает текст из нужнйо позиции
			///\param nItem - № итема 
			///\param nSubItem - № сабитема 
			///\param lpszText - буфер для получения текста
			///\param nLen - длина буфера
			///\return TRUE в случае успеха
			BOOL GetItemText(int nItem,int nSubItem,LPTSTR lpszText,int nLen);
			///\brief Получение колличества итемов
			///\return колличествj итемов
			int GetItemCount();
			///\brief устанавливает расширеный стиль листконтрола
			///\param dwNewStyle - новый стиль
			///\return возвращает предыдущий расширеный стиль листконтрола
			DWORD SetExtendedStyle(DWORD dwNewStyle);
			///\brief удаляет все итемы в листконтроле
			///\return TRUE в случае успеха
			BOOL DeleteAllItems();
			///\brief получение информации о столбце
			///\param nCol - номер столбца
			///\param pCol - указатель на структуру с информацией о столбце
			///\return  TRUE в случае успеха
			BOOL GetColumn(int nCol,LPLVCOLUMN pCol);
			///\brief Определение позиции итема в листконтроле по координате
			///\param pInfo - указатель на структуру с инфой
			///\return индекс итема по которому, на котором координата, иначе -1
			int HitTest(LPLVHITTESTINFO pInfo);
			///\brief Определение позиции сабитема в листконтроле по координате
			///\param pInfo - указатель на структуру с инфой
			///\return индекс итема по которому, на котором координата, иначе -1
			int SubItemHitTest(LPLVHITTESTINFO pInfo);
			///\brief Привязывает к итему данные
			///\param nItem - номер итема
			///\param dwData - данные
			///\return TRUE в случае успеха
			BOOL SetItemData(int nItem,DWORD_PTR dwData);
			///\brief возвращает привязанные данныеданные 
			///\param nItem - номер итема
			///\return данные
			DWORD_PTR GetItemData(int nItem);
			///\brief получение номера первого выбранного элемента
			///\return номер выбранного элемента
			int GetFirstSelectedItemPosition();
			///\brief получение номера следующего выбранного элемента
			///\param pos - номер позиции
			///\return номер выбранного элемента
			int GetNextSelectedItemPosition(int pos);
			///\brief удаляет выбранную позицию
			///\param nItem - позиция для удаления
			///\return TRUE в случае успеха
			BOOL DeleteItem(int nItem);
			///\brief возвращает координаты и размер области итема
			///\param nItem - номер итема
			///\param lpRect - указатель на объект RECT для координат
			///\param nCode - параметр запроса
			///\return TRUE в случае успеха
			BOOL GetItemRect(int nItem,LPRECT lpRect,UINT nCode);
			///\brief возвращает координаты и размер области сабитема
			///\param iItem - номер итема
			///\param iSubItem - номер сабитема
			///\param nArea - параметр запроса
			///\param lpRect - указатель на объект RECT для координат
			///\return TRUE в случае успеха
			BOOL GetSubItemRect(int iItem, int iSubItem, int nArea, RECT* lpRect);
			///\brief возвращает число сабитемов в итеме
			///\return число сабитемов в итеме
			int GetSubItemCount();
		};
	}
}

#endif //_ULLISTCTRL_H__
