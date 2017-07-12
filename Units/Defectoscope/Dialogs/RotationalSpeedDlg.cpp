#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"

PARAM_TITLE(SpeedBit<oRL>, L"������� ��� ���� ��������� ��������")
PARAM_TITLE(SpeedBit<oRM>, L"������� ��� ���� ��������� ��������")
PARAM_TITLE(SpeedBit<oRH>, L"������� ��� ���� ��������� ��������")

void RotationSpeedDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, RotationalSpeedTable, DlgItem>(Singleton<RotationalSpeedTable>::Instance()).Do(h, L"�������� ��������"))
	{
	}
}