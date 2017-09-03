#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "App/App.h"

MIN_EQUAL_VALUE(MedianFiltreWidth<Cross>, 3)
MAX_EQUAL_VALUE(MedianFiltreWidth<Cross>, 15)
PARAM_TITLE(MedianFiltreWidth<Cross>, L"Ширина медианного фильта поперечных дефектов")
PARAM_TITLE(MedianFiltreOn<Cross>, L"Подключение медианного фильтра поперечных дефектов")

MIN_EQUAL_VALUE(MedianFiltreWidth<Long>, 3)
MAX_EQUAL_VALUE(MedianFiltreWidth<Long>, 15)
PARAM_TITLE(MedianFiltreWidth<Long>, L"Ширина медианного фильтра продольных дефектов")
PARAM_TITLE(MedianFiltreOn<Long>, L"Подключение медианного фильтра продольных дефектов")

MIN_EQUAL_VALUE(MedianFiltreWidth<Thick>, 3)
MAX_EQUAL_VALUE(MedianFiltreWidth<Thick>, 15)
PARAM_TITLE(MedianFiltreWidth<Thick>, L"Ширина медианного фильтра толщины")
PARAM_TITLE(MedianFiltreOn<Thick>, L"Подключение медианного фильтра толщины")

void MedianFiltreDlg::Do(HWND h)
{
	if(TemplDialogList<ParametersBase, MedianFiltreTable
		, TL::MkTlst<
		MedianFiltreWidth<Cross>
		, MedianFiltreOn<Cross> 
		, MedianFiltreWidth<Long>
		, MedianFiltreOn<Long> 		
		>::Result
	>(Singleton<MedianFiltreTable>::Instance()).Do(h, L"Медианный фильтр"))
	{
	}
}