#include "stdafx.h"
#include "Dialogs.h"
#include "FrameWindow\FrameWindow.h"
#include "window_tool\Emptywindow.h"
#include "window_tool\WindowsPosition.h"

void FrameWindowDlg::Do(HWND h)
	{
		HWND hh = FindWindow(WindowClass<FrameWindow>()(), 0);
		if(NULL != hh)
		{
			RepaintWindow(hh);
			SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
			SetForegroundWindow(hh);
		}
		else
		{
			RECT r;
			WindowPosition::Get<FrameWindow>(r);
			hh = WindowTemplate(&Singleton<FrameWindow>::Instance(), L"", r.left, r.top, r.right, r.bottom);
			ShowWindow(hh, SW_SHOWNORMAL);
		}
	}

