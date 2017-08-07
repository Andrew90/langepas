#include "stdafx.h"
#include "LongWindow.h"
#include "window_tool/MenuApi.h"
#include "ViewersMenu.hpp"
#include "tools_debug\DebugMess.h"
#include "Common.h"
#include "tools_debug\DebugMess.h"
#include "App/App.h"
//#include "MessageText\SelectMessage.h"

LongWindow::LongWindow()
    : viewer(viewers.get<NoSubMenu<LongViewer>>())
	, adjustItem(*this)
	, count_sensors(App::count_long_sensors)
{
	viewer.cursor.SetMouseMoveHandler(this, &LongWindow::DrawCursor);	
}

void LongWindow::operator()(TSize &l)
{
    RECT r;
	GetClientRect(l.hwnd, &r);
	
	Common::__data__ data = {Common::window_height, r.right,  int(2.0 * (r.bottom - Common::window_height) 
		/ count_sensors)};
	TL::foreach<viewers_list, Common::__draw__>()(&viewers, &data);
}
void LongWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}
VIEWERS_MENU(LongWindow)
LRESULT LongWindow::operator()(TCreate &l)
{
	TL::foreach<line_list, Common::__set_border_color__>()(&viewers, this);
	lastZone = -1;
	Menu<ViewersMenuLongWindow::MainMenu>().Init(l.hwnd);
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &l.hwnd);
	return 0;
}
//----------------------------------------------------------------------------------------------
void LongWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, LongWindow>(*this, l)
		);
}
//-----------------------------------------------------------------------------------
void LongWindow::operator()(TCommand &l)
{
	EventDo(l);
}
//--------------------------------------------------------------------
void LongWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
	TL::foreach<viewers_list, Common::__destroy__window__>()(&viewers);
	delete this;
}
//----------------------------------------------------------------------
wchar_t *LongWindow::Title()
{
	return L"Просмотр продольных дефектов";
}
//--------------------------------------------------------------------------------------
bool LongWindow::DrawCursor(TMouseMove &l, VGraphics &g)
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

