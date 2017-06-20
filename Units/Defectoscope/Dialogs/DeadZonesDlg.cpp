#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"

MIN_EQUAL_VALUE(DeadAreaMM0<Cross>, 0)
MAX_EQUAL_VALUE(DeadAreaMM0<Cross>, 500)
PARAM_TITLE(    DeadAreaMM0<Cross>, L"Поперечный контроль начало трубы")

MIN_EQUAL_VALUE(DeadAreaMM1<Cross>, 0)
MAX_EQUAL_VALUE(DeadAreaMM1<Cross>, 500)
PARAM_TITLE(    DeadAreaMM1<Cross>, L"Поперечный контроль конец трубы")

MIN_EQUAL_VALUE(DeadAreaMM0<Long>, 0)
MAX_EQUAL_VALUE(DeadAreaMM0<Long>, 500)
PARAM_TITLE(    DeadAreaMM0<Long>, L"Продольный контроль начало трубы")
							
MIN_EQUAL_VALUE(DeadAreaMM1<Long>, 0)
MAX_EQUAL_VALUE(DeadAreaMM1<Long>, 500)
PARAM_TITLE(    DeadAreaMM1<Long>, L"Продольный контроль конец трубы")

MIN_EQUAL_VALUE(DeadAreaMM0<Thick>, 0)
MAX_EQUAL_VALUE(DeadAreaMM0<Thick>, 500)
PARAM_TITLE(    DeadAreaMM0<Thick>, L"Контроль толщины начало трубы")
							
MIN_EQUAL_VALUE(DeadAreaMM1<Thick>, 0)
MAX_EQUAL_VALUE(DeadAreaMM1<Thick>, 500)
PARAM_TITLE(    DeadAreaMM1<Thick>, L"Контроль толщины конец трубы")

void DeadZonesDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, DeadAreaTable, DlgItem>(Singleton<DeadAreaTable>::Instance()).Do(h, L"Мёртвые зоны"))
	{
	}
}