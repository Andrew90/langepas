#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"

typedef Tresh<Temperature, 0> Tresh_Temperature_0;
typedef Tresh<Temperature, 1> Tresh_Temperature_1;
typedef Tresh<Magn, 0> Tresh_Magn_0;

DO_NOT_CHECK(Tresh_Temperature_0)
DO_NOT_CHECK(Tresh_Temperature_1)
DO_NOT_CHECK(Tresh_Magn_0)

PARAM_TITLE(Tresh_Temperature_0, L"Температура соленоида 1")
PARAM_TITLE(Tresh_Temperature_1, L"Температура соленоида 2")
PARAM_TITLE(Tresh_Magn_0    , L"Порог амплитуды для тестирования размагничивания")

void AdditionalParamsDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, AdditionalParams502Table, DlgItem>(Singleton<AdditionalParams502Table>::Instance()).Do(h, L"Дополнительные параметры"))
	{
	}
}

