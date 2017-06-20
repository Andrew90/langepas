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
PARAM_TITLE(    CutoffFrequency<Cross>, L"������� ��������� ������� ������ ������ ����������� ������")
PARAM_TITLE(    CutoffFrequencyOn<Cross>, L"��������� ������� ������ ������ ����������� ������")

MIN_EQUAL_VALUE(CutoffFrequency<Long>, 10)
MAX_EQUAL_VALUE(CutoffFrequency<Long>, 4000)
PARAM_TITLE(    CutoffFrequency<Long>, L"������� ��������� ������� ������ ������ ����������� ������")
PARAM_TITLE(    CutoffFrequencyOn<Long>, L"��������� ������� ������ ������ ����������� ������")

MIN_EQUAL_VALUE(CutoffFrequency<InputSignal>, 10)
MAX_EQUAL_VALUE(CutoffFrequency<InputSignal>, 4000)
PARAM_TITLE(    CutoffFrequency<InputSignal>, L"������� ��������� ������� ������ ������ ������� ������ ������ ���������")
PARAM_TITLE(    CutoffFrequencyOn<InputSignal>, L"��������� ������� ������ ������ ������� ������ ������ ���������")

//MIN_EQUAL_VALUE(CutoffFrequency<ReferenceSignal>, 10)
//MAX_EQUAL_VALUE(CutoffFrequency<ReferenceSignal>, 4000)
//PARAM_TITLE(    CutoffFrequency<ReferenceSignal>, L"������� ��������� ������� ������ ������ �������� ������� ������ ������ ���������")
//PARAM_TITLE(    CutoffFrequencyOn<ReferenceSignal>, L"��������� ������� ������ ������ �������� ������� ������ ������ ���������")


void FilderDlg::Do(HWND h)
{
	if(TemplDialogList<ParametersBase, AnalogFilterTable
		, TL::MkTlst<CutoffFrequency<Cross>, CutoffFrequencyOn<Cross>
		,  CutoffFrequency<Long>,  CutoffFrequencyOn<Long>
		>::Result
	>(Singleton<AnalogFilterTable>::Instance()).Do(h, L"��������� ����������� �������"))
	{
	}
}

void SolidDlg::Do(HWND h)
{
	if(TemplDialogList<ParametersBase, AnalogFilterTable
		, TL::MkTlst<CutoffFrequency<InputSignal>, CutoffFrequencyOn<InputSignal>
		>::Result
	>(Singleton<AnalogFilterTable>::Instance()).Do(h, L"��������� ����������� �������"))
	{
		ComputeSolid::Recalculation();
		FrameWindow &f = Singleton<FrameWindow>::Instance();
		RepaintWindow(f.thresholdsViewer.hWnd);
		RepaintWindow(f.frameViewer.hWnd);
	}
}