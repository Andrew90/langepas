#include "stdafx.h"
#include "Dialogs.h"
#include "window_tool/DlgFiles.h"
#include "Compute\SaveData.h"

void LoadDlg::Do(HWND h)
{
	OpenData o(h, L"Загрузить");
	if(o())
	{
		ComputeData::Load(o.sFile);
	}
}

void SaveDlg::Do(HWND h)
{
	SaveData o(h, L"Сохранить");
	if(o())
	{
		int offs = (int)wcslen(o.sFile) - 4;
		bool archive = 0 == wcscmp(&o.sFile[offs], L".bz2");
		ComputeData::Save(o.sFile);
	}
}