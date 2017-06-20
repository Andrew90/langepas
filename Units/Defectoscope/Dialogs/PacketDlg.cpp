#include "stdafx.h"
#include "PacketDlg.h"
#include "StoredData\PacketBase.h"
#include "App\App.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "DlgTemplates\EditWithButton.hpp"
#include "Windows/MainWindow.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\PacketTemplateWindow.hpp"

namespace
{
	CHECK_EMPTY_STRING(Package)
	CHECK_EMPTY_STRING(Customer)
	CHECK_EMPTY_STRING(Operator)

	struct _OkBtn
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Применить";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			if(__ok_table_btn__<PackageBase, PacketParamTable, ParametersBase::one_row_table_list>()(h, owner))
			{
				EndDialog(h, TRUE);
			}
		}
	};	

	template<class T>struct Btn;
	template<>struct Btn<Customer>;
	template<>struct Btn<Operator>;
	typedef TemplDialog<PackageBase, PacketParamTable,  DlgItem, TL::MkTlst<_OkBtn, CancelBtn, Btn<Customer>, Btn<Operator>>::Result> TmplDlg;
	
	template<class X, class P>struct __make_btn__<Btn<X>, P>
	{
		void operator()(Btn<X> *o, P *p){}
	};

	HEADER_TABLE(Customer, 512, L"Заказчик")

	template<>struct Btn<Customer>
	{
		static const int width = 0;
		static const int ID = 112;
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			zprint(" \n");
			PacketTemplateWindow<PackageBase, CustomerTable, Customer>::Show(owner.items.get<DlgItem<Customer>>().hWnd);
		}
	};

	HEADER_TABLE(Operator, 512, L"Оператор")

	template<>struct Btn<Operator>
	{
		static const int width = 0;
		static const int ID = 113;
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			zprint(" \n");
			PacketTemplateWindow<PackageBase, OperatorTable, Operator>::Show(owner.items.get<DlgItem<Operator>>().hWnd);
		}
	};	

	PARAM_TITLE(Package, L"Номер пакета")
	PARAM_TITLE(Customer, L"Заказчик")
	PARAM_TITLE(Operator, L"Оператор")

	template<int N>struct DlgSubItems<Customer, Holder<N> >: EditWithButtonItems<Customer, Btn<Customer>::ID>{};
	template<int N>struct DlgSubItems<Operator, Holder<N> >: EditWithButtonItems<Operator, Btn<Operator>::ID>{};
}

bool PacketDlg::Do()
{
	HWND h = Singleton<MainWindow>::Instance().hWnd;
	PacketParamTable t;
	CopyFactory::Do(t.items, Singleton<PacketParamTable>::Instance().items);
	if(TmplDlg(t).Do(h, L"Пакет"))
	{
		CopyFactory::Do(Singleton<PacketParamTable>::Instance().items, t.items);
		return true;
	}
	return false;
}