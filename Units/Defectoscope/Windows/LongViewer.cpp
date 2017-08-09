#include "stdafx.h"
#include "Windows\LongViewer.h"
#include "DataItem/ConstData.h"
#include "MessageText\StatusMessages.h"
#include "templates/templates.hpp"
#include "App/AppBase.h"
#include "window_tool\MenuAPI.h"
#include "Windows/ViewersMenu.hpp"
#include "Windows\LongWindow.h"

LongViewer::LongViewer()
	: CommonViewer(&tchart, App::count_long_sensors) 
    , tchart(backScreen)
	, viewerData(Singleton<ItemData<Long>>::Instance())
{
	chart = &tchart;

	tchart.items.get<FixedGridSeries>().SetColorCellHandler(this, &LongViewer::GetColorBar);
	cursor.SetMouseMoveHandler(this, &LongViewer::Draw);	
}

bool LongViewer::GetColorBar(unsigned sensor, int zone, double &data, unsigned &color)
{
	--sensor;
	data = viewerData.buffer[sensor][zone];
	color = StatusColor()(viewerData.status[sensor][zone]);
	return zone < viewerData.currentOffsetZones - 1;
}

bool LongViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = currentX;
	int y = currentY;

	if(y >= App::count_cross_sensors) return false;

	bool drawZones =  x < viewerData.currentOffsetZones - 1;
	if(drawZones)
	{		
		unsigned color;
		bool b;
		wchar_t s[256];
		StatusText()(viewerData.status[y][x], color, b, s);
		wchar_t buf[128];
		if(b)
		{
			wsprintf(buf, L"<ff>значение <ff0000>%s", Wchar_from<double, 1>(viewerData.buffer[y][x])());
		}
		else
		{
			buf[0] = 0;
		}
		wsprintf(label.buffer, L"<ff>продольный зона %d  датчик <ff00>%d   <%6x>%s %s"
			, 1 + x
			, 1 + y
			, color
			, s
			, buf
			);
		label.Draw(g());
	}
	return drawZones;
}

CONTEXT_MENU(LongWindow)
void LongViewer::operator()(TRButtonDown &l)
{
	if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Long>>().value)
	PopupMenu<ContextMenuLongWindow::items_list>::Do(l.hwnd, l.hwnd);
}