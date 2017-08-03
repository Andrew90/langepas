#include "stdafx.h"
#include "Windows\ResultViewer.h"
#include "DataItem/ConstData.h"
#include "MessageText\StatusMessages.h"
#include "templates/templates.hpp"
#include "App/AppBase.h"
#include "window_tool\MenuAPI.h"
#include "Windows/ViewersMenu.hpp"
#include "Windows\CrossWindow.h"
#include "DataItem\DataItem.h"
#include "DataItem\DataItem.h"

using namespace Gdiplus;

ResultViewer::Cut0::Cut0(Chart &c):VBorder(c){color = 0xff000000;widthPen = 3; dashStyle = DashStyleDashDotDot;}
void ResultViewer::Cut0::Draw()
{
	value = Singleton<ResultData>::Instance().cutZone0;
	if(0 != value)VBorder::Draw();
}

ResultViewer::Cut1::Cut1(Chart &c):VBorder(c){color = 0xff000000; widthPen = 3; dashStyle = DashStyleDashDotDot;}
void ResultViewer::Cut1::Draw()
{
	value = Singleton<ResultData>::Instance().cutZone1;
	if(0 != value)VBorder::Draw();
}

ResultViewer::ResultViewer()
	: CommonViewer(&tchart, App::count_long_sensors) 
    , tchart(backScreen)
	, viewerData(Singleton<ResultData>::Instance())
{
	chart = &tchart;

	tchart.items.get<FixedGridSeries>().SetColorCellHandler(this, &ResultViewer::GetColorBar);
	cursor.SetMouseMoveHandler(this, &ResultViewer::Draw);
	cursor.horizontalLine = false;
}

bool ResultViewer::GetColorBar(unsigned sensor, int zone, double &data, unsigned &color)
{
	data = 0;
	color = StatusColor()(viewerData.status[zone]);
	return zone < viewerData.currentOffsetZones - 1;
}

bool ResultViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = currentX;

	bool drawZones =  x < viewerData.currentOffsetZones - 1;
	if(drawZones)
	{		
		unsigned color;
		bool b;
		wchar_t s[256];
		StatusText()(viewerData.status[x], color, b, s);
		wsprintf(label.buffer, L"<ff>результат зона <ff>%d <%6x>%s"
			, 1 + x
			, color
			, s
			);
		label.Draw(g());
	}
	return drawZones;
}

//CONTEXT_MENU(LongWindow)
void ResultViewer::operator()(TRButtonDown &l)
{
	//if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Cross>>().value)
	//PopupMenu<ContextMenuLongWindow::items_list>::Do(l.hwnd, l.hwnd);
}