///\file ULProperty.h
///\brief фаил содержащий классы свойств
#pragma warning(disable:4355)
///\namespace ULProperty
///\brief простронство имён содержащее классы свойств
namespace ULProperty
{
	///\class CULProperty
	///\brief класс свойства, требующий хендлеры Set/Get
	template<typename Prop_t,class OwnerClass_t,
		void (OwnerClass_t::*SetHandler)(Prop_t*),
		void (OwnerClass_t::*GetHandler)(Prop_t*)>
	class CULProperty
	{
	protected:
		///\brief само поле свойства
		Prop_t m_PropVal;
		///\brief указатель на класс, содержащий свойство
		OwnerClass_t* m_pOwnerClass;
	public:
		///\brief конструктор
		///\param pOwnerClass - указатель на класс, содержащий свойство
		CULProperty(OwnerClass_t* pOwnerClass):
			m_pOwnerClass(pOwnerClass)
		{
		}
		///\brief оператор присваивания
		///\param propVal - новое значение свойства
		///\return новое значение свойства
		Prop_t operator=(const Prop_t propVal)
		{
			m_PropVal=propVal;
			(m_pOwnerClass->*SetHandler)(&m_PropVal);
			return m_PropVal;
		}
		///\brief оператор приведения типа
		operator Prop_t()
		{
			(m_pOwnerClass->*GetHandler)(&m_PropVal);
			return m_PropVal;
		}
	};
	///\class CULGetProperty
	///\brief класс свойства, требующий хендлеры Get
	template<typename Prop_t,class OwnerClass_t,
		void (OwnerClass_t::*GetHandler)(Prop_t*)>
	class CULGetProperty
	{
	protected:
		Prop_t m_PropVal;
		OwnerClass_t* m_pOwnerClass;
	public:
		CULGetProperty(OwnerClass_t* pOwnerClass):
			m_pOwnerClass(pOwnerClass)
		{
		}
		Prop_t operator=(const Prop_t propVal)
		{
			m_PropVal=propVal;
			return m_PropVal;
		}
		operator Prop_t()
		{
			(m_pOwnerClass->*GetHandler)(&m_PropVal);
			return m_PropVal;
		}
	};
	///\class CULSetProperty
	///\brief класс свойства, требующий хендлеры Set
	template<typename Prop_t,class OwnerClass_t,
		typename void (OwnerClass_t::*SetHandler)(Prop_t*)>
	class CULSetProperty
	{
	protected:
		Prop_t m_PropVal;
		OwnerClass_t* m_pOwnerClass;
	public:
		CULSetProperty(OwnerClass_t* pOwnerClass):
			m_pOwnerClass(pOwnerClass)
		{
		}
		Prop_t operator=(const Prop_t propVal)
		{
			m_PropVal=propVal;
			(m_pOwnerClass->*SetHandler)(&m_PropVal);
			return m_PropVal;
		}
		operator Prop_t()
		{
			return m_PropVal;
		}
	};
}
