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

using namespace Gdiplus;

ThickViewer::ThickViewer()
	: CommonViewer(&tchart, App::count_long_sensors) 
    , tchart(backScreen)
	, viewerData(Singleton<ItemData<Thick>>::Instance())
{
	chart = &tchart;

	tchart.items.get<BarSeriesDouble>().SetColorBarHandler(this, &ThickViewer::GetColorBar);
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

bool ThickViewer::GetColorBar(int zone, double &data_, unsigned &color, double &data_1, unsigned &color1)
{
	data_1 = min3(viewerData.bufferMin[0][zone], viewerData.bufferMin[1][zone], viewerData.bufferMin[2][zone]);
	data_ =  max3(viewerData.bufferMax[0][zone], viewerData.bufferMax[1][zone], viewerData.bufferMax[2][zone]);

	int st[1 + App::count_Thick_sensors] = {
	   viewerData.status[0][zone]
	   , viewerData.status[1][zone]
	   , viewerData.status[2][zone]
	   , -1
	};

	int status = ResultMessageId(st);

#pragma message("Дописать цвет")
	ColorBar()(
		data_1
		, color1
		//, viewerData.status[zone]
		, status
	    , Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<Thick>>().value
		);
	color = color1;
	unsigned char *x = (unsigned char *) &color;
	x[0] = unsigned char(2.5 * x[0] / 4);
	x[1] = unsigned char(2.5 * x[1] / 4);
	x[2] = unsigned char(2.5 * x[2] / 4);
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
		int color;
		bool b;

		double dataMin = min3(viewerData.bufferMin[0][x], viewerData.bufferMin[1][x], viewerData.bufferMin[2][x]);
	    double dataMax =  max3(viewerData.bufferMax[0][x], viewerData.bufferMax[1][x], viewerData.bufferMax[2][x]);

		//char *s = StatusText()(viewerData.status[x], color, b);
		int mess[1 + App::count_Thick_sensors] = {
		  viewerData.status[0][x]
		  , viewerData.status[1][x]
		  , viewerData.status[2][x]
		  , -1
		};
		char *s = StatusText().FromSensors(mess, color, b);
		wchar_t buf[512];
		if(b)
		{
			wsprintf(buf, L"<ff>Минимальное значение <ff0000>%s <ff>максимальное значение <ff0000>%s"
				, Wchar_from<double, 1>(dataMin)()
				, Wchar_from<double, 1>(dataMax)()
				);
		}
		else
		{
			buf[0] = 0;
		}
		wsprintf(label.buffer, L"<ff>толщины зона <ff00>%d <%6x>%S %s"
			, 1 + x
			, color
			, s
			, buf
			);
		label.Draw(g());
	}
	//*/
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