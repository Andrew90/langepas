#include "stdafx.h"
#include "WindowAdjustingMultipliers.h"
#include "window_tool\MenuAPI.h"
#include "window_tool\Emptywindow.h"

WindowAdjustingMultipliers::OkBtn::OkBtn(WindowAdjustingMultipliers &o)
	: owner(o)
{}
void WindowAdjustingMultipliers::OkBtn::Do(TCommand &l)
{
	owner.OkBtnHandler(l);
}

WindowAdjustingMultipliers::CancelBtn::CancelBtn(WindowAdjustingMultipliers &o)
	: owner(o)
{}

void WindowAdjustingMultipliers::CancelBtn::Do(TCommand &l)
{
	owner.CancelBtnHandler(l);
	DestroyWindow(l.hwnd);
}

#pragma warning(disable : 4355)

WindowAdjustingMultipliers::WindowAdjustingMultipliers()
	: okBtn(*this)
	, cancelBtn(*this)
	, ptrUpdate(NULL)
{}

void WindowAdjustingMultipliers::operator()(TCommand &l)
{
	EventDo(l);
	dprint("mess %d\n", l.uMsg);
}

void WindowAdjustingMultipliers::operator()(TGetMinMaxInfo &l)
{
	if(NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = width;
		l.pMinMaxInfo->ptMinTrackSize.y = height;
		l.pMinMaxInfo->ptMaxTrackSize.x = width;
		l.pMinMaxInfo->ptMaxTrackSize.y = height;		
	}	
}

LRESULT WindowAdjustingMultipliers::operator()(TCreate &l)
{
	static const int btnH = 30;
	static const int w = (width - 35) / 2;
	HWND h = CreateWindow(L"button",L"Применить"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		, 5, height - btnH - 45, w, btnH, l.hwnd, NULL, l.create->hInstance, NULL
		);
	
	SetWindowLongPtr(h, GWLP_USERDATA, (DWORD)&okBtn);
	h = CreateWindow(L"button",L"Закрыть"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		, 10 + w, height - btnH - 45, w, btnH, l.hwnd, NULL, l.create->hInstance, NULL
		);
	
	SetWindowLongPtr(h, GWLP_USERDATA, (DWORD)&cancelBtn);
	RECT r;
	GetClientRect(GetDesktopWindow(), &r);
	MoveWindow(l.hwnd, (r.right - width)/ 2, (r.bottom - height)/2, width, height, FALSE);
	return 0;
}
