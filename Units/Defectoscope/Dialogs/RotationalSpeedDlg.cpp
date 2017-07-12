#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"

PARAM_TITLE(SpeedBit<oRL>, L"Младший бит кода частототы вращения")
PARAM_TITLE(SpeedBit<oRM>, L"Средний бит кода частототы вращения")
PARAM_TITLE(SpeedBit<oRH>, L"Старший бит кода частототы вращения")

void RotationSpeedDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, RotationalSpeedTable, DlgItem>(Singleton<RotationalSpeedTable>::Instance()).Do(h, L"Скорость вращения"))
	{
	}
}