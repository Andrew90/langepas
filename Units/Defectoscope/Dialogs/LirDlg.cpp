#include "stdafx.h"
#include "Dialogs.h"
#include "App\AppBase.h"
#include "App\App.h"
#include "DlgTemplates\ParamDlg.hpp"

/// \brief ���������� ���� ��� ��������� ����

namespace
{
	 PARAM_TITLE(DescriptorLir, L"���������� ���")
	 DO_NOT_CHECK(DescriptorLir)
}

void LirDescriptorDlg::Do(HWND h)
{
	if(TemplDialogList<
		ParametersBase
		, LirParamTable
		, TL::MkTlst<DescriptorLir>::Result
	   >(Singleton<LirParamTable>::Instance()).Do(h, L"���������� ���")
	   )
	{}
}

namespace
{
	PARAM_TITLE(TickPerZoneLir0, L"������ 1")
	MIN_EQUAL_VALUE(TickPerZoneLir0, 10)
	MAX_EQUAL_VALUE(TickPerZoneLir0, 1000)

	PARAM_TITLE(TickPerZoneLir1, L"������ 2")
	MIN_EQUAL_VALUE(TickPerZoneLir1, 10)
	MAX_EQUAL_VALUE(TickPerZoneLir1, 1000)
}

void LirDlg::Do(HWND h)
{
	if(TemplDialogList<
		ParametersBase
		, LirParamTable
		, TL::MkTlst<TickPerZoneLir0, TickPerZoneLir1>::Result
	   >(Singleton<LirParamTable>::Instance()).Do(h, L"����� ��������� ��� �� ���� ������ �������������� ������")
	   )
	{}
}