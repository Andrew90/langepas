#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"

MIN_EQUAL_VALUE(MinimumThicknessPipeWall, 2)
MAX_EQUAL_VALUE(MinimumThicknessPipeWall, 30)

MIN_EQUAL_VALUE(MaximumThicknessPipeWall, 2)
MAX_EQUAL_VALUE(MaximumThicknessPipeWall, 30)

PARAM_TITLE(MinimumThicknessPipeWall, L"Минимальная стенка трубы")
PARAM_TITLE(MaximumThicknessPipeWall, L"Максималная стенка трубы")

void PipeThicknessDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, MinMaxThresholdsTable, DlgItem>(Singleton<MinMaxThresholdsTable>::Instance()).Do(h, L"Минимальная и максимальная стенка трубы"))
	{
	}
}