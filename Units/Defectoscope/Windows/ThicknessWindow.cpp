#include "stdafx.h"
#include "ThicknessWindow.h"
#include "window_tool/MenuApi.h"
#include "ViewersMenu.hpp"
#include "tools_debug\DebugMess.h"
#include "Common.h"
#include "tools_debug\DebugMess.h"
#include "App/App.h"
#include "MessageText\SelectMessage.h"

template<int N>struct __line_sub_menu__<ThickWindow, N>
{
	typedef ThickWindow T;
	void operator()(HWND h)
	{
		typedef TL::MkTlst<	
			TopMenu<MenuNum<Line<T, N>, &Line<T, N>::MenuItemScan> >	
			, TopMenu<MenuNum<Line<T, N>, &Line<T, N>::MenuZoneDisable> >
		>::Result items_list;
		PopupMenu<items_list>::Do(h, h);
	}
};

ThickWindow::ThickWindow()
    : viewer(viewers.get<NoSubMenu<ThickViewer>>())
//	, count_sensors(App::count_Thick_sensors)
{
	viewer.cursor.SetMouseMoveHandler(this, &ThickWindow::DrawCursor);	
}
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
}

void ThickWindow::operator()(TSize &l)
{
    RECT r;
	GetClientRect(l.hwnd, &r);
	
	Common::__data__ data = {Common::window_height, r.right,  int((r.bottom - Common::window_height) / App::count_Thick_sensors)};
	TL::foreach<viewers_list, Common::__draw__>()(&viewers, &data);
}
void ThickWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}
VIEWERS_MENU(ThickWindow)
LRESULT ThickWindow::operator()(TCreate &l)
{
	TL::foreach<line_list, Common::__set_border_color__>()(&viewers, this);
	lastZone = -1;
	Menu<ViewersMenuThickWindow::MainMenu>().Init(l.hwnd);
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &l.hwnd);
	return 0;
}
//----------------------------------------------------------------------------------------------
void ThickWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, ThickWindow>(*this, l)
		);
}
//-----------------------------------------------------------------------------------
void ThickWindow::operator()(TCommand &l)
{
	EventDo(l);
}
//--------------------------------------------------------------------
void ThickWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
	TL::foreach<viewers_list, Common::__destroy__window__>()(&viewers);
	delete this;
	HWND h = FindWindow(WindowClass<SignalWindow>()(), 0);
	if(NULL != h)
	{
		DestroyWindow(h);
	}
}
//----------------------------------------------------------------------
wchar_t *ThickWindow::Title()
{
	return L"Просмотр толщины";
}
//--------------------------------------------------------------------------------------
bool ThickWindow::DrawCursor(TMouseMove &l, VGraphics &g)
{	
	int x = viewer.currentX;
	int y = viewer.currentY;
	drawZones =	x < viewer.viewerData.currentOffsetZones - 1;
	lastZone = x;
	if(drawZones)
	{
		viewer.Draw(l, g);
		TL::foreach<line_list, Common::__update__>()(&viewers, this);
	}
	else
	{
		viewer.label.buffer[0] = 0;
		TL::foreach<line_list, Common::__clr__>()(&viewers, this);
	}
	viewer.label.Draw(g());
	return drawZones;
}
//-------------------------------------------------------------------------------------
//void  NoSubMenu<ThickViewer>::operator()(TMouseWell &l)
//{
//	(*(Parent::Parent *)this)(l);
//	zprint(" nnnnnnnn\n");
//}

