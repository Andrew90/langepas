#include "stdafx.h"
#include "LaunchExternalProgram.h"
#include <Shlwapi.h>
#include "templates\typelist.hpp"

void LaunchExternalProgram(wchar_t *progName)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {};
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow =  SW_SHOWNORMAL;
	si.cb = sizeof(STARTUPINFO);
	wchar_t path[1024];
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wsprintf(&path[wcslen(path)], L"\\%s", progName);
	BOOL b = CreateProcess(
		path
		, NULL
		, NULL
		, NULL
		, FALSE
		, CREATE_NEW_CONSOLE
		, NULL
		, NULL
		, &si
		, &pi
		);
}