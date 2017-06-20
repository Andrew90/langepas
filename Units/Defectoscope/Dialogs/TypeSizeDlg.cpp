#include "stdafx.h"
#include "Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"
#include "Windows/MainWindow.h"

namespace
{
	struct AddOkBtn
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"���������";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			if(TestPassword<Owner::Base, Owner::Table>()(h))
			{
				wchar_t buf[128];

				GetWindowText(owner.items.get<DlgItem<CrossCountSensors> >().hWnd, buf, dimention_of(buf));
				owner.table.items.get<CrossCountSensors>().value = _wtoi(buf);

				GetWindowText(owner.items.get<DlgItem<NameParam> >().hWnd, buf, dimention_of(buf));
				if(0 == buf[0] || 0 == wcscmp(L"NONAME", buf))
				{
					MessageBox(h, L"������� �������� �����������", L"������!!!", MB_ICONERROR);
					return;
				}				
				CBase base(Owner::Base().name());
				if(base.IsOpen())
				{
					owner.table.items.get<NameParam>().value = buf;
					int id = Select<Owner::Table>(base).eq<NameParam>(owner.table.items.get<NameParam>().value).Execute();
					if(0 != id)
					{
						MessageBox(h, L"�������� ����������� ���� � ���� ������", L"��������������!!!", MB_ICONEXCLAMATION);
					}
					else
					{
						Insert_Into<Owner::Table>(owner.table, base).Execute<Owner::Table::items_list>();
						id = Select<Owner::Table>(base).eq<NameParam>(owner.table.items.get<NameParam>().value).Execute();	
						MessageBox(h, L"���������� ������", L"���������!!!", MB_ICONINFORMATION);
					}
					CurrentParametersTable &curr = Singleton<CurrentParametersTable>::Instance();
					curr.items.get<CurrentID>().value = id;
					UpdateWhere<CurrentParametersTable>(curr, base).ID(1).Execute();

					MainWindow &o = Singleton<MainWindow>::Instance();
					o.select.AddMenuItem(buf);					
					o.select.Do(TCommand(o.hWnd, 0, 1, o.select.hWnd));
				}
				EndDialog(h, TRUE);
			}
		}
	};

	template<class T, class D>struct  __more_than_one__
	{
		bool operator()(unsigned, T &t, D &d)
		{
			++d;
			return d > 1;
		}
	};

	struct DelOkBtn
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"���������";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			if(TestPassword<Owner::Base, Owner::Table>()(h))
			{
				wchar_t buf[128];
				GetWindowText(owner.items.get<DlgItem<NameParam> >().hWnd, buf, dimention_of(buf));
				if(0 == buf[0])
				{
					MessageBox(h, L"������� �������� �����������", L"������!!!", MB_ICONERROR);
					return;
				}
				CBase base(Owner::Base().name());
				if(base.IsOpen())
				{
					int countItems = 0;
					Select<Owner::Table>(base).ExecuteLoop<__more_than_one__>(countItems);
					if(countItems < 2)
					{
						MessageBox(h, L"� ���� ������ ���� ������ ������ �����������", L"��������������!!!", MB_ICONEXCLAMATION);
						return;
					}					
					wchar_t buf2[128];
					wcsncpy_s(buf2, buf, wcslen(buf));

					HWND hMain = GetParent(h);
					MainWindow *o = (MainWindow *)GetWindowLongPtr(hMain, GWLP_USERDATA);
					o->select.DelMenuItem(buf);

					if(buf[0] != 0)
					{
						NameParam n;
						n.value = buf2;
						Delete<Owner::Table>(base).eq<NameParam>(n.value).Execute();
						n.value = buf;
						int id = Select<Owner::Table>(base).eq<NameParam>(n.value).Execute();
						if(id)
						{
							Update<CurrentParametersTable>(base).set<CurrentID>(id).Where().ID(1).Execute();
							AppBase::InitTypeSizeTables(base);

							MainWindow &o = Singleton<MainWindow>::Instance();
							o.select.AddMenuItem(buf);					
							o.select.Do(TCommand(o.hWnd, 0, 1, o.select.hWnd));
						}
					}
				}
				EndDialog(h, TRUE);
			}
		}
	};
}

DO_NOT_CHECK(NameParam)
PARAM_TITLE(NameParam, L"")
template<int N>struct DlgSubItems<NameParam, Holder<N> >: EditItems<NameParam, 420>{};

PARAM_TITLE(CrossCountSensors, L"���������� ���������� ��������");
template<>struct DlgSubItems<CrossCountSensors, int>: ComboBoxSubItem<CrossCountSensors>{};

template<>struct FillComboboxList<CrossCountSensors>
{
	void operator()(HWND h, CrossCountSensors &t)
	{
		ComboBox_AddString(h, L"10");
		ComboBox_AddString(h, L"12");
	}
};

void AddTypeSizeDlg::Do(HWND h)
{
	ParametersTable t;
	if(TemplDialogList<
		ParametersBase
		, ParametersTable
		, TL::MkTlst<NameParam, CrossCountSensors>::Result
		, TL::MkTlst<AddOkBtn, CancelBtn>::Result
	   >(t).Do(h, L"�������� ����������")
	   )
	{}
}

void DelTypeSizeDlg::Do(HWND h)
{
	ParametersTable t;
	t.items.get<NameParam>().value = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	if(TemplDialogList<
		ParametersBase
		, ParametersTable
		, TL::MkTlst<NameParam>::Result
		, TL::MkTlst<DelOkBtn, CancelBtn>::Result
	   >(t).Do(h, L"������� ����������")
	   )
	{}
}

