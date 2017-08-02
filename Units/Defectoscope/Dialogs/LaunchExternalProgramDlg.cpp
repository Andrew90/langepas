#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "window_tool\LaunchExternalProgram.h"

void LaunchExternalProgramDlg::Do(HWND)
{
	LaunchExternalProgram(L"Messages.exe");
}