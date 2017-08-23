#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"

PARAM_TITLE(MinimumLengthPipe, L"Длина трубы в зонах")
MIN_EQUAL_VALUE(MinimumLengthPipe, 30)
MAX_EQUAL_VALUE(MinimumLengthPipe, 60)

void MinimalTubeLengthDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, MinimumLengthPipeTable, DlgItem>(Singleton<MinimumLengthPipeTable>::Instance()).Do(h, L"Мининальная длина трубы в зонах"))
	{
	}
}

