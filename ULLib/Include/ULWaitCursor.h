///\file ULWaitCursor.h 
///\brief фаил объявления класса курсора ожидания
#include <windows.h>
namespace ULOther
{
	///\class CULWaitCursor
	///\brief класса курсора ожидания
	class CULWaitCursor
	{
		///\brief хендл курсора ожидания
		HCURSOR m_hWaitCursor;
		///\brief хендл предыдущего курсора
		HCURSOR m_hCursor;
	public:
		///\brief конструктор
		CULWaitCursor();
		///\brief деструктор
		~CULWaitCursor();
		///\brief функция для востановления начального курсора
		void Restore();
	};
}
