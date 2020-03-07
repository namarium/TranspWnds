///\file ULPtr.inl
///\brief Заголовочный файл классов указатель и строк(29.12.2007)
#include <windows.h>
#include <tchar.h>
#include "ULRes.h"
namespace ULOther
{
	///\Класс динамического массива указателя (30.07.2007)
	template<typename object_t>
	class CULArr
	{
		///\brief Текущий размер в единицах типа
		size_t m_nObjSize;
	public:
		///\brief Сам указатель на буфер
		object_t* m_lpObject;
		///\brief Конструктор,не выделяющий память
		CULArr():
			m_nObjSize(0),
			m_lpObject(NULL)
		{
		}
		///\brief Конструктор для массива элементов
		///\param nObjSize стартовый размер
		CULArr(size_t nObjSize):
			m_nObjSize(nObjSize)
		{
			m_lpObject=new object_t[nObjSize];
		}
		///\brief Конструктор копирования
		CULArr(CULArr<object_t>& Arr):
			m_nObjSize(Arr.m_nObjSize)
		{
			m_lpObject=new object_t[m_nObjSize];
			for(size_t i=0;i<m_nObjSize;++i)
				m_lpObject[i]=Arr.m_lpObject[i];
		}
		///\brief оператор присваивания
		void operator=(CULArr<object_t>& Arr)
		{
			m_nObjSize=Arr.m_nObjSize;
			m_lpObject=new object_t[m_nObjSize];
			for(size_t i=0;i<m_nObjSize;++i)
				m_lpObject[i]=Arr.m_lpObject[i];
		}
		///\brief Изменение размера выделенной памяти с сохранением элементов
		///\param nObjSize новый размер
		///\return указатеь на буфер
		object_t* Resize(size_t nObjSize)
		{
			object_t* lpNewObj=NULL;
			if(nObjSize==0)
				Free();
			else
			{
				lpNewObj=new object_t[nObjSize];
				size_t size = (m_nObjSize<nObjSize)?m_nObjSize:nObjSize;
				for(size_t  i = 0; i < size; ++i)
					lpNewObj[i]=m_lpObject[i];
				Free();
			}
			m_lpObject=lpNewObj;
			m_nObjSize=nObjSize;
			return m_lpObject;
		};
		///\brief Возвращает размер указателя
		inline 	size_t GetSize()
		{
			return m_nObjSize;
		}
		///\brief Возвращает указатель
		inline operator object_t*()
		{
			return m_lpObject;
		}
		///\brief Освобождает выделенную память
		inline void Free()
		{
			if((m_lpObject!=NULL)&&(m_nObjSize!=0))
			{
				delete[] m_lpObject;
				m_lpObject=NULL;
				m_nObjSize=0;
			}
		}
		///\brief отсоединяет выделенную память от класса
		///\return указатель на выделенную память
		object_t* Detach()
		{
			object_t* lpRet=m_lpObject;
			m_lpObject=NULL;
			m_nObjSize=0;
			return lpRet;
		}
		///\brief Деструктор
		virtual ~CULArr()
		{
 			Free();
		}
	};

	template<typename object_t>
	///\class CULStrT
	///\brief Класс строки(29.12.2007)
	class CULStrT:public CULArr<typename object_t> 
	{
	public:
		///\brief Конструктор
		///\param nObjSize - размер выделяемой памяти
		CULStrT(size_t nObjSize=32767):
			CULArr<object_t>(nObjSize)
			{
				(*this)[0]=0;
			};
		///\brief Деструктор
		virtual ~CULStrT(){};
		///\brief Возвращает длину строки
		///\return длина строки
		int GetLen()
		{
			return (int)_tcslen(*this);
		}
		/*
		///\brief оператор присвоения
		object_t* operator=(const object_t pszRight[])
		{
			if(!pszRight)
				return *this;
			size_t nSize=_tcslen(pszRight);
			if(nSize>GetSize())
				Resize(nSize+1);
			_tcscpy_s(*this,(int)GetSize(),pszRight);
			return *this;
		};
		*/
		///\brief оператор присвоения
		object_t* operator=(const object_t* pszRight)
		{
			if(!pszRight)
				return *this;
			size_t nSize=_tcslen(pszRight);
			if(nSize>GetSize())
				Resize(nSize+1);
			_tcscpy_s(*this,(int)GetSize(),pszRight);
			return *this;
		};
		///\brief оператор присвоения
		object_t* operator=(CULStrT<object_t> &pszRight)
		{
			if(pszRight.GetSize()>GetSize())
				Resize(pszRight.GetSize()+1);
			_tcscpy_s(*this,(int)GetSize(),pszRight);
			return *this;
		};
		///\brief оператор сложения строк
		///\return получившаяся строка
		CULStrT operator+(const object_t* pszRight)
		{
			CULString pszTmp;
			pszTmp=*this;
			size_t nSize=_tcslen(pszRight);
			if(nSize>pszTmp.GetSize())
				pszTmp.Resize(pszTmp.GetSize()+nSize);
			_tcscat_s(pszTmp,pszRight);
			return pszTmp;
		}
		///\brief оператор прибавления к строке
		void operator+=(const object_t* pszRight)
		{
			size_t nSize=GetSize()+_tcslen(pszRight);
			if(nSize>GetSize())
				Resize(nSize);
			_tcscat_s(*this,GetSize(),pszRight);
		}
		///\brief оператор сравнения строк
		///\return 0 если равны
		int operator==(const object_t* pszRight)
		{
			return _tcscmp(*this,pszRight);
		}
		///\brief оператор сравнения строк
		///\return 0 если равны
		int operator==(const CULStrT<object_t> &pszRight)	
		{
			return _tcscmp(*this,pszRight);
		}
		///\brief Выводит форматированный текст в строку
		///\param szFormat - Формат вывода
		///\param ... - набор параметров
		///\return число выведенных символов
		int Format(LPTSTR szFormat,...)
		{
			va_list va;
			va_start( va, szFormat );	
			int nRet=_vstprintf(*this,szFormat,va);
			va_end( va );
			return nRet;
		}
	};
	///\brief переопределенный тип класса строки
	typedef CULStrT<TCHAR> CULStr;	
	///\class CULStrTable
	///\brief Класс для ускорения загрузки строк из StringTable(05.10.2008)
	class CULStrTable
	{
		///\brief id строки
		UINT m_uID;
		///\brief строка вывода
		TCHAR m_szStr[MAX_PATH];
	public:
		///\brief конструктор
		///\param uID - идентификатор строки
		CULStrTable(UINT uID):m_uID(uID)
		{}
		///\оператор LPTSTR
		operator LPCTSTR()
		{
			LoadString(ULGetResourceHandle(),m_uID,m_szStr,MAX_PATH);
			return m_szStr;
		}		
	};
}
