#include "stdafx.h"
#include "Dialogs.h"
#include "App\AppBase.h"
#include "App\App.h"
#include "DlgTemplates\ParamDlg.hpp"

namespace
{
	PARAM_TITLE(OffsetPointsMin, L"Ось Y минимум")
	PARAM_TITLE(OffsetPointsMax, L"Ось Y максимум")

	MIN_EQUAL_VALUE(OffsetPointsMin, -200)
    MAX_EQUAL_VALUE(OffsetPointsMin, 200)

	MIN_EQUAL_VALUE(OffsetPointsMax, -200)
    MAX_EQUAL_VALUE(OffsetPointsMax, 200)

	PARAM_TITLE(PrimarySignalMin   , L"Ось Y минимум")
	MIN_EQUAL_VALUE(PrimarySignalMin, -200)
    MAX_EQUAL_VALUE(PrimarySignalMin, 200)

	PARAM_TITLE(PrimarySignalMax   , L"Ось Y максимум")
	MIN_EQUAL_VALUE(PrimarySignalMax, -200)
    MAX_EQUAL_VALUE(PrimarySignalMax, 200)

	PARAM_TITLE(PrimarySignalOffset, L"Смещение по оси X")
	MIN_EQUAL_VALUE(PrimarySignalOffset, 50)
    MAX_EQUAL_VALUE(PrimarySignalOffset, 10000)

	PARAM_TITLE(PrimarySignalWidth , L"Ширина кадра")
	MIN_EQUAL_VALUE(PrimarySignalWidth, 50)
    MAX_EQUAL_VALUE(PrimarySignalWidth, 10000)
}

void ThresholdDlg::Do(HWND h)
{
	TemplDialogList<ParametersBase, GraphAxesTable, TL::MkTlst<OffsetPointsMin, OffsetPointsMax>::Result>
		(Singleton<GraphAxesTable>::Instance()).Do(h, L"Оси графика порогов");
}

void PrimarySignalDlg::Do(HWND h)
{
	TemplDialogList<ParametersBase, GraphAxesTable, TL::MkTlst<
		PrimarySignalMin
		, PrimarySignalMax		
		, PrimarySignalOffset
		, PrimarySignalWidth
	>::Result>
		(Singleton<GraphAxesTable>::Instance()).Do(h, L"Оси графика первичного сигнала");
}