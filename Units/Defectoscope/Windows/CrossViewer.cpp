#include "stdafx.h"
#include "Windows\CrossViewer.h"
#include "DataItem/ConstData.h"
//#include "MessageText/SelectMessage.h"
#include "MessageText\StatusMessages.h"
#include "templates/templates.hpp"
#include "App/AppBase.h"
#include "window_tool\MenuAPI.h"
#include "Windows/ViewersMenu.hpp"
#include "Windows\CrossWindow.h"

CrossViewer::CrossViewer()
	: CommonViewer(&tchart, Singleton<ParametersTable>::Instance().items.get<CrossCountSensors>().value)
    , tchart(backScreen)
	, viewerData(Singleton<ItemData<Cross>>::Instance())
{
	chart = &tchart;

	tchart.items.get<FixedGridSeries>().SetColorCellHandler(this, &CrossViewer::GetColorBar);
	cursor.SetMouseMoveHandler(this, &CrossViewer::Draw);	
}

bool CrossViewer::GetColorBar(unsigned sensor, int zone, double &data, unsigned &color)
{
	--sensor;
	data = viewerData.buffer[sensor][zone];
	color = StatusColor()(viewerData.status[sensor][zone]);
	return zone < viewerData.currentOffsetZones - 1;
}

bool CrossViewer::Draw(TMouseMove &l, VGraphics &g)
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
		wsprintf(label.buffer, L"<ff>поперечный зона %d  датчик <ff00>%d  <%6x>%s %s"
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

CONTEXT_MENU(CrossWindow)
void CrossViewer::operator()(TRButtonDown &l)
{
//	if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Cross>>().value)
	PopupMenu<ContextMenuCrossWindow::items_list>::Do(l.hwnd, l.hwnd);
}
