#include "stdafx.h"
#include "SignalWindow.h"
#include "window_tool/MenuApi.h"
#include "ViewersMenu.hpp"
#include "tools_debug\DebugMess.h"
#include "Common.h"
#include "tools_debug\DebugMess.h"
#include "App/App.h"
#include "MessageText\SelectMessage.h"
#include "Windows\Common.h"
#include "Compute\ComputeSignal.h"
#include "window_tool\Pass.h"

SignalWindow::SignalWindow()
	: viewers(*this) 
	, leftAcfCutOff(0)
	, rightAcfCutOff(0)
{}

namespace Common
{
	template<template<class, int>class L, int N, class P>struct __draw__<L<ThickWindow, N>, P>
	{
		typedef L<ThickWindow, N> O;
		void operator()(O *o, P *p)
		{
			MoveWindow(o->hWnd, 0, p->y, p->width, p->height, TRUE);
			p->y += p->height;
		}
	};

	struct __data2__
	{
		int x, y, width, height, dx;
	};

	template<class O, class P>struct __draw2__
	{
		void operator()(O &o, P &p)
		{
			MoveWindow(o.hWnd, p.x, p.y, p.width, p.height, TRUE);
			p.x += p.dx;
		}
	};
}

void SignalWindow::operator()(TSize &l)
{
	RECT r;
	GetClientRect(l.hwnd, &r);

	static const int topLabelHeight = 28;
	MoveWindow(topLabelViewer.hWnd , 0, 0, r.right, topLabelHeight, true);

	int width = r.right / TL::Length<viewers_list>::value;
	Common::__data2__ data = {0, topLabelHeight, width,  r.bottom - topLabelHeight, width};
	TL::foreach<viewers_list, Common::__draw2__>()(viewers, data);
}
void SignalWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}
LRESULT SignalWindow::operator()(TCreate &l)
{	
	CreateChildWindow(l.hwnd, &topLabelViewer);
	topLabelViewer.label.fontHeight = 16;
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &l.hwnd);
	return 0;
}
//----------------------------------------------------------------------------------------------
void SignalWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, SignalWindow>(*this, l)
		);
}
//-----------------------------------------------------------------------------------
void SignalWindow::operator()(TCommand &l)
{
	EventDo(l);
}
//--------------------------------------------------------------------
void SignalWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
	TL::foreach<viewers_list, Common::__destroy__window__>()(&viewers);
	delete this;
}
//----------------------------------------------------------------------
wchar_t *SignalWindow::Title()
{
	return L"Просмотр толщины";
}
//--------------------------------------------------------------------------------------
bool SignalWindow::DrawCursor(TMouseMove &l, VGraphics &g)
{	
	//int x = viewer.currentX;
	//int y = viewer.currentY;
	//drawZones =	x < viewer.viewerData.currentOffsetZones;
	//lastZone = x;
	//if(drawZones)
	//{
	//viewer.Draw(l, g);
	//TL::foreach<viewers_list, Common::__update__>()(&viewers, this);
	//}
	//else
	//{
	//	viewer.label.buffer[0] = 0;
	//	TL::foreach<viewers_list, Common::__clr__>()(&viewers, this);
	//}
	//viewer.label.Draw(g());
	//return drawZones;
	return true;
}
//-----------------------------------------------------------------------------------------
namespace
{
	struct __data__
	{
		int zone, sensor, offsetInZone;
	};

	template<class O, class P>struct __signal_open__
	{
		void operator()(O &o, P &p)
		{
			ComputeSignal<O>()(o, p.zone, p.sensor, p.offsetInZone);
		}
	};

	template<class O, class P>struct __set_border__
	{
		void operator()(O &, P &){}
	};
	template<int N, class P>struct __set_border__<LeftBorderACFCutOff<N>, P>
	{
		typedef LeftBorderACFCutOff<N> O;
		void operator()(O &o, P &p)
		{
			if(N == p.sensor)
			{
				p.leftAcfCutOff = o.value;
			}
		}
	};
	template<int N, class P>struct __set_border__<RightBorderACFCutOff<N>, P>
	{
		typedef RightBorderACFCutOff<N> O;
		void operator()(O &o, P &p)
		{
			if(N == p.sensor)
			{
				p.rightAcfCutOff = o.value;
			}
		}
	};

	template<class O, class P>struct __repaint__
	{
		  void operator()(O &o)
		  {
			  RepaintWindow(o.hWnd);
		  }
	};
}

void SignalWindow::Open(int zone, int sensor, int offsetInZone)
{
	HWND h = FindWindow(WindowClass<SignalWindow>()(), 0);
	SignalWindow *e = NULL != h 
		? (SignalWindow *)GetWindowLongPtr(h, GWLP_USERDATA)
		: new SignalWindow
		;

	e->zone = zone;
	e->sensor = sensor;
	e->offsetInZone = offsetInZone;

	bool b = NULL == h;

	if(b)
	{
		RECT r;
		WindowPosition::Get<SignalWindow>(r);
		h = WindowTemplate(e, SignalWindow::Title(), r.left, r.top, r.right, r.bottom);
		memmove(&e->bordersCutOff, &Singleton<BorderACFCutOffTable>::Instance().items, sizeof(e->bordersCutOff));
	}
	TL::foreach<BorderACFCutOffTable::items_list, __set_border__>()(e->bordersCutOff, *e);
	__data__ data = {zone, sensor, offsetInZone};
	TL::foreach<viewers_list, __signal_open__>()(e->viewers, data);			

	wsprintf(e->topLabelViewer.label.buffer, L"<ff>зона <ff0000>%d  <ff>датчик <ff00>%d  <ff>смещение <ff0000>%d", 1 + e->zone, 1 + e->sensor, e->offsetInZone);

	if(!b)
	{
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
	else
	{
		ShowWindow(h, SW_SHOWNORMAL);
	}
	RepaintWindow(e->topLabelViewer.hWnd);
	TL::foreach<viewers_list, __repaint__>()(e->viewers);
}

void SignalWindow::ChangeOffset(int zone, int offsetInZone)
{
	HWND h = FindWindow(WindowClass<SignalWindow>()(), 0);
	if(NULL != h)
	{
		SignalWindow *e = (SignalWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		e->offsetInZone = offsetInZone;
		e->zone = zone;
		TL::foreach<BorderACFCutOffTable::items_list, __set_border__>()(e->bordersCutOff, *e);
		__data__ data = {e->zone, e->sensor, e->offsetInZone};
		TL::foreach<viewers_list, __signal_open__>()(e->viewers, data);	
		
		wsprintf(e->topLabelViewer.label.buffer, L"<ff>зона <ff0000>%d  <ff>датчик <ff00>%d  <ff>смещение <ff0000>%d", 1 + e->zone, 1 + e->sensor, e->offsetInZone);
		RepaintWindow(e->topLabelViewer.hWnd);		
		TL::foreach<viewers_list, __repaint__>()(e->viewers);
	}
}

namespace
{
	template<class O, class P>struct __before_close__
	{
		bool operator()(O &o, P &p)
		{
			return o.value == p.get<O>().value;
		}
	};
}

void SignalWindow::operator()(TClose &l)
{
	if(!TL::find<BorderACFCutOffTable::items_list, __before_close__>()(bordersCutOff, Singleton<BorderACFCutOffTable>::Instance().items)
		&& IDYES == MessageBox(l.hwnd, L"Сохранить изменения границ отсечения?", L"Cообщение", MB_ICONQUESTION | MB_YESNO)
		&& TypesizePasswordDlg().Do(l.hwnd)
	)
	{
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{
			memmove(&Singleton<BorderACFCutOffTable>::Instance().items, &bordersCutOff, sizeof(bordersCutOff));
			int id = CurrentId<ID<BorderACFCutOffTable> >();
			UpdateWhere<BorderACFCutOffTable>(Singleton<BorderACFCutOffTable>::Instance(), base).ID(id).Execute();
		}
	}
	DestroyWindow(l.hwnd);
}
