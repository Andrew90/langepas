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
		ComputeData::Save(o.sFile);
	}
}