#include "stdafx.h"
#include "Test502Window.h"
#include "App\AppBase.h"
#include "App\App.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "window_tool\Emptywindow.h"

namespace
{
	PARAM_TITLE(PrimarySignalMin   , L"Ось Y минимум")
	MIN_EQUAL_VALUE(PrimarySignalMin, -100)
    MAX_EQUAL_VALUE(PrimarySignalMin, 100)
	//
	PARAM_TITLE(PrimarySignalMax   , L"Ось Y максимум")
	MIN_EQUAL_VALUE(PrimarySignalMax, -100)
    MAX_EQUAL_VALUE(PrimarySignalMax, 100)
	//
	PARAM_TITLE(PrimarySignalOffset, L"Смещение по оси X")
	MIN_EQUAL_VALUE(PrimarySignalOffset, 50)
    MAX_EQUAL_VALUE(PrimarySignalOffset, 10000)
	
	PARAM_TITLE(PrimarySignalWidth , L"Ширина кадра")
	MIN_EQUAL_VALUE(PrimarySignalWidth, 50)
    MAX_EQUAL_VALUE(PrimarySignalWidth, 10000)

	struct GraphicOptionsDlgOkBtn
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Применить";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			if(__ok_table_btn__<
				Owner::Base, Owner::Table
				, typename TL::SubListFromMultyList<typename Owner::Base::multy_type_list, Owner::Table>::Result
			>()(h, owner))  
			{
				EndDialog(h, TRUE);
			}
		}
	};
}

void GraphicOptionsDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, TestGraphAxesTable, DlgItem
	, TL::MkTlst<GraphicOptionsDlgOkBtn, CancelBtn>::Result
	>(Singleton<TestGraphAxesTable>::Instance()).Do(h, L"Оси графика"))
	{
		HWND h = FindWindow(WindowClass<Test502Window>()(), 0);
		if(NULL != h)
		{
			Test502Window *t = (Test502Window *)GetWindowLongPtr(h, GWLP_USERDATA);
			t->SetParam();
		}
	}
}