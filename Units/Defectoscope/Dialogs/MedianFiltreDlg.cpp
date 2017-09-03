#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"

MIN_EQUAL_VALUE(MedianFiltreWidth<Cross>, 3)
MAX_EQUAL_VALUE(MedianFiltreWidth<Cross>, 15)
PARAM_TITLE(MedianFiltreWidth<Cross>, L"������ ���������� ������ ���������� ��������")
PARAM_TITLE(MedianFiltreOn<Cross>, L"����������� ���������� ������� ���������� ��������")

MIN_EQUAL_VALUE(MedianFiltreWidth<Long>, 3)
MAX_EQUAL_VALUE(MedianFiltreWidth<Long>, 15)
PARAM_TITLE(MedianFiltreWidth<Long>, L"������ ���������� ������� ���������� ��������")
PARAM_TITLE(MedianFiltreOn<Long>, L"����������� ���������� ������� ���������� ��������")

MIN_EQUAL_VALUE(MedianFiltreWidth<Thick>, 3)
MAX_EQUAL_VALUE(MedianFiltreWidth<Thick>, 15)
PARAM_TITLE(MedianFiltreWidth<Thick>, L"������ ���������� ������� �������")
PARAM_TITLE(MedianFiltreOn<Thick>, L"����������� ���������� ������� �������")

void MedianFiltreDlg::Do(HWND h)
{
	if(TemplDialogList<ParametersBase, MedianFiltreTable
		, TL::MkTlst<
		MedianFiltreWidth<Cross>
		, MedianFiltreOn<Cross> 
		, MedianFiltreWidth<Long>
		, MedianFiltreOn<Long> 		
		>::Result
	>(Singleton<MedianFiltreTable>::Instance()).Do(h, L"��������� ������"))
	{
	}
}