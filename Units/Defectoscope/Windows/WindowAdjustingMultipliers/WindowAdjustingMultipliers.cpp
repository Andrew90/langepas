#include "stdafx.h"
#include "WindowAdjustingMultipliers.h"
#include "window_tool\MenuAPI.h"
#include "window_tool\Emptywindow.h"


//Писать корректирующие множители для корректирующего сигнала

WindowAdjustingMultipliers::OkBtn::OkBtn(WindowAdjustingMultipliers &o)
	: owner(o)
{}
void WindowAdjustingMultipliers::OkBtn::Do(TCommand &l)
{
	//if(!TL::find<dlg_list, __test__>()(&owner->dlg_items, &l.hwnd))return;
	//TL::foreach<dlg_list, TreshWindow_ok_btn__>()(owner->dlg_items);
	//ComputeSolidGroup &solidGroup = Singleton<ComputeSolidGroup>::Instance();
	//TL::foreach<offset_list, __get_tresh__>()(owner->items, solidGroup.persents);
	//solidGroup.persentsChanged = true;
	//solidGroup.UpdateTresholds();
	//HWND hh = FindWindow(WindowClass<FrameWindow>()(), 0);
	//if(NULL != hh)
	//{
	//	((FrameWindow *)GetWindowLongPtr(hh, GWLP_USERDATA))->IncDecFrame();
	//}
	if(owner.ptrOk) (owner.*owner.ptrOk)(l);
	DestroyWindow(l.hwnd);
}

WindowAdjustingMultipliers::CancelBtn::CancelBtn(WindowAdjustingMultipliers &o)
	: owner(o)
{}

void WindowAdjustingMultipliers::CancelBtn::Do(TCommand &l)
{
	//ComputeSolidGroup &solidGroup = Singleton<ComputeSolidGroup>::Instance();
	//if(solidGroup.persentsChanged)
	//{
	//	if(TypesizePasswordDlg().Do(l.hwnd))
	//	{
	//		solidGroup.Save();
	//	}
	//	else
	//	{
	//		//todo	РАЗОБРАТЬСЯ ЧТО ТАКОЕ 
	//		//SelectHandler::Restore();
	//	}
	//	solidGroup.UpdateTresholds();
	//	HWND hh = FindWindow(WindowClass<FrameWindow>()(), 0);
	//	if(NULL != hh)
	//	{
	//		((FrameWindow *)GetWindowLongPtr(hh, GWLP_USERDATA))->IncDecFrame();
	//	}
	//	solidGroup.persentsChanged = false;
	//	solidGroup.changeTresholds = false;
	//}
	//SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
	//DestroyWindow(l.hwnd);
	if(owner.ptrCancel) (owner.*owner.ptrCancel)(l);
	DestroyWindow(l.hwnd);
}

#pragma warning(disable : 4355)

WindowAdjustingMultipliers::WindowAdjustingMultipliers()
	: okBtn(*this)
	, cancelBtn(*this)
	, ptrOk(NULL)
	, ptrCancel(NULL)
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
	HWND h = CreateWindow(L"button",L"Обновить"
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
