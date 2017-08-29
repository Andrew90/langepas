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


PARAM_TITLE(BorderKlass2<Long>, L"Порог продольный 2 класс")
PARAM_TITLE(BorderDefect<Long>, L"Порог продольный дефект")

PARAM_TITLE(BorderKlass2<Cross>, L"Порог поперечный 2 класс")
PARAM_TITLE(BorderDefect<Cross>, L"Порог поперечный дефект")

//PARAM_TITLE(BorderAbove<Thick>, L"Макс.отклонение толщины вверх(мм)")
//PARAM_TITLE(BorderLower<Thick>, L"Макс.отклонение толщины вниз(мм)")
//PARAM_TITLE(BorderNominal<Thick>, L"Номинальная толщина(mm)")

PARAM_TITLE(BorderKlass2<Thick>, L"Порог толщина 2 класс")
PARAM_TITLE(BorderKlass3<Thick>, L"Порог толщина 3 класс")
PARAM_TITLE(BorderDefect<Thick>, L"Порог толщина дефект")

void ThresholdCrossDlg::Do(HWND h)
{
	if(TemplDialogList<ParametersBase, ThresholdsTable
		, TL::MkTlst<BorderKlass2<Cross>, BorderDefect<Cross>>::Result
	>(Singleton<ThresholdsTable>::Instance()).Do(h, L"Поперечные пороги"))
	{}
}

void ThresholdLongDlg::Do(HWND h)
{
	if(TemplDialogList<ParametersBase, ThresholdsTable
		, TL::MkTlst<BorderKlass2<Long>, BorderDefect<Long>>::Result
	>(Singleton<ThresholdsTable>::Instance()).Do(h, L"Продольные пороги"))
	{}
}

void ThresholdThickDlg::Do(HWND h)
{
	if(TemplDialogList<ParametersBase, ThresholdsTable
		, TL::MkTlst<BorderKlass2<Thick>, BorderKlass3<Thick>, BorderDefect<Thick>>::Result
	>(Singleton<ThresholdsTable>::Instance()).Do(h, L"Пороги толщины"))
	{}
}