#include "stdafx.h"
#include "Dialogs.h"
#include "App\AppBase.h"
#include "App\App.h"
#include "DlgTemplates\ParamDlg.hpp"

namespace
{
	PARAM_TITLE(OffsetPointsMin, L"��� Y �������")
	PARAM_TITLE(OffsetPointsMax, L"��� Y ��������")

	MIN_EQUAL_VALUE(OffsetPointsMin, -20)
    MAX_EQUAL_VALUE(OffsetPointsMin, 20)

	MIN_EQUAL_VALUE(OffsetPointsMax, -20)
    MAX_EQUAL_VALUE(OffsetPointsMax, 20)

	PARAM_TITLE(PrimarySignalMin   , L"��� Y �������")
	MIN_EQUAL_VALUE(PrimarySignalMin, -20)
    MAX_EQUAL_VALUE(PrimarySignalMin, 20)

	PARAM_TITLE(PrimarySignalMax   , L"��� Y ��������")
	MIN_EQUAL_VALUE(PrimarySignalMax, -20)
    MAX_EQUAL_VALUE(PrimarySignalMax, 20)

	PARAM_TITLE(PrimarySignalOffset, L"�������� �� ��� X")
	MIN_EQUAL_VALUE(PrimarySignalOffset, 50)
    MAX_EQUAL_VALUE(PrimarySignalOffset, 10000)

	PARAM_TITLE(PrimarySignalWidth , L"������ �����")
	MIN_EQUAL_VALUE(PrimarySignalWidth, 50)
    MAX_EQUAL_VALUE(PrimarySignalWidth, 10000)
}

void ThresholdDlg::Do(HWND h)
{
	TemplDialogList<ParametersBase, GraphAxesTable, TL::MkTlst<OffsetPointsMin, OffsetPointsMax>::Result>
		(Singleton<GraphAxesTable>::Instance()).Do(h, L"��� ������� �������");
}

void PrimarySignalDlg::Do(HWND h)
{
	TemplDialogList<ParametersBase, GraphAxesTable, TL::MkTlst<
		PrimarySignalMin
		, PrimarySignalMax		
		, PrimarySignalOffset
		, PrimarySignalWidth
	>::Result>
		(Singleton<GraphAxesTable>::Instance()).Do(h, L"��� ������� ���������� �������");
}