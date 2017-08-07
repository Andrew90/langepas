#include "stdafx.h"
#include "CrossWindow.h"
#include "window_tool/MenuApi.h"
#include "ViewersMenu.hpp"
#include "tools_debug\DebugMess.h"
#include "Common.h"
#include "tools_debug\DebugMess.h"
#include "App/App.h"
#include "MessageText\StatusMessages.h"

CrossWindow::CrossWindow()
    : viewer(viewers.get<NoSubMenu<CrossViewer>>())
	, adjustItem(*this)
	, count_sensors(Singleton<ParametersTable>::Instance().items.get<CrossCountSensors>().value)
{
	viewer.cursor.SetMouseMoveHandler(this, &CrossWindow::DrawCursor);	
}

void CrossWindow::operator()(TSize &l)
{
    RECT r;
	GetClientRect(l.hwnd, &r);
	
	Common::__data__ data = {Common::window_height, r.right,  int(2.0 * (r.bottom - Common::window_height) 
		/ count_sensors)};
	TL::foreach<viewers_list, Common::__draw__>()(&viewers, &data);
}
void CrossWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}
VIEWERS_MENU(CrossWindow)
LRESULT CrossWindow::operator()(TCreate &l)
{
	TL::foreach<line_list, Common::__set_border_color__>()(&viewers, this);
	lastZone = -1;
	Menu<ViewersMenuCrossWindow::MainMenu>().Init(l.hwnd);
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &l.hwnd);
	return 0;
}
//----------------------------------------------------------------------------------------------
void CrossWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, CrossWindow>(*this, l)
		);
}
//-----------------------------------------------------------------------------------
void CrossWindow::operator()(TCommand &l)
{
	EventDo(l);
}
//--------------------------------------------------------------------
void CrossWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
	TL::foreach<viewers_list, Common::__destroy__window__>()(&viewers);
	delete this;
}
//----------------------------------------------------------------------
wchar_t *CrossWindow::Title()
{
	return L"Просмотр поперечных дефектов";
}
//--------------------------------------------------------------------------------------
bool CrossWindow::DrawCursor(TMouseMove &l, VGraphics &g)
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

