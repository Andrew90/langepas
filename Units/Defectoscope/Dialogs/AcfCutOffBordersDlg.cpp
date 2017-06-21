#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"

#if 0
MIN_EQUAL_VALUE(LeftBorderACFCutOff<0>, 0)
MAX_EQUAL_VALUE(LeftBorderACFCutOff<0>, 500)
PARAM_TITLE(    LeftBorderACFCutOff<0>, L"Левая граница отсечения частоты датчика 1")
MIN_EQUAL_VALUE(RightBorderACFCutOff<0>, 0)
MAX_EQUAL_VALUE(RightBorderACFCutOff<0>, 500)
PARAM_TITLE(    RightBorderACFCutOff<0>, L"Правая граница отсечения частоты датчика 1")

MIN_EQUAL_VALUE(LeftBorderACFCutOff<1>, 0)
MAX_EQUAL_VALUE(LeftBorderACFCutOff<1>, 500)
PARAM_TITLE(    LeftBorderACFCutOff<1>, L"Левая граница отсечения частоты датчика 2")
MIN_EQUAL_VALUE(RightBorderACFCutOff<1>, 0)
MAX_EQUAL_VALUE(RightBorderACFCutOff<1>, 500)
PARAM_TITLE(    RightBorderACFCutOff<1>, L"Правая граница отсечения частоты датчика 2")

MIN_EQUAL_VALUE(LeftBorderACFCutOff<2>, 0)
MAX_EQUAL_VALUE(LeftBorderACFCutOff<2>, 500)
PARAM_TITLE(    LeftBorderACFCutOff<2>, L"Левая граница отсечения частоты датчика 3")
MIN_EQUAL_VALUE(RightBorderACFCutOff<2>, 0)
MAX_EQUAL_VALUE(RightBorderACFCutOff<2>, 500)
PARAM_TITLE(    RightBorderACFCutOff<2>, L"Правая граница отсечения частоты датчика 3")
#endif
void AcfCutOffBordersDlg::Do(HWND h)
{
	//if(TemplDialog<ParametersBase, BorderACFCutOffTable, DlgItem>(Singleton<BorderACFCutOffTable>::Instance()).Do(h, L"Границы отсечения в частотной области"))
	//{
	//}
}