#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"

MIN_EQUAL_VALUE(BorderKlass2<Long>, 5)
MIN_EQUAL_VALUE(BorderDefect<Long>, 5)

MIN_EQUAL_VALUE(BorderKlass2<Cross>, 5)
MIN_EQUAL_VALUE(BorderDefect<Cross>, 5)

MIN_EQUAL_VALUE(BorderKlass2<Thick>, 3)
MIN_EQUAL_VALUE(BorderKlass3<Thick>, 3)
MIN_EQUAL_VALUE(BorderDefect<Thick>, 3)




MAX_EQUAL_VALUE(BorderKlass2<Long>, 110)
MAX_EQUAL_VALUE(BorderDefect<Long>, 110)

MAX_EQUAL_VALUE(BorderKlass2<Cross>, 110)
MAX_EQUAL_VALUE(BorderDefect<Cross>, 110)

MAX_EQUAL_VALUE(BorderKlass2<Thick>, 40)
MAX_EQUAL_VALUE(BorderKlass3<Thick>, 40)
MAX_EQUAL_VALUE(BorderDefect<Thick>, 40)


PARAM_TITLE(BorderKlass2<Long>, L"���������� 2 �����")
PARAM_TITLE(BorderDefect<Long>, L"���������� ������")

PARAM_TITLE(BorderKlass2<Cross>, L"���������� 2 �����")
PARAM_TITLE(BorderDefect<Cross>, L"���������� ������")

//PARAM_TITLE(BorderAbove<Thick>, L"����.���������� ������� �����(��)")
//PARAM_TITLE(BorderLower<Thick>, L"����.���������� ������� ����(��)")
//PARAM_TITLE(BorderNominal<Thick>, L"����������� �������(mm)")

PARAM_TITLE(BorderKlass2<Thick>, L"������� 2 �����")
PARAM_TITLE(BorderKlass3<Thick>, L"������� 3 �����")
PARAM_TITLE(BorderDefect<Thick>, L"������� ������")

void ThresholdCrossDlg::Do(HWND h)
{
	if(TemplDialogList<ParametersBase, ThresholdsTable
		, TL::MkTlst<BorderKlass2<Cross>, BorderDefect<Cross>>::Result
	>(Singleton<ThresholdsTable>::Instance()).Do(h, L"���������� ������"))
	{}
}

void ThresholdLongDlg::Do(HWND h)
{
	if(TemplDialogList<ParametersBase, ThresholdsTable
		, TL::MkTlst<BorderKlass2<Long>, BorderDefect<Long>>::Result
	>(Singleton<ThresholdsTable>::Instance()).Do(h, L"���������� ������"))
	{}
}

void ThresholdThickDlg::Do(HWND h)
{
	if(TemplDialogList<ParametersBase, ThresholdsTable
		, TL::MkTlst<BorderKlass2<Thick>, BorderKlass3<Thick>, BorderDefect<Thick>>::Result
	>(Singleton<ThresholdsTable>::Instance()).Do(h, L"������ �������"))
	{}
}