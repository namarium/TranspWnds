///\file ULClassFactory.h
///\brief Заголовочный фаил класса фабрики классов
#pragma once
#ifndef _ULCLASSFACTORY_H__
#define _ULCLASSFACTORY_H__
#include <objbase.h>
#include "ULCOMDllApp.h"
namespace ULCOMs
{
	///\namespace ULAgregation
	///\brief Пространство имён классов агрегации
	namespace ULAgregation
	{
		;
		///\class CAgregateNone
		///\brief Класс заглушка при отсутствии агрегации
		///\param CComponent_t - Имя класса компонента
		template<class CComponent_t>class CULAgregateNone
		{
		public:
			///\brief Конструкор
			CULAgregateNone(IUnknown* /*pUnknownOuter*/)
			{		
			}
			///\brief Проверяет входные параметры CULClassFactory::CreateInstance 
			///\ на возможность агрегирования
			///\param pUnknownOuter - Запрашиваемый указатель на компонент
			///\param iid - идентификатор запрашиваемого интерфейса
			///\return S_OK если всё впорядке, иначе CLASS_E_NOAGGREGATION
			HRESULT Check(IUnknown* pUnknownOuter,const IID& iid)
			{
				if(pUnknownOuter!=NULL)
					return CLASS_E_NOAGGREGATION;
				else 
					return S_OK;
			}
			///\brief Создаёт коспонент
			///\return указатель на компонент
			CComponent_t* GetComponent()
			{
				return new CComponent_t;
			}
		};
		///\class CULAgregate
		///\brief Класс для агрегации
		///\param CComponent_t - Имя класса компонента
		template<class CComponent_t>class CULAgregate
		{
		private:
			///\brief Указатель на запрашиваемый компонент
			IUnknown* m_pUnknownOuter;
		public:
			///\brief Конструкор
			///\param pUnknownOuter - Запрашиваемый указатель на компонент
			CULAgregate(IUnknown* pUnknownOuter):
				m_pUnknownOuter(pUnknownOuter)
			{		
			}
			///\brief Проверяет входные параметры CULClassFactory::CreateInstance 
			///\ на возможность агрегирования
			///\param pUnknownOuter - Запрашиваемый указатель на компонент
			///\param iid - идентификатор запрашиваемого интерфейса
			///\return S_OK если всё впорядке, иначе CLASS_E_NOAGGREGATION
			HRESULT Check(IUnknown* pUnknownOuter,const IID& iid)
			{
				if ((pUnknownOuter != NULL) && (iid != IID_IUnknown))
					return CLASS_E_NOAGGREGATION;
				else 
					return S_OK;
			}
			///\brief Создаёт коспонент
			///\return указатель на компонент
			CComponent_t* GetComponent()
			{
				return new CComponent_t(pUnknownOuter);
			}
		};
	}
	template<class CComponent_t,class CAgregate_t=ULAgregation::CULAgregateNone<CComponent_t>>class CULClassFactory:
	///\class CULClassFactory
	///\brief класс фабрики классов
	///\param CComponent_t - тип регистрируемого компонента
	public IClassFactory
	{
	private: 
		///brief Счётчик ссылок
		long m_nRef; 
	public:
		///\brief Конструктор
		CULClassFactory():
			m_nRef(1)
		{
		};
		///\brief Деструктор
		~CULClassFactory()
		{
		};
		
		//=================Реализация  интерфейса IUnknown==========================
		///\brief Возвращает указатель на IClassFactory
		///\param iid - Идентификатор интерфейса
		///\param ppvObject - Указатель на указатель на интерфейс
		///\return S_OK в случае создания интерфеса, 
		///\ E_NOINTERFACE - в слечае отсутствия запрашиваемого интерфейса
		virtual HRESULT __stdcall CULClassFactory::QueryInterface(const IID& iid, void** ppvObject)
		{    
			if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
				*ppvObject = static_cast<IClassFactory*>(this); 
			else
			{
				*ppvObject = NULL ;
				return E_NOINTERFACE ;
			}
			reinterpret_cast<IUnknown*>(*ppvObject)->AddRef() ;
			return S_OK ;
		}
		///\brief Увеличивает счетчик числа ссылок на интерфейс
		///\return текущее число ссылок
		virtual ULONG __stdcall CULClassFactory::AddRef()
		{
			return InterlockedIncrement(&m_nRef) ;
		}
		///\brief Уменьщает счетчик числа ссылок на интерфейс
		///\return текущее число ссылок
		virtual ULONG __stdcall CULClassFactory::Release() 
		{
			if (InterlockedDecrement(&m_nRef) == 0)
			{
				delete this ;
				return 0 ;
			}
			return m_nRef ;
		}

		// реализация LockServer
		///\brief Блокирует сервер при от преждевременной выгрузки необходимости
		///\param fLock - TRUE - заблокировать, FALSE - разблокировать
		///\return S_OK
		virtual HRESULT __stdcall CULClassFactory::LockServer(BOOL fLock) 
		{
			if (fLock)
				static_cast<ULApps::CULCOMDllApp*>(ULApps::CULCOMDllApp::GetULDllApp())->ServerLock();
			else
				static_cast<ULApps::CULCOMDllApp*>(ULApps::CULCOMDllApp::GetULDllApp())->ServerUnlock();
			return S_OK ;
		}

		///\brief Выполняет создание компонента. Вызывается из CoCreateInstance
		///\param pUnknownOuter - если поддерживается агрегирование, 
		/// то к нему присваевается указатель на компонент
		///\param iid - Идентификатор интерфейса, поддерживаемого компонентом
		///\param ppvObject - Указатель на указатель на интерфейс
		///\return COM код возврата
		HRESULT __stdcall CreateInstance(IUnknown* pUnknownOuter,
												 const IID& iid,
												 void** ppvObject)
		{
			CAgregate_t Agregation(pUnknownOuter);
			HRESULT hr=Agregation.Check(pUnknownOuter,iid);
			if(FAILED(hr))
				return hr;
			//Создание компонента
			CComponent_t* pComponent = Agregation.GetComponent();
			if (pComponent == NULL)
				return E_OUTOFMEMORY;
			//Получение запрошенного интерфейса
			hr = pComponent->QueryInterface(iid, ppvObject) ;
			//Освобожение указателя IUnknown .
			// (если QueryInterface выполнилась не верно, то произойдёт удаление компонента.)
			pComponent->Release() ;
			return hr ;
		}
	};
}
#endif//_ULCLASSFACTORY_H__
