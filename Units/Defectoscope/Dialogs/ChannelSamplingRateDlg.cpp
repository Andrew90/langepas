#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"

DO_NOT_CHECK(ChannelSamplingRate)
PARAM_TITLE(ChannelSamplingRate, L"Частота сбора кадров платы 502")

void ChannelSamplingRateDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, L502ParametersTable, DlgItem>(Singleton<L502ParametersTable>::Instance()).Do(h, L"Параметры платы 502"))
	{
	}
}