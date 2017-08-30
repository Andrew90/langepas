#include "stdafx.h"
#include "Windows\ThicknessViewer.h"
#include "DataItem/ConstData.h"
//#include "MessageText/SelectMessage.h"
#include "templates/templates.hpp"
#include "App/AppBase.h"
#include "window_tool\MenuAPI.h"
#include "Windows/ViewersMenu.hpp"
//#include "Windows\ThicknessWindow.h"
#include "Graphics\Color.h"
#include "MessageText\StatusMessages.h"

using namespace Gdiplus;

ThickViewer::ThickViewer()
	: CommonViewer(&tchart, App::count_long_sensors) 
    , tchart(backScreen)
	, viewerData(Singleton<ItemData<Thick>>::Instance())
{
	chart = &tchart;

	//tchart.items.get<BarSeriesDouble>().SetColorBarHandler(this, &ThickViewer::GetColorBar);
	tchart.items.get<BarSeries>().SetColorBarHandler(this, &ThickViewer::GetColorBar);
	cursor.SetMouseMoveHandler(this, &ThickViewer::Draw);	
	cursor.horizontalLine = false;
}

void ThickViewer::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;	
	
	if(NULL != backScreen)
	{
		if(backScreen->GetWidth() < l.Width || backScreen->GetHeight() < l.Height)
		{
			delete backScreen;
		    backScreen = new Bitmap(l.Width, l.Height);
		}
	}
	else if(l.Width > 0 && l.Height > 0)
	{
		backScreen = new Bitmap(l.Width, l.Height);
	}
	else
	{
		return;
	}
	chart->rect.right = l.Width;
	chart->rect.bottom = l.Height;

	chart->minAxesY = 0;
	chart->maxAxesY = 15;

    Graphics g(backScreen);
	SolidBrush solidBrush(Color((ARGB)BACK_GROUND));
	g.FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);  
		
	chart->Draw(g);
}

#define xmin(a, b) ((a) < (b) ? (a) : (b))
#define min3(a,b,c) xmin(xmin(a, b), c)

#define xmax(a, b) ((a) > (b) ? (a) : (b))
#define max3(a,b,c) xmax(xmax(a, b), c)

//bool ThickViewer::GetColorBar(int zone, double &data, unsigned &color)//, double &data_1, unsigned &color1)
//{
//	data_1 = min3(viewerData.buffer[zone], viewerData.bufferMin[1][zone], viewerData.bufferMin[2][zone]);
//	data_ =  max3(viewerData.bufferMax[0][zone], viewerData.bufferMax[1][zone], viewerData.bufferMax[2][zone]);
//
//	unsigned st[1 + App::count_Thick_sensors] = {
//	   viewerData.status[0][zone]
//	   , viewerData.status[1][zone]
//	   , viewerData.status[2][zone]
//	   , -1
//	};
//
//	int status = ResultMessageId(st);
//
//#pragma message("Дописать цвет")
//	ColorBar()(
//		data_1
//		, color1
//		, status
//	    , Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<Thick>>().value
//		);
//	color = color1;
//	unsigned char *x = (unsigned char *) &color;
//	x[0] = unsigned char(2.5 * x[0] / 4);
//	x[1] = unsigned char(2.5 * x[1] / 4);
//	x[2] = unsigned char(2.5 * x[2] / 4);
//	return zone < viewerData.currentOffsetZones - 1;	  
//}
bool ThickViewer::GetColorBar(int zone, double &data, unsigned &color)
{
	data = viewerData.buffer[zone];
	//color = ConstData::ZoneColor(viewerData.status[zone]);
	color = StatusColor()(viewerData.status[zone]);
	return zone < viewerData.currentOffsetZones - 1;
}



bool ThickViewer::Draw(TMouseMove &l, VGraphics &g)
{
//	int y = currentY;
  //
//	if(y >= App::count_cross_sensors) return false;

	int x, y;
	tchart.CoordCell(l.x, l.y, x, y);

	bool drawZones =  x < viewerData.currentOffsetZones - 1;
#pragma message("дописать вывод сообщения")
	//*
	if(drawZones)
	{		
		unsigned color;
		bool b;

		double dataMin = viewerData.buffer[x];

		unsigned mess[] = {
		  viewerData.status[x]
		  , -1
		};
		wchar_t s[128];
		StatusText().FromSensors(mess, color, b, s);
		wchar_t buf[512];
		if(b)
		{
			wsprintf(buf, L"<ff>значение <ff0000>%s "
				, Wchar_from<double, 1>(dataMin)()
				);
		}
		else
		{
			buf[0] = 0;
		}

		ThresholdsTable::TItems &tt = Singleton<ThresholdsTable>::Instance().items;

		wsprintf(label.buffer, L"<ff>толщины зона %d <%6x>%s %s <7514f6>\"класс 2\" %s \"класс 3\" %s \"брак\" %s"
			, 1 + x
			, color
			, s
			, buf
			, Wchar_from<double, 1>(tt.get<BorderKlass2<Thick>>().value)()
			, Wchar_from<double, 1>(tt.get<BorderKlass3<Thick>>().value)()
			, Wchar_from<double, 1>(tt.get<BorderDefect<Thick>>().value)()
			);
		label.Draw(g());
	}
	return drawZones;
}

//CONTEXT_MENU(ThickWindow)
void ThickViewer::operator()(TRButtonDown &l)
{
	//if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thick>>().value)
	//PopupMenu<ContextMenuThickWindow::items_list>::Do(l.hwnd, l.hwnd);
}

#undef xmin
#undef xmax
#undef min3
#undef max3