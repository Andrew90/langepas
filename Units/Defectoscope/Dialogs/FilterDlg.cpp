#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"
#include "Dates\ComputeSolid.h"
#include "FrameWindow\FrameWindow.h"
#include "window_tool\EmptyWindow.h"

MIN_EQUAL_VALUE(CutoffFrequency<Cross>, 10)
MAX_EQUAL_VALUE(CutoffFrequency<Cross>, 4000)
PARAM_TITLE(    CutoffFrequency<Cross>, L"Частота отсечения фильтра нижних частот поперечного модуля")
PARAM_TITLE(    CutoffFrequencyOn<Cross>, L"Включение фильтра нижних частот поперечного модуля")

MIN_EQUAL_VALUE(CutoffFrequency<Long>, 10)
MAX_EQUAL_VALUE(CutoffFrequency<Long>, 4000)
PARAM_TITLE(    CutoffFrequency<Long>, L"Частота отсечения фильтра нижних частот продольного модуля")
PARAM_TITLE(    CutoffFrequencyOn<Long>, L"Включение фильтра нижних частот продольного модуля")

MIN_EQUAL_VALUE(CutoffFrequency<Voltage>, 10)
MAX_EQUAL_VALUE(CutoffFrequency<Voltage>, 4000)
PARAM_TITLE(    CutoffFrequency<Voltage>, L"Частота отсечения фильтра нижних частот сигнала модуля группы прочности")
PARAM_TITLE(    CutoffFrequencyOn<Voltage>, L"Включение фильтра нижних частот сигнала модуля группы прочности")

//MIN_EQUAL_VALUE(CutoffFrequency<ReferenceSignal>, 10)
//MAX_EQUAL_VALUE(CutoffFrequency<ReferenceSignal>, 4000)
//PARAM_TITLE(    CutoffFrequency<ReferenceSignal>, L"Частота отсечения фильтра нижних частот опорного сигнала модуля группы прочности")
//PARAM_TITLE(    CutoffFrequencyOn<ReferenceSignal>, L"Включение фильтра нижних частот опорного сигнала модуля группы прочности")


void FilderDlg::Do(HWND h)
{
	if(TemplDialogList<ParametersBase, AnalogFilterTable
		, TL::MkTlst<CutoffFrequency<Cross>, CutoffFrequencyOn<Cross>
		,  CutoffFrequency<Long>,  CutoffFrequencyOn<Long>
		>::Result
	>(Singleton<AnalogFilterTable>::Instance()).Do(h, L"Настройки аналогового фильтра"))
	{
	}
}

void SolidDlg::Do(HWND h)
{
	if(TemplDialogList<ParametersBase, AnalogFilterTable
		, TL::MkTlst<CutoffFrequency<Voltage>, CutoffFrequencyOn<Voltage>
		>::Result
	>(Singleton<AnalogFilterTable>::Instance()).Do(h, L"Настройки аналогового фильтра"))
	{
		ComputeSolid::Recalculation();
		FrameWindow &f = Singleton<FrameWindow>::Instance();
		RepaintWindow(f.thresholdsViewer.hWnd);
		RepaintWindow(f.frameViewer.hWnd);
	}
}