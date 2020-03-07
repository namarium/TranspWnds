///\file ULCOMDllApp.h
///\brief Заголовочный файл класса динамической библиотеки COM-сервера(05.01.2007)
#pragma once
#ifndef _ULCOMDLLAPP_H__
#define _ULCOMDLLAPP_H__
#include "ULDllApp.h"
#include <objbase.h>
#include "ULRegServ.h"

namespace ULApps
{
	///\class CULCOMDllApp
	///\brief Класс динамической библиотеки COM-сервера
	class CULCOMDllApp:public ULApps::CULDllApp
	{
	private:
		volatile LONG m_nComponentsLocks;
		volatile LONG m_nServerLocks;
	protected:
		///\brief Обект класса ULCOMs::CULRegServ, необходимый для регистрации компонентов
		ULCOMs::CULRegServ m_RegServ;
	public:
		///\brief Конструктор
		CULCOMDllApp();
		///\brief Деструктор
		virtual ~CULCOMDllApp();
		///\brief Вызывается при увеличении колличества активных компонентов
		void ComponentLock();
		///\brief 
		///\brief Вызывается при уменьшении колличества активных компонентов
		void ComponentUnlock();
		///\brief Вызывается при увеличении колличества активных фабрик классов
		///\или еще каких либо значимых объектов
		void ServerLock();
		///\brief Вызывается при уменьшении колличества активных фабрик классов
		///\или еще каких либо значимых объектов
		void ServerUnlock();
		///\brief Создаёт компонент посредством фабрики классов, тип которой указывается в ClassFactory_t
		///\param clsidComponent - идентификатор имеющегося класса компонента
		///\param clsidRequest - идентификатор класса компонета
		///\param iid - идентификатор интерфейса фабрики классов
		///\param ppvObject - возвращаемый указатель на интерфейс фабрики классов
		///\return Код возврата COM
		template<class ClassFactory_t>HRESULT GetClassObjectFromClassFactory(const CLSID& clsidComponent,
			const CLSID& clsidRequest,
			const IID& iid,
			void** ppvObject)
		{
			// Проверяем возможность создания компонента?
			if (clsidRequest != clsidComponent)
				return CLASS_E_CLASSNOTAVAILABLE;
			// Создаём фабрику классов
			ClassFactory_t* pFactory = new ClassFactory_t ;  // No AddRef in constructor
			if (pFactory == NULL)
				return E_OUTOFMEMORY;
			// Получаем запрошенный интерфейс
			HRESULT hr = pFactory->QueryInterface(iid, ppvObject) ;
			pFactory->Release();
			return hr;
		};
		///\brief Чисто виртуальная функция необходимая для 
		///\классов наследников. Вызывается из DllRegisterServer
		///\return возвращает код возврата регистрации сервера
		virtual HRESULT OnRegisterServer()=0;
		///\brief Чисто виртуальная функция необходимая для 
		///\классов наследников. Вызывается из DllUnregisterServer
		///\return возвращает код возврата отмены регистрации сервера
		virtual HRESULT OnUnregisterServer()=0;
		///\brief Чисто виртуальная функция необходимая для 
		///\классов наследников. Вызывается из DllGetClassObject
		///\param clsid - идентификатор класса компонета
		///\param iid - идентификатор интерфейса фабрики классов
		///\param ppvObject - возвращаемый указатель на интерфейс фабрики классов
		///\return Код возврата COM
		virtual HRESULT OnGetClassObject(const CLSID& clsid,const IID& iid,void** ppvObject)=0;
		///\brief Возвращает опрашивоему клиенту, по  CoCreateUnusedLibraries
		///\что данная DLL поддерживает еще какие либо объекты
		///\return S_OK - можно выгрузить DLL,S_FALSE - нельзя выгрузить DLL
		virtual HRESULT OnCanUnloadNow();
		///\brief Служит для регистрации компонента(ов).
		///\Вызывается через regsvr32.exe my.dll
		///\return возвращает код возврата регистрации сервера
		HRESULT DllRegisterServer();
		///\brief Служит для отмены регистрации компонента(ов).
		///\Вызывается через regsvr32.exe -u my.dll
		///\return возвращает код возврата отмены регистрации сервера
		HRESULT DllUnregisterServer();
		///\brief Точка входа DLL для создания фабрики класса компонента (см. CoGetClassObject)
		///\param clsid - идентификатор класса компонета
		///\param iid - идентификатор интерфейса фабрики классов
		///\param ppvObject - возвращаемый указатель на интерфейс фабрики классов
		///\return Код возврата COM
		HRESULT DllGetClassObject(const CLSID& clsid,
			const IID& iid,
			void** ppvObject);
		///\brief Возвращает опрашивоему клиенту, по  CoCreateUnusedLibraries
		///\что данная DLL поддерживает еще какие либо объекты
		///\return S_OK - можно выгрузить DLL,S_FALSE - нельзя выгрузить DLL
		HRESULT DllCanUnloadNow();
	};
}
#endif//_ULCOMDLLAPP_H__
