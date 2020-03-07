///\file ULPropSheet.h
///\brief Заголовочный файл класса root окна(2006)
#pragma once
#ifndef __UL_PROPSHEET_H__
#define __UL_PROPSHEET_H__

#include "ULSubClass.h"
#include "ULOther.h"
#pragma comment(lib,"comctl32.lib")
namespace ULWnds
{
	namespace ULDlgs
	{
		///\class CULPropSheet
		///\brief Класс окна свойств(2006)
		class CULPropSheet:
			public CULSubClass
		{
		private:
			///\brief флаг режима отображения
			BOOL m_fWizard;
		protected:
			///\brief Массив для кранения хендлов созданных стариц свойств
			ULOther::CULArr<HPROPSHEETPAGE> m_phPropSheetPage;
		public:
			///\brief Перечисление типов мастера
			enum enWizardFlags
			{
				wfWizard=PSH_WIZARD,
				wfWizard97=PSH_WIZARD97,
			#if _WIN32_IE>0x0400
				wfWizardLite=PSH_WIZARD_LITE,
			#endif
				wfWizardContextHelp=PSH_WIZARDCONTEXTHELP,
				wfWizardHasFinish=PSH_WIZARDHASFINISH,
				wfNonWizard=0
			};
			PROPSHEETHEADER m_psh;
		public:
			///\brief Конструктор
			CULPropSheet();
			///\brief Деструктор
			virtual ~CULPropSheet();
			/*!\brief Функция добавляет вмассив m_phPropSheetPage
			хендл созданной страницы свойств*/
			///\param hPropSheetPage страница свойств
			///\return TRUE в случае успеха
			BOOL AddPage(HPROPSHEETPAGE hPropSheetPage);
			///\brief Функция создаёт окно свойств
			///\param hParenWnd - хендл родительского окна
			///\param szCaption - заголовок окна
			///\param hbmHeader - хендл картинки, которая отобразится в заголовке
			///\param hbmWatermark - хендл картинки, которая отобразится в левой части окна
			///\param nWizardFlags - режим мастера/TabControla
			///\param fModal - модальный/немодальный
			///	если немодальный то функция вернет хендл окна
			///	ЗЫ:с модальностью не разобрался,т.к.окно не очень 
			///	хочет закрыватсья 8-)
			INT_PTR Create(HWND hParenWnd,
				LPCTSTR szCaption,
				HBITMAP hbmHeader=NULL,
				HBITMAP hbmWatermark=NULL,
				UINT nWizardFlags=wfNonWizard,
				BOOL fModal=TRUE);
			///\brief Функция создаёт окно свойств
			///\param hParenWnd - хендл родительского окна
			///\param szCaption - заголовок окна
			///\param rscHeader - ID картинки из ресурсов, которая отобразится в заголовке
			///\param rscWatermark - ID картинки из ресурсов, 
			///	которая отобразитсяв левой части окна
			///\param nWizardFlags - режим мастера/TabControla
			///\param fModal - модальный/немодальный если немодальный,
			///	то функция вернет хендл окна
			INT_PTR Create(HWND hParenWnd,
				LPCTSTR szCaption,
				int rscHeader=0,
				int rscWatermark=0,
				UINT nWizardFlags=wfNonWizard,
				BOOL fModal=TRUE);
			///\brief аозвращает режим отображения
			///\return TRUE, если визард
			BOOL IsWizard();
		protected:
			virtual LRESULT OnDestroy(WPARAM,LPARAM);
		};
	}
}
#endif //__UL_PROPSHEET_H__
