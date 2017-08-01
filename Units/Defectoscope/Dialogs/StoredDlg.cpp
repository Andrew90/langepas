#include "stdafx.h"
#include "Dialogs.h"
#include "window_tool/DlgFiles.h"
#include "Compute\SaveData.h"
#include "window_tool\Zip.h"

void LoadDlg::Do(HWND h)
{
	OpenData o(h, L"Загрузить");
	if(o())
	{
		int offs = (int)wcslen(o.sFile) - 4;
		bool deleteFile = false;
		if(0 == wcscmp(&o.sFile[offs], L".bz2"))
		{
			Zip::UnZipFile2(o.sFile);
			o.sFile[offs] = 0;
			deleteFile = true;
		}

		ComputeData::Load(o.sFile);
		if(deleteFile) DeleteFile(o.sFile);
	}
}

void SaveDlg::Do(HWND h)
{
	SaveData o(h, L"Сохранить");
	if(o())
	{
		int offs = (int)wcslen(o.sFile) - 4;
		bool archive = 0 == wcscmp(&o.sFile[offs], L".bz2");
		if(archive) o.sFile[offs] = '\0';
		ComputeData::Save(o.sFile);
		if(archive)
		{
			Zip::ZipFile2(o.sFile);
		}
	}
}