#include "stdafx.h"
#include "PointsWindow.h"
#include "window_tool\MenuAPI.h"
#include "window_tool\EmptyWindow.h"
#include "window_tool\WindowsPosition.h"
#include "templates/templates.hpp"


LRESULT PointsWindow::operator()(TCreate &l)
{
	 pointsViewer.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<PointsViewer>::Proc, WindowClass<PointsViewer>()(), &pointsViewer);
	 return 0;
}

void PointsWindow::operator()(TSize &l)
{
	l.hwnd = pointsViewer.hWnd;
	MESSAGE(l);
	MoveWindow(pointsViewer.hWnd , 0, 0, l.Width, l.Height, true);
	
}

void PointsWindow::operator()(TCommand &l){EventDo(l);}

LRESULT PointsWindow::operator()(TNotify &l){return EventDo(l);}

void PointsWindow::operator()(TGetMinMaxInfo &l)
{
	if(NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = 600;
		l.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}

void PointsWindow::operator()(TMouseWell &l)
{
	pointsViewer(l);
}

void PointsWindow::Show()
{
	HWND hh = FindWindow(WindowClass<PointsWindow>()(), 0);
	if(NULL != hh)
	{
		RepaintWindow(hh);
		SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hh);
	}
	else
	{
		RECT r;
		WindowPosition::Get<PointsWindow>(r);
		hh = WindowTemplate(&Singleton<PointsWindow>::Instance(), L"", r.left, r.top, r.right, r.bottom);
		ShowWindow(hh, SW_SHOWNORMAL);
	}
}

void PointsWindow::IsOpenUpdate()
{
	HWND hh = FindWindow(WindowClass<PointsWindow>()(), 0);
	if(NULL != hh)
	{
		PointsWindow *w = (PointsWindow *)GetWindowLongPtr(hh, GWLP_USERDATA);
		w->pointsViewer.FillSignals();
		RepaintWindow(w->pointsViewer.hWnd);
	}
}